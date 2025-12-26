#include "cspxstring.h"

//friend functions used by CSPXString object

CSPXString operator+(CSPXString &cstring, char* string)
{
    CSPXString cp = cstring;
    if (string)
        cp.Concat(string);
    return cp;
}

CSPXString operator+(CSPXString &cstring1, CSPXString &cstring2)
{
    CSPXString cp = cstring1;
    cp.Concat(cstring2);
    return cp;
}

CSPXString operator+(CSPXString &cstring1, std::string &cstring2)
{
    CSPXString cp = cstring1;
    cp.Concat(cstring2.c_str());
    return cp;
}

CSPXString operator+(CSPXString &cstring1, QString &cstring2)
{
    CSPXString cp = cstring1;
    cp.Concat(cstring2.toStdString().c_str());
    return cp;
}

CSPXString& operator+=(CSPXString &cstring1, CSPXString &cstring2)
{
   cstring1.Concat(cstring2);
   return cstring1;
}

CSPXString& operator+=(CSPXString &cstring1, std::string &cstring2)
{
   cstring1.Concat(cstring2.c_str());
   return cstring1;
}

CSPXString& operator+=(CSPXString &cstring1, QString &cstring2)
{
   cstring1.Concat(cstring2.toStdString().c_str());
   return cstring1;
}
