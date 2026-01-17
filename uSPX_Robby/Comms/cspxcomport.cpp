#include "cspxcommunication.h"

#include "cspxcomport.h"

bool CSPXCOMPort::IsReadyToSend()
{
    try
    {
        return QSerialPort::waitForBytesWritten();//SPXGetReadyToSentEvent(this);
    }
    catch(...)
    {
    }

    return false;
}

bool CSPXCOMPort::IsReadyToRead()
{
    try
    {
        return QSerialPort::waitForReadyRead();//SPXGetReadyToSentEvent(this);
    }
    catch(...)
    {
    }

    return false;
}


int CSPXCOMPort::Open(const char* deviceId)
{
    int rtc = 0;

    try
    {
        SetPortId(deviceId);
        SetBaudRate(38400);
        SetParity(0); //no parity
        SetStopbits(1);
        SetCharacters(8);
        SetFlowControl(2); //0:No flow control, 1, software, 2=hardware (RTS-CTS)

        if (!QSerialPort::isOpen()) {

            if (!open(QIODevice::ReadWrite))
            {
                rtc = false;
            }
            if (rtc) {
                setDataTerminalReady(true);
                m_opened = true;
            }
            else
                m_opened = false;
        }

    }
    catch(...)
    {
    }

    return rtc;
}

int CSPXCOMPort::Close()
{
    int rtc = 0;

    try
    {
        QSerialPort::close();//SPXCloseComPort(this);
        rtc = !QSerialPort::isOpen();
    }
    catch(...)
    {
    }

    if (rtc)
        m_opened = false;
    else
        m_opened = true;

    return rtc;
}


int CSPXCOMPort::SendData(CSPXMessage* msg)
{
    int bytesWritten;
    try
    {
        bytesWritten = QSerialPort::write(msg->GetData(),msg->GetDataLength());//SPXSendDataComPort(this, msg);
        waitForBytesWritten(5000);
    }
    catch(...)
    {
    }

    return bytesWritten;
}

int CSPXCOMPort::SendData(const char *msg,int length)
{
    int bytesWritten;
    try
    {
        bytesWritten = QSerialPort::write(msg,length);
        waitForBytesWritten(5000);
    }
    catch(...)
    {
    }

    return bytesWritten;
}

int CSPXCOMPort::ReceiveData(CSPXMessage *msg)
{
    try
    {
        const QByteArray data = readAll();
        msg->SetData(data.toStdString().c_str(),data.length());
    }
    catch(...)
    {
    }

    return 0;
}

int CSPXCOMPort::SendReceivePacket(CSPXMessage* sendmsg,CSPXMessage* recvmsg,unsigned int SendDelay, unsigned int ReceiveDelay)
{
    return 0;
}

enum EChannel CSPXCOMPort::CommunicationChannelType()
{
    return COM;  // COM Port
}


