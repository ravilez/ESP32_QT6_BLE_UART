#ifndef CSPXROBBYCONTROLLER_H
#define CSPXROBBYCONTROLLER_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QPen>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QDebug>

#include "../Comms/cspxcommunication.h"

class MainWindow;
class SerialReceiverThread;

class CSPXRobbyController : public QObject
{
    Q_OBJECT

public:
    explicit CSPXRobbyController(QObject* parent = 0);
    virtual ~CSPXRobbyController();

signals:
    void finished_NewHomePosition();
    void resetMachine();
    void changeSpindleCoordinates(double x, double y, double z);

public slots:
    void checkResponse(QString);
    void abortProcess();

public:
    bool isConnected() { return connected; }
    CSPXCOMPort *GetComs() { return coms; }

    /*!
     *  opens port and resumes reader thread
     */
    bool connectRS232Comms();

    /*!
     *  send commands from the user interface
     */
    bool sendRobbyDCCCommand(CSPXString command);
    bool sendRobbyHpglLineCommand(CSPXString command);
    bool sendRobbyFile(int fileIndex);
    bool sendRobbyModeCommand();
    void SetHpglLineMode();
    void SetHpglFileMode();
    void SetLocalMode();
    void SetDCCMode();

    void SendData(CSPXMessage &msg);


    /*!
     *  suspends thread and closes port
     */
    void disconnectRS232Comms();

    static void SetMachineDialog(MainWindow *dlg) { machineDialog = dlg; }
    static MainWindow *GetMachineDialog() { return machineDialog; }

    public: CSPXString response;
    public: char GetMode() { return mode; }
    public: void SetMode(char m) { mode = m; }

public slots:
        void readData();
        void handleError(QSerialPort::SerialPortError error);
        void checkBusyState();

    private:
        static MainWindow *machineDialog;

    CSPXCOMPort *coms;
    QFile file;
    QTimer busyTimer;

    bool connected;
    char mode;

    SerialReceiverThread *receiverThread;

    char lastHpglCommand[128];

    QPen whitePen;
};

class SerialReceiverThread : public QThread
{
    Q_OBJECT
protected:
    void run() override;

public:
    void pause();
    void resume();

signals:
    void haveResponse(const QString &s);

public slots:
    void readData();

public:
    QMutex sync;
    QWaitCondition pauseCond;
    bool _pause;


    CSPXRobbyController *controller;
};


#endif // CSPXROBBYCONTROLLER_H
