#include "MainWindow.h"
#include "blecontroller.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // --- UI ---
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    auto* row1 = new QHBoxLayout;
    scanBtn_ = new QPushButton(tr("Scan"));
    row1->addWidget(new QLabel(tr("Device:")));
    row1->addWidget(scanBtn_);
    layout->addLayout(row1);

    logView_ = new QTextEdit; logView_->setReadOnly(true);
    layout->addWidget(logView_, 1);

    auto* row2 = new QHBoxLayout;
    cmdEdit_ = new QLineEdit;
    cmdEdit_->setPlaceholderText(tr("Type command (e.g., STATUS)"));
    sendBtn_ = new QPushButton(tr("Send"));
    sendFSBtn_ = new QPushButton(tr("FS_Cmd"));
    row2->addWidget(cmdEdit_, 1);
    row2->addWidget(sendBtn_);
    row2->addWidget(sendFSBtn_);
    layout->addLayout(row2);

    statusLbl_ = new QLabel(tr("Idle"));
    layout->addWidget(statusLbl_);

    setCentralWidget(central);
    setWindowTitle(tr("ESP32 BLE UART (Qt Widgets)"));
    resize(720, 480);


    // --- Signals ---
    connect(scanBtn_, &QPushButton::clicked, this, &MainWindow::startScan);
    connect(sendBtn_, &QPushButton::clicked, this, &MainWindow::sendCommand);
    connect(sendFSBtn_, &QPushButton::clicked, this, &MainWindow::sendFSCommand);

    bleCtrl = new BLEController();
}

void MainWindow::startScan() {
    statusLbl_->setText(tr("Scanning..."));
    log("Scanning for BLE devices...");
    bleCtrl->setMainWindow(this);
    bleCtrl->startBLE();
}

void MainWindow::sendCommand() {
	
    auto text = cmdEdit_->text().toUtf8();
    if (text.isEmpty()) return;
    if (!text.endsWith('\n')) 
		text.append('\n');  // newline framing
	
	log("Sending:"); log(text);

    bleCtrl->sendData(text);
}

void MainWindow::sendFSCommand() {

    auto text = cmdEdit_->text().toUtf8();
    if (text.isEmpty()) return;
    if (!text.endsWith('\n'))
        text.append('\n');  // newline framing

    log("Sending:"); log(text);

    bleCtrl->sendFSData(text);
}

void MainWindow::log(const QString& line) {
    logView_->append(line);
}

void MainWindow::clearBle() {
}


