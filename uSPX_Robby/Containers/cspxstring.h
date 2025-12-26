#ifndef CSPXSTRING_H
#define CSPXSTRING_H

#pragma once

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <string>
#include <QString>

#include "../Containers/cspxlist.h"

class CSPXString
{
    private:char* m_string { nullptr };
    public:unsigned int m_initSize { 0 };
    
    public: static CSPXString IntToStr(int number) { CSPXString nbr(""); nbr.Convert(number); return nbr; }
    public: static CSPXString DoubleToStr(double number) { CSPXString nbr(""); nbr.Convert(number); return nbr; }

    public:CSPXString()
    {
    }

    public:CSPXString(const std::string& str)
    {
        SetString(str.c_str());
    }

    public:CSPXString(CSPXString& str)
    {
        m_initSize=str.m_initSize;
        if (m_initSize > 0)
        {
            m_string = new char[m_initSize];
            memset(m_string,0,m_initSize);
        }
        SetString(str.GetString());
    }

    public:CSPXString(unsigned long initSize):m_string(nullptr),m_initSize(initSize)
    {
        m_string = new char[m_initSize];
        if (m_initSize > 0)
            memset(m_string,0,m_initSize);
    }

    /*public:CSPXString(char *str):m_string(NULL),m_initSize(0)
    {
        SetString(str);
    }*/

    public:CSPXString(const char *str)
    {
        SetString((char*)str);
    }

    public:CSPXString(std::string &str)
    {
        SetString(str.c_str());
    }

    public:CSPXString(QString &str)
    {
        SetString(str.toStdString().c_str());
    }

    public:CSPXString(char *str, unsigned long len)
    {
        SetString(str,len);
    }

    public:virtual ~CSPXString(void)
    {
        if (m_string)
            delete m_string;
    }

    public: CSPXString& Convert(std::string str)
    {
        SetString(str.c_str());

        return *this;
    }

    public: std::string Convert2STDString()
    {
        std::string str(this->m_string);

        return str;
    }

    public: QString Convert2QString()
    {
        QString str(this->m_string);

        return str;
    }

    public: CSPXString& Convert(int number)
    {
        char txt[16];
        sprintf(txt,"%d",number);
        Concat(txt);

        return *this;
    }

    public: CSPXString& Convert(long number)
    {
        char txt[16];
        sprintf(txt,"%ld",number);
        Concat(txt);

        return *this;
    }

    public: CSPXString& Convert(double number, const char *fmt = "%06.3f")
    {
        char txt[16];
        sprintf(txt,fmt,number);
        Concat(txt);

        return *this;
    }

    public: CSPXString& Remove(const char c)
    {
        if (c == '\0')
            return *this;

        char *sz_temp = (char*)new char[Length()];
        memset(sz_temp,0,Length());
        char *temp = sz_temp;
        char *psz = m_string;
        for (unsigned int i = 0; i < Length(); i++)
        {
            if (*psz != c)
            {
                *temp = *psz;
                temp++;
            }


            psz++;

        }

        SetString(sz_temp);
        delete sz_temp;

        return *this;
    }



    public:char* Append(const char *text, int length)
    {
        if (text == nullptr)
            return m_string;

        if (m_string == nullptr)
        {
            SetString((char*)text,length);
            return m_string;
        }

        char *newstring = new char[strlen(m_string)+length+m_initSize+1];
        memset(newstring,0,strlen(m_string)+length+m_initSize+1);
        strcpy(newstring,m_string);
        strncat(newstring,text,length);
        delete m_string;
        m_string = newstring;
        return m_string;
    }

    public:char* Concat(const char *text)
    {
        if (text == nullptr)
            return m_string;

        if (m_string == nullptr)
        {
            SetString((char*)text);
            return m_string;
        }

        char *newstring = new char[strlen(m_string)+strlen(text)+m_initSize+1];
        memset(newstring,0,strlen(m_string)+strlen(text)+m_initSize+1);
        strcpy(newstring,m_string);
        strcat(newstring,text);
        delete m_string;
        m_string = newstring;
        return m_string;
    }

    public:char* GetString() { return m_string; }

    public:void SetString(const char* string)
    {
        if (m_string != nullptr)
            delete m_string;

        m_string = nullptr;

        if (string == nullptr)
            return;

        m_string = new char[strlen(string)+m_initSize+1];
        memset(m_string,0,strlen(string)+m_initSize+1);
        strcpy(m_string,string);
    }

    public:void SetString(char* string, unsigned int maxLen)
    {
        if (m_string != nullptr)
            delete m_string;

        m_string = nullptr;

        if (string == nullptr)
            return;

        if (m_string != nullptr)
            delete m_string;

        size_t minl = (strlen(string)>maxLen)?maxLen:strlen(string);
        m_string = new char[minl+m_initSize+1];
        memset(m_string,0,minl+m_initSize+1);
        strncpy(m_string,string,minl);
        m_string[minl] = '\0';
    }

    public:CSPXString& SubStr(unsigned int iidx, int fidx)
    {
        CSPXString str(Length());
        if (fidx < 0)
            strcpy(str,GetString());
        else
            strncpy(str,GetString(),(unsigned int)(fidx-iidx+1));

        return str;
    }

    public: unsigned int Length() const
    {
        if (m_string==nullptr)
            return 0;

        size_t sl = strlen(m_string);

        if ((unsigned long long)m_initSize > sl)
        {
            for (int i = m_initSize - 1; i >= 0; i--)
            {
                if (m_string[i] != 0)
                    return i+1;
            }
        }
        else
            return (unsigned int )sl;
    }

    public: size_t Size() const
    {
        if (m_string==nullptr)
            return m_initSize;

        if (m_initSize > strlen(m_string))
            return m_initSize;

        return strlen(m_string);
    }

    public:CSPXString& operator=(const char* string)
    {
        SetString((char*)string);
        return *this;
    }

    public:CSPXString& operator=(char* string)
    {
        SetString(string);
        return *this;
    }

    public:CSPXString& operator=(CSPXString& string)
    {
        SetString(string);
        return *this;
    }

    public:CSPXString& operator=(std::string& string)
    {
        SetString(string.c_str());
        return *this;
    }

    public:CSPXString& operator=(QString& string)
    {
        SetString(string.toStdString().c_str());
        return *this;
    }
    //cannot change member
    public:char& operator[] (const int index) const
    {
        if (index > strlen(m_string)-1)
            throw;

        return m_string[index];
    }

    public:CSPXString& operator+=(char* string)
    {
        if (string)
            Concat(string);
        return *this;
    }

    public:CSPXString& operator+=(const char* string)
    {
        if (string)
            Concat(string);
        return *this;
    }

    public:CSPXString& operator+=(std::string &string)
    {
        if (string.length()>0)
            Concat(string.c_str());
        return *this;
    }

    public:CSPXString& operator+=(QString &string)
    {
        if (string.length()>0)
            Concat(string.toStdString().c_str());
        return *this;
    }

    public:CSPXString& operator+=(char ch)
    {
        char c[2];
        c[0] = ch;
        c[1] = 0;
        Concat(c);
        return *this;
    }

    public:CSPXString operator+(char* string)
    {
        CSPXString cp;
        if (string)
            cp.Concat(string);
        return cp;    }

    public:CSPXString operator+(const char* string)
    {
        CSPXString cp;
        if (string)
            cp.Concat(string);
        return cp;
    }

    //conversion operator to include standard c function ,ie strcmp(...,...)
    public:operator char* ()
    {
        return m_string;
    }

    //conversion operator to include standard c function ,ie strcmp(...,...)
    public:operator const char* ()
    {
        return (const char*)m_string;
    }

    //comparator operators
    public: bool operator == (const char* cstr)
    {
        if (cstr == nullptr)
        {
            if (m_string == nullptr)
                return true;
            else
                return false;
        }
        if (m_string == nullptr)
        {
            if (m_string != cstr)
                return false;
            else
                return true;
        }
        return !strcmp(m_string,cstr);
    }

    public: bool operator == (std::string &str)
    {
        if (!str.empty())
        {
            if (m_string == nullptr)
                return true;
            else
                return false;
        }
        if (m_string == nullptr)
        {
            if (m_string != str)
                return false;
            else
                return true;
        }
        return !strcmp(m_string,str.c_str());
    }
    public: bool operator != (const char* cstr)
    {
        if (m_string == nullptr)
        {
            if (m_string != cstr)
                return true;
            else
                return false;
        }
        return strcmp(m_string,cstr);
    }
    public: bool operator != (char* cstr)
    {
        if (m_string == nullptr)
        {
            if (m_string != cstr)
                return true;
            else
                return false;
        }
        return strcmp(m_string,cstr);
    }

public: bool operator != (std::string &str)
    {
        if (m_string == nullptr)
        {
            if (m_string != str)
                return true;
            else
                return false;
        }
        return strcmp(m_string,str.c_str());
    }

    friend CSPXString operator+(CSPXString &cstring, char* string);

    friend CSPXString operator+(CSPXString &cstring1, CSPXString &cstring2);

    friend CSPXString operator+(CSPXString &cstring1, std::string &cstring2);

    friend CSPXString operator+(CSPXString &cstring1, QString &cstring2);

    friend CSPXString& operator+=(CSPXString &cstring1, CSPXString &cstring2);

    friend CSPXString& operator+=(CSPXString &cstring1, std::string &cstring2);

    friend CSPXString& operator+=(CSPXString &cstring1, QString &cstring2);
};


class CSPXStringList : public CSPXList<CSPXString>
{

    public:CSPXStringList(): CSPXList<CSPXString>() {} ;
    public:void Parse(char *text, char delimiter)
    {
       if (text == nullptr)
           return;

        char *ptext = text;
        char *nexttoken = strchr(ptext,delimiter);
        size_t maxlen = strlen(text);
        while(nexttoken - ptext > 0 && nexttoken - text < maxlen)
        {
            /*CSPXString*/
            auto *string = new CSPXString();
            string->SetString(ptext,nexttoken - ptext);
            Append(string,true);

            //advance to next token
            ptext += nexttoken - ptext + 1;
            nexttoken = strchr(ptext,delimiter);
        }
    }
};

#endif // CSPXSTRING_H
