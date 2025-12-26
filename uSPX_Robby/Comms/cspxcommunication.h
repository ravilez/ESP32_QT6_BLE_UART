#pragma once

#include <QSerialPort>

enum EChannel
{
    NOD = 0,
    COM = 1,
    USB = 2,
    TCPIP = 3,
};

// The communication comprises the following functions
// 1. SendPacket
// 2. GetPacket
// 3. OpenPIC
// 4. ClosePIC can be used for Demo Mode operation with COM Port
// Other functions are not required to be used.

#include <string.h>

#include "cspxMessage.h"

class CSPXDevice;

class CSPXCommunicationChannel
{
    public:CSPXCommunicationChannel():m_device(nullptr),m_opened(false),m_devInterface(nullptr) {}
    public:virtual ~CSPXCommunicationChannel() {}

    //can be used to keep the pointer of the OS dependent driver class,ie CStandardComm etc
	public:void *m_devInterface;
	public:virtual void *GetReadyToSentEvent() { return nullptr; }
	public:virtual void *GetReadyToReadEvent() { return nullptr; }

    public:virtual bool IsReadyToSend() { return false; }
    public:virtual bool IsReadyToRead() { return false; }

    protected:CSPXDevice *m_device;
	protected:bool m_opened;
	public:bool IsOpen() { return m_opened; }

    public:virtual int Open() { return 0; }
    public:virtual int Close() { return 0; }
    public:virtual int SendData(CSPXMessage *msg) { return 0; }
    public:virtual int SendData(const char* msg, int length) { return 0; }
    public:virtual int ReceiveData(CSPXMessage *msg) { return 0; }
    public:virtual enum EChannel CommunicationChannelType() { return NOD; }
	public:virtual int SendReceivePacket(CSPXMessage *sendmsg, CSPXMessage *recvmsg,unsigned int SendDelay=500, unsigned int ReceiveDelay=1000) { return 0; };
};

//interface classes used to communicated with the os dependent api through Qt classes
class CSPXCOMPort: public CSPXCommunicationChannel, public QSerialPort
{
    public:CSPXCOMPort(const char* port)
        : QSerialPort()
        ,m_threadFunction(nullptr)
        ,m_baudRate(38400)
        ,m_stopbits(1)
        ,m_parity(0)
        ,m_characters(8)
        ,m_flowControl(2)
        ,m_eventMask(0)
        {
            m_device = nullptr;
            strcpy(m_portId,port);

            QSerialPort::setPortName(QString(port));
            QSerialPort::setBaudRate(BaudRate::Baud38400);
            QSerialPort::setStopBits(StopBits::OneStop);
            QSerialPort::setParity(Parity::NoParity);
            QSerialPort::setFlowControl(FlowControl::HardwareControl);
            QSerialPort::setDataBits(DataBits::Data8);
        }

    public:CSPXCOMPort(const char *port,CSPXDevice *parent)
        :QSerialPort()
        ,m_threadFunction(nullptr)
        ,m_baudRate(38400)
        ,m_stopbits(1)
        ,m_parity(0)
        ,m_characters(8)
        ,m_flowControl(2)
        ,m_eventMask(0)
        {
            m_device = parent;
            strcpy(m_portId,port);

            QSerialPort::setPortName(QString(port));
            QSerialPort::setBaudRate(BaudRate::Baud38400);
            QSerialPort::setStopBits(StopBits::OneStop);
            QSerialPort::setParity(Parity::NoParity);
            QSerialPort::setFlowControl(FlowControl::HardwareControl);
            QSerialPort::setDataBits(DataBits::Data8);

        }
    public:virtual ~CSPXCOMPort() {}

    private:char m_portId[6];
    public:char* GetPortId() { return m_portId; }
    public:void SetPortId(const char *pid) { strcpy(m_portId,pid); }

    private:void* m_threadFunction;
    public:void* GetUserThreadFunction() { return m_threadFunction; }
    public:void SetUserThreadFunction(void *threadFunction) { m_threadFunction = threadFunction; }

    private:int m_baudRate;
    public:int GetBaudRate() { return m_baudRate; }
    public:void SetBaudRate(int br)
    {
        m_baudRate = br;

        switch(m_baudRate)
        {
            case 1200:
                QSerialPort::setBaudRate(BaudRate::Baud1200);
            break;

            case 2400:
                QSerialPort::setBaudRate(BaudRate::Baud2400);
            break;

            case 4800:
                QSerialPort::setBaudRate(BaudRate::Baud4800);
            break;

            case 9600:
                QSerialPort::setBaudRate(BaudRate::Baud9600);
            break;

            case 19200:
                QSerialPort::setBaudRate(BaudRate::Baud19200);
            break;

            case 38400:
                QSerialPort::setBaudRate(BaudRate::Baud38400);
            break;

            case 57600:
                QSerialPort::setBaudRate(BaudRate::Baud57600);
            break;

            case 115200:
                QSerialPort::setBaudRate(BaudRate::Baud115200);
            break;

            default:
                QSerialPort::setBaudRate(BaudRate::Baud9600);
        }
    }

    private:int m_parity;
    public:int GetParity() { return m_parity; }
    public:void SetParity(int p)
    {
        m_parity = p;
        if (m_parity == 0)
            QSerialPort::setParity(Parity::NoParity);
        else if (m_parity == 1)
            QSerialPort::setParity(Parity::OddParity);
        else if (m_parity == 2)
            QSerialPort::setParity(Parity::EvenParity);
        else
        {
            m_parity = 0;
            QSerialPort::setParity(Parity::NoParity);
        }
    }

    private:int m_stopbits;
    public:int GetStopbits() { return m_stopbits; }
    public:void SetStopbits(int sb)
    {
        m_stopbits = sb;
        if (m_stopbits == 1)
            QSerialPort::setStopBits(StopBits::OneStop);
        else if (m_parity == 3)
            QSerialPort::setStopBits(StopBits::OneAndHalfStop);
        else if (m_parity == 2)
            QSerialPort::setStopBits(StopBits::TwoStop);
        else
        {
            m_stopbits = 1;
            QSerialPort::setStopBits(StopBits::OneStop);
        }

    }

    private:int m_characters;
    public:int GetCharacters() { return m_characters; }
    public:void SetCharacters(int cs)
    {
        m_characters = cs;
        switch(m_characters)
        {
            case 5:
                QSerialPort::setDataBits(DataBits::Data5);
            break;

            case 6:
                QSerialPort::setDataBits(DataBits::Data6);
            break;

            case 7:
                QSerialPort::setDataBits(DataBits::Data7);
            break;

            case 8:
                QSerialPort::setDataBits(DataBits::Data8);
            break;

            default:
                QSerialPort::setDataBits(DataBits::Data8);
        }
    }

    private:int m_flowControl;
    public:int GetFlowControl() { return m_flowControl; }
    public:void SetFlowControl(int fc)
    {
        m_flowControl = fc;
        if (m_flowControl == 0)
            QSerialPort::setFlowControl(FlowControl::NoFlowControl);
        else if (m_flowControl == 1)
            QSerialPort::setFlowControl(FlowControl::SoftwareControl);
        else if (m_flowControl == 2)
            QSerialPort::setFlowControl(FlowControl::HardwareControl);
        else
        {
            m_flowControl = 0;
            QSerialPort::setFlowControl(FlowControl::NoFlowControl);
        }
    }

    private:unsigned long m_eventMask;
    public:unsigned long GetEventMask() { return m_eventMask; }
    public:void SetEventMask(unsigned long em)
    {
        m_eventMask = em;
    }

    public:virtual bool IsReadyToSend();
    public:virtual bool IsReadyToRead();

	public:virtual int Open();
	public:virtual int Close();
	public:virtual int SendData(CSPXMessage *msg);
    public:virtual int SendData(const char *msg,int length);

	public:virtual int ReceiveData(CSPXMessage *msg);
	public:virtual enum EChannel CommunicationChannelType();
	public:virtual int SendReceivePacket(CSPXMessage *sendmsg, CSPXMessage *recvmsg, unsigned int SendDelay=500, unsigned int ReceiveDelay=1000);

};

