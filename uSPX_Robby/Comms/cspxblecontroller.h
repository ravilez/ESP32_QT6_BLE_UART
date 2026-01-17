#ifndef CSPXBLEController_H
#define CSPXBLEController_H

#include "../mainwindow.h"
#include <QObject>

#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyService>

#include "cspxcommunication.h"

class CSPXBLEController : public CSPXCommunicationChannel
{
    Q_OBJECT
public:
    explicit CSPXBLEController();

    void setMainWindow(MainWindow* mw) {
        mainWindow = mw;
    }
    QString serviceStateToString(QLowEnergyService::ServiceState state);

    public:int Open(const char* deviceId) override;
    public:int Close() override;
    public:int SendData(CSPXMessage *msg) override;
    public:int SendData(const char *msg,int length) override;
    public:int ReceiveData(CSPXMessage *msg) override;

signals:
    void haveResponse(const QString &s);

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

#endif // CSPXBLEController_H
