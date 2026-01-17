#pragma once

#include <QSerialPort>

enum EChannel
{
    NOD = 0,
    COM = 1,
    USB = 2,
    TCPIP = 3,
    BLE = 4
};

// The communication comprises the following functions
// 1. SendPacket
// 2. GetPacket
// 3. OpenPIC
// 4. ClosePIC can be used for Demo Mode operation with COM Port
// Other functions are not required to be used.

#include <string.h>

#include "cspxMessage.h"

class CSPXCommunicationChannel: public QObject
{
    Q_OBJECT

    public:CSPXCommunicationChannel():m_opened(false) {}
    public:virtual ~CSPXCommunicationChannel() {}

	public:virtual void *GetReadyToSentEvent() { return nullptr; }
	public:virtual void *GetReadyToReadEvent() { return nullptr; }

    public:virtual bool IsReadyToSend() { return false; }
    public:virtual bool IsReadyToRead() { return false; }

    protected:bool m_opened;
	public:bool IsOpen() { return m_opened; }

    public:virtual int Open(const char* deviceId) = 0;
    virtual int Close() = 0;
    public:virtual int SendData(CSPXMessage *msg) = 0;
    public:virtual int SendData(const char* msg, int length) = 0;
    public:virtual int ReceiveData(CSPXMessage *msg) = 0;


    public:virtual enum EChannel CommunicationChannelType() { return NOD; }
	public:virtual int SendReceivePacket(CSPXMessage *sendmsg, CSPXMessage *recvmsg,unsigned int SendDelay=500, unsigned int ReceiveDelay=1000) { return 0; };
};

