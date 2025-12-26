#include "cspxutility.h"

#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <cstring>
#include <cstdlib>

#include "cspxutility.h"


double MachEps()
{
    double macheps = 1.,eps = 2.;
    while(eps != 1.)
    {
        macheps /= 2.;
        eps = 1. + macheps; // in simple rounded form
    }
    return macheps*2.;
}

double MACH_EPS = MachEps();

void Message(char *myFormat,...)
{
    va_list argPointer;
    va_start(argPointer,myFormat);
    vfprintf(stderr,myFormat,argPointer);
    va_end(argPointer);
}

void Error(char *myFormat,...)
{
    va_list argPointer;
    va_start(argPointer,myFormat);
    vfprintf(stderr,myFormat,argPointer);
    va_end(argPointer);
    exit(1);
}

double Control(double value)
{
    if(value > MAX_DBL)
        return MAX_DBL;
    else if(value < -MAX_DBL)
        return -MAX_DBL;
    return value;
}

double DegreesToRadians(double angle)
{
    return 1.0;
}

double RadiansToDegrees(double angle)
{
    return 1.0;
}

// Swap double
void Swap(double *x,double *y) {
    double temp = *x; *x = *y; *y = temp;
}

// Swap int
void Swap(int *x,int *y)  {
    int temp = *x; *x = *y; *y = temp;
}

// Swap double pointers
// Swap(&p1,&p2);
void Swap(double **x,double **y)  {
    double *temp = *x; *x = *y; *y = temp;
}

// Swap int pointers
// Swap(&p1,&p2);
void Swap(int **x,int **y)  {
    int *temp = *x; *x = *y; *y = temp;
}

double Max(int n,double *x,int &im)
{
    if(n < 0)
        return x[0];
    double temp = x[0];
    im = 0;
    for(int i = 1; i < n; i++)
        if(temp < x[i])
        {
            temp = x[i];
            im = i;
        }
    return temp;
}

double MaxAbs(int n,double *x,int &im)
{
    double temp = Abs(x[0]);
    if(n < 0)
        return temp;
    im = 0;
    for(int i = 1; i < n; i++)
        if(temp < Abs(x[i]))
        {
            temp = Abs(x[i]);
            im = i;
        }
    return temp;
}

double Min(int n,double *x,int &im)
{
    if(n < 0)
        return x[0];
    double temp = x[0];
    im = 0;
    for(int i = 1; i < n; i++)
        if(temp > x[i])
        {
            temp = x[i];
            im = i;
        }
    return temp;
}


double MinAbs(int n,double *x,int &im)
{
    double temp = Abs(x[0]);
    if(n < 0)
        return temp;
    im = 0;
    for(int i = 1; i < n; i++)
        if(temp > Abs(x[i]))
        {
            temp = Abs(x[i]);
            im = i;
        }
    return temp;
}

void Sum(int n,double *lval,double *rval,double *result)
{
    double sum;
    for(int i=0; i < n; i++)
    {
        sum = (*lval++) + (*rval++);
        *result++ = Control(sum);
    }
}

void Sum(int n,double *lvalAndResult,double *rval)
{
    double sum;
    for(int i=0; i < n; i++)
    {
        sum = (*lvalAndResult) + (*rval++);
        *lvalAndResult++ = Control(sum);
    }
}

void Sum(int n,double *lvalRvalAndResult)
{
    double sum;
    for(int i=0; i < n; i++)
    {
        sum = (*lvalRvalAndResult) + (*lvalRvalAndResult);
        *lvalRvalAndResult++ = Control(sum);
    }
}

void Difference(int n,double *lval,double *rval,double *result)
{
    double diff;
    for(int i=0; i < n; i++)
    {
        diff = (*lval++) - (*rval++);
        *result++ = Control(diff);
    }
}

void Difference(int n,double *lvalAndResult,double *rval)
{
    double diff;
    for(int i=0; i < n; i++)
    {
        diff = (*lvalAndResult) - (*rval++);
        *lvalAndResult++ = Control(diff);
    }
}

void Difference(int n,double *lval,double *rvalAndResult,int)
{
    double diff;
    for(int i=0; i < n; i++)
    {
        diff = (*lval++) - (*rvalAndResult);
        *rvalAndResult++ = Control(diff);
    }
}

double Dot(int n,double *lval,double *rval)
{
    double result = 0.;
    for(int i=0; i < n; i++)
        result += (*lval++) * (*rval++);
    return Control(result);
}

void Product(int n,double lval,double *rval,double *result)
{
    for(int i=0; i < n; i++)
        *result++ = Control(lval * (*rval++));
}

void Product(int n,double lval,double *rvalAndResult)
{
    for(int i=0; i < n; i++)
    {
        *rvalAndResult = Control(lval * (*rvalAndResult));
        rvalAndResult++;
    }
}

void Division(int n,double *lval,double rval,double *result)
{
    if(rval == 0.)
        rval = MIN_DBL;
    for(int i=0; i < n; i++)
        *result++ = Control((*lval++)/rval);
}

void Division(int n,double *lval,double rval)
{
    if(rval == 0.)
        rval = MIN_DBL;
    for(int i=0; i < n; i++)
    {
        *lval = Control((*lval)/rval);
        lval++;
    }
}

double SqrtSumSqr(int n,double *x)
{
    if(n <= 0)
        Error((char*)"Can't take square root of 0 or negative number");
    double norm = 0.,aux;
    for(int i = 0; i < n; i++)
    {
        aux = x[i];
        norm += aux*aux;
    }
    norm=sqrt(norm);
    if(norm > MAX_DBL)
        norm = MAX_DBL;

    return norm;
}


void Sort(int n,double *x)
{
    if(n <= 1)
        return;
    int node,i,j,k,ik,jk;
    for(node = 1; node < n; node++)
    {
        i = node;
        j = ((i+1)/2)-1;
        while(i != 0 && x[j] <= x[i])
        {
            Swap(x+j,x+i);
            i=j;
            j=((i+1)/2)-1;
        }
    }
    for(i = n-1; i >= 1; i--)
    {
        Swap(x+i,x);
        k = i-1;
        ik = 0;
        jk = 1;
        if(k >= 2 && x[2] > x[1])
            jk=2;
        while(jk <= k && x[jk] > x[ik])
        {
            Swap(x+jk,x+ik);
            ik = jk;
            jk = (2*(ik+1))-1;
            if(jk+1 <= k)
                if(x[jk+1] > x[jk])
                    jk++;
        }
    }
}

CSPXUtility::CSPXUtility(void)
{
}

CSPXUtility::~CSPXUtility(void)
{
}

char* CSPXUtility::GetStrDate(time_t t,char *strdate)
{
    struct tm *local {nullptr};
    local = gmtime(&t);
    char str_date[26];
    strcpy(str_date,asctime(local));
    if (strdate)
        strncpy(strdate,str_date,24);
    return strdate;
}

char* CSPXUtility::GetStrToday(char *strdate)
{
    struct tm *local {nullptr};
    time_t t;

    t = time(nullptr);
    local = gmtime(&t);
    char str_date[26];
    strcpy(str_date,asctime(local));
    str_date[24] = 0;
    if (strdate)
        strncpy(strdate,str_date,24);
    return strdate;
}

char* CSPXUtility::SetString(char** dest, char* name)
{
    if (name == nullptr)
        return nullptr;

    if (*dest != nullptr)
        delete *dest;

    *dest = new char[strlen(name)+1];
    strcpy(*dest,name);

    return *dest;
}

char* CSPXUtility::asciiToHex(char *acsiiStr)
{
    size_t hexLen = strlen(acsiiStr)/2;
    char *hexStr = new char[hexLen];

    for (size_t i = 0; i < strlen(acsiiStr); i+=2)
    {
        char hl = (acsiiStr[i]-48);
        if (hl > 10)
            hl = (acsiiStr[i]-48-7);

        char hu = (acsiiStr[i+1]-48);
        if (hu > 10)
            hu = (acsiiStr[i+1]-48-7);

        char h = (hl<<4) + hu;

        hexStr[i/2] = h;
    }

    return hexStr;
}

long CSPXUtility::binaryToDecimal(char *binaryStr)
{
    int decimal = 0;

    int shift = 0;
    for (size_t i = strlen(binaryStr)-1; i >= 0; i--)
    {
        if (binaryStr[i] == '1')
            decimal |= (1<<shift);

        shift++;
    }

    return decimal;
}

long long CSPXUtility::hexToDecimal(char *hexStr)
{
    size_t l = strlen(hexStr);
    int iinit = 0;
    if (hexStr[0] == '0' && (hexStr[1] == 'x' || hexStr[1] == 'X'))
    {
        iinit = 2;
    }
    long long dec = 0;


    int j = 0;
    unsigned long long d;
    unsigned long long ld;
    for (size_t i = l-1; i > iinit; i-=2)
    {
        d = getDecimalValue(&hexStr[i-1]);
        ld = d<<(8*j);
        dec += d<<(8*j);
        j++;
        if (i==1)
            break;
    }
    return dec;
}

bool CSPXUtility::isNumeric(const char *str)
{
    if (str == nullptr)
        return false;

    for (size_t i = 0; i < strlen(str); i++)
    {
        char test = str[i];
        switch(test)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        default:
            return false;
        }
    }

    return true;
}

void CSPXUtility::strip(char *str,char ch)
{
    char *tmp = new char[strlen(str)+1];
    memset(tmp,0,strlen(str)+1);

    int j = 0;
    for (size_t i = 0; i < strlen(str); i++)
    {
        char test = str[i];
        if (test != ch)
            tmp[j++] = str[i];
    }

    memset(str,0,strlen(str));

    strcpy(str, tmp);

    delete[] tmp;
}

int CSPXUtility::getDecimalValue(char *hexCode)
{
    int retVal = 0;
    int nibble = 0;

    //validate

    for (int i = 0; i < 2; i++)
    {
        char test = hexCode[i];
        switch(test)
        {
        case '0':
            nibble = 0;
            break;
        case '1':
            nibble = 1;
            break;
        case '2':
            nibble = 2;
            break;
        case '3':
            nibble = 3;
            break;
        case '4':
            nibble = 4;
            break;
        case '5':
            nibble = 5;
            break;
        case '6':
            nibble = 6;
            break;
        case '7':
            nibble = 7;
            break;
        case '8':
            nibble = 8;
            break;
        case '9':
            nibble = 9;
            break;
        case 'A':
        case 'a':
            nibble = 10;
            break;
        case 'B':
        case 'b':
            nibble = 11;
            break;
        case 'C':
        case 'c':
            nibble = 12;
            break;
        case 'D':
        case 'd':
            nibble = 13;
            break;
        case 'E':
        case 'e':
            nibble = 14;
            break;
        case 'F':
        case 'f':
            nibble = 15;
            break;

        default:
            nibble = 0;
            break;

        }

        retVal += nibble<<(4*(1-i));
    }

    return retVal;
}

