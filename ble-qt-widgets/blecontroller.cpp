#include "BLEController.h"
#include <QTimer>

// UUID usługi i charakterystyki
const QString TARGET_DEVICE_NAME = "ESP32_BLE_UART";
const QString SERVICE_UUID = "a05fde7e-bacb-40b9-9856-efb85cdb8f66";
const QString CHARACTERISTIC_UUID = "eb99eb2b-048a-4fa7-a81f-4f62ca333f07";
const QString CHARACTERISTIC_RX_UUID = "eb99eb2b-048a-4fa7-a81f-4f62ca333f08";


BLEController::BLEController(QObject *parent)
    : QObject{parent}
{}

void BLEController::startBLE()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BLEController::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BLEController::scanFinished);

    discoveryAgent->start();
}

void BLEController::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    mainWindow->log("Discovered device:" + device.name() + device.address().toString());

    if (device.name() == TARGET_DEVICE_NAME) {
        mainWindow->log("Target device found.");
        discoveryAgent->stop();

        controller = QLowEnergyController::createCentral(device, this);
        connect(controller, &QLowEnergyController::connected, this, &BLEController::deviceConnected);
        connect(controller, &QLowEnergyController::disconnected, this, &BLEController::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered, this, &BLEController::serviceDiscovered);
        connect(controller, &QLowEnergyController::discoveryFinished, this, &BLEController::serviceScanDone);

        controller->connectToDevice();
    }
}

void BLEController::scanFinished()
{
    mainWindow->log("Device scan finished.");
    if (!controller) {
        mainWindow->log("Target device not found.");
    }
}

void BLEController::deviceConnected()
{
    mainWindow->log("Connected to device.");
    controller->discoverServices();


}

void BLEController::deviceDisconnected()
{
    mainWindow->log("Disconnected from device.");
}

void BLEController::serviceDiscovered(const QBluetoothUuid &newService)
{
    mainWindow->log("Service discovered:" + newService.toString());
}

// Helper function to convert ServiceState enum to string
QString BLEController::serviceStateToString(QLowEnergyService::ServiceState state) {
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

void BLEController::serviceScanDone()
{
    mainWindow->log("Service scan done.");
    service = controller->createServiceObject(QBluetoothUuid(SERVICE_UUID), this);
    if (!service) {
        mainWindow->log("Service not found.");
        return;
    }

    mainWindow->log("Creating service object: " + service->serviceUuid().toString());

    connect(service, &QLowEnergyService::stateChanged, this, [this](const QLowEnergyService::ServiceState &newState){
        mainWindow->log("State changed to: " + serviceStateToString(newState));
        if (newState == QLowEnergyService::RemoteServiceDiscovered) {
            characteristic = service->characteristic(QBluetoothUuid(CHARACTERISTIC_UUID));
            characteristic_rx = service->characteristic(QBluetoothUuid(CHARACTERISTIC_RX_UUID));
            mainWindow->log("Characteristic: " + characteristic.uuid().toString());
            mainWindow->log("Characteristic RX: " + characteristic_rx.uuid().toString());
            if (!characteristic.isValid()) {
                mainWindow->log("Characteristic not found.");
                return;
            }
            if (!characteristic_rx.isValid()) {
                mainWindow->log("Characteristic RX not found.");
            }
            // Enable notifications by writing to the CCCD
            if (!(characteristic.properties() & QLowEnergyCharacteristic::Notify)) {
                mainWindow->log("Characteristic does not support notifications.");
            }
            else
            {
                QLowEnergyDescriptor cccd = characteristic.clientCharacteristicConfiguration();
                if (cccd.isValid()) {
                    service->writeDescriptor(cccd, QByteArray::fromHex("0100")); // 0x0100 = notifications on
                }
            }
        }
    });

    connect(service, &QLowEnergyService::characteristicRead, this, &BLEController::characteristicChanged);
    connect(service, &QLowEnergyService::characteristicChanged, this, &BLEController::characteristicChanged);

    service->readCharacteristic(characteristic);

    service->discoverDetails();
}

void BLEController::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value)
{
    if (characteristic.uuid() == QBluetoothUuid(CHARACTERISTIC_UUID)) {
        mainWindow->log("Characteristic value changed:" + value);
        m_distance = value.toFloat();
        emit distanceChanged();
        //service->readCharacteristic(characteristic);
    }
}

void BLEController::sendData(const QByteArray &value)
{
    if (characteristic_rx.isValid())
        service->writeCharacteristic(characteristic_rx,value,QLowEnergyService::WriteWithoutResponse);
}
