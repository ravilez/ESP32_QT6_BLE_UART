#include "CSPXBLEController.h"
#include <QTimer>

#include "../mainwindow.h"
#include "../ui_mainwindow.h"

// UUID usługi i charakterystyki
QString TARGET_DEVICE_NAME = "ESP32_BLE_UART";
const QString SERVICE_UUID = "a05fde7e-bacb-40b9-9856-efb85cdb8f66";
const QString CHARACTERISTIC_TX_UUID = "eb99eb2b-048a-4fa7-a81f-4f62ca333f07";
const QString CHARACTERISTIC_RX_UUID = "eb99eb2b-048a-4fa7-a81f-4f62ca333f08";
const QString CHARACTERISTIC_FS_UUID = "eb99eb2b-048a-4fa7-a81f-4f62ca333f09";


CSPXBLEController::CSPXBLEController() {}

void CSPXBLEController::startBLE()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &CSPXBLEController::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &CSPXBLEController::scanFinished);

    discoveryAgent->start();
}

void CSPXBLEController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    mainWindow->ui->statusMessage->setPlainText(QString("Discovered device:" + device.name() + device.address().toString()));

    if (device.name() == TARGET_DEVICE_NAME) {

        mainWindow->ui->statusMessage->setPlainText(QString("Target device found."));

        controller = QLowEnergyController::createCentral(device, this);
        connect(controller, &QLowEnergyController::connected, this, &CSPXBLEController::deviceConnected);
        connect(controller, &QLowEnergyController::disconnected, this, &CSPXBLEController::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered, this, &CSPXBLEController::serviceDiscovered);
        connect(controller, &QLowEnergyController::discoveryFinished, this, &CSPXBLEController::serviceScanDone);

        controller->connectToDevice();
    }
}

void CSPXBLEController::scanFinished()
{
    mainWindow->ui->statusMessage->setPlainText(QString("Device scan finished."));
    if (!controller) {
        mainWindow->ui->statusMessage->setPlainText(QString("Target device not found."));
    }
}

void CSPXBLEController::deviceConnected()
{
    mainWindow->ui->statusMessage->setPlainText(QString("Connected to device."));
    controller->discoverServices();


}

void CSPXBLEController::deviceDisconnected()
{
    mainWindow->ui->statusMessage->setPlainText(QString("Disconnected from device."));
}

void CSPXBLEController::serviceDiscovered(const QBluetoothUuid &newService)
{
    mainWindow->ui->statusMessage->setPlainText(QString("Service discovered:" + newService.toString()));

    if (discoveryAgent->isActive())
        discoveryAgent->stop();
}

// Helper function to convert ServiceState enum to string
QString CSPXBLEController::serviceStateToString(QLowEnergyService::ServiceState state) {
    switch (state) {
    case QLowEnergyService::InvalidService:
        return "InvalidService";
    case QLowEnergyService::RemoteService:
        return "Remote service";
    case QLowEnergyService::RemoteServiceDiscovering: // discoverDetails() called and running
        return "Discovey details running";
    case QLowEnergyService::RemoteServiceDiscovered:  // all details have been synchronized
        return "Details synchronnized";
    default:
        return QString("Unknown (%1)").arg(static_cast<int>(state));
    }
}

void CSPXBLEController::serviceScanDone()
{
    mainWindow->ui->statusMessage->setPlainText(QString("Service scan done."));
    service = controller->createServiceObject(QBluetoothUuid(SERVICE_UUID), this);
    if (!service) {
        mainWindow->ui->statusMessage->setPlainText(QString("Service not found."));
        return;
    }

    mainWindow->ui->statusMessage->setPlainText(QString("Creating service object: " + service->serviceUuid().toString()));

    connect(service, &QLowEnergyService::stateChanged, this, [this](const QLowEnergyService::ServiceState &newState){
        mainWindow->ui->statusMessage->setPlainText(QString("State changed to: " + serviceStateToString(newState)));
        if (newState == QLowEnergyService::RemoteServiceDiscovered) {

            characteristic_tx = service->characteristic(QBluetoothUuid(CHARACTERISTIC_TX_UUID));
            characteristic_rx = service->characteristic(QBluetoothUuid(CHARACTERISTIC_RX_UUID));
            characteristic_fs = service->characteristic(QBluetoothUuid(CHARACTERISTIC_FS_UUID));

            mainWindow->ui->statusMessage->setPlainText(QString("Characteristic TX: " + characteristic_tx.uuid().toString()));
            mainWindow->ui->statusMessage->setPlainText(QString("Characteristic RX: " + characteristic_rx.uuid().toString()));
            mainWindow->ui->statusMessage->setPlainText(QString("Characteristic FS: " + characteristic_fs.uuid().toString()));


            if (!characteristic_tx.isValid()) {
                mainWindow->ui->statusMessage->setPlainText(QString("Characteristic TX not found."));
                return;
            }
            if (!characteristic_rx.isValid()) {
                mainWindow->ui->statusMessage->setPlainText(QString("Characteristic RX not found."));
            }

            if (!characteristic_fs.isValid()) {
                mainWindow->ui->statusMessage->setPlainText(QString("Characteristic FS not found."));
                return;
            }

            // Enable notifications by writing to the TX CCCD
            if (!(characteristic_tx.properties() & QLowEnergyCharacteristic::Notify)) {
                mainWindow->ui->statusMessage->setPlainText(QString("Characteristic TX does not support notifications."));
            }
            else
            {
                QLowEnergyDescriptor cccd = characteristic_tx.clientCharacteristicConfiguration();
                if (cccd.isValid()) {
                    service->writeDescriptor(cccd, QByteArray::fromHex("0100")); // 0x0100 = notifications on
                }
            }
            // Enable notifications by writing to the FS WRITE CCCD
            if (!(characteristic_fs.properties() & QLowEnergyCharacteristic::Notify)) {
                mainWindow->ui->statusMessage->setPlainText(QString("Characteristic FS does not support notifications."));
            }
            else
            {
                QLowEnergyDescriptor cccd = characteristic_fs.clientCharacteristicConfiguration();
                if (cccd.isValid()) {
                    service->writeDescriptor(cccd, QByteArray::fromHex("0100")); // 0x0100 = notifications on
                }
            }
        }
    });

    connect(service, &QLowEnergyService::characteristicRead, this, &CSPXBLEController::characteristicChanged);
    connect(service, &QLowEnergyService::characteristicChanged, this, &CSPXBLEController::characteristicChanged);

    service->readCharacteristic(characteristic_tx);

    service->discoverDetails();
}

void CSPXBLEController::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    if (characteristic.uuid() == QBluetoothUuid(CHARACTERISTIC_TX_UUID)) {
        mainWindow->ui->statusMessage->setPlainText(QString("Characteristic TX value changed:" + value));

        emit haveResponse(value.toStdString().c_str());

    }
    else if (characteristic.uuid() == QBluetoothUuid(CHARACTERISTIC_FS_UUID)) {
        mainWindow->ui->statusMessage->setPlainText(QString("Characteristic FS value changed:" + value));
    }
}

void CSPXBLEController::sendData(const QByteArray &value)
{
    if (characteristic_rx.isValid())
        service->writeCharacteristic(characteristic_rx,value,QLowEnergyService::WriteWithoutResponse);
}

int CSPXBLEController::Open(const char* BLE_target) {

    TARGET_DEVICE_NAME = BLE_target;
    startBLE();
    return 0;
}

int CSPXBLEController::Close() {

    controller->disconnectFromDevice();

    return 0;
}

int CSPXBLEController::SendData(CSPXMessage *msg) {

    sendData(msg->GetData());

    return 0;
}

int CSPXBLEController::SendData(const char *msg,int length) {

    QByteArray ba(msg,length);
    sendData(ba);

    return 0;
}

int CSPXBLEController::ReceiveData(CSPXMessage *msg) {
    return 0;
}

