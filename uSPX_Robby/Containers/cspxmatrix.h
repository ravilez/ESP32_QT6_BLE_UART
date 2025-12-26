#ifndef CSPXMATRIX_H
#define CSPXMATRIX_H


class CSPXVector;

 class CSPXMatrix
{
    //CSPXVector is declared as a friend and allowed to access private data of CSPXMatrix
    friend class CSPXVector;
    //Non-member < operator can access private matrix data
    friend bool operator < (const CSPXMatrix &lval, const CSPXMatrix &rval);
    //Non-member == operator can access private matrix data
    friend bool operator == (const CSPXMatrix &lval, const CSPXMatrix &rval);

public:

    double **matrix;
    int numRows,numColumns;
    int size;

private:
    // initialisation constructors
    void Initialize(int mrows,int ncolumns);
    // deinitialisation
    void Deinitialize();
    // prepares assignments
    void PrepCopy(int rRows,int rColumns);

public:
    // default // CSPXMatrix A;
    CSPXMatrix();
    // copy-initializer // CSPXMatrix A = B;
    CSPXMatrix(const CSPXMatrix &rval);
    // sizes and initialises at 0 //CSPXMatrix A(3,5);
    CSPXMatrix(int rows,int columns);
    // same size as CSPXMatrix argument
    CSPXMatrix(int rows,int columns,const CSPXMatrix &rval);
    // sizes and initialises
    // CSPXMatrix A(2,3,1.,2.,3.,4.,5.,6.);
    CSPXMatrix(int rows,int columns,double a11,...);
    // initialises from array
    // double w[6]={1.,2.,3.,4.,5.6}; CSPXMatrix A(3,5,w);
    CSPXMatrix(int rows,int columns,double *initvalues);
    // from CSPXVector
    CSPXMatrix(const CSPXVector &rval);
    // CSPXMatrix A(3,v1,v2,v3);
    CSPXMatrix(int colums, CSPXVector &v1, ...);

    ~CSPXMatrix();

    double **GetMatrix() const { return matrix; }
    // (read only) number of rows
    int Rows(void) const { return numRows; }
    // (read only) number of columns
    int Columns(void) const { return numColumns; }

    int GetSize() const { return size; }
    bool IsIdentity();

    // receives the values with control
    double GetValue(int row,int col) const;
    //gets the ith row
    CSPXVector GetRow(int i) const;
    //gets the jth column
    CSPXVector GetColumn(int j) const;

    // makes the identity matrix
    void Identity();

    void Star(CSPXVector &v);

    // assigns and get the matrix element m[row][column] with control
    double &operator () (int row,int col);
    // assigns and get the row element m[row] without control
    double *operator [] (int r) const { return matrix[r]; }
    // assigns values with control
    void SetValue(int row,int column,double val);
    //substitutes column i with rval
    void SetRow(int i,const CSPXVector &rval);
    // substitutes column j with rval
    void SetColumn(int j,const CSPXVector &rval);

    // assignment operator
    CSPXMatrix &operator = (const CSPXMatrix &rval);

    // C = A + B;
    friend CSPXMatrix operator + (const CSPXMatrix &lval,const CSPXMatrix &rval);
    // A += B;
    CSPXMatrix &operator += (const CSPXMatrix &rval);

    // C = A - B;
    friend CSPXMatrix operator - (const CSPXMatrix &lval,const CSPXMatrix &rval);
    // A -= B;
    CSPXMatrix &operator -= (const CSPXMatrix &rval);

    void Negate();
    friend CSPXMatrix operator - (const CSPXMatrix &rval);

    // C=A*B;
    friend CSPXMatrix operator * (const CSPXMatrix &lval,const CSPXMatrix &rval);
    // A = A * B;
    CSPXMatrix &operator *= (const CSPXMatrix &rval);
    // y = A*x;
    CSPXVector operator * (const CSPXVector &rval);
    // A = 3.*B;
    friend CSPXMatrix operator * (double lval,const CSPXMatrix &rval);
    // A = 3.*A;
    CSPXMatrix &operator *= (double rval);

    // C = A%B
    friend CSPXMatrix operator % (const CSPXMatrix &lval,const CSPXMatrix &rval);
    // y = A%x;
    friend CSPXVector operator % (const CSPXMatrix &lval,const CSPXVector &rval);
    // A %= B;
    CSPXMatrix &operator %= (const CSPXMatrix &rval);

    // A = x->*y; A = xyT;
    friend CSPXMatrix operator ->* (const CSPXVector &lval,const CSPXVector &rval);

    // A= B/3.;
    friend CSPXMatrix operator /(const CSPXMatrix &lval,double rval);
    // A /= 3.;
    CSPXMatrix &operator /= (double rval);

    // B = A(-1) : Gauss Jordan
    CSPXMatrix Inverse();
    void gaussj2();

    // invert matrix : result in inv
    bool GaussJ(CSPXMatrix &inv);

    // B = A(double)
    CSPXMatrix Transpose();
    // B = ~A
    friend CSPXMatrix operator ~ (const CSPXMatrix &rval);
    //sse optimized
    void TransposeIntoXMM();

    // B = A(n)
    friend CSPXMatrix operator ^ (const CSPXMatrix &rval, int n);

    double Trace();
    double Norm();

    // conversion operators
    operator double**(void) { return (double**)matrix; }

    double Determinant();
    double Determinant3By3();

    void EulerMatrix(CSPXMatrix &m, double alpha, double beta, double gamma);
    void SphericalToCartesianUnitMatrix(CSPXMatrix &m, double theta, double phi);
    void CartesianToSphericalUnitMatrix(CSPXMatrix &m, double theta, double phi);
    void CylindricalToCartesianUnitMatrix(CSPXMatrix &m, double phi);
    void CartesianToCylindricalUnitMatrix(CSPXMatrix &m, double phi);

    //HPC
    void BatchMultiply4(double *vin, double *vout, int vecLen);
    void VectorArrayTransform(struct s_vectorArray *positionArray);
};

/*
bool VectorCompare(CSPXVector *v1, CSPXVector *v2)
{
    // call friend comparion function;
    if (v1 == v2)
        return true;

    return false;
}


bool MatrixCompare(CSPXMatrix *m1, CSPXMatrix *m2)
{
    // call friend comparion function;
    if (m1 == m2)
        return true;

    return false;
}
*/



#endif // CSPXMATRIX_H
