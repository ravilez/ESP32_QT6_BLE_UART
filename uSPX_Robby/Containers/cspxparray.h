#ifndef CSPXPARRAY_H
#define CSPXPARRAY_H

#pragma once

#ifndef FALSE
    #define FALSE 0
    #define TRUE !FALSE
#endif


#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

template<class Type> class CSPXPArray
{
private:bool m_owner;
public:
    CSPXPArray(int lower = 0, int upper = 500, int del = 500) : m_owner(false)
    {
        lower_limit = lower;
        upper_limit = upper;
        if (del == 0)
            delta = 1;
        else
            delta = del;
        if (upper > lower)
            Array = new pType[upper-lower];
        else
            Array = nullptr;
        DataCount = 0;
        first_lower_limit = lower_limit;
        first_upper_limit = upper_limit;
    }

    ~CSPXPArray() { if (m_owner) Flush(); else delete [] Array; }

    bool Destroy(int location);
    bool Flush();
    Type* operator [](int location);
    int Add(Type* element);
    int Insert(Type* element,int location);
    int GetSize() { return DataCount; }
    void BubbleSort();
    void QuickSort(int left, int right);
    int BinarySearch(Type* element);

private:
    int lower_limit;
    int upper_limit;
    int delta;
    int first_lower_limit;
    int first_upper_limit;
    int DataCount;

    typedef Type* pType;

    pType *Array;//== Type** Array or arry of pointers
};

template<class Type> Type* CSPXPArray<Type>::operator [](int location)
{
    //range checking
    if (DataCount < location || location < lower_limit || location > upper_limit)
        return nullptr;
    else
        return Array[location];
}

template<class Type> int CSPXPArray<Type>::Add(Type* element)
{
    if (upper_limit > DataCount)
    {
        if (Array == nullptr) //fluh deleted the array
            Array = new pType[upper_limit];

        Array[DataCount] = element;
        DataCount++;
    }
    else if (DataCount == upper_limit)
    {
        pType *tempArray = new pType[upper_limit+delta];
        upper_limit += delta;

        // move data into temporary array (note, we are shifting pointers)
        for (int i = 0; i < DataCount; i++)
            tempArray[i] = Array[i];

        delete Array;

        Array = tempArray;
        // Add pointer to the new element
        Array[DataCount] = element;
        DataCount++;
    }

    return DataCount;
}

template<class Type> bool CSPXPArray<Type>::Destroy(int location)
{
    if (DataCount < location+1 || location < lower_limit || !m_owner)
        return FALSE;

    if (DataCount == 1)
    {
        // last element
        delete Array[location];
        delete Array;
        Array = nullptr;
        DataCount = 0;
        return TRUE;
    }

    pType *tempArray = new pType[DataCount-1];
    for (int i = 0 ; i < location; i++)
    {
        tempArray[i] = Array[i];
    } // skip if location = 0

    for ( int j = location+1 ; j < DataCount; j++)
    {
        tempArray[j-1] = Array[j];
    }


    delete Array[location];
    delete Array;

    Array = tempArray;
    DataCount--;

    return TRUE;
}

template<class Type> bool CSPXPArray<Type>::Flush()
{
    if (DataCount == 0)
    {
    return FALSE;
    }

    // delete all elements
    for ( int i = 0 ; i < DataCount; i++)
    {
        delete Array[i];
    }

    // delete pointer holding elements
    delete [] Array;
    Array = nullptr;
    DataCount = 0;

    return TRUE;
}

template<class Type> int CSPXPArray<Type>::Insert(Type* element,int location)
{
    if (DataCount == 0)
    {
        // Create Array of size one.
        Array = new pType[DataCount+1];
        Array[0] = element;
        DataCount++;
    }
    else if (DataCount < upper_limit && location < upper_limit && location >= lower_limit)
    {
        pType *tempArray = new pType[DataCount+1];

        // Copy previous data into temporary array.
        for (int i = 0; i < location; i++)
            tempArray[i] = Array[i];

        // Insert the new element
        tempArray[location] = element;

        for (int j = location + 1; j < DataCount+1; j++)
            tempArray[j] = Array[j-1];

        delete Array;

        Array = tempArray;
        DataCount++;
    }

    return DataCount;
}
template<class T> void CSPXPArray<T>::BubbleSort()
{
    bool swapped = true;
    int j = 0;
    T* tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < DataCount - j; i++) {
            if (*Array[i] > *Array[i + 1]) {
                tmp = Array[i];
                Array[i] = Array[i + 1];
                Array[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

template<class T> void CSPXPArray<T>::QuickSort(int left, int right)
{
    int i = MAX(left,0), j = MIN(right,DataCount);
    T *tmp;
    T *pivot = Array[(left + right) / 2];

    /* partition */
    while (i <= j)
    {
        while (*Array[i] < *pivot) //class needs to overload < and > operators
            i++;
        while (*Array[j] > *pivot)
            j--;
        if (i <= j)
        {
            tmp = Array[i];
            Array[i] = Array[j];
            Array[j] = tmp;
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

template<class Type> int CSPXPArray<Type>::BinarySearch(Type* key)
{
    int low, high, mid;

    low = 0;
    high = mid = DataCount-1;

    while(low <= high)
    {
        mid = (low+high)/2;
        if (*key < *Array[mid])
            high = mid - 1;
        else if (*key > *Array[mid])
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

#endif // CSPXPARRAY_H
