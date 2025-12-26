#ifndef CSPXLIST_H
#define CSPXLIST_H

#include <string.h>

/* T defines a non-pointer type, but the list elements are pointers
 * The container may own the element in which case it will be deleted automatically when the list is destroyed
 *
 *
 */


template<class T, bool autoDelete = false> class CSPXListNode
{
    private:bool m_autodelete;

    public: T *m_element;
    public: char *m_key;

    public:
        CSPXListNode() : m_key(nullptr), m_element(nullptr), nextnode(nullptr), prevnode(nullptr), m_autodelete(autoDelete) {}
        virtual ~CSPXListNode();

        CSPXListNode<T>* nextnode;
        CSPXListNode<T>* prevnode;

        CSPXListNode<T> *GetNext() { return nextnode; }
        CSPXListNode<T> *GetPrev() { return prevnode; }
        void SetNext(CSPXListNode<T> *sn) { nextnode = sn; }
        void SetPrev(CSPXListNode<T> *sn) { prevnode = sn; }

        T* GetElement()
        {
            return m_element;
        }
        void SetElement(T* element)
        {
            m_element = element;
        }

        char* GetKey()
        {
            return m_key;
        }
        void SetKey(const char* key)
        {
            if (m_key != nullptr)
                delete m_key;

            m_key = nullptr;

            if (key == nullptr)
                return;

            if (m_key != nullptr)
                delete m_key;

            int minl = strlen(key);
            m_key = new char[minl+1];
            strncpy(m_key,key,minl);
            m_key[minl] = '\0';
        }

        void SetAutoDelete(bool flag) { m_autodelete = flag; }
        bool GetAutoDelete() { return m_autodelete; }
};

template<class T> class Iterator
{
    template<class L> friend class CSPXList;

    private:CSPXListNode<T> *m_node;
    public:
        Iterator() : m_node(nullptr) {};

        T* GetItem() { return (m_node==nullptr)?nullptr:m_node->GetElement(); }

        bool hasItems() { return m_node==nullptr?false:true; }

        T* operator -> () { return (m_node==nullptr)?nullptr:m_node->GetElement(); }

        // pre and postfix increment and decrement operators (int is a dummy param to distinguish between pre and post operators)
        Iterator<T>& operator ++ ()
        {
            if (m_node)
                m_node = m_node->GetNext();
            return *this;
        }
        Iterator<T>& operator ++ (int)
        {
            if (m_node)
                m_node = m_node->GetNext();
            return *this;
        }
        Iterator<T>& operator -- ()
        {
            if (m_node)
                m_node = m_node->GetPrev();
            return *this;
        }
        Iterator<T>& operator -- (int)
        {
            if (m_node)
                m_node = m_node->GetPrev();
            return *this;
        }

        //dereference operator
        T& operator*()
        {
            return *(T*)(m_node->GetElement());
        }
};

template<class T> class CSPXList
{
    public:	CSPXList() : size(0),m_iterator() { headnode = tailnode = nullptr; }
    public:	CSPXList(CSPXList<T> &copy) { Copy(&copy); }
    public:	~CSPXList();

    private:CSPXListNode<T>* headnode;
    private:CSPXListNode<T>* tailnode;

    private:int size;

    protected:void SetHead(CSPXListNode<T>* hn) { headnode = hn; }
    protected:void SetTail(CSPXListNode<T>* tn) { tailnode = tn; }
    protected:void SetSize(int s) { size = s; }
    protected:void DecSize() { size--; }

    public:CSPXListNode<T>* GetHead() { return headnode; }
    public:CSPXListNode<T>* GetTail() { return tailnode; }

    public:	void Append(T* element, bool autodelete = false);
    public:	bool AppendUnique(T* element, bool autodelete, bool (*compare)(T*, T*) = nullptr);
    public:	void Insert(T* element, int index, bool autodelete = true);
    public:	void Insert(T* element, T* here, bool after, bool autodelete = true);
    public:	int DeleteElement(T* element, bool (*compare)(T*, T*) = nullptr);
    public:	void Delete(T * element);
    public:	void Delete(int index);
    public:	int Unlink(T* element);
    public:	void Move2Back(T* element);
    public:	void Move2Front(T* element);

    public:	T* GetElement(int index);
    // assigns and receives the pointer set by the index
    public:T* operator [] (int i);
    public:	void SetElement(int index, T* element);
    public:	int FindElement(T* element, bool (*compare)(T*, T*) = nullptr);
    public:	int FindElements(T* element, CSPXList<T> *list, bool (*compare)(T*, T*));
    public:	int GetSize() { return size; }
    public:	void Flush();
    public:	void Copy(CSPXList<T> *copy);

    public:	CSPXList<T>& operator = (CSPXList<T> &rvalue) { Copy(&rvalue); return *this; };

    //Iterator pattern
    private:Iterator<T> m_iterator;
    public:Iterator<T>& GetIterator() { m_iterator.m_node = GetHead(); return m_iterator; }

    //functor for Visitor? pattern
    private:template <class L> friend class Visitor;
};

template<class T> CSPXList<T>::~CSPXList()
{
    Flush();
}

template<class T, bool b> CSPXListNode<T,b>::~CSPXListNode()
{
    if (m_autodelete)
        delete m_element;

}

template<class T> void CSPXList<T>::Flush()
{
    if (size > 0)
    {
        CSPXListNode<T> *current = GetTail();
        if(current)
        {
            CSPXListNode<T> *prev = current->GetPrev();
            while (prev)
            {
                delete current;
                current = prev;
                prev = prev->GetPrev();
            }

            delete current;
        }

        size = 0;
        headnode = tailnode = nullptr;
    }
}

template<class T> void CSPXList<T>::Copy(CSPXList<T> *copy)
{
    if (size > 0)
    {
        CSPXListNode<T> *current = GetHead();
        while(current)
        {
            copy->Append(current->GetElement());
            current = current->GetNext();
        }
    }
}

template<class T> void CSPXList<T>::Append(T* element, bool autodelete)
{
    CSPXListNode<T> *sn = new CSPXListNode<T>;
    sn->SetElement(element);
    sn->SetAutoDelete(autodelete);

    if (size <= 0)
    {
        headnode = tailnode = sn;
        headnode->SetNext(nullptr);
        headnode->SetPrev(nullptr);
    }
    else
    {
        tailnode->SetNext(sn);
        sn->SetPrev(tailnode);
        sn->SetNext(nullptr);
        tailnode = sn;
    }

    size++;
}

template<class T> bool CSPXList<T>::AppendUnique(T* element, bool autodelete, bool (*compare)(T*, T*))
{
    if (FindElement(element, compare) != -1)
        return false;

    CSPXListNode<T> *sn = new CSPXListNode<T>;
    sn->SetElement(element);
    sn->SetAutoDelete(autodelete);

    if (size == 0)
    {
        headnode = tailnode = sn;
        headnode->SetNext(nullptr);
        headnode->SetPrev(nullptr);
    }
    else
    {
        tailnode->SetNext(sn);
        sn->SetPrev(tailnode);
        sn->SetNext(nullptr);
        tailnode = sn;
    }

    size++;

    return true;
}

template<class T> void CSPXList<T>::Insert(T* element, int index, bool autodelete)
{
    CSPXListNode<T> * currentnode = nullptr;
    CSPXListNode<T> *sn = new CSPXListNode<T>;
    sn->SetElement(element);
    sn->SetAutoDelete(autodelete);

    if (size == 0)
    {
        // ignore the index
        headnode = tailnode = sn;
    }
    else if (size <= index)
    {
        tailnode->SetNext(sn);
        sn->SetPrev(tailnode);
        tailnode = sn;
    }
    else
    {
        if (index == 0)
        {
            sn->SetPrev(nullptr);
            sn->SetNext(headnode);
            headnode->SetPrev(sn);
            headnode = sn;
        }
        else
        {
            currentnode = headnode;
            int count = 1;
            while(currentnode != nullptr)
            {
                if (count == index)
                    break;
                currentnode = currentnode->GetNext();
                count++;
            }
        }

        if (currentnode != nullptr && index > 0)
        {
            // insert
            sn->SetPrev(currentnode);
            sn->SetNext(currentnode->GetNext());
            currentnode->GetNext()->SetPrev(sn);
            currentnode->SetNext(sn);
        }
    }

    size++;
}

template<class T> void CSPXList<T>::Insert(T* element, T* here, bool after , bool autodelete)
{
    CSPXListNode<T> * currentnode = nullptr;
    CSPXListNode<T> *sn = new CSPXListNode<T>;
    sn->SetElement(element);
    sn->SetAutoDelete(autodelete);

    if (size == 0)
    {
        // ignore the after
        headnode = tailnode = sn;
    }
    else if (here == nullptr)
    {
        sn->SetNext(headnode);
        sn->GetNext()->SetPrev(sn);
        sn->SetPrev(nullptr);
        headnode = sn;
    }
    else if (after)
    {
        currentnode = headnode;
        while(currentnode != nullptr)
        {
            if (currentnode->GetElement() == here)
                break;
            currentnode = currentnode->GetNext();
        }

        // insert after
        sn->SetPrev(currentnode);
        sn->SetNext(currentnode->GetNext());
        if (currentnode->GetNext() == nullptr)
            tailnode = sn;
        if (tailnode == currentnode->GetNext())
            tailnode->SetPrev(sn);

        currentnode->SetNext(sn);


    }
    else //before
    {
        currentnode = headnode;
        while(currentnode != nullptr)
        {
            if (currentnode->GetElement() == here)
                break;
            currentnode = currentnode->GetNext();
        }

        // insert before
        sn->SetNext(currentnode);
        sn->SetPrev(currentnode->GetPrev());
        currentnode->SetPrev(sn);
        if (currentnode == headnode)
            headnode = sn;
    }

    size++;
}

template<class T> int CSPXList<T>::Unlink(T * element)
{
    CSPXListNode<T> *currentnode = headnode;

    while (currentnode != nullptr)
    {
        T* te = currentnode->GetElement();
        if (te == element)
        {
            bool ad = currentnode->GetAutoDelete();
            // unlink
            if (currentnode->GetPrev() != nullptr)
                currentnode->GetPrev()->SetNext(currentnode->GetNext());
            if (currentnode->GetNext() != nullptr && currentnode != headnode)
                currentnode->GetNext()->SetPrev(currentnode->GetPrev());
            else if (currentnode == headnode)
            {
                headnode = currentnode->GetNext();
                if (headnode != nullptr)
                    headnode->SetPrev(nullptr);
            }

            if (currentnode == tailnode)
                tailnode = currentnode->GetPrev();

            //delete node NOT the element
            delete currentnode;

            size--;
            if (size == 0)
                tailnode = headnode = nullptr;

            return ad?1:0;
        }
        currentnode = currentnode->GetNext();
    }

    return -1;
}

template<class T> void CSPXList<T>::Move2Back(T * element)
{
    int ad = Unlink(element);
    if (ad >= 0)
        Append(element,ad);
}

template<class T> void CSPXList<T>::Move2Front(T * element)
{
    int ul = Unlink(element);
    if (ul>=0)
        Insert(element,0,(bool)ul);
}

template<class T> void CSPXList<T>::Delete(T * element)
{
    bool (*compare)(T*, T*);

    if (size == 0)
        return;

    int index = DeleteElement(element, nullptr);

    if (index < 0)
        return;

    if (size == 0)
        headnode = tailnode = nullptr;
}

template<class T> void CSPXList<T>::Delete(int index)
{
    CSPXListNode<T> *currentnode = nullptr;

    if (size == 0 || index > size || index < 0)
    {
        return;
    }
    else
    {
        currentnode = headnode;
        int count = 0;
        while(currentnode != nullptr)
        {
            if (count == index)
                break;
            currentnode = currentnode->GetNext();
            count++;
        }

        // unlink
        if (currentnode->GetPrev() != nullptr)
            currentnode->GetPrev()->SetNext(currentnode->GetNext());
        if (currentnode->GetNext() != nullptr && currentnode != headnode)
            currentnode->GetNext()->SetPrev(currentnode->GetPrev());
        else if (currentnode == headnode)
        {
            headnode = currentnode->GetNext();
            if (headnode != nullptr)
                headnode->SetPrev(nullptr);
        }

        if (currentnode->GetAutoDelete())
        {
            //if the element is a referenced object, it will be deleted if its m_autodelete member is true
            if (currentnode->m_element != nullptr)
            {
                delete currentnode->m_element;
                currentnode->m_element = nullptr;
            }
        }
        delete currentnode;
        currentnode = nullptr;
        size--;
    }

    if (size == 0)
        headnode = tailnode = nullptr;
}

template<class T> void CSPXList<T>::SetElement(int index, T* element)
{
    if (index > size)
        return;

    CSPXListNode<T> *currentnode = nullptr;
    currentnode = headnode;
    int count = 0;
    while(currentnode != nullptr)
    {
        if (count == index)
        {
            currentnode->SetElement(element);
            break;
        }

        currentnode = currentnode->GetNext();
        count++;
    }
}

template<class T> T* CSPXList<T>::GetElement(int index)
{
    if (index > size)
        return nullptr;

    CSPXListNode<T> *currentnode = nullptr;
    currentnode = headnode;
    int count = 0;
    while(currentnode != nullptr)
    {
        if (count == index)
            return currentnode->GetElement();

        currentnode = currentnode->GetNext();
        count++;
    }

    return nullptr;
}

template<class T> T* CSPXList<T>::operator [] (int i)
{
    return GetElement(i);
}

template<class T> int CSPXList<T>::FindElement(T* element, bool (*compare)(T*, T*))
{
    int depth = 0;
    CSPXListNode<T> *currentnode = headnode;

    while (currentnode != nullptr)
    {
        T* te = currentnode->GetElement();
        if (compare == nullptr)
        {
            if (te == element)
                return depth;
        }
        else if (compare(te,element))
            return depth;

        currentnode = currentnode->GetNext();
        depth++;
    }

    return -1;
}

template<class T> int CSPXList<T>::FindElements(T* element, CSPXList<T> *list, bool (*compare)(T*, T*))
{
    int count = 0;
    CSPXListNode<T> *currentnode = headnode;

    while (currentnode != nullptr)
    {
        T* te = currentnode->GetElement();
        if (compare(te,element))
        {
            count++;
            list->Append(te);
        }

        currentnode = currentnode->GetNext();
    }

    return count;
}

template<class T> int CSPXList<T>::DeleteElement(T* element, bool (*compare)(T*, T*))
{
    int depth = 0;
    CSPXListNode<T> *currentnode = headnode;

    while (currentnode != nullptr)
    {
        T* te = currentnode->GetElement();
        if (compare == nullptr)
        {
            if (te == element)
            {
                // unlink
                if (currentnode->GetPrev() != nullptr)
                    currentnode->GetPrev()->SetNext(currentnode->GetNext());
                if (currentnode->GetNext() != nullptr && currentnode != headnode)
                    currentnode->GetNext()->SetPrev(currentnode->GetPrev());
                else if (currentnode == headnode)
                {
                    headnode = currentnode->GetNext();
                    if (headnode != nullptr)
                        headnode->SetPrev(nullptr);
                }

                if (currentnode == tailnode)
                    tailnode = currentnode->GetPrev();

                if (currentnode->GetAutoDelete())
                {
                    if (currentnode->m_element != nullptr)
                    {
                        delete currentnode->m_element;
                        currentnode->m_element = nullptr;
                    }
                }

                delete currentnode;

                size--;
                if (size == 0)
                    tailnode = headnode = nullptr;

                return depth;
            }
            currentnode = currentnode->GetNext();
        }
        else if (compare(te,element))
        {
            // unlink
            if (currentnode->GetPrev() != nullptr)
                currentnode->GetPrev()->SetNext(currentnode->GetNext());
            if (currentnode->GetNext() != nullptr && currentnode != headnode)
                currentnode->GetNext()->SetPrev(currentnode->GetPrev());
            else if (currentnode == headnode)
            {
                headnode = currentnode->GetNext();
                if (headnode != nullptr)
                    headnode->SetPrev(nullptr);
            }

            if (currentnode == tailnode)
            {
                tailnode = currentnode->GetPrev();
            }

            if (currentnode->GetAutoDelete())
            {
                if (currentnode->m_element != nullptr)
                {
                    delete currentnode->m_element;
                    currentnode->m_element = nullptr;
                }
            }
            delete currentnode;

            size--;
            if (size == 0)
                tailnode = headnode = nullptr;

            return depth;
        }
        else
            currentnode = currentnode->GetNext();
        depth++;
    }

    return -1;
}

//can be called from functor
template <class T>
bool compare(T* a, T* b)
{
    return *a==*b;//T for complex objects needs to overload the == binary operator
};

//every functor calls the specific non-template or generic template friend functions
template <class T>
class CompareFunctor
{
    private:template<class L> friend bool compare(T* a, T* b);
    public:bool operator() (CSPXList<T> &lst, T* object)
    {
        Iterator<T> iter = lst.GetIterator();
        while(iter.hasItems())
        {
            if (compare(iter.GetItem(),object))
                return true;
            iter++;
        }
        return false;
    }
};

//functor wrapper Visitor includes several Functors
template <class T>
class Visitor
{
    public:Visitor() {}
    public: CompareFunctor<T> m_comparer;
};

#endif // CSPXLIST_H
