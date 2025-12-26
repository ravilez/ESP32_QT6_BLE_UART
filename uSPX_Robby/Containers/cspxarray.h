#ifndef CSPXARRAY_H
#define CSPXARRAY_H

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

template<class T> class CSPXArray
{
public:
    CSPXArray(): size(0) { Element = nullptr; }
    ~CSPXArray() { DestroyAll(); }

    bool Destroy(int location);
    bool DestroyAll();
    T &operator [](int location);
    int  Add(T *element);
    void AddUnique(T* element, bool (*compare)(T*, T*) = nullptr);
    int  Insert(T *element,int location);
    int  GetSize() { return size; }
    T &  GetElement(int idx);
    int FindElement(T* element, bool (*compare)(T*, T*));
    void QuickSort(int left, int right);
    int BinarySearch(T* element);

protected:
    int size;
    T *Element;
};

template<class T> T &CSPXArray<T>::operator [](int location)
{
    if (size <= location || size == 0)
   {
      for (int i = size; i <= location; i++)
           Add(0);
   }

    return Element[location];
}

template<class T> T &CSPXArray<T>::GetElement(int idx)
{
    return Element[idx];
}

template<class T> int CSPXArray<T>::Add(T *element)
{
    if (size == 0)
    {
        // Create Element pointer of size one.
        Element = new T[1];
    // store the data in Element
        // (for deep copies a assignment operator must be provided )
        if (element != nullptr)
            Element[0] = *element;
        size++;
    }
    else
    {
        T *tempArray = new T[size+1];

        // move data into temporary array (note, we are shifting pointers)
        for (int i = 0; i < size; i++)
            tempArray[i] = Element[i];

        delete [] Element;

        Element = tempArray;
        // Add pointer to the new element
        if (element != nullptr)
            Element[size] = *element;
        size++;
    }

    return size;
}

template<class T> bool CSPXArray<T>::Destroy(int location)
{
    if (size < location + 1)
        return false;

    if (size == 1)
    {
        // last element
        delete Element;
        Element = nullptr;
        size = 0;
        return true;
    }

    T *tempArray = new T[size-1];
    for (int i = 0 ; i < location; i++)
    {
        tempArray[i] = Element[i];
    } // skip if location = 0

    for ( int j = location+1 ; j < size; j++)
    {
        tempArray[j-1] = Element[j];
    }

    delete [] Element;

    Element = tempArray;
    size--;

    return true;
}

template<class T> bool CSPXArray<T>::DestroyAll()
{
    if (size == 0)
        return false;

    // delete all elements
    delete [] Element;
    Element = nullptr;
    size = 0;

    return true;
}

template<class T> int CSPXArray<T>::Insert(T *element,int location)
{
    if (size == 0)
    {
        // Create Element of size one.
        Element = new T[size+1];
        Element[0] = *element;
        size++;
    }
    else
    {
        T *tempArray = new T[size+1];

        // Copy previous data into temporary array.
        for (int i = 0; i < location; i++)
            tempArray[i] = Element[i];

        // Insert the new element
        tempArray[location] = *element;

        for (int j = location + 1; j < size+1; j++)
            tempArray[j] = Element[j-1];

        delete [] Element;
        Element = tempArray;
        size++;
    }

    return size;
}

template<class T> int CSPXArray<T>::FindElement(T* element, bool (*compare)(T*, T*))
{
    for (int i = 0; i < size; i++)
    {
        if (compare == nullptr)
        {
            if (Element[i] == element)
                return i;
        }
        else
        {
            if (compare(Element[i],element))
                return i;
        }
    }

    return -1;
}

template<class T> void CSPXArray<T>::AddUnique(T* element, bool (*compare)(T*, T*))
{
    if (FindElement(element, compare) != -1)
        return;
    else
        Add(element);
}

template<class T> void CSPXArray<T>::QuickSort(int left, int right)
{
    int i = MAX(left,0), j = MIN(right,size);
    T *tmp;
    T *pivot = Element[(left + right) / 2];

    /* partition */
    while (i <= j)
    {
        while (Element[i] < pivot) //class needs to overload < and > operators
            i++;
        while (Element[j] > pivot)
            j--;
        if (i <= j)
        {
            tmp = Element[i];
            Element[i] = Element[j];
            Element[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        QuickSort(left, j);
    if (i < right)
        QuickSort(i, right);
}

template<class T> int CSPXArray<T>::BinarySearch(T* key)
{
    int low, high, mid;

    low = 0;
    high = mid = size-1;

    while(low <= high)
    {
        mid = (low+high)/2;
        if (key < Element[mid])
            high = mid - 1;
        else if (key > Element[mid])
            low = mid + 1;
        else
            return Element[mid];
    }
    return -1;
}


#endif // CSPXARRAY_H
