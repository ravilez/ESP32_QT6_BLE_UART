#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "CNC/cspxcheckboxdelegate.h"
#include "Containers/cspxvector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    green = new QPixmap("Images/greenLEDoff.png");
    red = new QPixmap("Images/redLEDoff.png");

    Initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {

    auto reply = QMessageBox::question(
        this,
        "Confirm Exit",
        "Are you sure you want to exit?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {

        robbyController.disconnectRS232Comms();

        event->accept();
    } else {
        event->ignore();
    }
}


void MainWindow::Initialize()
{
    robbyController.SetMachineDialog(this);

    ui->toolSafetyCheckBox->setChecked(false);
    ui->zSafetyStep->setText("40.0");

    robbyController.SetMode(-1);
    ui->localMode->setChecked(true);

    ui->localMode->setEnabled(false);
    ui->lineMode->setEnabled(false);
    ui->fileMode->setEnabled(false);
    ui->dccMode->setEnabled(false);

    ui->connectButton->setText(QString("Connect"));
    ui->sendFileButton->setEnabled(false);
    ui->homeButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->upButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->zupButton->setEnabled(false);
    ui->zdownButton->setEnabled(false);
    ui->homeButton->setEnabled(false);
    ui->zhomeButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    ui->leftupButton->setEnabled(false);
    ui->leftdownButton->setEnabled(false);
    ui->rightupButton->setEnabled(false);
    ui->rightdownButton->setEnabled(false);

    QPixmap newHome("Images/Images20x20/newhome.jpg");
    QPixmap newHomexy("Images/Images20x20/newhomexy.jpg");
    QPixmap newHomez("Images/Images20x20/newhomez.jpg");

    ui->setXYZHomeButton->setEnabled(false);
    ui->setXYZHomeButton->setToolTip("Set new xyz home position");

    ui->setXYHomeButton->setEnabled(false);
    ui->setXYHomeButton->setToolTip("Set new xy home position");

    ui->setZHomeButton->setEnabled(false);
    ui->setZHomeButton->setToolTip("Set new z home position");

    ui->moveWorkpieceToXYHomePositionButton->setEnabled(false);

    last_zHome = 0.;
    last_xHome = 0.;
    last_yHome = 0.;

    ui->xHome->setEnabled(false);
    ui->yHome->setEnabled(false);
    ui->zHome->setEnabled(false);

    ui->xStep->setEnabled(false);
    ui->yStep->setEnabled(false);
    ui->zStep->setEnabled(false);

    ui->xStep->setText("10.000");
    ui->yStep->setText("10.000");
    ui->zStep->setText("10.000");

    ui->xStep->setEnabled(false);
    ui->yStep->setEnabled(false);
    ui->zStep->setEnabled(false);

    ui->serialComboBox->clear();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {

        qDebug() << "\nPort:" << serialPortInfo.portName();
        ui->serialComboBox->addItem(serialPortInfo.portName());
    }

    ui->label_CTS_Status->setPixmap(*red);
    ui->label_Busy_Status->setPixmap(*red);



    // Dialog with image and grid
    ui->workspace->setFixedSize(151, 201); // FIXED VIEW SIZE
    scene = new QGraphicsScene(this);
    ui->workspace->setScene(scene);

    ui->workspace->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->workspace->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Optional: remove margins so the scene fits exactly
    ui->workspace->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // (Load image)
    QPixmap pixmap(151,201);

    // Add image to scene
    QGraphicsPixmapItem *imageItem = scene->addPixmap(pixmap);
    imageItem->setZValue(0);

    // Draw grid over image
    drawGrid(pixmap.size(), 10); // 20px spacing


    ui->workspace->setEnabled(false);

    QPen pen(Qt::red, 1, Qt::SolidLine);
    pen.setCosmetic(true);
    xSpindleLine = scene->addLine(-5, 0, +5, 0, pen);
    xSpindleLine->setZValue(1);
    ySpindleLine = scene->addLine(0, -5, 0, +5, pen);
    ySpindleLine->setZValue(1);

    // Connect click signal
    connect(ui->workspace, &CSPXGraphicsView::clicked, this, &MainWindow::onImageClicked);

}

void MainWindow::on_connectButton_clicked()
{
    if (robbyController.isConnected())
    {
        ui->connectButton->setText(QString("Connect"));
        robbyController.disconnectRS232Comms();
        ui->moveWorkpieceToXYHomePositionButton->setEnabled(false);

        robbyController.SetMode(-1);
        ui->localMode->setChecked(true);

        ui->localMode->setEnabled(false);
        ui->lineMode->setEnabled(false);
        ui->fileMode->setEnabled(false);
        ui->dccMode->setEnabled(false);

        ui->sendFileButton->setEnabled(false);
        ui->homeButton->setEnabled(false);
        ui->downButton->setEnabled(false);
        ui->upButton->setEnabled(false);
        ui->leftButton->setEnabled(false);
        ui->rightButton->setEnabled(false);
        ui->zupButton->setEnabled(false);
        ui->zdownButton->setEnabled(false);
        ui->homeButton->setEnabled(false);
        ui->zhomeButton->setEnabled(false);
        ui->resetButton->setEnabled(false);
        ui->leftupButton->setEnabled(false);
        ui->leftdownButton->setEnabled(false);
        ui->rightupButton->setEnabled(false);
        ui->rightdownButton->setEnabled(false);

        ui->workspace->setEnabled(false);
    }
    else
    {
        ui->x_lcdNumber->setPalette(Qt::red);
        ui->x_lcdNumber->display(0.000);
        ui->y_lcdNumber->setPalette(Qt::blue);
        ui->y_lcdNumber->display(0.000);
        ui->z_lcdNumber->setPalette(Qt::green);
        ui->z_lcdNumber->display(0.000);

        //reconnect
        robbyController.connectRS232Comms();
        if (robbyController.isConnected())
        {
            ui->moveWorkpieceToXYHomePositionButton->setEnabled(true);
            ui->connectButton->setText(QString("Disconnect"));
            ui->sendFileButton->setEnabled(true);
            ui->homeButton->setEnabled(true);
            ui->downButton->setEnabled(true);
            ui->upButton->setEnabled(true);
            ui->leftButton->setEnabled(true);
            ui->rightButton->setEnabled(true);
            ui->zdownButton->setEnabled(true);
            ui->zupButton->setEnabled(true);
            ui->homeButton->setEnabled(true);
            ui->zhomeButton->setEnabled(true);
            ui->resetButton->setEnabled(true);
            ui->leftupButton->setEnabled(true);
            ui->leftdownButton->setEnabled(true);
            ui->rightupButton->setEnabled(true);
            ui->rightdownButton->setEnabled(true);

            ui->xStep->setEnabled(true);
            ui->yStep->setEnabled(true);
            ui->zStep->setEnabled(true);

            ui->xHome->setEnabled(true);
            ui->yHome->setEnabled(true);
            ui->zHome->setEnabled(true);
            ui->xHome->setPalette(Qt::red);
            ui->xHome->display(0.000);
            ui->yHome->setPalette(Qt::blue);
            ui->yHome->display(0.000);
            ui->zHome->setPalette(Qt::green);
            ui->zHome->display(0.000);

            ui->localMode->setEnabled(true);
            ui->lineMode->setEnabled(true);
            ui->fileMode->setEnabled(true);
            ui->dccMode->setEnabled(true);

            ui->setZHomeButton->setEnabled(true);
            ui->setXYHomeButton->setEnabled(true);
            ui->setXYZHomeButton->setEnabled(true);

            ui->workspace->setEnabled(true);
        }
    }
}


void MainWindow::on_homeButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveXYZHome");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }

}

void MainWindow::on_zhomeButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveZHome");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_upButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveUp");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_leftButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveLeft");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_rightButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveRight");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_downButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveDown");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_zupButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveZUp");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_zdownButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveZDown");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_statusMessage_textChanged()
{

}

void MainWindow::on_leftupButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveLeftUp");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_rightupButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveRightUp");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_rightdownButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveRightDown");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_leftdownButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("MoveLeftDown");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_localMode_clicked()
{
    ui->resetButton->setEnabled(false);
    ui->lineMode->setEnabled(true);
    ui->fileMode->setEnabled(true);
    ui->dccMode->setEnabled(true);

    if (robbyController.isConnected())
        robbyController.SetLocalMode();
}

void MainWindow::on_lineMode_clicked()
{
    ui->resetButton->setEnabled(true);
    ui->fileMode->setEnabled(false);
    ui->dccMode->setEnabled(false);

    if (robbyController.isConnected())
        robbyController.SetHpglLineMode();
}

void MainWindow::on_fileMode_clicked()
{
    ui->resetButton->setEnabled(true);
    ui->lineMode->setEnabled(false);
    ui->dccMode->setEnabled(false);

    if (robbyController.isConnected())
        robbyController.SetHpglFileMode();
}

void MainWindow::on_dccMode_clicked()
{
    ui->resetButton->setEnabled(false);

    if (robbyController.isConnected())
        robbyController.SetDCCMode();
}

void MainWindow::on_openFileButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open Robby File"), "", tr("Colinbus (*.cbr);;GCode (*.nc);;Hpgl (*.plt);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
    {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        if(cncFile)
            delete cncFile;

        cncFile = new QByteArray;
        *cncFile = file.readAll();

        file.close();

        ui->label_Filename->setText(fileName);

        //if gcode, first convert to cbr file
        /*if (fileName.contains(".nc"))
        {
            CSPXGCode *gcode = new CSPXGCode();
            CSPXString cbr_filename = gcode->Translate2Hpgl(CSPXString(fileName.toStdString().c_str()),0);
            ui->label_Filename->setText(cbr_filename.GetString());
            file.setFileName(cbr_filename.GetString());

            if (!file.open(QIODevice::ReadOnly))
            {
                QMessageBox::information(this, tr("Unable to open file"),
                                         file.errorString());
                return;
            }

            if(cncFile)
                delete cncFile;

            cncFile = new QByteArray;
            *cncFile = file.readAll();

            file.close();
        }*/


        //create an HPGL object
        if (hpglCommands != nullptr)
            delete hpglCommands;
        hpglCommands = new CSPXHpglCommands();
        hpglCommands->LoadCommands(cncFile->data(),cncFile->size());

        model = new QStandardItemModel(hpglCommands->fileChunks,5,this);
        QList<QString> header = {"Tool Id", "Description", "Height", "Diameter", "Process", "CodeIndex"};
        model->setHorizontalHeaderLabels( header );

        QString description;
        double ToolHeight, ToolDiameter;
        int rindex = -1;
        QRegularExpression regex("T:(\\d*)N:(\\w*\\d*\\.?\\d*\\w*)H:(\\d*\\.?\\d*)D:(\\d*\\.?\\d*)");
        for (int i = 0; i < hpglCommands->fileChunks; i++)
        {
            CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNode = hpglCommands->commandList[i].GetHead();
            if (cmdNode)
            {
                struct CSPXHpglCommands::s_command *cmd = cmdNode->GetElement();
                if (cmd->code[0] == 'T')
                {
                    rindex++;
                    QRegularExpressionMatch match = regex.match(cmd->code);
                    if (match.hasMatch()) {
                        cmd->u_tool_time.tool_index = match.captured(1).toInt();
                        description = match.captured(2);
                        ToolHeight = match.captured(3).toFloat();
                        ToolDiameter = match.captured(4).toFloat();

                        QModelIndex index = model->index(rindex,0,QModelIndex());
                        model->setData(index,QString::number(cmd->u_tool_time.tool_index));
                        model->itemFromIndex(index)->setFlags(model->itemFromIndex(index)->flags() & ~Qt::ItemIsEditable);

                        index = model->index(rindex,1,QModelIndex());
                        model->setData(index,description);
                        model->itemFromIndex(index)->setFlags(model->itemFromIndex(index)->flags() & ~Qt::ItemIsEditable);

                        index = model->index(rindex,2,QModelIndex());
                        model->setData(index,QString::number(ToolHeight));
                        model->itemFromIndex(index)->setFlags(model->itemFromIndex(index)->flags() & ~Qt::ItemIsEditable);

                        index = model->index(rindex,3,QModelIndex());
                        model->setData(index,QString::number(ToolDiameter));
                        model->itemFromIndex(index)->setFlags(model->itemFromIndex(index)->flags() & ~Qt::ItemIsEditable);

                        index = model->index(rindex,4,QModelIndex());
                        //ui->tableView->setIndexWidget(model->index(rindex, 4, QModelIndex()),new QCheckBox());
                        model->setData(index, QVariant(Qt::Checked));

                        index = model->index(rindex,5,QModelIndex());
                        model->setData(index,rindex);
                        model->itemFromIndex(index)->setFlags(model->itemFromIndex(index)->flags() & ~Qt::ItemIsEditable);
                    }
                }
            }
        }

        ui->tableView->itemModel = model;
        ui->tableView->setModel(model);
        ui->tableView->setItemDelegateForColumn(4,new CSPXCheckBoxDelegate(this));
        ui->tableView->resizeColumnsToContents ();
        ui->tableView->verticalHeader()->hide();
        ui->tableView->horizontalHeader()->hideSection(5);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->itemModel->removeRow(rindex+1);

        //translate coordinates to home position
        int xhome = std::lround(ui->xHome->value()*40.);
        int yhome = std::lround(ui->yHome->value()*40.);
        int zhome = std::lround(ui->zHome->value()*40.);

        ui->xOrigin->setText(QString::number(hpglCommands->x_min/40.)+" mm");
        ui->yOrigin->setText(QString::number(hpglCommands->y_min/40.)+" mm");

        //Note: The cbr file should have relative coordinated or be already adjusted to home position

        connect(GetController(), &CSPXRobbyController::finished_NewHomePosition, this, &MainWindow::continue_NextFile);

        workerThread = new FileModeCheckThread();
        workerThread->dlg = this;
        connect(workerThread, &FileModeCheckThread::sendModeCommand, this, &MainWindow::issueModeCommand);
        connect(workerThread, &FileModeCheckThread::finishedProcessing, this, &MainWindow::resetWidgets);
        connect(workerThread, &FileModeCheckThread::waitNextTool, this, &MainWindow::handleToolChange);
        //connect(workerThread, &FileModeCheckThread::finished, &workerThread, &MainWindow::deleteLater);

    }
}

CSPXVector MainWindow::GetAbsoluteSpindlePosition()
{
    CSPXVector xyz(3);

    xyz[0] = ui->xHome->value()+ui->x_lcdNumber->value();
    xyz[1] = ui->yHome->value()+ui->y_lcdNumber->value();
    xyz[2] = ui->zHome->value()+ui->z_lcdNumber->value();

    return xyz;
}


void MainWindow::on_resetButton_clicked()
{

    if (robbyController.isConnected())
    {
        CSPXString cmd("Reset");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_SystemButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("System Config");
        robbyController.sendRobbyDCCCommand(cmd);
    }
}

void MainWindow::on_sendFileButton_clicked()
{

    if (hpglCommands->x_min != 0 || hpglCommands->y_min != 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this,"Stock Position", "The workpiece is not set to the relative home position (0,0). "
                                                             "Please cancel to use the Hpgl viewer to virtually move the workpiece to a new spindle position, "
                                                             "then close the viewer and click the \"move workpiece\" button to effectively move the workpiece. "
                                                             "Do you want to continue anyway?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
    }

    if (robbyController.isConnected() && cncFile != nullptr && ui->fileMode->isChecked())
    {
        abort = false;
        ui->sendFileButton->setEnabled(false);
        if (robbyController.sendRobbyFile(0))//prolog
            ((QThread*)workerThread)->start();
    }
}

void MainWindow::on_setXYHomeButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("NewXYHomePosition");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_setZHomeButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("NewZHomePosition");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}

void MainWindow::on_setXYZHomeButton_clicked()
{
    if (robbyController.isConnected())
    {
        CSPXString cmd("NewXYZHomePosition");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }
}


void MainWindow::on_modeButton_clicked()
{
    if (robbyController.isConnected())
    {
        robbyController.sendRobbyModeCommand();
    }
}

void MainWindow::handleToolChange(QString toolInfo)
{
    workerThread->pause();

    if (robbyController.isConnected())
    {
        //change to local mode
        ui->localMode->clicked(true);
        while (robbyController.GetMode() != 0)
        {
            if (robbyController.GetMode() == 3)
                robbyController.SetLocalMode();

            robbyController.sendRobbyModeCommand();
        }

        //change to line mode
        ui->lineMode->clicked(true);
        while (robbyController.GetMode() != 2)
        {
            if (robbyController.GetMode() == 0)
                robbyController.SetHpglLineMode();

            robbyController.sendRobbyModeCommand();
        }

        if (robbyController.GetMode() == 2)
        {
            last_zHome = ui->zHome->value();
            last_xHome = ui->xHome->value();
            last_yHome = ui->yHome->value();

            CSPXString cmd("MoveZHome");
            robbyController.sendRobbyHpglLineCommand(cmd);

            QMessageBox::warning(this,"Insert new tool ",toolInfo,QMessageBox::Yes|QMessageBox::No);

            QMessageBox::warning(this,"Set a new tool specific z-home position","After the new home position is set, the controller will start to execute automativcally.",QMessageBox::Yes|QMessageBox::No);

            wait_NewHomePosition = true;
        }
    }

}

void MainWindow::continue_NextFile()
{
    wait_NewHomePosition = false;

    //change to local mode
    ui->localMode->clicked(true);
    while (robbyController.GetMode() != 0)
    {
        if (robbyController.GetMode() == 3)
            robbyController.SetLocalMode();

        robbyController.sendRobbyModeCommand();
    }


    //change to file mode
    ui->fileMode->clicked(true);
    while (robbyController.GetMode() != 3)
    {
        if (robbyController.GetMode() == 0)
            robbyController.SetHpglFileMode();

        robbyController.sendRobbyModeCommand();
    }

    workerThread->resume();

    toolReady = true;

}

void MainWindow::issueModeCommand()
{
    robbyController.sendRobbyModeCommand();
}

void MainWindow::resetWidgets()
{
    ui->sendFileButton->setEnabled(true);

    //    //change to local mode
    //    ui->localMode->clicked(true);
    //    while (robbyController.GetMode() != 0)
    //    {
    //        if (robbyController.GetMode() == 3)
    //            robbyController.SetLocalMode();
    //        robbyController.sendRobbyModeCommand();
    //    }

    //    //change to line mode
    //    ui->lineMode->clicked(true);
    //    while (robbyController.GetMode() != 2)
    //    {
    //        if (robbyController.GetMode() == 0)
    //            robbyController.SetHpglLineMode();
    //        robbyController.sendRobbyModeCommand();
    //    }

    CSPXString cmd("MoveXYZHome");
    robbyController.sendRobbyHpglLineCommand(cmd);

    if (((QThread*)workerThread)->isRunning())
        ((QThread*)workerThread)->quit();
}

void FileModeCheckThread::run()
{
    QString toolRequest;
    char mode;

    for (int j = 1; dlg->ui->tableView->model()->rowCount(); j++)
    {
        QModelIndex indexId = dlg->ui->tableView->model()->index(j,0,QModelIndex());
        unsigned int toolId = dlg->ui->tableView->model()->data(indexId).toUInt();

        QModelIndex indexCheck = dlg->ui->tableView->model()->index(j,4,QModelIndex());
        bool checked = dlg->ui->tableView->model()->data(indexCheck).toBool();

        QModelIndex indexCode = dlg->ui->tableView->model()->index(j,5,QModelIndex());
        unsigned int codeIndex = dlg->ui->tableView->model()->data(indexCode).toUInt();

        for (int i = 1; i < dlg->hpglCommands->fileChunks; i++)
        {
            CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNode = dlg->hpglCommands->commandList[i].GetHead();
            struct CSPXHpglCommands::s_command *cmd = cmdNode->GetElement();
            if(checked && codeIndex == i && cmd->u_tool_time.tool_index == toolId)
            {
                sleep(3);
                //send signal to dialog to update mode
                emit sendModeCommand();
                mode = dlg->GetController()->GetMode();
                while (mode != 3)
                {
                    emit sendModeCommand();
                    sleep(1);
                    mode = dlg->GetController()->GetMode();
                }

                toolRequest = dlg->hpglCommands->commandList[i].GetHead()->GetElement()->code;

                dlg->toolReady = false;
                emit waitNextTool(toolRequest);
                sleep(3);

                sync.lock();
                if(_pause)
                    pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
                sync.unlock();
                sleep(3);

                while (!dlg->toolReady)
                {
                    sleep(3);
                }

                dlg->GetController()->sendRobbyFile(i);
                if (dlg->abort)
                {
                    emit finishedProcessing();
                    return;
                }

                dlg->toolReady = false;

                //we should see the red busy flag while the last file is being processed
                dlg->GetController()->SetMode(-1);
                emit sendModeCommand();
                int count = 0;
                mode = dlg->GetController()->GetMode();
                while (mode == 4 || (mode == -1 && count < 12) )
                {
                    if (mode == -1)
                        count += 1;
                    emit sendModeCommand();
                    sleep(5);
                    mode = dlg->GetController()->GetMode();
                }
            }
        }
    }

    emit finishedProcessing();
}

void FileModeCheckThread::pause()
{
    sync.lock();
    _pause = true;
    sync.unlock();
}

void FileModeCheckThread::resume()
{
    sync.lock();
    _pause = false;
    sync.unlock();

    pauseCond.wakeAll();
}


// void MainWindow::on_viewSourceFile_clicked()
// {
//     if (cncFile)
//     {
//         if (hpglDialog==nullptr)
//         {
//             hpglDialog = new CSPXHPGLDialog(this);
//             hpglDialog->setAttribute(Qt::WA_DeleteOnClose);
//         }
//         //hpglDialog->loadhpgl(fileName);
//         ui->moveWorkpieceToXYHomePositionButton->setEnabled(false);
//         hpglDialog->sendhpgl(this,cncFile);

//         connect(&robbyController,&CSPXRobbyController::changeSpindleCoordinates, hpglDialog, &CSPXHPGLDialog::updateSpindleCoordinates);

//         hpglDialog->setModal(false);
//         hpglDialog->show();
//         //hpglDialog->raise();
//         hpglDialog->activateWindow();
//         connect(hpglDialog, &CSPXHPGLDialog::hpglDialogClosed, this, &MainWindow::hpglDialogClosing);
//     }
// }

void MainWindow::hpglDialogClosing()
{
    //disconnect(hpglDialog, &CSPXHPGLDialog::hpglDialogClosed, this, &MainWindow::hpglDialogClosing);
    //disconnect(&robbyController,&CSPXRobbyController::changeSpindleCoordinates, hpglDialog, &CSPXHPGLDialog::updateSpindleCoordinates);

    //hpglDialog->close();

    //hpglDialog = nullptr;

    ui->moveWorkpieceToXYHomePositionButton->setEnabled(true);

}

void MainWindow::on_moveWorkpieceToXYHomePositionButton_clicked()
{
    if (hpglCommands == nullptr)
        QMessageBox::warning(this,"Machine Alert","Missing CNC file");
    else
    {
        //this translates to the origin of the home position
        hpglCommands->TranslateBy(-hpglCommands->x_min,-hpglCommands->y_min,0.0);

        ui->xOrigin->setText(QString::number(hpglCommands->x_min/40.)+" mm");
        ui->yOrigin->setText(QString::number(hpglCommands->y_min/40.)+" mm");
    }

    int idx;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this,"Spindle position", "Do you want to save the absolute position?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QString sx = QString::number(ui->xHome->value()+ui->x_lcdNumber->value());
        QString sy = QString::number(ui->yHome->value()+ui->y_lcdNumber->value());

        ui->savedPositionBox->addItem(sx+":"+sy);
        ui->savedPositionBox->setCurrentText(sx+":"+sy);
        idx = ui->savedPositionBox->currentIndex();
    }

}

void MainWindow::on_HelpButton_clicked()
{

    //CSPXCNCHelpDialog helpDlg(this);

    //helpDlg.exec();
}

void MainWindow::updateData(QByteArray* data)
{
    ui->tableView->reset();

    //create an HPGL object
    if (hpglCommands != nullptr)
        delete hpglCommands;
    hpglCommands = new CSPXHpglCommands();
    hpglCommands->LoadCommands(data->data(),data->size());

    QString description;
    double ToolHeight, ToolDiameter;
    int index = -1;
    QRegularExpression regex("T:(\\d)N:(\\w*\\d*\\.?\\d*\\w*)H:(\\d*\\.?\\d*)D:(\\d*\\.?\\d*)");
    for (int i = 0; i < hpglCommands->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNode = hpglCommands->commandList[i].GetHead();
        struct CSPXHpglCommands::s_command *cmd = cmdNode->GetElement();
        if (cmd->code[0] == 'T')
        {
            index++;
            QRegularExpressionMatch match = regex.match(cmd->code);
            if (match.hasMatch()) {
                cmd->u_tool_time.tool_index = match.captured(1).toInt();
                description = match.captured(2);
                ToolHeight = match.captured(3).toFloat();
                ToolDiameter = match.captured(4).toFloat();
            }
        }
    }

    ui->xOrigin->setText(QString::number(hpglCommands->x_min/40.)+" mm");
    ui->yOrigin->setText(QString::number(hpglCommands->y_min/40.)+" mm");

}

void MainWindow::on_savedPositionBox_currentTextChanged(const QString &arg1)
{
    QStringList xy = arg1.split(':');
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Saved XY home positions","Do you want to move the spindle to this position "+arg1+ "?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QString sx = QString::number(ui->xHome->value()+ui->x_lcdNumber->value());
        QString sy = QString::number(ui->yHome->value()+ui->y_lcdNumber->value());
    }
}

void MainWindow::on_closeFileButton_clicked()
{
    ui->tableView->reset();

    //delete HPGL commands
    if (hpglCommands != nullptr)
        delete hpglCommands;

    hpglCommands = nullptr;

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    //    int selectedRow = ui->tableView->currentIndex().row() ;
    //    if(selectedRow == ui->tableView->model()->rowCount()-1)
    //    {
    //       return;
    //    }
    //    QList<QStandardItem *> list = model->takeRow(ui->tableView->currentIndex().row());
    //    model->insertRow(selectedRow+1,list);
}

void MainWindow::drawGrid(const QSize &size, int spacing) {
    QPen pen(Qt::red, 1, Qt::SolidLine);
    pen.setCosmetic(true);

    for (int x = 0; x <= size.width(); x += spacing) {
        scene->addLine(x, 0, x, size.height(), pen)->setZValue(1);
    }
    for (int y = 0; y <= size.height(); y += spacing) {
        scene->addLine(0, y, size.width(), y, pen)->setZValue(1);
    }
}


void MainWindow::onImageClicked(const QPointF &pos) {
    QMessageBox::information(this, "Click Detected",
                             QString("You clicked at: (%1, %2)")
                                 .arg(pos.x()*2., 0, 'f', 2) // width=0, fixed-point, 2 decimals
                                 .arg((200.0-pos.y())*2., 0, 'f', 2));

    QPen pen(Qt::darkGreen, 1, Qt::SolidLine);
    pen.setCosmetic(true);

    if (xSpindleLine)
        scene->removeItem(xSpindleLine);
    if (ySpindleLine)
        scene->removeItem(ySpindleLine);


    if (robbyController.isConnected())
    {
        xSpindlePos = pos.x()*2.0;
        ySpindlePos = (200.0-pos.y())*2.;
        CSPXString cmd("MoveXY");
        robbyController.sendRobbyHpglLineCommand(cmd);
    }

}


