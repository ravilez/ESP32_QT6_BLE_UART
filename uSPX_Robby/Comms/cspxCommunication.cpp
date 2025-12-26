
#include "cspxcommunication.h"



void* picBootHandle;

void* (*OpenPIC)(char* ComPort, unsigned long BitRate, unsigned long ReadTimeOut);
int (*ClosePIC)(void* hComPort);
int (*GetPacket)(void* hComPort, unsigned char PacketData[], unsigned int ByteLimit);
int (*SendPacket)(void* hComPort, unsigned char PacketData[], unsigned int NumOfBytes);
int (*SendGetPacket)(void* hComPort, unsigned char PacketData[], unsigned int SendNumOfBytes, unsigned int RecvByteLimit, unsigned int NumOfRetrys);



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


int CSPXCOMPort::Open()
{
	int rtc = 0;

    try
    {
        if (!QSerialPort::isOpen())
        rtc = QSerialPort::open(ReadWrite);//SPXOpenComPort(this);
    }
    catch(...)
    {
    }

	if (rtc)
		m_opened = true;
	else 
		m_opened = false;

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


