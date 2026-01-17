#include <math.h>
#include <stdarg.h>
#include <cstring>

#include <initializer_list>

#include "cspxutility.h"
#include "cspxvector.h"
#include "cspxmatrix.h"



bool CSPXMatrix::IsIdentity()
{
    if (numRows != numColumns)
        return false;

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            if ((i == j) && !((*this)[i][j] == 1.0))
                return false;
            else if ((i != j) && !((*this)[i][j] == 0.0))
                return false;
        }
    }

    return true;
}

CSPXMatrix CSPXMatrix::Inverse()
{
    if(numRows != numColumns)
      Error((char*)"");

   CSPXMatrix inv(*this);
   // inv will hold the inverse after GJ elimination
   if (!GaussJ(inv))
   {
      for (int i = 0; i < this->Columns(); i++)
         inv[i][i] = 0.0;
   }

   return inv;
}

void CSPXMatrix::Initialize(int rows,int columns)
{
    if(rows <= 0 || columns <= 0)
    {
        numRows = numColumns = size = 0;
        matrix = nullptr;
        return;
    }
    numRows = rows;
    numColumns = columns;
    size = numRows * numColumns;
    //matrix = (double**)malloc(numRows*sizeof(double));
    matrix = new double*[numRows*sizeof(double)];
    if(!matrix)
        Error((char*)"");
    matrix[0]= new double[size*sizeof(double)];
    if(!matrix[0])
        Error((char*)"");
    for(int i = 1; i < numRows; i++)
        matrix[i] = matrix[i-1] + numColumns;
}


void CSPXMatrix::Deinitialize(void)
{
    if(matrix == nullptr)
        return;
    delete [] matrix[0];
    delete [] matrix;
}

void CSPXMatrix::PrepCopy(int rRows,int rColumns)
{
    if(numRows != rRows || numColumns != rColumns)
    {
        Deinitialize();
        Initialize(rRows,rColumns);
    }
}

CSPXMatrix::CSPXMatrix()
{
    Initialize(0,0);
}

CSPXMatrix::CSPXMatrix(const CSPXMatrix &rval)
{
    Initialize(rval.Rows(),rval.Columns());
    if(Rows() != 0)
        memcpy(matrix[0],rval.matrix[0],size*sizeof(double));
}

CSPXMatrix::CSPXMatrix(int rows,int columns)
{
    Initialize(rows,columns);
    if(size != 0)
        memset(matrix[0],0,size*sizeof(double));
}

CSPXMatrix::CSPXMatrix(int rows,int columns,double a11,...)
{
    Initialize(rows,columns);
    double *w=matrix[0];
    va_list puntaList;
    va_start(puntaList,a11);
    int i;
    *w = Control(a11);
    for(i = 1; i < size;i++)
        *++w = Control(va_arg(puntaList,double));
    va_end(puntaList);
}

CSPXMatrix::CSPXMatrix(int rows,int columns,double *initvalues)
{
    Initialize(rows,columns);
    double *w=matrix[0];
    ++w;
    if(Rows() != 0)
        memcpy(w,initvalues,size*sizeof(double));
}

CSPXMatrix::CSPXMatrix(int rows,int columns,const CSPXMatrix &rval)
{
    if(rows < 0 || rows >= rval.Rows())
        Error((char*)"");
    if(columns < 0 || columns >= rval.Columns())
        Error((char*)"");
    Initialize(rows,columns);
    for(int row = 0; row < numRows; row++)
        memcpy(matrix[row],rval.matrix[row],numColumns*sizeof(double));
}

CSPXMatrix::CSPXMatrix(const CSPXVector &rval)
{
    Initialize(rval.GetDimension(),1);
    if(numRows != 0)
        memcpy(matrix[0],rval.vector,size*sizeof(double));
}

CSPXMatrix::CSPXMatrix(std::initializer_list<CSPXVector> rows)
{    
    const int r = static_cast<int>(rows.size());
    const int c = rows.begin()->GetDimension();
    Initialize(r,c);


    int i = 0;
    for (const auto& v : rows) {
        // (Optional) check v.GetDimension() == c
        std::memcpy(matrix[i++], v.vector, static_cast<size_t>(c) * sizeof(double));
    }
}

CSPXMatrix::~CSPXMatrix()
{
    Deinitialize();
}


void CSPXMatrix::Identity()
{
    if(size != 0)
        memset(matrix[0],0,size*sizeof(double));

    for (int i = 0; i < numRows; i++)
        matrix[i][i] = 1.0;
}

void CSPXMatrix::Star(CSPXVector &v)
{
    if( numRows != 3 && numColumns != 3)
      Error((char*)"Require 3x3 matrix");

    matrix[0][0] = 0.0; matrix[0][1] = -v[2]; matrix[0][2] = v[1];
    matrix[1][0] = v[2]; matrix[1][1] = 0.0; matrix[0][2] = -v[0];
    matrix[2][0] = -v[1]; matrix[2][1] = v[0]; matrix[2][2] = 0.0;
}

double CSPXMatrix::GetValue(int row,int col) const
{
    if( (row < 0 || row >= numRows) || (col < 0 || col >= numColumns) )
    {
        Error((char*)"");
        return 0;
    }
    return matrix[row][col];
}

CSPXVector CSPXMatrix::GetRow(int i) const
{
    if(i < 0 || i >= numRows)
    {
        Error((char*)"");
        return 0;
    }
    CSPXVector result(numColumns);
    memcpy(result.vector,matrix[i],numColumns*sizeof(double));
    return result;
}

CSPXVector CSPXMatrix::GetColumn(int j) const
{
    if(j < 0 || j >= Columns())
    {
        Error((char*)"");
        return 0;
    }
    CSPXVector result(numRows);
    double *r = result.vector;
    for(int k = 0; k < numRows; k++)
        *++r = matrix[k][j];
    return result;
}

double &CSPXMatrix::operator () (int row,int col)
{
    if( (row < 0 || row >= numRows) || (col < 0 || col >= numColumns) )
        Error((char*)"");
    return matrix[row][col];
}

void CSPXMatrix::SetValue(int row,int col,double val)
{
    if( (row < 0 || row >= numRows) || (col < 0 || col >= numColumns) )
        Error((char*)"");
    matrix[row][col] = val;
}

void CSPXMatrix::SetRow(int i,const CSPXVector &rval)
{
    if( (i < 0 || i >= numRows) || (rval.GetDimension() != numColumns) )
        Error((char*)"");
    memcpy(matrix[i],rval.vector, numColumns*sizeof(double));
}

void CSPXMatrix::SetColumn(int j,const CSPXVector &rval)
{
    if( (j < 0 || j >= numColumns) || (rval.GetDimension() != numRows) )
        Error((char*)"");
    double *r = rval.vector;
    for(int k = 0; k < numRows; k++)
        matrix[k][j] = *++r;
}

CSPXMatrix &CSPXMatrix::operator = (const CSPXMatrix &rval)
{
    PrepCopy(rval.Rows(),rval.Columns());
    if(numRows != 0)
        memcpy(matrix[0],rval.matrix[0],size*sizeof(double));
    return *this;
}

CSPXMatrix operator && (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    if(lval.Columns() != rval.Columns())
    {
        Error((char*)"");
        return lval;
    }

    CSPXMatrix result(lval.Rows()+rval.Rows(),lval.Columns());
    if(lval.Rows() != 0)
    {
        memcpy(result.GetMatrix(),lval.GetMatrix(),lval.GetSize()*sizeof(double));
        double *r = &result[lval.Rows()][lval.Columns()];
        memcpy(r,rval.GetMatrix(),rval.GetSize()*sizeof(double));
    }
    return result;
}

CSPXMatrix operator || (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    if(lval.Rows() != rval.Rows())
    {
        Error((char*)"");
        return lval;
    }
    CSPXMatrix result(lval.Rows(),lval.Columns()+rval.Columns());
    double **s=lval.GetMatrix();
    double **d=rval.GetMatrix();
    double **r=result.GetMatrix();
    if(result.Rows() != 0)
    {
        for(int i=0; i<lval.Rows(); i++)
        {
            memcpy(r[i],s[i],lval.Columns()*sizeof(double));
            memcpy(r[i]+lval.Columns(),d[i],rval.Columns()*sizeof(double));
        }
    }
    return result;
}

//also declared as a friend in CSPXMatrix
bool operator == (const CSPXMatrix &lval, const CSPXMatrix &rval)
{
    if ( (rval.numRows != lval.numRows) || (rval.numColumns != lval.numColumns) )
        return false;

    for(int i = 0; i < rval.numRows; i++)
    {
        for(int j = 0;j < rval.numColumns;j++)
        {
            if (lval[j][i] != rval[i][j])
                return false;
        }
    }

    return true;
}

bool operator != (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    if(lval.Rows() != rval.Rows() || lval.Columns() != rval.Columns())
        return false;
    else
        return (bool)memcmp(lval.GetMatrix(),rval.GetMatrix(),rval.GetSize()*sizeof(double));
}

CSPXMatrix operator + (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    if(lval.Columns() != rval.Columns() || lval.Rows() != rval.Rows())
        Error((char*)"");

    CSPXMatrix result(lval.Rows(),lval.Columns());
    for(int i = 0; i < lval.numRows; i++)
    {
        for(int j = 0; j < lval.numColumns; j++)
        {
            result[i][j] = lval[i][j] + rval[i][j];
        }
    }
    return result;
}

CSPXMatrix &CSPXMatrix::operator += (const CSPXMatrix &rval)
{
    if(numColumns != rval.Columns() || numRows != rval.Rows())
        Error((char*)"");

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            (*this)[i][j] += rval[i][j];
        }
    }
    return *this;
}

CSPXMatrix operator - (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    if(lval.Columns() != rval.Columns() || lval.Rows() != rval.Rows())
        Error((char*)"");

    CSPXMatrix result(lval.Rows(),lval.Columns());
    for(int i = 0; i < lval.numRows; i++)
    {
        for(int j = 0; j < lval.numColumns; j++)
        {
            result[i][j] = lval[i][j] - rval[i][j];
        }
    }
    return result;
}

CSPXMatrix &CSPXMatrix::operator -= (const CSPXMatrix &rval)
{
    if(numColumns != rval.Columns() || numRows != rval.Rows())
        Error((char*)"");

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            (*this)[i][j] -= rval[i][j];
        }
    }
    return *this;
}

void CSPXMatrix::Negate()
{
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            (*this)[i][j] = -(*this)[i][j];
        }
    }
}

CSPXMatrix operator - (const CSPXMatrix &rval) //unary minus
{
    CSPXMatrix result(rval.Rows(),rval.Columns());
    for(int i = 0; i < rval.numRows; i++)
    {
        for(int j = 0; j < rval.numColumns; j++)
        {
            result[i][j] = -rval[i][j];
        }
    }
    return result;
}

CSPXMatrix operator * (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    CSPXMatrix result(lval.Rows(),rval.Columns());
    for(int i = 0; i < lval.numRows; i++)
    {
        for(int j = 0; j < rval.numColumns; j++)
        {
            result[i][j] = 0.0;
            for(int k = 0; k < rval.numRows; k++)
            {
                result[i][j] += lval[i][k]*rval[k][j];
            }
        }
    }

    return result;
}

CSPXMatrix &CSPXMatrix::operator *= (const CSPXMatrix &rval)
{
    if(numColumns != rval.Rows())
        Error((char*)"");

        // if rval is not square *this changes GetDimension
        if(rval.Rows() == rval.Columns())
        {
            double *b;
            int nc = rval.Columns();
             double *v = GetMatrix()[0];
             //double *w = (double*)malloc(numColumns*sizeof(double));
             double *w = new double[numColumns*sizeof(double)];
             for(int row = 0; row < numRows; row++)
             {
                 memcpy(w,GetMatrix()[row],numColumns*sizeof(double));
                 for(int column = 0; column < numColumns; column++)
                 {
                     b = rval.GetMatrix()[0] + column;
                     double sum=0.;
                    for(int i = 0; i < numColumns; i++)
                    {
                        sum += w[i] * (*b);
                        b += nc;
                    }
                    *++v = Control(sum);
                }
            }
             delete [] w;
        }
        else
        {
            CSPXMatrix result;
        }

    return *this;
}

CSPXMatrix operator * (double lval,const CSPXMatrix &rval)
{
    CSPXMatrix result(rval.Rows(),rval.Columns());
    for(int i = 0; i < rval.numRows; i++)
    {
        for(int j = 0; j < rval.numColumns; j++)
        {
            result[i][j] = rval[i][j]*lval;
        }
    }
    return result;
}

CSPXVector CSPXMatrix::operator * (const CSPXVector &rval)
{
    CSPXVector v(numRows);
    for(int i = 0; i < numRows; i++)
    {
        v[i] = 0;
        for(int j = 0; j < numColumns; j++)
        {
            v[i] += (*this)[i][j]*rval[j];
        }
    }
    return v;
}

CSPXMatrix &CSPXMatrix::operator *= (double lval)
{
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            (*this)[i][j] *= lval;
        }
    }
    return *this;
}

CSPXMatrix operator % (const CSPXMatrix &lval,const CSPXMatrix &rval)
{
    CSPXMatrix result(lval.Columns(),rval.Columns());

    return result;
}

CSPXMatrix &CSPXMatrix::operator %=(const CSPXMatrix &rval)
{
    if(numRows != rval.Rows())
        Error((char*)"");

    if(numRows == numColumns && rval.Rows() == rval.Columns())
    { // to optimise if square
        CSPXMatrix result;
    }
    else
    {
        // general case
        CSPXMatrix result;
    }
    return *this;
}

CSPXMatrix operator / (const CSPXMatrix &lval,double rval)
{
    CSPXMatrix result(lval.Rows(),lval.Columns());
    return result;
}

CSPXMatrix &CSPXMatrix::operator /= (double rval)
{
    return *this;
}

double CSPXMatrix::Norm()
{
    return 0;
}

CSPXMatrix CSPXMatrix::Transpose()
{
    int i,j;
    CSPXMatrix m(numColumns,numRows);
    for(i = 0; i < numRows; i++)
        for(j = 0;j < numColumns;j++)
            m[j][i] = (*this)[i][j];

    return m;
}

void CSPXMatrix::TransposeIntoXMM()
{
    if (numRows == 4 && numColumns ==4)
    {
        double *tempElts = matrix[0];

 /*       __asm {
            mov		eax,  tempElts
            movlps	xmm4, [eax]		   // load x and y of r0 to low xmm4
            movhps 	xmm4, [eax+0x10] // load x and y of r1 to high xmm4
            movlps 	xmm3, [eax+0x20] // load x and y of r2 to low xmm3
            movhps 	xmm3, [eax+0x30] // load x and y of r3 to high xmm3
            movaps 	xmm5, xmm4
            shufps 	xmm4, xmm3, 0x88 // create x (extract x-values)
            shufps 	xmm5, xmm3, 0xDD // create y (extract y-values)

            movlps 	xmm6, [eax+0x08] // load z and w of r0 to low xmm6
            movhps 	xmm6, [eax+0x18] // load z and w of r1 to high xmm6
            movlps 	xmm3, [eax+0x28] // load z and w of r2 to low xmm3
            movhps 	xmm3, [eax+0x38] // load z and w of r3 to high xmm3
            movaps	xmm7,	xmm6
            shufps 	xmm6, xmm3, 0x88 // create z (extract z-values)
            shufps	xmm7,	xmm3,	0xDD // create w (extract w-values)
        }
*/
    }
}



// BatchMultiply4 -- A modification to BatchMultiply3 which uses
// SSE prefetching instructions to improve performance with large
// input sets.
//
// Performance: 21 cycles/vector
void CSPXMatrix::BatchMultiply4(double *vin, double *vout, int vecLen)
{
    // transpose the matrix into the xmm4-7
    TransposeIntoXMM();

/*    __asm {
        mov			esi, vin
        mov			edi, vout
        mov			ecx, vecLen

BM4_START:

        movups   xmm0, [esi]
        add      esi, vecLen

        // we'll store the final result in xmm2; initialize it to zero
        xorps    xmm2, xmm2

        // broadcast x into xmm1, multiply it by the first
        // column of the matrix (xmm4), and add it to the total
        movups   xmm1, xmm0
        shufps   xmm1, xmm1, 0x00
        mulps    xmm1, xmm4
        addps    xmm2, xmm1

        // repeat the process for y, z and w
        movups   xmm1, xmm0
        shufps   xmm1, xmm1, 0x55
        mulps    xmm1, xmm5
        addps    xmm2, xmm1
        movups   xmm1, xmm0
        shufps   xmm1, xmm1, 0xAA
        mulps    xmm1, xmm6
        addps    xmm2, xmm1
        movups   xmm1, xmm0
        shufps   xmm1, xmm1, 0xFF
        mulps    xmm1, xmm7
        addps    xmm2, xmm1

        // write the results to vout, and advance the output pointer
        movups   [edi], xmm2
        add      edi, vecLen
        dec      ecx
        jnz		BM4_START
    }
*/
}

void CSPXMatrix::VectorArrayTransform(struct s_vectorArray *positionArray)
{
    // transpose the matrix into the xmm4-7
    TransposeIntoXMM();

    int arrayLength = positionArray->length;
/*    __m128** vin = (__m128**)positionArray->vectorArray;


    __asm {
        mov			esi, vin
        mov			ecx, arrayLength

BM4_START:

        mov			eax, [esi]    //put pointer's pointer
        movaps		xmm0, [eax]   //load into mmx actual vector values

        // we'll store the final result in xmm2; initialize it to zero
        xorps    xmm2, xmm2

        // broadcast x into xmm1, multiply it by the first
        // column of the matrix (xmm4), and add it to the total
        movaps   xmm1, xmm0
        shufps   xmm1, xmm1, 0x00
        mulps    xmm1, xmm4
        addps    xmm2, xmm1

        // repeat the process for y, z and w
        movaps   xmm1, xmm0
        shufps   xmm1, xmm1, 0x55
        mulps    xmm1, xmm5
        addps    xmm2, xmm1
        movaps   xmm1, xmm0
        shufps   xmm1, xmm1, 0xAA
        mulps    xmm1, xmm6
        addps    xmm2, xmm1
        movaps   xmm1, xmm0
        shufps   xmm1, xmm1, 0xFF
        mulps    xmm1, xmm7
        addps    xmm2, xmm1

        // write the results to vout, and advance the output pointer
        movaps   [eax], xmm2

        add      esi, 4
        dec      ecx
        jnz		BM4_START
    }
*/
}

double CSPXMatrix::Determinant()
{
    return 0;
}

double CSPXMatrix::Determinant3By3()
{
    if ( numRows != 3 || numColumns != 3)
        return 0;

    double d =  (*this)[0][0] * ((*this)[1][1]*(*this)[2][2] - (*this)[1][2]*(*this)[2][1]);
                 d -= (*this)[0][1] * ((*this)[1][0]*(*this)[2][2] - (*this)[1][2]*(*this)[2][0]);
                 d += (*this)[0][2] * ((*this)[1][0]*(*this)[2][1] - (*this)[1][1]*(*this)[2][0]);

    return fabs(d);
}

//also declared as a friend in CSPXMatrix (i.e. useful as a comparison function on a Matrix container)
bool operator < (const CSPXMatrix &lval, const CSPXMatrix &rval)
{
    if (rval.size < lval.size)
        return true;
    else
        return false;
}

/*
void CSPXMatrix::GaussJ(CSPXMatrix &inv)
{
    for (int k = 0; k < Rows(); k++)
    {
        double var = inv[k][k];
        inv[k][k] = 1.0;
        for (int j = 0; j < Rows(); j++)
        {
            inv[k][j] = inv[k][j]/var;
        }
        for (int i = 0; i < Rows(); i++)
        {
            if (i == k)
                break;
            var = inv[i][k];
            inv[i][k] = 0.0;
            for (j = 0; j < Rows(); j++)
            {
                inv[i][j] = inv[i][j]-inv[k][j]*var;
            }
        }
    }
}
*/

bool CSPXMatrix::GaussJ(CSPXMatrix &inv)
{
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

    int *indxc,*indxr,*ipiv;
    int n;
    int i,icol,irow,j,k,l,ll;
    double big,dum,pivinv,temp;

    n = Rows();
    indxc = new int[n];
    indxr = new int[n];
    ipiv  = new int[n];

    for (j=0;j<n;j++)
        ipiv[j]=0;

    for (i=0;i<n;i++)
    {
        big=0.0;
        for (j=0;j<n;j++)
        {
            if (ipiv[j] != 1)
            {
                for (k=0;k<n;k++)
                {
                    if (ipiv[k] == 0)
                    {
                        if (fabs(inv[j][k]) >= big)
                        {
                            big=fabs(inv[j][k]);
                            irow=j;
                            icol=k;
                        }
                    }
                    else if (ipiv[k] > 1)
                        Error((char*)"GaussJ: Singular Matrix-1");
                }
            }
        }
        ++(ipiv[icol]);
        if (irow != icol) {
            for (l=0;l<n;l++)
                SWAP(inv[irow][l],inv[icol][l])
        }
        indxr[i]=irow;
        indxc[i]=icol;
        if (inv[icol][icol] == 0.0)
      {
            //Error("gaussj: Singular Matrix-2");
            delete [] ipiv;
            delete [] indxr;
            delete [] indxc;
         return false;
      }
        pivinv=1.0/inv[icol][icol];
        inv[icol][icol]=1.0;
        for (l=0;l<n;l++) inv[icol][l] *= pivinv;
        for (ll=0;ll<n;ll++)
            if (ll != icol) {
                dum=inv[ll][icol];
                inv[ll][icol]=0.0;
                for (l=0;l<n;l++) inv[ll][l] -= inv[icol][l]*dum;
            }
    }
    for (l=n-1;l>=0;l--) {
        if (indxr[l] != indxc[l])
            for (k=0;k<n;k++)
                SWAP(inv[k][indxr[l]],inv[k][indxc[l]]);
    }

    delete [] ipiv;
    delete [] indxr;
    delete [] indxc;

   return true;
}

void CSPXMatrix::gaussj2()
{
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

    int *indxc,*indxr,*ipiv;
    int i,icol,irow,j,k,l,ll;
    double big,dum,pivinv,temp;

    int n = this->Rows();

    indxr= new int[n];
    ipiv= new int[n];

    for (j=0;j<n;j++)
        ipiv[j]=0;
    for (i=0;i<n;i++)
    {
        big=0.0;
        for (j=0;j<n;j++)
        {
            if (ipiv[j] != 1)
            {
                for (k=0;k<n;k++)
                {
                    if (ipiv[k] == 0)
                    {
                        if (fabs(matrix[j][k]) >= big)
                        {
                            big=fabs(matrix[j][k]);
                            irow=j;
                            icol=k;
                        }
                    }
                    else if (ipiv[k] > 1)
                    {
                        Error((char*)"gaussj: Singular Matrix-1");
                        delete [] ipiv;
                    }
                }
            }
        }
        ++(ipiv[icol]);
        if (irow != icol)
        {
            for (l=0;l<n;l++)
                SWAP(matrix[irow][l],matrix[icol][l])
        }
        indxr[i]=irow;
        if (matrix[icol][icol] == 0.0)
            Error((char*)"gaussj: Singular Matrix-2");
        pivinv=1.0/matrix[icol][icol];
        matrix[icol][icol]=1.0;
        for (l=0;l<n;l++)
            matrix[icol][l] *= pivinv;
        for (ll=0;ll<n;ll++)
        {
            if (ll != icol)
            {
                dum=matrix[ll][icol];
                matrix[ll][icol]=0.0;
                for (l=0;l<n;l++)
                    matrix[ll][l] -= matrix[icol][l]*dum;
            }
        }
    }

    delete [] ipiv;
    delete [] indxr;
}

void CSPXMatrix::EulerMatrix(CSPXMatrix &m, double alpha, double beta, double gamma)
{
    if (m.Columns() != m.Rows() && m.Rows() < 3)
        Error((char*)"");

    m[0][0] = cos(gamma)*cos(beta)*cos(alpha) - sin(gamma)*sin(alpha);
    m[0][1] = -sin(gamma)*cos(beta)*cos(alpha) - cos(gamma)*sin(alpha);
    m[0][2] = sin(beta)*cos(alpha);

    m[1][0] = cos(gamma)*cos(beta)*sin(alpha) + sin(gamma)*cos(alpha);
    m[1][1] = -sin(gamma)*cos(beta)*sin(alpha) + cos(gamma)*cos(alpha);
    m[1][2] = sin(beta)*sin(alpha);

    m[2][0] = -cos(gamma)*sin(beta);
    m[2][1] = sin(gamma)*sin(beta);
    m[2][2] = cos(beta);
}

void CSPXMatrix::SphericalToCartesianUnitMatrix(CSPXMatrix &m, double theta, double phi)
{
    if (m.Columns() != m.Rows() && m.Rows() < 3)
        Error((char*)"");

    m[0][0] = sin(theta)*cos(phi);
    m[0][1] = cos(theta)*cos(phi);
    m[0][2] = -sin(phi);

    m[1][0] = sin(theta)*sin(phi);
    m[1][1] = cos(theta)*cos(phi);
    m[1][2] = cos(phi);

    m[2][0] = cos(theta);
    m[2][1] = -sin(theta);
    m[2][2] = 0.0;
}

void CSPXMatrix::CartesianToSphericalUnitMatrix(CSPXMatrix &m, double theta, double phi)
{
    if (m.Columns() != m.Rows() && m.Rows() < 3)
        Error((char*)"");

    m[0][0] = sin(theta)*cos(phi);
    m[0][1] = sin(theta)*sin(phi);
    m[0][2] = cos(theta);

    m[1][0] = cos(theta)*cos(phi);
    m[1][1] = cos(theta)*sin(phi);
    m[1][2] = -sin(theta);

    m[2][0] = -sin(phi);
    m[2][1] = cos(phi);
    m[2][2] = 0.0;
}

void CSPXMatrix::CylindricalToCartesianUnitMatrix(CSPXMatrix &m, double phi)
{
    if (m.Columns() != m.Rows() && m.Rows() < 3)
        Error((char*)"");

    m[0][0] = cos(phi);
    m[0][1] = -sin(phi);
    m[0][2] = 0.0;

    m[1][0] = sin(phi);
    m[1][1] = cos(phi);
    m[1][2] = 0.0;

    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
}

void CSPXMatrix::CartesianToCylindricalUnitMatrix(CSPXMatrix &m, double phi)
{
    if (m.Columns() != m.Rows() && m.Rows() < 3)
        Error((char*)"");

    m[0][0] = cos(phi);
    m[0][1] = sin(phi);
    m[0][2] = 0.0;

    m[1][0] = -sin(phi);
    m[1][1] = cos(phi);
    m[1][2] = 0.0;

    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
}

