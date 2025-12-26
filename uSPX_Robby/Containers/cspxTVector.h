#pragma once

template<class T> class CSPXTMatrix;

template<class T> class CSPXTVector
{
    // grant member function of CSPXTMatrix class access privileges
    friend class CSPXTMatrix<T>;

private:
   T *vector;
   int dimension;

public:

    // enable Runtime class creation
    static CSPXTVector<T>* CreateVector();

	// default constructor
    CSPXTVector();
	// copy-initializer
    CSPXTVector(const CSPXTVector<T> &rval);
	// sizes and initialises at 0
    CSPXTVector(int nc);
	// sizes and initialises
    CSPXTVector(int nc, T v1,...);
	// data pointer constructor
    CSPXTVector(int nc, T *initvalues);

	// destructor
    ~CSPXTVector(void);

   int GetDimension() const { return dimension; }  

   // initialise constructors
   void Initialize(int nc);

   // Implicit conversion  operator
	operator T* (void) { return (T *)vector; }

   // assigns and get vector values with control d
   T &operator () (int i) { return vector[i]; }
   // assigns and receives values 
   T &operator [] (int i) const { return vector[i]; }

   // assign one vector to another : V1.operator=(V2);
   CSPXTVector<T> &operator = (const CSPXTVector<T> &rval);

   // testing equality
   friend bool operator == (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval);

   // a = b + c
   friend CSPXTVector<T> operator + (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval);
   // a += a;
   CSPXTVector<T> &operator += (const CSPXTVector<T> &rval);

   // c = a - b;
   friend CSPXTVector<T> operator - (const CSPXTVector<T> &lval, const CSPXTVector<T> &rval);
   // a -= b;
   CSPXTVector<T> &operator -= (const CSPXTVector<T> &rval);

   // Negate -a
   void Negate();
   // b = (-)a
   friend CSPXTVector<T> operator - (const CSPXTVector<T> &rval);
   // -a
   CSPXTVector<T> operator -();

   // y = x*A;
   CSPXTVector<T> operator * (const CSPXTMatrix<T> &rval);
   // y = 3.*x;
   friend CSPXTVector<T> operator * (T lval,const CSPXTVector<T> &rval);
   // y = x*3.;
   CSPXTVector<T> operator * (T rval);
   // x *= 3.
   CSPXTVector<T> &operator *= (T rval);

   // c = x.Dot(y);
   T Dot(const CSPXTVector<T> &rval);

   // A = x->*y;
   friend CSPXTMatrix<T> operator ->* (const CSPXTVector<T> &lval,const CSPXTVector<T> &rval);
   // y = x/3.
   friend CSPXTVector<T> operator / (const CSPXTVector<T> &lval,T rval);
   // x /= 3.; 
   CSPXTVector<T> &operator /= (T rval);

   T Norm();

    // x.Normalize(); 
   void Normalize(); 
   // orders the vector
   void Order(); 
   
   // swaps the contents of two vectors y = x.Swap()
   CSPXTVector<T> Swap();
	T SqrtSumSqr(int n, int m);
};


template<class T> class Vector
{
	private: T* v;
	private: int sz;

	public:	Vector() {};
	public:	Vector(int) {};
    public:	T& elem(int i) {return v[i];}
	public:	T&operator[](int i);
	public:	void swap(Vector&);
};

//specialization of Vector<T>
template<> class Vector<void*>
{
    private:void**p;
    public:	void*& elem(int i) {return p[i]; }
	public:void*& operator[](int i) { return *p; };
};

template<class T> class Vector<T*>:private Vector<void*>
{
	public:	typedef Vector<void*> Base;
	public:	Vector():Base(){}
	public:	explicit Vector(int i):Base(i) {}
	public:	T*& elem(int i){ return static_cast<T*&>(Base::elem(i)); }
	public:	T*& operator[](int i){ return static_cast<T*&>(Base::operator[](i)); }
};
