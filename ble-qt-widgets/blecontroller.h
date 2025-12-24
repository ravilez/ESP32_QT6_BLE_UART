#ifndef BLECONTROLLER_H
#define BLECONTROLLER_H

#include "MainWindow.h"
#include <QObject>

#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyService>

class BLEController : public QObject
{
    Q_OBJECT
public:
    explicit BLEController(QObject *parent = nullptr);

    void setMainWindow(MainWindow* mw) {
        mainWindow = mw;
    }
    QString serviceStateToString(QLowEnergyService::ServiceState state);


public slots:
    void startBLE();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanFinished();
    void deviceConnected();
    void deviceDisconnected();
    void serviceDiscovered(const QBluetoothUuid &newService);
    void serviceScanDone();
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void sendData(const QByteArray &value);

signals:
    void distanceChanged();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyCharacteristic characteristic_tx;
    QLowEnergyCharacteristic characteristic_rx;
    QLowEnergyCharacteristic characteristic_fs;
    float m_distance = 0;

    MainWindow* mainWindow = nullptr;
};

#endif // BLECONTROLLER_H
