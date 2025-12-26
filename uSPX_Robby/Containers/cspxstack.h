#ifndef CSPXSTACK_H
#define CSPXSTACK_H

template<class T> class CSPXStackNode
{
    private:
        T* Element;

    public:
        CSPXStackNode() : Element(NULL) { nextnode = prevnode = NULL; }
        ~CSPXStackNode() {}

        CSPXStackNode<T>* nextnode;
        CSPXStackNode<T>* prevnode;

        CSPXStackNode<T> *GetNext() { return nextnode; }
        CSPXStackNode<T> *GetPrev() { return prevnode; }
        void SetNext(CSPXStackNode<T> *sn) { nextnode = sn; }
        void SetPrev(CSPXStackNode<T> *sn) { prevnode = sn; }

        T* GetElement() { return Element; }
        void SetElement(T* element) { Element = element; }
};

template<class T> class CSPXStack
{
    private:
        CSPXStackNode<T>* headnode;
        CSPXStackNode<T>* tailnode;

        int size;

    public:
        CSPXStack() : size(0) { tailnode = headnode = NULL; }
        virtual ~CSPXStack();

        CSPXStackNode<T>* GetHead() { return headnode; }
        CSPXStackNode<T>* GetTail() { return tailnode; }

        void Clear();
        int GetSize() { return size; }
        void PushFifo(T* element);
        void Push(T* element);
        T* PopFifo(); //Fifo
        T* Pop();
        T* Peek()
        {
            if (GetTail() == NULL)
                return NULL;

            return GetTail()->GetElement();
        }
        T* Peek(int index)
        {
            if (GetTail() == NULL)
                return NULL;

            int count = 0;
            CSPXStackNode<T> *node = GetTail();
            while(node)
            {
                if (count == index)
                    return node->GetElement();

                count++;
                node = node->GetPrev();
            }

            return NULL;
        }
        int FindElement(T* element, bool (*compare)(T*, T*));
};

template<class T> void CSPXStack<T>::Clear()
{
    if (size != 0)
        while(size != 0) Pop();
}

template<class T> CSPXStack<T>::~CSPXStack()
{
    Clear();
}

template<class T> void CSPXStack<T>::Push(T* element)
{
    if (element == NULL)
        return;
    CSPXStackNode<T> *sn = new CSPXStackNode<T>;
    sn->SetElement(element);
    if (size == 0)
    {
        headnode = tailnode = sn;
    }
    else
    {
        tailnode->SetNext(sn);
        sn->SetPrev(tailnode);
        tailnode = sn;
    }

    size++;
}

template<class T> void CSPXStack<T>::PushFifo(T* element)
{
    if (element == NULL)
        return;
    CSPXStackNode<T> *sn = new CSPXStackNode<T>;
    sn->SetElement(element);
    if (size == 0)
    {
        headnode = tailnode = sn;
    }
    else
    {
        sn->SetNext(headnode);
        sn->SetPrev(NULL);
        headnode->SetPrev(sn);
        headnode = sn;
    }

    size++;
}

template<class T> T* CSPXStack<T>::Pop()
{
    CSPXStackNode<T> *temp = NULL;

    T* element = NULL;

    if (size == 0)
        return NULL;

    if (tailnode != NULL)
    {
        temp = tailnode->GetPrev();
        element = tailnode->GetElement();
        delete tailnode;
        tailnode = temp;
    }

    size--;

    if (size == 0)
        headnode = tailnode = NULL;

    return element;
}

template<class T> T* CSPXStack<T>::PopFifo()
{
    CSPXStackNode<T> *temp = NULL;

    T* element = NULL;

    if (size == 0)
        return NULL;

    if (headnode != NULL)
    {
        temp = headnode->GetNext();
        element = headnode->GetElement();
        delete headnode;
        headnode = temp;
        if (headnode != NULL)
            headnode->SetPrev(NULL);
    }

    size--;

    if (size == 0)
        headnode = tailnode = NULL;

    return element;
}

template<class T> int CSPXStack<T>::FindElement(T* element, bool (*compare)(T*, T*))
{
    int depth = 0;
    CSPXStackNode<T> *tail = tailnode;

    while (tail != NULL)
    {
        T* te = tail->GetElement();
        if (compare(te,element))
            return depth;
        else
            tail = tail->GetPrev();
        depth++;
    }

    return -1;
}


#endif // CSPXSTACK_H
