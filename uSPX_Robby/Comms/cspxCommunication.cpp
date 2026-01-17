
#include "cspxcommunication.h"



void* picBootHandle;

void* (*OpenPIC)(char* ComPort, unsigned long BitRate, unsigned long ReadTimeOut);
int (*ClosePIC)(void* hComPort);
int (*GetPacket)(void* hComPort, unsigned char PacketData[], unsigned int ByteLimit);
int (*SendPacket)(void* hComPort, unsigned char PacketData[], unsigned int NumOfBytes);
int (*SendGetPacket)(void* hComPort, unsigned char PacketData[], unsigned int SendNumOfBytes, unsigned int RecvByteLimit, unsigned int NumOfRetrys);



