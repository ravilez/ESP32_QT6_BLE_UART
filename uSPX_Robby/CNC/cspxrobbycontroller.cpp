#include <QFileDialog>
#include <QMessageBox>

#include "cspxrobbycontroller.h"

#include "../mainwindow.h"
#include "../ui_mainwindow.h"
#include "cspxhpglcommands.h"
#include "../Comms/cspxcomport.h"
#include "../Comms/cspxblecontroller.h"


#include <QtSerialPort/QSerialPort>
#include <QGraphicsLineItem>

#ifndef _WIN32
#define sscanf_s sscanf
#endif


MainWindow *CSPXRobbyController::machineInterface = nullptr;

//QSerialPort::readyRead signal is emitted once every time new data is available for reading from the device
void SerialReceiverThread::run()
{
    while(1)
    {
        msleep(10000);

        sync.lock();
        if(_pause)
            pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
        sync.unlock();
    }
}

void SerialReceiverThread::readData()
{
    MainWindow* machineUI = CSPXRobbyController::GetMainWindow();
    CSPXRobbyController *ctrl = machineUI->GetController();

    //machineInterface->ui->serialComboBox->currentText().toStdString().c_str();

    CSPXMessage rec_msg;
    ctrl->GetComs()->ReceiveData(&rec_msg);

    if (rec_msg.GetData())
        emit haveResponse(rec_msg.GetData());
}

void CSPXRobbyController::readData()
{
    //machineInterface->ui->serialComboBox->currentText().toStdString().c_str();

    CSPXMessage rec_msg;
    GetComs()->ReceiveData(&rec_msg);

    machineInterface->GetController()->response += rec_msg.GetData();

    if (strstr((char*)machineInterface->GetController()->response,"\r"))
    {
        machineInterface->ui->statusMessage->setPlainText(QString(machineInterface->GetController()->response));

        float x,y,z;
        char *pcoords = nullptr;
        char *pmode = nullptr;

        //if (lastHpglCommand[0] != 'O' && lastHpglCommand[1] != 'F')
        {
            pcoords = strstr((char*)machineInterface->GetController()->response,"X:");
            if (pcoords)
            {
                sscanf_s(pcoords,"X: %f;Y: %f;Z: %f;",&x,&y,&z);
                machineInterface->ui->x_lcdNumber->display(x);
                machineInterface->ui->y_lcdNumber->display(y);
                machineInterface->ui->z_lcdNumber->display(z);
            }
            pcoords =  strstr((char*)machineInterface->GetController()->response,"Y:");
            if (pcoords)
            {
                sscanf_s(pcoords,"Y: %f;Z: %f;",&y,&z);
                machineInterface->ui->y_lcdNumber->display(y);
                machineInterface->ui->z_lcdNumber->display(z);
            }
            pcoords =  strstr((char*)machineInterface->GetController()->response,"Z:");
            if (pcoords)
            {
                sscanf_s(pcoords,"Z: %f;",&z);
                machineInterface->ui->z_lcdNumber->display(z);
            }
        }

        pmode =  strstr((char*)machineInterface->GetController()->response,"MD:");
        if (pmode)
        {
            int mod;
            if (sscanf_s(pmode,"MD:%d;",&mod) == 1)
                SetMode(mod);
            switch (machineInterface->GetController()->GetMode())
            {
                case 0:
                {
                    machineInterface->ui->localMode->setChecked(true);
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 1:
                {
                    machineInterface->ui->dccMode->setChecked(true);
                    //dccModeCtrl->ChangeGroupCheck();
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 2:
                {
                    machineInterface->ui->lineMode->setChecked(true);
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 3:
                {
                    machineInterface->ui->fileMode->setChecked(true);
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 4: //busy
                {
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->red));
                }
            }
        }

        pmode = strstr((char*)rec_msg.GetData(),"FM:");
        if (pmode)
        {
            //MD: recheck the new current profiler mode
            const char fmdata[] = { 0x1b, 'M', 'D', ':', 0x0d, 0x1a};
            ((CSPXCOMPort*)GetComs())->write(fmdata,6);

            if (machineInterface->ui->radioSerialButton->isChecked())
                ((CSPXCOMPort*)GetComs())->waitForBytesWritten(300);
        }

        pmode = strstr((char*)rec_msg.GetData(),"UC:");
        if (pmode)
        {
            CSPXMessage msg;
            msg.SetData("OE;");
            SendData(msg);
        }

        machineInterface->GetController()->response = "";
    }

    emit changeSpindleCoordinates(machineInterface->ui->xHome->value()+machineInterface->ui->x_lcdNumber->value(), machineInterface->ui->yHome->value() + machineInterface->ui->y_lcdNumber->value(), machineInterface->ui->zHome->value()+machineInterface->ui->z_lcdNumber->value());
}

void CSPXRobbyController::handleError(QSerialPort::SerialPortError error)
{
    if (machineInterface->ui->radioSerialButton->isChecked()) {
        if (error == QSerialPort::ResourceError) {
            MainWindow* dlg = CSPXRobbyController::GetMainWindow();
            dlg->ui->statusMessage->setPlainText(QString("Serial Port error : " + QString::number(error)));
        }
    }
}

CSPXRobbyController::CSPXRobbyController(QObject* parent) : QObject(parent)
  ,connected(false)
  ,receiverThread(nullptr)
{
    whitePen.setColor(Qt::white);
    whitePen.setWidth(1);
    whitePen.setStyle(Qt::SolidLine);
    whitePen.setCosmetic(true);
}

CSPXRobbyController::~CSPXRobbyController()
{
    if (receiverThread != nullptr)
        receiverThread->terminate();
}

bool CSPXRobbyController::connectComms()
{
    //create communiation channel
    if (machineInterface->ui->radioSerialButton->isChecked())
    {
        QString port = machineInterface->ui->serialComboBox->currentText();
        if (port.isEmpty())
            return false;

        coms = new CSPXCOMPort();

        if (coms->Open(port.toStdString().c_str())) {

            receiverThread = new SerialReceiverThread();
            receiverThread->controller = this;
            connect(receiverThread, &SerialReceiverThread::haveResponse, this, &CSPXRobbyController::checkResponse);

            connect((CSPXCOMPort*)coms, &QSerialPort::errorOccurred, this, &CSPXRobbyController::handleError);
            connect((CSPXCOMPort*)coms,  &QSerialPort::readyRead, receiverThread, &SerialReceiverThread::readData);
            connect(this,  &CSPXRobbyController::resetMachine, this, &CSPXRobbyController::abortProcess);
        } else {
            machineInterface->ui->statusMessage->setPlainText(QString("Serial Port error : Not Connected"));
            return connected = false;
        }

        connected = true;

        receiverThread->start();

        //start timer to check busy (CTS flag) every 500ms
        connect(&busyTimer,&QTimer::timeout,this,&CSPXRobbyController::checkBusyState);
        busyTimer.setSingleShot(true);
        busyTimer.setInterval(500);
        busyTimer.start();
    }
    else {

        QString BLE_target = machineInterface->ui->targetBLE->displayText();
        if (BLE_target.isEmpty())
            return false;

        coms = new CSPXBLEController();
        if (coms->Open(BLE_target.toStdString().c_str())) {
            connect((CSPXBLEController*)coms, &CSPXBLEController::haveResponse, this, &CSPXRobbyController::checkResponse);
        }
    }


    //query mode
    const char mdq_data[] = { 0x1b, 'M', 'D', ':', 0x0d, 0x1a };

    SetMode(-1);
    CSPXMessage msg;
    msg.SetData(mdq_data,6);
    SendData(msg);

    //change to local mode
    emit machineInterface->ui->localMode->clicked(true);
    if (GetMode()==0)
        //change to line mode
        emit machineInterface->ui->lineMode->clicked(true);

    if (GetMode()==2)
    {
        CSPXString cmd("MoveXYZHome");
        sendRobbyHpglLineCommand(cmd);
     }

    return connected;
}

void CSPXRobbyController::checkBusyState()
{
    if (machineInterface->ui->radioSerialButton->isChecked()) {
        bool CTS_Status = ((CSPXCOMPort*)GetComs())->pinoutSignals() & QSerialPort::ClearToSendSignal;
        if (CTS_Status)
            machineInterface->ui->label_CTS_Status->setPixmap(*(machineInterface->green));
        else
            machineInterface->ui->label_CTS_Status->setPixmap(*(machineInterface->red));
    }

    busyTimer.start();

}

void CSPXRobbyController::disconnectComms()
{
    if (connected) {
        busyTimer.stop();

        coms->Close();
        MainWindow* dlg = CSPXRobbyController::GetMainWindow();
        dlg->Initialize();

        connected = false;
    }
}


bool CSPXRobbyController::sendRobbyHpglLineCommand(CSPXString command)
{
    memset(lastHpglCommand,0,128);

    MainWindow* dlg = CSPXRobbyController::GetMainWindow();

    double freeTravelSpeed = machineInterface->ui->travelSpeedSpinBox->value();

    if (dlg->GetController()->GetMode() != 2)
    {
        machineInterface->ui->statusMessage->setPlainText(QString("Not in Hpgl Line Mode !"));
        return false;
    }

    int z = std::lround(40.0*machineInterface->ui->z_lcdNumber->value());
    int zHome = std::lround(40.0*machineInterface->ui->zHome->value());
    int zTrack = std::lround(40.0*machineInterface->ui->zSafetyStep->text().toFloat());

    if (zTrack > zHome + z)
        zTrack = zHome + z;
    if (zTrack < 0)
        zTrack = 0;

    QString errorMsg("out-of-bounds movement (300.0 x 400.0).");

    if (command == "Reset")
    {
        disconnect(((CSPXCOMPort*)coms), &QSerialPort::readyRead, this, &CSPXRobbyController::readData);
        sprintf(lastHpglCommand,"@XX;%c%c",0x0d,0x1a);
    }
    else if (command == "Get Coordinates")
    {
        sprintf(lastHpglCommand,"OA;%c%c",0x0d,0x1a);
    }
    else if (command == "MoveXY")
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PA %ld,%ld;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->xSpindlePos),std::lround(40.0*machineInterface->ySpindlePos),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PA %ld,%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->xSpindlePos),std::lround(40.0*machineInterface->ySpindlePos),0x0d,0x1a);
    }
    else if (command == "MoveXYZHome")
    {
        sprintf(lastHpglCommand,"@HM;OA;%c%c",0x0d,0x1a);
        machineInterface->ui->xHome->display(0.000);
        machineInterface->ui->yHome->display(0.000);
        machineInterface->ui->zHome->display(0.000);
    }
    else if (command == "MoveZHome")
    {
        sprintf(lastHpglCommand,"@HZ;OA;%c%c",0x0d,0x1a);
        machineInterface->ui->zHome->display(0.000);
    }
    else if (command == "MoveLeftUp" && machineInterface->ui->x_lcdNumber->value() + machineInterface->ui->xHome->value() > machineInterface->ui->xStep->text().toFloat() && machineInterface->ui->y_lcdNumber->value() + machineInterface->ui->yHome->value() < 400.0 - machineInterface->ui->yStep->text().toFloat())
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR -%ld,%ld;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR -%ld,%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveLeftDown" && machineInterface->ui->x_lcdNumber->value() + machineInterface->ui->xHome->value() > machineInterface->ui->xStep->text().toFloat() && machineInterface->ui->y_lcdNumber->value() + machineInterface->ui->yHome->value() > machineInterface->ui->yStep->text().toFloat())
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR -%ld,-%ld;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR -%ld,-%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveRightUp" && machineInterface->ui->x_lcdNumber->value() + machineInterface->ui->xHome->value() + machineInterface->ui->xStep->text().toFloat() < 300. && machineInterface->ui->y_lcdNumber->value() + machineInterface->ui->yHome->value() + machineInterface->ui->yStep->text().toFloat() < 400.)
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR %ld,%ld;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR %ld,%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveRightDown" && machineInterface->ui->x_lcdNumber->value() + machineInterface->ui->xHome->value() > machineInterface->ui->xStep->text().toFloat() && machineInterface->ui->y_lcdNumber->value() + machineInterface->ui->yHome->value() > machineInterface->ui->yStep->text().toFloat())
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR %ld,-%ld;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR %ld,-%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),std::lround(40.0*machineInterface->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveZUp" && machineInterface->ui->z_lcdNumber->value() > -machineInterface->ui->zHome->value())
        sprintf(lastHpglCommand,"@ZR -%ld;OA;%c%c",std::lround(40.0*machineInterface->ui->zStep->text().toFloat()),0x0d,0x1a);
    else if (command == "MoveZDown" && machineInterface->ui->z_lcdNumber->value() < 90-machineInterface->ui->zHome->value())
        sprintf(lastHpglCommand,"@ZR %ld;OA;%c%c",std::lround(40.0*machineInterface->ui->zStep->text().toFloat()),0x0d,0x1a);
    else if (command == "MoveLeft" && machineInterface->ui->x_lcdNumber->value() + machineInterface->ui->xHome->value() >= machineInterface->ui->xStep->text().toFloat())
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR -%ld,0;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR -%ld,0;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->xStep->displayText().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveDown" && machineInterface->ui->y_lcdNumber->value() + machineInterface->ui->yHome->value() >= machineInterface->ui->yStep->text().toFloat())
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR 0,-%ld;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR 0,-%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->yStep->displayText().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveRight" && machineInterface->ui->x_lcdNumber->value() + machineInterface->ui->xHome->value() + machineInterface->ui->xStep->text().toFloat() <= 300.0)
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR %ld,0;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR %ld,0;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->xStep->displayText().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveUp" && machineInterface->ui->y_lcdNumber->value() + machineInterface->ui->yHome->value() + machineInterface->ui->yStep->text().toFloat() <= 400.0)
    {
        if (machineInterface->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR 0,%ld;@ZR %dOA;%c%c;",freeTravelSpeed,zTrack,std::lround(40.0*machineInterface->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR 0,%ld;OA;%c%c",freeTravelSpeed,std::lround(40.0*machineInterface->ui->yStep->displayText().toFloat()),0x0d,0x1a);
    }

    if (command == "NewXYZHomePosition")
    {
        double dx = machineInterface->ui->x_lcdNumber->value();
        double dy = machineInterface->ui->y_lcdNumber->value();
        double dz = machineInterface->ui->z_lcdNumber->value();

        double hdx = machineInterface->ui->xHome->value();
        double hdy = machineInterface->ui->yHome->value();
        double hdz = machineInterface->ui->zHome->value();

        machineInterface->ui->xHome->display(dx+hdx);
        machineInterface->ui->yHome->display(dy+hdy);
        machineInterface->ui->zHome->display(dz+hdz);

        machineInterface->ui->x_lcdNumber->display(0.000);
        machineInterface->ui->y_lcdNumber->display(0.000);
        machineInterface->ui->z_lcdNumber->display(0.000);

        sprintf(lastHpglCommand,"@NH;%c%c",0x0d,0x1a);

        if (machineInterface->wait_NewHomePosition)
            emit finished_NewHomePosition();
    }
    else if (command == "NewXYHomePosition")
    {
        double dx = machineInterface->ui->x_lcdNumber->value();
        double dy = machineInterface->ui->y_lcdNumber->value();

        double hdx = machineInterface->ui->xHome->value();
        double hdy = machineInterface->ui->yHome->value();

        machineInterface->ui->xHome->display(dx+hdx);
        machineInterface->ui->yHome->display(dy+hdy);

        machineInterface->ui->x_lcdNumber->display(0.000);
        machineInterface->ui->y_lcdNumber->display(0.000);

        sprintf(lastHpglCommand,"@NO;%c%c",0x0d,0x1a);

        if (machineInterface->wait_NewHomePosition)
            emit finished_NewHomePosition();
    }
    else if (command == "NewZHomePosition")
    {
        double dz = machineInterface->ui->z_lcdNumber->value();
        double hdz = machineInterface->ui->zHome->value();

        machineInterface->ui->zHome->display(dz+hdz);

        machineInterface->ui->z_lcdNumber->display(0.000);

        sprintf(lastHpglCommand,"@NZ;%c%c",0x0d,0x1a);

        if (machineInterface->wait_NewHomePosition)
            emit finished_NewHomePosition();
    }


    if (lastHpglCommand[0] != 0)
    {
        CSPXMessage msg;
        msg.SetData(lastHpglCommand);
        SendData(msg);
    }
    else
    {
        machineInterface->ui->statusMessage->setPlainText(command + ": " + errorMsg);

    }


    if (command == "Reset")
        machineInterface->GetController()->disconnectComms();

    return true;
}

void CSPXRobbyController::SetLocalMode()
{
    if (mode == 1)
    {
        const char mdq_data[] = { 0x1b, 'L', 'O', ':', 0x0d, 0x1a };

        SetMode(-1);
        CSPXMessage msg;
        msg.SetData(mdq_data,6);
        SendData(msg);

    }
    else if (mode == 2)
    {
        const char dcc_data[] = { 0x1b, '.', 'Z', 0x0d,0x1a };

        CSPXMessage msg;
        msg.SetData(dcc_data,5);
        SendData(msg);

    }
    else if (mode == 3)
    {
        //cr = 0x0d lf=0x0a sub = 0x1a => <eof>???
        const char dcc_data[] = { 0x1b, '.', ')', 0x1a, 0x0d };

        CSPXMessage msg;
        msg.SetData(dcc_data,5);
        SendData(msg);

    }

    const char mdq_data[] = { 0x1b, 'M', 'D', ':', 0x0d,0x1a};

    SetMode(-1);
    CSPXMessage msg;
    msg.SetData(mdq_data,6);
    SendData(msg);

}

void CSPXRobbyController::SetHpglLineMode()
{
    if (mode == 0 || mode == 1) //Local/DCC->Line
    {
        const char lm_data[] = { 0x1b, 'L', 'M', ':', 0x0d,0x1a };

        CSPXMessage msg;
        msg.SetData(lm_data,6);
        SendData(msg);
    }
    else
    {
        machineInterface->ui->statusMessage->setPlainText(QString("Only via Local Mode 0"));
    }

    const char mdq_data[] = { 0x1b, 'M', 'D', ':', 0x0d,0x1a};
    SetMode(-1);

    CSPXMessage msg;
    msg.SetData(mdq_data,6);
    SendData(msg);
}

void CSPXRobbyController::SetHpglFileMode()
{
    if (mode == 0 || mode == 1)
    {
        const char fm_data[] = { 0x1b, 'F', 'M', ':', 0x0d,0x1a};

        CSPXMessage msg;
        msg.SetData(fm_data,6);
        SendData(msg);
    }
    else
    {
        machineInterface->ui->statusMessage->setPlainText(QString("Only via Local Mode 0"));
    }

}

void CSPXRobbyController::SetDCCMode()
{
    if (mode == 0)
    {
        const char mdq_data[] = { 0x1b, 'L', 'I', ':', 0x0d,0x1a};
        SetMode(-1);

        CSPXMessage msg;
        msg.SetData(mdq_data,6);
        SendData(msg);
    }
    const char mdq_data[] = { 0x1b, 'M', 'D', ':', 0x0d,0x1a };
    SetMode(-1);

    CSPXMessage msg;
    msg.SetData(mdq_data,6);
    SendData(msg);
}


bool CSPXRobbyController::sendRobbyFile(int fileIndex)
{
    CSPXHpglCommands *commands = machineInterface->hpglCommands;

    int xhome = std::lround(machineInterface->ui->xHome->value()*40.);
    int yhome = std::lround(machineInterface->ui->yHome->value()*40.);
    int zhome = std::lround(machineInterface->ui->zHome->value()*40.);

    //validate negative values
    if (commands->x_min+xhome < 0 || commands->y_min+yhome < 0 /*|| commands->z_min+zhome < 0*/)
    {
        machineInterface->ui->statusMessage->setPlainText(QString("workpiece is still out-of-bounds with respect to current (x,y) home position"
                                                               "Set new home position and move workpice to it."));
        machineInterface->abort = true;
        resetMachine();
        return false;
    }
    else
    {
        QByteArray *file = commands->GetByteArray(fileIndex);

        file->append(0x1b); //<ESC>
        file->append(0x4d); //M
        file->append(0x44); //D
        file->append(0x3A); //:
        file->append(0x1A); //<SUB>
        file->append(0x0D); //<CR>
        if (((CSPXCOMPort*)coms)->pinoutSignals() & QSerialPort::ClearToSendSignal)
            ((CSPXCOMPort*)coms)->SendData(file->data(),file->size());

        if (machineInterface->ui->radioSerialButton->isChecked())
            ((CSPXCOMPort*)coms)->waitForReadyRead(300);
    }

    return true;
}

bool CSPXRobbyController::sendRobbyDCCCommand(CSPXString command)
{
    char lastHpglCommand[128];
    memset(lastHpglCommand,0,128);

    if (command == "Reset System")
        sprintf(lastHpglCommand,"%cRS:%c", 0x1b, 0x0d );
    else if (command == "Acc/Dec Params")
        sprintf(lastHpglCommand,"%cTB:%c", 0x1b, 0x0d );
    else if (command == "Restore Default")
        sprintf(lastHpglCommand,"%c:%c", 0x1b, 0x0d );
    else if (command == "DRAM Buffer")
        sprintf(lastHpglCommand,"%cBT:%c", 0x1b, 0x0d );
    else if (command == "Step Config")
        sprintf(lastHpglCommand,"%cST:%c", 0x1b, 0x0d );
    else if (command == "System Config")
        sprintf(lastHpglCommand,"%cSC:%c", 0x1b, 0x0d );

    CSPXMessage msg;
    msg.SetData(lastHpglCommand);
    SendData(msg);

    return true;
}

bool CSPXRobbyController::sendRobbyModeCommand()
{
    const char mdq_data[] = { 0x1b, 'M', 'D', ':', 0x0d,0x1a };

    SetMode(-1);
    CSPXMessage msg;
    msg.SetData(mdq_data,6);
    SendData(msg);

    return true;

}

void CSPXRobbyController::abortProcess()
{
    machineInterface->ui->sendFileButton->setEnabled(true);

    if (machineInterface->workerThread) {
        if (machineInterface->workerThread->isRunning())
            machineInterface->workerThread->terminate();
    }
}

void CSPXRobbyController::checkResponse(QString serialResponse)
{
    response += serialResponse.toStdString().c_str();
    machineInterface->ui->statusMessage->setPlainText(serialResponse);

    if (strstr((char*)response,"\r"))
    {
        machineInterface->ui->statusMessage->setPlainText(QString("Serial Port Resonse : " + QString(machineInterface->GetController()->response)));

        float x = 0., y = 0., z = 0.;
        char *pZcoords = nullptr;
        char *pXcoords = nullptr, *pYcoords = nullptr;
        char *pmode = nullptr;

        //current values
        x = machineInterface->ui->x_lcdNumber->value();
        y = machineInterface->ui->y_lcdNumber->value();
        z = machineInterface->ui->z_lcdNumber->value();

        pXcoords = strstr((char*)response,"X:");
        if (pXcoords)
        {
            sscanf_s(pXcoords,"X: %f;Y: %f;Z: %f;",&x,&y,&z);
            machineInterface->ui->x_lcdNumber->display(x);
            machineInterface->ui->y_lcdNumber->display(y);
            machineInterface->ui->z_lcdNumber->display(z);
        }
        pYcoords =  strstr((char*)response,"Y:");
        if (pYcoords)
        {
            sscanf_s(pYcoords,"Y: %f;Z: %f;",&y,&z);
            machineInterface->ui->y_lcdNumber->display(y);
            machineInterface->ui->z_lcdNumber->display(z);
        }

        if (pXcoords && pYcoords) {
            if (machineInterface->xSpindleLine)
                machineInterface->scene->removeItem(machineInterface->xSpindleLine);
            if (machineInterface->ySpindleLine)
                machineInterface->scene->removeItem(machineInterface->ySpindleLine);
	
            machineInterface->xSpindleLine = machineInterface->scene->addLine(std::lround(x/2.)-5, std::lround(200.-y/2.), std::lround(x/2.)+5, std::lround(200.-y/2.), whitePen);
            machineInterface->xSpindleLine->setZValue(1);
            machineInterface->ySpindleLine = machineInterface->scene->addLine(std::lround(x/2.), std::lround(200.-y/2.)-5, std::lround(x/2.), std::lround(200.-y/2.)+5, whitePen);
            machineInterface->ySpindleLine->setZValue(1);
        }

        pZcoords =  strstr((char*)response,"Z:");
        if (pZcoords)
        {
            sscanf_s(pZcoords,"Z: %f;",&z);
            machineInterface->ui->z_lcdNumber->display(z);
        }

        pmode =  strstr((char*)response,"MD:");
        if (pmode)
        {
            int mod;
            if (sscanf_s(pmode,"MD:%d;",&mod) == 1)
                SetMode(mod);
            switch (GetMode())
            {
                case 0:
                {
                    machineInterface->ui->localMode->setChecked(true);
                    machineInterface->ui->lineMode->setEnabled(true);
                    machineInterface->ui->fileMode->setEnabled(true);
                    machineInterface->ui->dccMode->setEnabled(true);
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 1:
                {
                    machineInterface->ui->localMode->setEnabled(true);
                    machineInterface->ui->dccMode->setChecked(true);
                    machineInterface->ui->lineMode->setEnabled(false);
                    machineInterface->ui->fileMode->setEnabled(false);
                    //dccModeCtrl->ChangeGroupCheck();
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 2:
                {
                    machineInterface->ui->localMode->setEnabled(true);
                    machineInterface->ui->dccMode->setEnabled(false);
                    machineInterface->ui->lineMode->setChecked(true);
                    machineInterface->ui->fileMode->setEnabled(false);
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 3:
                {
                    machineInterface->ui->localMode->setEnabled(true);
                    machineInterface->ui->dccMode->setEnabled(false);
                    machineInterface->ui->lineMode->setChecked(false);
                    machineInterface->ui->fileMode->setChecked(true);
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->green));
                }
                break;
                case 4: //busy
                {
                    machineInterface->ui->label_Busy_Status->setPixmap(*(machineInterface->red));
                }
            }
        }

        pmode = strstr((char*)response,"FM:");
        if (pmode)
        {
            //MD: recheck the new current profiler mode
            const char fmdata[] = { 0x1b, 'M', 'D', ':', 0x0d, 0x1a};
            CSPXMessage msg;
            msg.SetData(fmdata,6);
            SendData(msg);
        }

        pmode = strstr((char*)response,"UC:");
        if (pmode)
        {
            CSPXMessage msg;
            msg.SetData("OE;");
            SendData(msg);

            emit resetMachine();

        }

        response = "";
    }

    emit changeSpindleCoordinates(machineInterface->ui->xHome->value()+machineInterface->ui->x_lcdNumber->value(), machineInterface->ui->yHome->value() + machineInterface->ui->y_lcdNumber->value(), machineInterface->ui->zHome->value()+machineInterface->ui->z_lcdNumber->value());

}

void CSPXRobbyController::SendData(CSPXMessage &msg)
{   
    if (machineInterface->ui->radioSerialButton->isChecked()) {
        if (((CSPXCOMPort*)coms)->pinoutSignals() & QSerialPort::ClearToSendSignal)
        {
            ((CSPXCOMPort*)coms)->SendData(&msg);
            machineInterface->ui->statusMessage->setPlainText(QString(msg.GetData()));
        }
        else
            machineInterface->ui->statusMessage->setPlainText(QString("CTS not asserted"));


        ((CSPXCOMPort*)GetComs())->waitForBytesWritten(300);
    }
}

void SerialReceiverThread::pause()
{
    sync.lock();
    _pause = true;
    sync.unlock();
}

void SerialReceiverThread::resume()
{
    sync.lock();
    _pause = false;
    sync.unlock();

    pauseCond.wakeAll();
}
