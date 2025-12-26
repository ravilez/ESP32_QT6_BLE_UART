#pragma once

template<class T> class CSPXTMatrix
{
private:
	T* **matrix;
	int numRows,numColumns;
	int size;

	// prepares assignments
	void PrepCopy(int rRows,int rColumns);

	// deinitialisation
	public:void Deinitialize();

public:
	// initialisation constructors
	void Initialize(int mrows,int ncolumns);
	// default // CSPXTMatrix<T> A;
	CSPXTMatrix();
	// copy-initializer // CSPXTMatrix<T> A = B;
	CSPXTMatrix(const CSPXTMatrix<T> &rval);
	// sizes and initialises at 0 //CSPXTMatrix<T> A(3,5);
	CSPXTMatrix(int rows,int columns);
    // same size as CSPXTMatrix<T> argument
    CSPXTMatrix(int rows,int columns,const CSPXTMatrix<T> &rval);
	// sizes and initialises
	// CSPXTMatrix<T> A(2,3,1.,2.,3.,4.,5.,6.);
	CSPXTMatrix(int rows,int columns,T &a11,...);
	// initialises from array
	// T w[6]={1.,2.,3.,4.,5.6}; CSPXTMatrix<T> A(3,5,w);
	CSPXTMatrix(int rows,int columns,T *initvalues);

	~CSPXTMatrix();  

	T* **GetMatrix() const { return matrix; }
	// (read only) number of rows
	int Rows(void) const { return numRows; }
	// (read only) number of columns
	int Columns(void) const { return numColumns; }

	int GetSize() const { return size; }

	// receives the values with control 
	T* GetValue(int row,int col) const;
	//gets the ith row

	// assigns and get the matrix element m[row][column] with control
	T* operator () (int row,int col);
	// assigns and get the row element m[row] without control
	T* *operator [] (int r) const { return matrix[r]; }
	// assigns values with control 
	void SetValue(int row,int column,T* val);

	// assignment operator
	CSPXTMatrix<T> &operator = (const CSPXTMatrix<T> &rval);
};

template<class T> void CSPXTMatrix<T>::Initialize(int rows,int columns)
{
	if(rows <= 0 || columns <= 0)
	{
		numRows = numColumns = size = 0;
		matrix = NULL; 
		return;
	}
	numRows = rows;
	numColumns = columns;
	size = numRows * numColumns;
	matrix = new T**[numRows];
	for (int i = 0; i < numRows; i++)
		matrix[i] = new T*[numColumns];

}


template<class T> void CSPXTMatrix<T>::Deinitialize(void)
{
	if(matrix == NULL) 
		return;
	
	for (int i = 0; i < numRows; i++)
		delete [] matrix[i];

	delete matrix;
}

template<class T> void CSPXTMatrix<T>::PrepCopy(int rRows,int rColumns)
{
	if(numRows != rRows || numColumns != rColumns)
	{
		Deinitialize();
		Initialize(rRows,rColumns);
	}
}

template<class T> CSPXTMatrix<T>::CSPXTMatrix()
{
	Initialize(0,0);
}

template<class T> CSPXTMatrix<T>::CSPXTMatrix(const CSPXTMatrix<T> &rval)
{
	Initialize(rval.Rows(),rval.Columns());
	if(Rows() != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(T));
}

template<class T> CSPXTMatrix<T>::CSPXTMatrix(int rows,int columns)
{
	Initialize(rows,columns);
	if(size != 0)
	{
		//memset(matrix[0],0,size*sizeof(T));
		for(int row = 0; row < rows; row++)
			for(int col = 0; col < columns; col++)
				matrix[row][col] = NULL;
	}
}

/*template<class T> CSPXTMatrix<T>::CSPXTMatrix(int rows,int columns,T &a11,...)
{
	Initialize(rows,columns);
	T *w=matrix[0];
	va_list puntaList;
    va_start(puntaList,a11);
	int i;
	*w = Control(a11);
	for(i = 1; i < size;i++)
        *++w = Control(va_arg(puntaList,T));
	va_end(puntaList);
}*/

template<class T> CSPXTMatrix<T>::CSPXTMatrix(int rows,int columns,T *initvalues)
{
	Initialize(rows,columns);
	T *w=matrix[0];
	++w;
	if(Rows() != 0)
		memcpy(w,initvalues,size*sizeof(T));
}

template<class T> CSPXTMatrix<T>::CSPXTMatrix(int rows,int columns,const CSPXTMatrix<T> &rval)
{
	Initialize(rows,columns);
	for(int row = 0; row < numRows; row++)
		memcpy(matrix[row],rval.matrix[row],numColumns*sizeof(T));
}


template<class T> CSPXTMatrix<T>::~CSPXTMatrix()
{
	Deinitialize();
}

template<class T> T* CSPXTMatrix<T>::GetValue(int row,int col) const
{
	if( (row < 0 || row >= numRows) || (col < 0 || col >= numColumns) )
	  return 0;
	return matrix[row][col];
}

template<class T> T* CSPXTMatrix<T>::operator () (int row,int col)
{
	if( (row < 0 || row >= numRows) || (col < 0 || col >= numColumns) )
        return NULL;
    else
	    return matrix[row][col];
}

template<class T> void CSPXTMatrix<T>::SetValue(int row,int col,T* val)
{
	if( !((row < 0 || row >= numRows) || (col < 0 || col >= numColumns)) )
		matrix[row][col] = val;
}

template<class T> CSPXTMatrix<T> &CSPXTMatrix<T>::operator =(const CSPXTMatrix<T> &rval)
{
	PrepCopy(rval.Rows(),rval.Columns());
	if(numRows != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(T));
	return *this;
}
