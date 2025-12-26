#include <QFileDialog>
#include <QMessageBox>

#include "cspxrobbycontroller.h"
#include "../MainWindow.h"
#include "../ui_mainwindow.h"
#include "cspxhpglcommands.h"

#include <QtSerialPort/QSerialPort>
#include <QGraphicsLineItem>

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
    MainWindow* machineDialog = CSPXRobbyController::GetMachineDialog();
    CSPXRobbyController *ctrl = machineDialog->GetController();

    //machineDialog->ui->serialComboBox->currentText().toStdString().c_str();

    CSPXMessage rec_msg;
    ctrl->GetComs()->ReceiveData(&rec_msg);

    if (rec_msg.GetData())
        emit haveResponse(rec_msg.GetData());
}

void CSPXRobbyController::readData()
{
    CSPXRobbyController *ctrl = machineDialog->GetController();

    //machineDialog->ui->serialComboBox->currentText().toStdString().c_str();

    CSPXMessage rec_msg;
    ctrl->GetComs()->ReceiveData(&rec_msg);

    machineDialog->GetController()->response += rec_msg.GetData();

    if (strstr((char*)machineDialog->GetController()->response,"\r"))
    {
        machineDialog->ui->statusMessage->setPlainText(QString(machineDialog->GetController()->response));

        float x,y,z;
        char *pcoords = nullptr;
        char *pmode = nullptr;

        //if (lastHpglCommand[0] != 'O' && lastHpglCommand[1] != 'F')
        {
            pcoords = strstr((char*)machineDialog->GetController()->response,"X:");
            if (pcoords)
            {
                sscanf(pcoords,"X: %f;Y: %f;Z: %f;",&x,&y,&z);
                machineDialog->ui->x_lcdNumber->display(x);
                machineDialog->ui->y_lcdNumber->display(y);
                machineDialog->ui->z_lcdNumber->display(z);
            }
            pcoords =  strstr((char*)machineDialog->GetController()->response,"Y:");
            if (pcoords)
            {
                sscanf(pcoords,"Y: %f;Z: %f;",&y,&z);
                machineDialog->ui->y_lcdNumber->display(y);
                machineDialog->ui->z_lcdNumber->display(z);
            }
            pcoords =  strstr((char*)machineDialog->GetController()->response,"Z:");
            if (pcoords)
            {
                sscanf(pcoords,"Z: %f;",&z);
                machineDialog->ui->z_lcdNumber->display(z);
            }
        }

        pmode =  strstr((char*)machineDialog->GetController()->response,"MD:");
        if (pmode)
        {
            char mod;
            sscanf(pmode,"MD:%d;",&mod);
            machineDialog->GetController()->SetMode(mod);
            switch (machineDialog->GetController()->GetMode())
            {
                case 0:
                {
                    machineDialog->ui->localMode->setChecked(true);
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 1:
                {
                    machineDialog->ui->dccMode->setChecked(true);
                    //dccModeCtrl->ChangeGroupCheck();
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 2:
                {
                    machineDialog->ui->lineMode->setChecked(true);
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 3:
                {
                    machineDialog->ui->fileMode->setChecked(true);
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 4: //busy
                {
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->red));
                }
            }
        }

        pmode = strstr((char*)rec_msg.GetData(),"FM:");
        if (pmode)
        {
            //MD: recheck the new current profiler mode
            const char fmdata[] = { 0x1b, 'M', 'D', ':', 0x0d, 0x1a};
            machineDialog->GetController()->GetComs()->write(fmdata,6);

            machineDialog->GetController()->GetComs()->waitForBytesWritten(30000);
        }

        pmode = strstr((char*)rec_msg.GetData(),"UC:");
        if (pmode)
        {
            CSPXMessage msg;
            msg.SetData("OE;");
            SendData(msg);
        }

        machineDialog->GetController()->response = "";
    }

    emit changeSpindleCoordinates(machineDialog->ui->xHome->value()+machineDialog->ui->x_lcdNumber->value(), machineDialog->ui->yHome->value() + machineDialog->ui->y_lcdNumber->value(), machineDialog->ui->zHome->value()+machineDialog->ui->z_lcdNumber->value());
}

void CSPXRobbyController::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        MainWindow* dlg = CSPXRobbyController::GetMachineDialog();
        dlg->ui->statusMessage->setPlainText(QString("Serial Port error : " + QString::number(error)));
    }
}

MainWindow * CSPXRobbyController::machineDialog = nullptr;

CSPXRobbyController::CSPXRobbyController(QObject* parent) : QObject(parent)
  ,connected(false)
  ,receiverThread(nullptr)
{
}

CSPXRobbyController::~CSPXRobbyController()
{
    if (receiverThread != nullptr)
        receiverThread->terminate();
}

//the generic RS232Comms
//this is a slot when connect to robby controller is triggered from the dialog
bool CSPXRobbyController::connectRS232Comms()
{

    //default values for now
    MainWindow* dlg = CSPXRobbyController::GetMachineDialog();

    QString port = machineDialog->ui->serialComboBox->currentText();
    if (port.isEmpty())
        return false;

    //create device
    coms = new CSPXCOMPort(port.toStdString().c_str());

    receiverThread = new SerialReceiverThread();
    receiverThread->controller = this;
    connect(receiverThread, &SerialReceiverThread::haveResponse, this, &CSPXRobbyController::checkResponse);

    connect(coms, &QSerialPort::errorOccurred, this, &CSPXRobbyController::handleError);
    //connect(coms, &QSerialPort::readyRead, this, &CSPXRobbyController::readData);
    connect(coms,  &QSerialPort::readyRead, receiverThread, &SerialReceiverThread::readData);
    connect(this,  &CSPXRobbyController::resetMachine, this, &CSPXRobbyController::abortProcess);

    coms->SetPortId(port.toStdString().c_str());
    coms->SetBaudRate(38400);
    coms->SetParity(0); //no parity
    coms->SetStopbits(1);
    coms->SetCharacters(8);
    coms->SetFlowControl(2); //0:No flow control, 1, software, 2=hardware (RTS-CTS)

    if (!coms->open(QIODevice::ReadWrite))
    {
        machineDialog->ui->statusMessage->setPlainText(QString("Serial Port error : Not Connected"));
        return connected = false;
    }
    coms->setDataTerminalReady(true);

    connected = true;

    receiverThread->start();

    //start timer to check busy (CTS flag) every 500ms
    connect(&busyTimer,&QTimer::timeout,this,&CSPXRobbyController::checkBusyState);
    busyTimer.setSingleShot(true);
    busyTimer.setInterval(500);
    busyTimer.start();

    //query mode
    const char mdq_data[] = { 0x1b, 'M', 'D', ':', 0x0d, 0x1a };

    SetMode(-1);
    CSPXMessage msg;
    msg.SetData(mdq_data,6);
    SendData(msg);

    //change to local mode
    machineDialog->ui->localMode->clicked(true);
    if (GetMode()==0)
        //change to line mode
        machineDialog->ui->lineMode->clicked(true);

    if (GetMode()==2)
    {
        CSPXString cmd("MoveXYZHome");
        sendRobbyHpglLineCommand(cmd);
     }

    return connected;
}

void CSPXRobbyController::checkBusyState()
{
    MainWindow* dlg = CSPXRobbyController::GetMachineDialog();

    bool CTS_Status = GetComs()->pinoutSignals() & QSerialPort::ClearToSendSignal;
    if (CTS_Status)
        dlg->ui->label_CTS_Status->setPixmap(*(dlg->green));
    else
        dlg->ui->label_CTS_Status->setPixmap(*(dlg->red));

    busyTimer.start();

}

void CSPXRobbyController::disconnectRS232Comms()
{
    busyTimer.stop();

    coms->close();
    MainWindow* dlg = CSPXRobbyController::GetMachineDialog();
    dlg->Initialize();

    connected = false;
}


bool CSPXRobbyController::sendRobbyHpglLineCommand(CSPXString command)
{
    memset(lastHpglCommand,0,128);

    MainWindow* dlg = CSPXRobbyController::GetMachineDialog();

    double freeTravelSpeed = machineDialog->ui->travelSpeedSpinBox->value();

    if (dlg->GetController()->GetMode() != 2)
    {
        machineDialog->ui->statusMessage->setPlainText(QString("Not in Hpgl Line Mode !"));
        return false;
    }

    int z = std::round(40.0*machineDialog->ui->z_lcdNumber->value());
    int zHome = std::round(40.0*machineDialog->ui->zHome->value());
    int zTrack = std::round(40.0*machineDialog->ui->zSafetyStep->text().toFloat());

    if (zTrack > zHome + z)
        zTrack = zHome + z;
    if (zTrack < 0)
        zTrack = 0;

    QString errorMsg("out-of-bounds movement (300.0 x 400.0).");

    if (command == "Reset")
    {
        disconnect(coms, &QSerialPort::readyRead, this, &CSPXRobbyController::readData);
        sprintf(lastHpglCommand,"@XX;%c%c",0x0d,0x1a);
    }
    else if (command == "Get Coordinates")
    {
        sprintf(lastHpglCommand,"OA;%c%c",0x0d,0x1a);
    }
    else if (command == "MoveXY")
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PA %d,%d;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->xSpindlePos),std::round(40.0*machineDialog->ySpindlePos),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PA %d,%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->xSpindlePos),std::round(40.0*machineDialog->ySpindlePos),0x0d,0x1a);
    }
    else if (command == "MoveXYZHome")
    {
        sprintf(lastHpglCommand,"@HM;OA;%c%c",0x0d,0x1a);
        machineDialog->ui->xHome->display(0.000);
        machineDialog->ui->yHome->display(0.000);
        machineDialog->ui->zHome->display(0.000);
    }
    else if (command == "MoveZHome")
    {
        sprintf(lastHpglCommand,"@HZ;OA;%c%c",0x0d,0x1a);
        machineDialog->ui->zHome->display(0.000);
    }
    else if (command == "MoveLeftUp" && machineDialog->ui->x_lcdNumber->value() + machineDialog->ui->xHome->value() > machineDialog->ui->xStep->text().toFloat() && machineDialog->ui->y_lcdNumber->value() + machineDialog->ui->yHome->value() < 400.0 - machineDialog->ui->yStep->text().toFloat())
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR -%d,%d;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR -%d,%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveLeftDown" && machineDialog->ui->x_lcdNumber->value() + machineDialog->ui->xHome->value() > machineDialog->ui->xStep->text().toFloat() && machineDialog->ui->y_lcdNumber->value() + machineDialog->ui->yHome->value() > machineDialog->ui->yStep->text().toFloat())
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR -%d,-%d;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR -%d,-%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveRightUp" && machineDialog->ui->x_lcdNumber->value() + machineDialog->ui->xHome->value() + machineDialog->ui->xStep->text().toFloat() < 300. && machineDialog->ui->y_lcdNumber->value() + machineDialog->ui->yHome->value() + machineDialog->ui->yStep->text().toFloat() < 400.)
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR %d,%d;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR %d,%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveRightDown" && machineDialog->ui->x_lcdNumber->value() + machineDialog->ui->xHome->value() > machineDialog->ui->xStep->text().toFloat() && machineDialog->ui->y_lcdNumber->value() + machineDialog->ui->yHome->value() > machineDialog->ui->yStep->text().toFloat())
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR %d,-%d;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR %d,-%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),std::round(40.0*machineDialog->ui->yStep->text().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveZUp" && machineDialog->ui->z_lcdNumber->value() > -machineDialog->ui->zHome->value())
        sprintf(lastHpglCommand,"@ZR -%d;OA;%c%c",std::round(40.0*machineDialog->ui->zStep->text().toFloat()),0x0d,0x1a);
    else if (command == "MoveZDown" && machineDialog->ui->z_lcdNumber->value() < 90-machineDialog->ui->zHome->value())
        sprintf(lastHpglCommand,"@ZR %d;OA;%c%c",std::round(40.0*machineDialog->ui->zStep->text().toFloat()),0x0d,0x1a);
    else if (command == "MoveLeft" && machineDialog->ui->x_lcdNumber->value() + machineDialog->ui->xHome->value() >= machineDialog->ui->xStep->text().toFloat())
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR -%d,0;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR -%d,0;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->xStep->displayText().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveDown" && machineDialog->ui->y_lcdNumber->value() + machineDialog->ui->yHome->value() >= machineDialog->ui->yStep->text().toFloat())
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR 0,-%d;@ZR %d;OA;%c%c",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR 0,-%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->yStep->displayText().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveRight" && machineDialog->ui->x_lcdNumber->value() + machineDialog->ui->xHome->value() + machineDialog->ui->xStep->text().toFloat() <= 300.0)
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR %d,0;@ZR %d;OA;%c%c",zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR %d,0;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->xStep->displayText().toFloat()),0x0d,0x1a);
    }
    else if (command == "MoveUp" && machineDialog->ui->y_lcdNumber->value() + machineDialog->ui->yHome->value() + machineDialog->ui->yStep->text().toFloat() <= 400.0)
    {
        if (machineDialog->ui->toolSafetyCheckBox->isChecked())
            sprintf(lastHpglCommand,"VS %2.1f;@ZR -%d;PR 0,%d;@ZR %dOA;%c%c;",freeTravelSpeed,zTrack,std::round(40.0*machineDialog->ui->xStep->text().toFloat()),zTrack,0x0d,0x1a);
        else
            sprintf(lastHpglCommand,"VS %2.1f;PR 0,%d;OA;%c%c",freeTravelSpeed,std::round(40.0*machineDialog->ui->yStep->displayText().toFloat()),0x0d,0x1a);
    }

    if (command == "NewXYZHomePosition")
    {
        double dx = machineDialog->ui->x_lcdNumber->value();
        double dy = machineDialog->ui->y_lcdNumber->value();
        double dz = machineDialog->ui->z_lcdNumber->value();

        double hdx = machineDialog->ui->xHome->value();
        double hdy = machineDialog->ui->yHome->value();
        double hdz = machineDialog->ui->zHome->value();

        machineDialog->ui->xHome->display(dx+hdx);
        machineDialog->ui->yHome->display(dy+hdy);
        machineDialog->ui->zHome->display(dz+hdz);

        machineDialog->ui->x_lcdNumber->display(0.000);
        machineDialog->ui->y_lcdNumber->display(0.000);
        machineDialog->ui->z_lcdNumber->display(0.000);

        sprintf(lastHpglCommand,"@NH;%c%c",0x0d,0x1a);

        if (machineDialog->wait_NewHomePosition)
            emit finished_NewHomePosition();
    }
    else if (command == "NewXYHomePosition")
    {
        double dx = machineDialog->ui->x_lcdNumber->value();
        double dy = machineDialog->ui->y_lcdNumber->value();

        double hdx = machineDialog->ui->xHome->value();
        double hdy = machineDialog->ui->yHome->value();

        machineDialog->ui->xHome->display(dx+hdx);
        machineDialog->ui->yHome->display(dy+hdy);

        machineDialog->ui->x_lcdNumber->display(0.000);
        machineDialog->ui->y_lcdNumber->display(0.000);

        sprintf(lastHpglCommand,"@NO;%c%c",0x0d,0x1a);

        if (machineDialog->wait_NewHomePosition)
            emit finished_NewHomePosition();
    }
    else if (command == "NewZHomePosition")
    {
        double dz = machineDialog->ui->z_lcdNumber->value();
        double hdz = machineDialog->ui->zHome->value();

        machineDialog->ui->zHome->display(dz+hdz);

        machineDialog->ui->z_lcdNumber->display(0.000);

        sprintf(lastHpglCommand,"@NZ;%c%c",0x0d,0x1a);

        if (machineDialog->wait_NewHomePosition)
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
        MainWindow* dlg = CSPXRobbyController::GetMachineDialog();
        dlg->ui->statusMessage->setPlainText(command + ": " + errorMsg);

    }


    if (command == "Reset")
        machineDialog->GetController()->disconnectRS232Comms();

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
        machineDialog->ui->statusMessage->setPlainText(QString("Only via Local Mode 0"));
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
        machineDialog->ui->statusMessage->setPlainText(QString("Only via Local Mode 0"));
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
    CSPXHpglCommands *commands = machineDialog->hpglCommands;

    int xhome = std::round(machineDialog->ui->xHome->value()*40.);
    int yhome = std::round(machineDialog->ui->yHome->value()*40.);
    int zhome = std::round(machineDialog->ui->zHome->value()*40.);

    //validate negative values
    if (commands->x_min+xhome < 0 || commands->y_min+yhome < 0 /*|| commands->z_min+zhome < 0*/)
    {
        machineDialog->ui->statusMessage->setPlainText(QString("workpiece is still out-of-bounds with respect to current (x,y) home position"
                                                               "Set new home position and move workpice to it."));
        machineDialog->abort = true;
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
        if (coms->pinoutSignals() & QSerialPort::ClearToSendSignal)
            coms->SendData(file->data(),file->size());

        coms->waitForReadyRead(30000);
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
    MainWindow* machineDialog = CSPXRobbyController::GetMachineDialog();
    machineDialog->ui->sendFileButton->setEnabled(true);

    if (machineDialog->workerThread) {
        if (machineDialog->workerThread->isRunning())
            machineDialog->workerThread->terminate();
    }
}

void CSPXRobbyController::checkResponse(QString serialResponse)
{
    MainWindow* machineDialog = CSPXRobbyController::GetMachineDialog();

    response += serialResponse.toStdString().c_str();
    machineDialog->ui->statusMessage->setPlainText(serialResponse);

    if (strstr((char*)response,"\r"))
    {
        machineDialog->ui->statusMessage->setPlainText(QString("Serial Port Resonse : " + QString(machineDialog->GetController()->response)));

        float x,y,z;
        char *pcoords = nullptr;
        char *pmode = nullptr;

        pcoords = strstr((char*)response,"X:");
        if (pcoords)
        {
            sscanf(pcoords,"X: %f;Y: %f;Z: %f;",&x,&y,&z);
            machineDialog->ui->x_lcdNumber->display(x);
            machineDialog->ui->y_lcdNumber->display(y);
            machineDialog->ui->z_lcdNumber->display(z);
        }
        pcoords =  strstr((char*)response,"Y:");
        if (pcoords)
        {
            sscanf(pcoords,"Y: %f;Z: %f;",&y,&z);
            machineDialog->ui->y_lcdNumber->display(y);
            machineDialog->ui->z_lcdNumber->display(z);
        }

        QPen pen(Qt::black, 1, Qt::SolidLine);
        pen.setCosmetic(true);

        if (machineDialog->xSpindleLine)
            machineDialog->scene->removeItem(machineDialog->xSpindleLine);
        if (machineDialog->ySpindleLine)
            machineDialog->scene->removeItem(machineDialog->ySpindleLine);

        machineDialog->xSpindleLine = machineDialog->scene->addLine(std::round(x/2.)-5, std::round(200.-y/2.), std::round(x/2.)+5, std::round(200.-y/2.), pen);
        machineDialog->xSpindleLine->setZValue(1);
        machineDialog->ySpindleLine = machineDialog->scene->addLine(std::round(x/2.), std::round(200.-y/2.)-5, std::round(x/2.), std::round(200.-y/2.)+5, pen);
        machineDialog->ySpindleLine->setZValue(1);

        pcoords =  strstr((char*)response,"Z:");
        if (pcoords)
        {
            sscanf(pcoords,"Z: %f;",&z);
            machineDialog->ui->z_lcdNumber->display(z);
        }

        pmode =  strstr((char*)response,"MD:");
        if (pmode)
        {
            char mod;
            sscanf(pmode,"MD:%d;",&mod);
            SetMode(mod);
            switch (GetMode())
            {
                case 0:
                {
                    machineDialog->ui->localMode->setChecked(true);
                    machineDialog->ui->lineMode->setEnabled(true);
                    machineDialog->ui->fileMode->setEnabled(true);
                    machineDialog->ui->dccMode->setEnabled(true);
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 1:
                {
                    machineDialog->ui->localMode->setEnabled(true);
                    machineDialog->ui->dccMode->setChecked(true);
                    machineDialog->ui->lineMode->setEnabled(false);
                    machineDialog->ui->fileMode->setEnabled(false);
                    //dccModeCtrl->ChangeGroupCheck();
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 2:
                {
                    machineDialog->ui->localMode->setEnabled(true);
                    machineDialog->ui->dccMode->setEnabled(false);
                    machineDialog->ui->lineMode->setChecked(true);
                    machineDialog->ui->fileMode->setEnabled(false);
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 3:
                {
                    machineDialog->ui->localMode->setEnabled(true);
                    machineDialog->ui->dccMode->setEnabled(false);
                    machineDialog->ui->lineMode->setChecked(false);
                    machineDialog->ui->fileMode->setChecked(true);
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->green));
                }
                break;
                case 4: //busy
                {
                    machineDialog->ui->label_Busy_Status->setPixmap(*(machineDialog->red));
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

    emit changeSpindleCoordinates(machineDialog->ui->xHome->value()+machineDialog->ui->x_lcdNumber->value(), machineDialog->ui->yHome->value() + machineDialog->ui->y_lcdNumber->value(), machineDialog->ui->zHome->value()+machineDialog->ui->z_lcdNumber->value());

}

void CSPXRobbyController::SendData(CSPXMessage &msg)
{

    if (coms->pinoutSignals() & QSerialPort::ClearToSendSignal)
    {
        coms->SendData(&msg);
        machineDialog->ui->statusMessage->setPlainText(QString(msg.GetData()));
    }
    else
        machineDialog->ui->statusMessage->setPlainText(QString("CTS not asserted"));


    GetComs()->waitForBytesWritten(30000);
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
