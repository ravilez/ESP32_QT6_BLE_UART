#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "cspxutility.h"
#include "cspxTVector.h"
#include "cspxTMatrix.h"

template<class T> T CSPXTVector<T>::SqrtSumSqr(int n, int m)
{
	if(n <= 0)
        Error((char*)"");

	T norm = 0.0;
	for(int i = 0; i < n; i++)
	{
		norm += vector[m]*vector[m];
		m++;
	}

	return sqrt(norm);
}

template<class T> void CSPXTVector<T>::Initialize(int size)
{
    if(size < 1)
	{
		dimension = 0;
		vector = nullptr;
	}
	else
	{
		dimension = size;
		vector = new T[size]; 
		if(!vector)
          Error((char*)"");
	}
}

template<class T> CSPXTVector<T>::CSPXTVector()
{
	Initialize(0);
}

template<class T> CSPXTVector<T>::CSPXTVector(const CSPXTVector &rval)
{
	Initialize(rval.dimension);
	if(dimension != 0)
		memcpy(vector,rval.vector,dimension*sizeof(T));
}

template<class T> CSPXTVector<T>::CSPXTVector(int nc)
{
	Initialize(nc);
	if(dimension != 0)
		memset(vector,0,dimension*sizeof(T));
}

template<class T> CSPXTVector<T>::CSPXTVector(int nc, T v1,...)
{
	Initialize(nc);
	T *w = vector;
	va_list puntaList;
	va_start(puntaList,v1);
	int i;
	*w = Control(v1);
	for(i = 1;i < nc;i++)
	  *++w = Control(va_arg(puntaList, T));
	va_end(puntaList);
}

template<class T> CSPXTVector<T>::CSPXTVector(int nc, T *initvalues)
{
	Initialize(nc);
	if(dimension != 0)
	{
		T *w = vector;
		w++;
		memcpy(w,initvalues,nc*sizeof(T));
	}
}

template<class T> CSPXTVector<T>::~CSPXTVector()
{
	delete vector;
}

// assigment operator
template<class T> CSPXTVector<T> &CSPXTVector<T>::operator = (const CSPXTVector<T> &rval)
{
	if(dimension != rval.dimension)
	{
		delete vector;
		Initialize(rval.dimension);
	}
	memcpy(vector,rval.vector,dimension*sizeof(T));
	return *this;
}

// equality operator
template<class T> bool operator == (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval)
{
	if ( lval.GetDimension() != rval.GetDimension() )
		return false;
	else
		// compare up to specified precision
		for (int i = 0; i < lval.GetDimension(); i++)
			if ( abs(lval[i] - rval[i]) > MACH_EPS)
				return false;
	return true;
}

template<class T> CSPXTVector<T> operator + (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval)
{
    CSPXTVector<T> result(lval.dimension);
	for (int i = 0; i < lval.dimension; i++)
		result[i] = lval[i] + rval[i];
	return result;
}

template<class T> CSPXTVector<T> &CSPXTVector<T>::operator += (const CSPXTVector<T> &rval)
{
	if(dimension != rval.dimension)
        Error((char*)"");
	for (int i = 0; i < rval.dimension; i++)
		(*this)[i] = (*this)[i] + rval[i];
	return *this;
}

template<class T> CSPXTVector<T> operator - (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval)
{
    CSPXTVector<T> result(lval.dimension);
	for (int i = 0; i < lval.dimension; i++)
		result[i] = lval[i] - rval[i];
	return result;
}

template<class T> CSPXTVector<T> &CSPXTVector<T>::operator -= (const CSPXTVector<T> &rval)
{
	if(dimension != rval.dimension)
       Error((char*)"");
	for (int i = 0; i < rval.dimension; i++)
		(*this)[i] = (*this)[i] - rval[i];
	return *this;
}

template<class T> void CSPXTVector<T>::Negate()
{
	for(int i = 0; i < dimension; i++)
		(*this)[i] = -(*this)[i];
}    

template<class T> CSPXTVector<T> CSPXTVector<T>::operator -() // unary minus
{
    CSPXTVector<T> result(dimension);
	for(int i = 0; i < dimension; i++)
		result[i] = -(*this)[i];
	return result;
}

template<class T> CSPXTVector<T> operator * (const CSPXTMatrix<T> &lval,const CSPXTVector<T> &rval)
{
    CSPXTVector<T> result(lval.Rows());
	T temp;
	for (int j = 0; j < lval.Rows(); j++)
	{
		temp = 0;
		for (int i = 0; i < lval.Columns(); i++)
			temp += lval[j][i]*rval[i];
		
		result[j] = temp;
	}

	return result;
}

template<class T> CSPXTVector<T> operator * (T lval,const CSPXTVector<T> &rval)
{
    CSPXTVector<T> result(rval.dimension);
	for (int i = 0; i < rval.dimension; i++)
		result[i] = lval*rval[i];
	return result;
}

template<class T> CSPXTVector<T> &CSPXTVector<T>::operator *= (T rval)
{
	for (int i = 0; i < GetDimension(); i++)
		(*this)[i] = (*this)[i]*rval;
	return *this;
}

template<class T> T CSPXTVector<T>::Dot(const CSPXTVector<T> &rval)
{
	T result = 0;
	for (int i = 0; i < rval.dimension; i++)
		result += (*this)[i]*rval[i];
	return result;
}

template<class T> T operator % (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval)
{
	T result;
	return result;
}

template<class T> CSPXTVector<T> operator % (const CSPXTMatrix<T> &lval,const CSPXTVector<T> &rval)
{
    CSPXTVector<T> result(lval.numColumns);
	return result;
}

template<class T> CSPXTMatrix<T> operator ->* (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval)
{
    CSPXTMatrix<T> result(lval.dimension,lval.dimension);

	return result;
}

template<class T> CSPXTVector<T> operator / (const CSPXTVector<T> &lval, T rval)
{
    CSPXTVector<T> result(lval.dimension);
	return result;
}

template<class T> CSPXTVector<T> &CSPXTVector<T>::operator /= (T rval)
{
	return *this;
}

template<class T> T CSPXTVector<T>::Norm()
{
	T norm = 0.0;
	for(int i = 0;i < dimension;i++)
		norm += (*this)[i]*(*this)[i];
	return sqrt(norm);
}

template<class T> void CSPXTVector<T>::Normalize()
{
	T norm = 0;
	for(int i = 0;i < dimension;i++)
		norm = Max(norm,(*this)[i]);
	if(norm == 0.0)
		return;
    for(int i = 0; i < dimension; i++)
		(*this)[i] /= norm;
}

template<class T> void CSPXTVector<T>::Order()
{
}

