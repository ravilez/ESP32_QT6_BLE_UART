#ifndef CSPXUTILITY_H
#define CSPXUTILITY_H

#pragma once

#include <math.h> // for inline functions
#include <time.h>


extern double MACH_EPS;

constexpr double MAX_DBL {1.7e+30};
constexpr double MIN_DBL {8.e-30};
//const long double MAX_LDBL = 1.7e+30;
//const long double MIN_LDBL = 8.e-30;
constexpr int MAX_INT {2147483647};
constexpr int MIN_INT {-2147483647};

inline double Abs(double a) {
    return fabs(a);
}
inline int Abs(int i)   {
    return abs(i);
}

inline double Max(double a,double b) {
    return (a > b ? a : b);
}
inline double Min(double a,double b) {
    return (a < b ? a : b);
}
inline int Max(int l,int k)  {
    return (l > k ? l : k);
}
inline int Min(int l,int k)  {
    return (l < k ? l : k);
}
inline double MaxAbs(double a,double b) {
    return (fabs(a) > fabs(b) ? fabs(a) : fabs(b));
}
inline double MinAbs(double a,double b) {
    return (fabs(a) < fabs(b) ? fabs(a) : fabs(b));
}
inline int MaxAbs(int l,int k) {
    return (abs(l) > abs(k) ? abs(l) : abs(k));
}
inline int MinAbs(int l,int k) {
    return (abs(l) < abs(k) ? abs(l) : abs(k));
}

double MachEps(void);
void Message(char *myFormat,...);
void Error(char *myFormat,...);

double Control(double value);
//double Control(long double value);

double DegreesToRadians(double angle);
double RadiansToDegrees(double angle);

void Swap(double *x,double *y);
void Swap(int *x,int *y);
void Swap(double **x,double **y);
void Swap(int **x,int **y);

double Max(int n,double *x,int &imax);
double MaxAbs(int n,double *x,int &imax);
double Min(int n,double *x,int &imax);
double MinAbs(int n,double *x,int &imax);

void Sum(int n,double *lval,double *rval,double *result);
void Sum(int n,double *lvalAndResult,double *rval);
void Sum(int n,double *lvalRvalAndResult);
void Difference(int n,double *lval,double *rval,double *result);
void Difference(int n,double *lvalAndResult,double *rval);
void Difference(int n,double *lval,double *rvalAndResult,int);
double Dot(int n,double *lval,double *rval);
void Product(int n,double lval,double *rval,double *result);
void Product(int n,double lval,double *rvalAndResult);
void Division(int n,double *lval,double rval,double *result);
void Division(int n,double *lvalAndResult,double rval);

double SqrtSumSqr(int n,double *x);

void Sort(int n,double *x);


enum EClipboardFormat
{
    SPX_CF_TEXT  = 1,
    SPX_CF_BITMAP = 2,
    SPX_CF_METAFILEPICT = 3,
    SPX_CF_SYLK = 4,
    SPX_CF_DIF = 5,
    SPX_CF_TIFF = 6,
    SPX_CF_OEMTEXT = 7,
    SPX_CF_DIB = 8,
    SPX_CF_PALETTE = 9,
    SPX_CF_PENDATA = 10,
    SPX_CF_RIFF = 11,
    SPX_CF_WAVE = 12,
    SPX_CF_UNICODETEXT = 13,
    SPX_CF_ENHMETAFILE = 14,
    SPX_CF_HDROP = 15,
    SPX_CF_LOCALE = 16,
    SPX_CF_DIBV5 = 17,
    SPX_CF_MAX = 18,
    SPX_CF_OWNERDISPLAY = 0x0080,
    SPX_CF_DSPTEXT = 0x0081,
    SPX_CF_DSPBITMAP = 0x0082,
    SPX_CF_DSPMETAFILEPICT = 0x0083,
    SPX_CF_DSPENHMETAFILE = 0x008E,
    SPX_CF_PRIVATEFIRST = 0x0200,
    SPX_CF_PRIVATELAST = 0x02FF,
    SPX_CF_GDIOBJFIRST = 0x0300,
    SPX_CF_GDIOBJLAST = 0x03FF,
};

class CSPXUtility
{
public: CSPXUtility(void);
public: ~CSPXUtility(void);

public: static size_t b64decode(char const *src, size_t srcLen, void *dest, size_t destSize);

public: static char *SetString(char** dest, char* name);
public: static char* asciiToHex(char *acsiiStr);
public: static long binaryToDecimal(char *binaryStr);
public: static long long hexToDecimal(char *hexStr);
private: static int getDecimalValue(char *hexCode);
public: static char* GetStrToday(char* strdata);
public: static char* GetStrDate(time_t t,char* strdata);
public: static long GetCurrentTime() {
        return clock();
}
public: static bool isNumeric(const char *str);
public: static void strip(char *str,char ch);
};

#endif // CSPXUTILITY_H
