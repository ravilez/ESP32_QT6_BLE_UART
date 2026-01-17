#include "cspxvector.h"

#include <math.h>
#include <stdarg.h>
#include <cstring>
#include <stdlib.h>

#include "cspxutility.h"
#include "cspxvector.h"
#include "cspxmatrix.h"

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif



struct vector4
{
    float x;
    float y;
    float z;
    float t;
};


double CSPXVector::SqrtSumSqr(int n, int m)
{
    if(n <= 0)
        Error((char*)"");

    double norm = 0.0;

    /*if (dimension == 4 && n == 3 && m == 0)
       {
        vector[3] = 0.0f;
        __m128 *sq = (__m128*)vector;
        __m128 r = _mm_sqrt_ps(*sq);
        return r.m128_f32[0];
       }
       else*/
    {
        for(int i = 0; i < n; i++)
        {
            norm += vector[m]*vector[m];
            m++;
        }
        norm = (double)sqrt(norm);
    }
    return norm;
}

void CSPXVector::Initialize(int size)
{
    if(size < 1)
    {
        dimension = 0;
        vector = nullptr;
    }
    else
    {
        dimension = size;
        //vector = (double*)malloc(size * sizeof(double));
        vector = new double[size];//new double[size]; if overloaded new operator, multiply with sizeof(type)
        if(!vector)
            Error((char*)"");
    }
}

CSPXVector::CSPXVector()
{
    Initialize(0);
}

CSPXVector::CSPXVector(const CSPXVector &rval)
{
    Initialize(rval.dimension);
    if(dimension != 0)
        memcpy(vector,rval.vector,dimension*sizeof(double));
}

CSPXVector::CSPXVector(int nc)
{
    Initialize(nc);
    if(dimension != 0)
        memset(vector,0,dimension*sizeof(double));
}

CSPXVector::CSPXVector(int nc, double *initvalues)
{
    Initialize(nc);
    if(dimension != 0)
    {
        double *w = vector;
        w++;
        memcpy(w,initvalues,nc*sizeof(double));
    }
}

CSPXVector::CSPXVector(int nc, double v1,...)
{
    Initialize(nc);
    if(dimension != 0)
    {
        va_list vl;
        va_start(vl,v1);
        (*this)[0] = v1;
        for (int i=1; i<nc; i++)
            //(*this)[i] = va_arg(vl,double);
            va_end(vl);
    }
}

CSPXVector::CSPXVector(double v1)
{
    Initialize(1);
    if(dimension != 0)
    {
        (*this)[0] = v1;
    }
}

CSPXVector::CSPXVector(double v1, double v2)
{
    Initialize(2);
    if(dimension != 0)
    {
        (*this)[0] = v1;
        (*this)[1] = v2;
    }
}

CSPXVector::CSPXVector(double v1, double v2, double v3)
{
    Initialize(3);
    if(dimension != 0)
    {
        (*this)[0] = v1;
        (*this)[1] = v2;
        (*this)[2] = v3;
    }
}

CSPXVector::~CSPXVector()
{
    if (vector == nullptr)
        return;

    //free(vector);
    delete[] vector;
    vector = nullptr;
}

// assigment operator
CSPXVector &CSPXVector::operator = (const CSPXVector &rval)
{
    if(dimension == 0 || dimension < rval.dimension)
    {
        //free(vector);
        delete[] vector;
        Initialize(rval.dimension);
    }
    memcpy(vector,rval.vector,rval.dimension*sizeof(double));
    return *this;
}

void CSPXVector::Matrix_Vector_Mult(float vector[4], float matrix[4][4], int vectorLength, float results[4])
{
    float *ptr1=matrix[0];
    float *ptr2=matrix[1];
    float *ptr3=matrix[2];
    float *ptr4=matrix[3];

    float temp1=0;
    float temp2=0;
    float temp3=0;
    float temp4=0;

    for (int j = 0; j< vectorLength; j++)
    {
        temp1 += ptr1[j] * vector[j];
        temp2 += ptr2[j] * vector[j];
        temp3 += ptr3[j] * vector[j];
        temp4 += ptr4[j] * vector[j];
    }
    results[0]=temp1;
    results[1]=temp2;
    results[2]=temp3;
    results[3]=temp4;
}


// equality operator
bool operator == (const CSPXVector &lval,const CSPXVector &rval)
{
    if ( lval.GetDimension() != rval.GetDimension() )
        return false;
    else
        // compare up to specified precision
        for (int i = 0; i < lval.GetDimension(); i++)
            if ( fabs(lval[i] - rval[i]) > MACH_EPS)
                return false;
    return true;
}

CSPXVector operator + (const CSPXVector &lval,const CSPXVector &rval)
{
    CSPXVector result(lval.dimension);
    for (int i = 0; i < lval.dimension; i++)
        result[i] = lval[i] + rval[i];
    return result;
}

CSPXVector &CSPXVector::operator += (const CSPXVector &rval)
{
    int maxrows = rval.dimension;
    if(dimension != rval.dimension)
    {
        //Error(""); //
        maxrows = MIN(dimension,rval.dimension);
    }

    {
        for (int i = 0; i < maxrows; i++)
            (*this)[i] = (*this)[i] + rval[i];
    }
    return *this;
}

CSPXVector operator - (const CSPXVector &lval,const CSPXVector &rval)
{
    CSPXVector result(MIN(lval.dimension,rval.dimension));
    {
        for (int i = 0; i < result.dimension; i++)
            result[i] = lval[i] - rval[i];
    }
    return result;
}

CSPXVector &CSPXVector::operator -= (const CSPXVector &rval)
{
    if(dimension != rval.dimension)
        Error((char*)"");

    {
        for (int i = 0; i < dimension; i++)
            (*this)[i] = (*this)[i] - rval[i];
    }
    return *this;
}

void CSPXVector::Negate()
{
    for(int i = 0; i < dimension; i++)
        (*this)[i] = -(*this)[i];
}

CSPXVector CSPXVector::operator - () // unary minus
{
    CSPXVector result(dimension);
    for(int i = 0; i < dimension; i++)
        result[i] = -(*this)[i];
    return result;
}

CSPXVector operator * (const CSPXMatrix &lval,const CSPXVector &rval)
{
    CSPXVector result(lval.Rows());
    double temp;
    for (int j = 0; j < lval.Rows(); j++)
    {
        temp = 0;
        for (int i = 0; i < lval.Columns(); i++)
            temp += lval[j][i]*rval[i];

        result[j] = temp;
    }

    return result;
}

CSPXVector operator * (double lval,const CSPXVector &rval)
{
    CSPXVector result(rval.dimension);
    /*if (rval.dimension==4)
       {
        result.MultiplyVector4(result.vector, rval.vector, &lval);
       }
       else*/
    {
        for (int i = 0; i < rval.dimension; i++)
            result[i] = lval*rval[i];
    }
    return result;
}

double CSPXVector::operator * (const CSPXVector &rval)
{
    double result = 0;
    for (int i = 0; i < rval.dimension; i++)
        result += (*this)[i]*rval[i];
    return result;
}

CSPXVector &CSPXVector::operator *= (double rval)
{
    for (int i = 0; i < GetDimension(); i++)
        (*this)[i] = (*this)[i]*rval;
    return *this;
}

CSPXVector CSPXVector::operator * (double rval)
{
    CSPXVector result(dimension);
    /*if (dimension==4)
       {
        MultiplyVector4(result.vector, vector, &rval);
       }
       else*/
    {
        for (int i = 0; i < dimension; i++)
            result[i] = (*this)[i]*rval;
    }
    return result;
}

double CSPXVector::Dot(const CSPXVector &rval)
{
    double result = 0;
    /*if(dimension == 4)
       {
        DotVector4(vector,rval.vector,&result);
       }
       else*/
    {
        for (int i = 0; i < MIN(this->dimension,rval.dimension); i++)
            result += (*this)[i]*rval[i];
    }
    return result;
}

CSPXVector CSPXVector::Cross(const CSPXVector &rval)
{
    // cross product for 3D or 4D homogeneous vector
    if ( rval.GetDimension() < 3 || dimension < 3)
        Error((char*)"");

    CSPXVector result(4);
    /*if (dimension == 4)
       {
        CrossProductVector4(result.vector,vector,rval.vector);
        result[3] = 0.0;
       }
       else*/
    {
        result[0] = (*this)[1]*rval[2] - (*this)[2]*rval[1];
        result[1] = (*this)[2]*rval[0] - (*this)[0]*rval[2];
        result[2] = (*this)[0]*rval[1] - (*this)[1]*rval[0];
        result[3] = 0.0;
    }
    return result;
}



double operator % (const CSPXVector &lval,const CSPXVector &rval)
{
    double result;
    return result;
}

CSPXVector operator % (const CSPXMatrix &lval,const CSPXVector &rval)
{
    // cross product for 3D or 4D homogeneous vector
    CSPXVector result(lval.numColumns);
    return result;
}

CSPXMatrix operator ->* (const CSPXVector &lval,const CSPXVector &rval)
{
    CSPXMatrix result(lval.dimension,lval.dimension);

    return result;
}

CSPXVector operator / (const CSPXVector &lval, double rval)
{
    CSPXVector result(lval.dimension);
    for(int i = 0; i < lval.dimension; i++)
        result[i] = lval[i]/rval;
    return result;
}

CSPXVector &CSPXVector::operator /= (double rval)
{
    for(int i = 0; i < dimension; i++)
        (*this)[i] = (*this)[i]/rval;
    return *this;
}

double CSPXVector::Norm()
{
    double norm = 0.0;
    for(int i = 0; i < dimension; i++)
        norm += (*this)[i]*(*this)[i];
    return (double)sqrt(norm);
}

/*
 * point on line
 */
double CSPXVector::Intersection(CSPXVector &v0, CSPXVector &v1, double &lambdax, double &lambday)
{
    // P = P1+lambda1*(P2-P1)
    lambdax = -1.0;
    lambday = -1.0;
    if ((v1[0]-v0[0]) != 0.0)
        lambdax = (vector[0]-v0[0])/(v1[0]-v0[0]);
    if ((v1[1]-v0[1]) != 0.0)
        lambday = (vector[1]-v0[1])/(v1[1]-v0[1]);

    return fabs((double)(lambday-lambdax));
}

/*
 * Intersection of 2 lines
 * this vector holds valid intersection coordinates, if 0 <= lambda <= 1
 */
void CSPXVector::Intersection(CSPXVector &v1, CSPXVector &v2, CSPXVector &v3, CSPXVector &v4, double &lambda)
{
    // P = P1+lambda1*(P2-P1) = P3+lambda2*(P4-P3)
    double x12 = (v2[0]-v1[0]); //vertical if 0
    double y12 = (v2[1]-v1[1]); //horizontal if 0
    if (y12 == 0) //horizontal
    {
        if ( (v4[1]-v3[1]) == 0)
        {
            lambda = -1;
            return;
        }
        lambda = (v1[1]-v3[1])/(v4[1]-v3[1]);
        if (lambda >= 0 && lambda <= 1)
        {
            (*this)[0] = v3[0]+lambda*(v4[0]-v3[0]);
            (*this)[1] = v1[1];
            lambda = ((*this)[0] - v1[0])/(v2[0]-v1[0]);
            return;
        }
    }
    else if (x12 == 0)
    {
        if ( (v4[0]-v3[0]) == 0)
        {
            lambda = -1;
            return;
        }
        lambda = (v1[0]-v3[0])/(v4[0]-v3[0]);
        if (lambda >= 0 && lambda <= 1)
        {
            (*this)[0] = v1[0];
            (*this)[1] = v3[1]+lambda*(v4[1]-v3[1]);
            lambda = ((*this)[1] - v1[1])/(v2[1]-v1[1]);
            return;
        }
    }

    double c1 = (v3[0] - v1[0])/x12;
    double c2 = (v3[1] - v1[1])/y12;
    double d1 = (v4[0] - v3[0])/x12;
    double d2 = (v4[1] - v3[1])/y12;
    lambda = (c2-c1)/(d1-d2);

    if (lambda >= 0 && lambda <= 1)
    {
        (*this)[0] = v3[0] + lambda*(v4[0]-v3[0]);
        (*this)[1] = v3[1] + lambda*(v4[1]-v3[1]);

        if (fabs(v2[0]-v1[0]) > fabs(v2[1]-v1[1]))
            lambda = ((*this)[0]-v1[0])/(v2[0]-v1[0]);
        else
            lambda = ((*this)[1]-v1[1])/(v2[1]-v1[1]);

    }
}

double CSPXVector::LinePlaneIntersection(const CSPXVector &v1, const CSPXVector &v2, const CSPXVector &planeNormal, const CSPXVector &normalPosition)
{
    CSPXVector v21 = v2-v1;
    //if line is || to plane normal.Dot(v2-v1) = 0
    CSPXVector pn = planeNormal;
    double nv2v1 = pn.Dot(v21);
    if (fabs(nv2v1) < 0.00001)
    {
        (*this)[0] = v2[0];
        (*this)[1] = v2[1];
        (*this)[2] = v2[2];

        return 0.;

    }

    // n.l = 0 at intersection, l = v1 + s*(v2-v1)
    //n.(v1 + s*(v2-v1) = 0 => find s
    //s = -n.v1/(n.(v2-v1)

    double s = -pn.Dot(v1-normalPosition)/nv2v1;

    (*this)[0] = v1[0] + s*v21[0];
    (*this)[1] = v1[1] + s*v21[1];
    (*this)[2] = v1[2] + s*v21[2];

    return s;
}

void CSPXVector::PlaneNormal(CSPXVector &v1,CSPXVector &v2,CSPXVector &v3)
{
    //L1 = v2-v1
    //L2 = v3-v2
    //n=L1^L2
    //return normalized n
    CSPXVector l1 = v2-v1;
    CSPXVector l2 = v3-v1;
    CSPXVector n = l1.Cross(l2);

    if (n.GetDimension()>3)
        n[3] = 0.0;

    n.Normalize();

    (*this)[0] = n[0];
    (*this)[1] = n[1];
    (*this)[2] = n[2];

    if (dimension>3)
        (*this)[3] = 0;
}

CSPXVector* CSPXVector::Normalize()
{
    /*if (dimension == 4)
       {
        NormalizeVector4(vector);
       }
       else*/
    {
        double norm = this->Norm();
        if(norm == 0.0)
            return this;
        for(int i = 0; i < dimension; i++)
            (*this)[i] /= norm;
    }

    return this;
}

void CSPXVector::Order()
{
}

void CSPXVector::GetBoundingBox(struct s_box *box)
{
    box->right = MAX((*this)[0],box->right);
    box->left = MIN((*this)[0],box->left);
    if (dimension > 1)
    {
        box->top = MAX((*this)[1],box->top);
        box->bottom = MIN((*this)[1],box->bottom);
    }
    if (dimension > 2)
    {
        box->front = MAX((*this)[2],box->front);
        box->back = MIN((*this)[2],box->back);
    }
}

CSPXVector CSPXVector::Rotate(double alpha, double beta, double gamma)
{
    CSPXVector rv(3);
    //all points lie in the plane of this face
    CSPXMatrix rmx(3,3);
    rmx[0][0] = 1.; rmx[0][1] = 0.; rmx[0][2] = 0.;
    rmx[1][0] = 0.; rmx[1][1] = cos(alpha); rmx[1][2] = -sin(alpha);
    rmx[2][0] = 0.; rmx[2][1] = sin(alpha); rmx[2][2] = cos(alpha);
    CSPXMatrix rmy(3,3);
    rmy[0][0] = cos(beta); rmy[0][1] = 0.; rmy[0][2] = -sin(beta);
    rmy[1][0] = 0.; rmy[1][1] = 1.; rmy[1][2] = 0.;
    rmy[2][0] = sin(beta); rmy[2][1] = 0.; rmy[2][2] = cos(beta);
    CSPXMatrix rmz(3,3);
    rmz[0][0] = cos(gamma); rmz[0][1] = -sin(gamma); rmz[0][2] = 0.;
    rmz[1][0] = -sin(gamma); rmz[1][1] = cos(gamma); rmz[1][2] = 0.;
    rmz[2][0] = 0.; rmz[2][1] = 0.; rmz[2][2] = 1.;

    CSPXMatrix rot(3,3);
    rot = rmx*rmy*rmz;

    CSPXVector safethis(3);
    safethis[0] = (*this)[0];
    safethis[1] = (*this)[1];
    safethis[2] = (*this)[2];
    rv = rot*safethis;
    return rv;
}

void CSPXVector::RotateAbout(CSPXVector &org, CSPXMatrix &rm)
{
    CSPXVector v = rm*(*this-org);
    *this = v-org;
};

CSPXVector CSPXVector::ParsePointPairs(char** points)
{
    CSPXVector v(3);

    char *ptr = *points;
    char *eptr = nullptr;
    int len = 0;
    bool f = true;
    char datum[32];
    while(ptr != nullptr)
    {
        if (f)
        {
            memset(datum,0,32);
            eptr = strstr(ptr,",");
            strncpy(ptr,datum,eptr-ptr);
            v[0] = atof(datum);

            f = false;
        }
        else
        {
            memset(datum,0,32);
            eptr = strstr(ptr," ");
            strncpy(ptr,datum,eptr-ptr);
            v[1] = atof(datum);

            f = true;
        }
        ptr += (eptr-ptr);
        len += (eptr-ptr);
    }

    *points = *points + len;

    return v;
}

void CSPXVector::VectorArrayAddition(struct s_vectorArray *positionArray)
{
/*    __m128 *trans = (__m128 *)this->vector;

    int arrayLength = positionArray->length;
    __m128** vin = (__m128**)positionArray->vectorArray;
    unsigned long oldEBX;

    __asm
    {
        //save frame pointer
        mov			oldEBX,ebx

        mov			ebx, trans
        movaps		xmm0, [ebx]   //load translation vector into xmm0

        mov			esi, vin
        mov			ecx, arrayLength

   _LOOP:
        mov			eax, [esi]    //put pointer's pointer
        movaps		xmm1, [eax]   //load into mmx actual vector values

        addps		xmm1, xmm0		// Add translation vector
        movaps		[eax], xmm1		// Save the result vector

        add         esi, 4

        dec			ecx
        jnz			_LOOP

        //restore frame pointer
        mov			ebx,oldEBX

    }
 */
}

void CSPXVector::VectorArraySubtraction(struct s_vectorArray *positionArray)
{
/*
    __m128 *trans = (__m128 *)this->vector;

    int arrayLength = positionArray->length;
    __m128** vin = (__m128**)positionArray->vectorArray;

    unsigned long oldEBX;

    __asm
    {
        //save frame pointer
        mov			oldEBX,ebx

        mov			ebx, trans
        movaps		xmm0, [ebx]   //load translation vector into xmm0

        mov			esi, vin
        mov			ecx, arrayLength

   _LOOP:
        mov			eax, [esi]    //put pointer's pointer
        movaps		xmm1, [eax]   //load into mmx actual vector values

        subps		xmm1, xmm0		// Add translation vector
        movaps		[eax], xmm1		// Save the result vector

        add         esi, 4

        dec			ecx
        jnz			_LOOP

        //restore frame pointer
        mov			ebx,oldEBX
    }
 */
}

void CSPXVector::VectorArrayMultiplication(struct s_vectorArray *positionArray)
{
/*
    __m128 *trans = (__m128 *)this->vector;

    int arrayLength = positionArray->length;
    __m128** vin = (__m128**)positionArray->vectorArray;
    unsigned long oldEBX;

    __asm
    {
        //save frame pointer
        mov			oldEBX,ebx

        mov			ebx, trans
        movaps		xmm0, [ebx]   //load translation vector into xmm0

        mov			esi, vin
        mov			ecx, arrayLength

   _LOOP:
        mov			eax, [esi]    //put pointer's pointer
        movaps		xmm1, [eax]   //load into mmx actual vector values

        mulps		xmm1, xmm0		// Multiply vector vector
        movaps		[eax], xmm1		// Save the result vector

        add         esi, 4

        dec			ecx
        jnz			_LOOP

        //restore frame pointer
        mov			ebx,oldEBX

    }
 */
}


CSPXQuaternion::CSPXQuaternion(const CSPXQuaternion &rval)
    : CSPXVector(rval)
{
    m_s = rval.m_s;
}

//[s1; v1][s2; v2] = [s1s2 - v1.v2; s1v2 + s2v1 +v1xv2]:
CSPXQuaternion operator * (const CSPXQuaternion &lq, const CSPXQuaternion &rq)
{
    CSPXQuaternion q;

    q.m_s = lq.m_s*rq.m_s - (lq[0]*rq[0]+lq[1]*rq[1]+lq[2]*rq[2]);

    CSPXVector v1(3);
    v1[0] = lq[1]*rq[2] - lq[2]*rq[1];
    v1[1] = lq[2]*rq[0] - lq[0]*rq[2];
    v1[2] = lq[0]*rq[1] - lq[1]*rq[0];

    CSPXVector v2(3);
    v2[0] = lq.m_s*rq[0] + rq.m_s*lq[0] + v1[0];
    v2[1] = lq.m_s*rq[1] + rq.m_s*lq[1] + v1[1];
    v2[2] = lq.m_s*rq[2] + rq.m_s*lq[2] + v1[2];

    q[0] = v2[0];
    q[1] = v2[1];
    q[2] = v2[2];

    return q;
}

CSPXQuaternion* CSPXQuaternion::Normalize()
{
    CSPXVector::Normalize();
    return this;
}

CSPXMatrix CSPXQuaternion::ToMatrix()
{
    CSPXMatrix mat(3,3);
    double s = m_s;

    mat[0][0] = 1 - 2*(*this)[1]*(*this)[1] -2*(*this)[2]*(*this)[2];
    mat[0][1] = 2*(*this)[0]*(*this)[1] -2*m_s*(*this)[2];
    mat[0][2] = 2*(*this)[0]*(*this)[2] +2*m_s*(*this)[1];
    mat[1][0] = 2*(*this)[0]*(*this)[1] +2*m_s*(*this)[2];
    mat[1][1] = 1 - 2*(*this)[0]*(*this)[0] -2*(*this)[2]*(*this)[2];
    mat[1][2] = 2*(*this)[1]*(*this)[2] -2*m_s*(*this)[0];
    mat[2][0] = 2*(*this)[0]*(*this)[2] -2*m_s*(*this)[1];
    mat[2][1] = 2*(*this)[1]*(*this)[2] +2*m_s*(*this)[0];
    mat[2][2] = 1 - 2*(*this)[0]*(*this)[0] -2*(*this)[1]*(*this)[1];

    return mat;
}

CSPXQuaternion::CSPXQuaternion(const CSPXMatrix &m)
{
    double tr, s;
    tr = m[0][0] + m[1][1] + m[2][2];

    if(tr >= 0)
    {
        s = sqrt(tr + 1);
        m_s = 0.5 * s;
        s = 0.5 / s;
        (*this)[0] = (m[2][1] - m[1][2]) * s;
        (*this)[1] = (m[0][2] - m[2][0]) * s;
        (*this)[2] = (m[1][0] - m[0][1]) * s;
    }
    else
    {
        int i = 0;
        if(m[1][1] > m[0][0])
            i = 1;
        if(m[2][2] > m[i][i])
            i = 2;
        switch (i)
        {
        case 0:
            s = sqrt((m[0][0] - (m[1][1] + m[2][2])) + 1);
            (*this)[0] = 0.5 * s;
            s = 0.5 / s;
            (*this)[1] = (m[0][1] + m[1][0]) * s;
            (*this)[2] = (m[2][0] + m[0][2]) * s;
            m_s = (m[2][1] - m[1][2]) * s;
            break;
        case 1:
            s = sqrt((m[1][1] - (m[2][2] + m[0][0])) + 1);
            (*this)[1] = 0.5 * s;
            s = 0.5 / s;
            (*this)[2] = (m[1][2] + m[2][1]) * s;
            (*this)[0] = (m[0][1] + m[1][0]) * s;
            m_s = (m[0][2] - m[2][0]) * s;
            break;
        case 2:
            s = sqrt((m[2][2] - (m[0][0] + m[1][1])) + 1);
            (*this)[2] = 0.5 * s;
            s = 0.5 / s;
            (*this)[0] = (m[2][0] + m[0][2]) * s;
            (*this)[1] = (m[1][2] + m[2][1]) * s;
            m_s = (m[1][0] - m[0][1]) * s;
        }
    }
}

// assigment CSPXQuaternion
CSPXQuaternion &CSPXQuaternion::operator = (const CSPXQuaternion &rval)
{
    if(dimension == 0 || dimension < rval.dimension)
    {
        free(vector);//delete vector;
        Initialize(rval.dimension);
    }
    memcpy(vector,rval.vector,rval.dimension*sizeof(double));
    return *this;
}


