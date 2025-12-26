#include "cspxMessage.h"

CSPXMessage::CSPXMessage(void)
    :m_deviceName(nullptr)
    ,m_data(nullptr)
    ,m_usageCount(1)
{
}

CSPXMessage::CSPXMessage(int initSize)
    :m_deviceName(nullptr)
    ,m_data(nullptr)
    ,m_usageCount(1)
{
    m_data = (char*)new char[initSize];
    memset(m_data,0,initSize);
}

CSPXMessage::~CSPXMessage(void)
{
    if (m_deviceName != nullptr)
        delete m_deviceName;

}

