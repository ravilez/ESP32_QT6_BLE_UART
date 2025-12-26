#ifndef CSPXVECTOR_H
#define CSPXVECTOR_H

#define M_PI 3.1415926536

#include "../Containers//cspxtypes.h"

#ifdef MSVC
#ifndef __FMA__
#define __FMA__
#endif
#endif

#include <immintrin.h>

typedef union f4_m128 { __m128 mv; float fv[4]; } f4_m128;

struct s_vectorArray
{
    int length;
    double **vectorArray;
};

class CSPXMatrix;

class CSPXVector
{
// grant member function of CSPXMatrix class access privileges
friend class CSPXMatrix;

public:
    double *vector;
    int dimension;

    /*inline void * operator new(size_t size)
    {
        return malloc(size);
    }
    inline void operator delete(void *p)
    {
        free(p);
    }*/

public:

    // enable Runtime class creation
    static CSPXVector* CreateVector();

    // default constructor
    CSPXVector();
    // copy-initializer
    CSPXVector(const CSPXVector &rval);
    // sizes and initialises at 0
    CSPXVector(int nc);
    // sizes and initialises
    CSPXVector(double v1);
    CSPXVector(double v1, double v2);
    CSPXVector(double v1, double v2, double v3);
    CSPXVector(int nc, double v1, ...);
    // data pointer constructor
    CSPXVector(int nc, double *initvalues);

    // destructor
    virtual ~CSPXVector(void);

    int GetDimension() const {
        return dimension;
    }

    // initialise constructors
    void Initialize(int nc);

    // Implicit conversion  operator
    operator double* (void) {
        return (double *)vector;
    }

    // assigns and get vector values with control d
    double &operator () (int i) {
        return vector[i];
    }
    // assigns and receives values
    double &operator [] (int i) const {
        return vector[i];
    }

    // assign one vector to another : V1.operator=(V2);
    CSPXVector &operator = (const CSPXVector &rval);

    // testing equality
    friend bool operator == (const CSPXVector &lval,const CSPXVector &rval);

    // a = b + c
    friend CSPXVector operator + (const CSPXVector &lval,const CSPXVector &rval);
    // a += a;
    CSPXVector &operator += (const CSPXVector &rval);
    //optimizer for 128 bit intrinsic op
    void AddVector4(f4_m128 *result, f4_m128 *lval, f4_m128 *rval);

    // c = a - b;
    friend CSPXVector operator - (const CSPXVector &lval, const CSPXVector &rval);
    // a -= b;
    CSPXVector &operator -= (const CSPXVector &rval);
    //optimizer for 128 bit intrinsic op
    void SubtractVector4(f4_m128 *result, f4_m128 *lval, f4_m128 *rval);

    // Negate -a
    void Negate();
    // b = (-)a
    friend CSPXVector operator - (const CSPXVector &rval);
    // -a
    CSPXVector operator -();

    // y = x*A;
    CSPXVector operator * (const CSPXMatrix &rval);
    // y = 3.*x;
    friend CSPXVector operator * (double lval,const CSPXVector &rval);
    // y = x*3.;
    CSPXVector operator * (double rval);
    void MultiplyVector4(f4_m128 *r, f4_m128 *a, f4_m128 *b);
    // x *= 3.
    CSPXVector &operator *= (double rval);

    // c = x.Dot(y);
    double Dot(const CSPXVector &rval);
    double operator * (const CSPXVector &rval);
    //optimizer for 128 bit intrinsic op
    void DotVector4(f4_m128 *a, f4_m128 *b, float *dp);

    // v3 = v1.Cross(v2);
    CSPXVector Cross(const CSPXVector &rval);
    //optimizer for 128 bit intrinsic op
    void CrossProductVector4(f4_m128 *r, f4_m128 *a, f4_m128 *b);

    // A = x->*y;
    friend CSPXMatrix operator ->* (const CSPXVector &lval,const CSPXVector &rval);
    // y = x/3.
    friend CSPXVector operator / (const CSPXVector &lval,double rval);
    // x /= 3.;
    CSPXVector &operator /= (double rval);

    double Norm();

    double Intersection(CSPXVector &v0, CSPXVector &v1, double &lambdax, double &lambday);
    void Intersection(CSPXVector &v1, CSPXVector &v2, CSPXVector &v3, CSPXVector &v4, double &lambda);
    void PlaneNormal(CSPXVector &v1,CSPXVector &v2,CSPXVector &v3);
    void GetBoundingBox(struct s_box *box);
    double LinePlaneIntersection(const CSPXVector &v1, const CSPXVector &v2, const CSPXVector &planeNormal, const CSPXVector &normalPosition);

    // x.Normalize();
    CSPXVector* Normalize();
    //optimizer for 128 bit intrinsic op
    void NormalizeVector4(f4_m128 *a);
    // orders the vector
    void Order();

    // swaps the contents of two vectors y = x.Swap()
    CSPXVector Swap();
    double SqrtSumSqr(int n, int m);
public: CSPXVector Rotate(double alpha, double beta, double gamma);
public: void RotateAbout(CSPXVector &org, CSPXMatrix &rm);

    static CSPXVector ParsePointPairs(char** points);

    void VectorArrayMultiplication(struct s_vectorArray *positionArray);
    void VectorArrayAddition(struct s_vectorArray *positionArray);
    void VectorArraySubtraction(struct s_vectorArray *positionArray);

    void TransformArray(f4_m128 *v,f4_m128 *m,f4_m128 *t,unsigned int length);
    f4_m128 Transform4(f4_m128* mat4, f4_m128 *vec4);

    void Matrix_Vector_Mult(float vector[4], float matrix[4][4], int vectorLength, float results[4]);


};

class CSPXQuaternion : public CSPXVector
{
public: CSPXQuaternion() : CSPXVector(3), m_s(0.) {
};
public: CSPXQuaternion(double s, CSPXVector &v) : CSPXVector(v), m_s(s) {
};
public: CSPXQuaternion(const CSPXMatrix &m);
public: CSPXQuaternion(const CSPXQuaternion &rval);
public: ~CSPXQuaternion() {
};

public: double m_s;

public: CSPXQuaternion* Normalize();
public: CSPXMatrix ToMatrix();

    // q = q1*q2
    friend CSPXQuaternion operator * (const CSPXQuaternion &lval,const CSPXQuaternion &rval);
    // assign one CSPXQuaternion to another : Q1.operator=(Q2);
    CSPXQuaternion &operator = (const CSPXQuaternion &rval);
};

#endif // CSPXVECTOR_H
