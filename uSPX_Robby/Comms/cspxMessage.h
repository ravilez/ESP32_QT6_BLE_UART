#pragma once

#include <cstring>
#include "../Containers/cspxstring.h"

class CSPXMessage
{
    public:CSPXMessage();
    public:CSPXMessage(int initSize);
    public:~CSPXMessage();

    //usage count set by get and put functions of the message container
    private:unsigned int m_usageCount;
    public:void SetUsageCount(unsigned int usageCount) { m_usageCount = usageCount; }
    public:unsigned int GetUsageCount() { return m_usageCount; }
    public:void IncUsageCount() { m_usageCount++; }
    public:void DecUsageCount() { if (m_usageCount > 0) m_usageCount--; }

    //overhead to determine whom the message is intended for
    private:char *m_deviceName;
    public:char* GetDeviceName() 
    { 
        return m_deviceName; 
    }
    public:void SetDeviceName(char* dev) 
    { 
        if (m_deviceName != nullptr)
            delete m_deviceName;

        m_deviceName = (char*)new char[strlen(dev)+1];
        strcpy(m_deviceName,dev); 
    }

    private:CSPXString m_data;
    public:unsigned int GetDataLength() { return m_data.Length(); }
    public:const char* GetData() { return m_data.GetString(); }
    public:void SetData(const char* data)
    { 
        m_data = data;
    }
    public:void SetData(const char* data, int len)
    {
        m_data = data;
        m_data.GetString()[len] = '\0';
    }

    public:void Clear()
    { 
        m_data = "";
    }
    
    char operator [] (int i) { return m_data[i]; }
};
