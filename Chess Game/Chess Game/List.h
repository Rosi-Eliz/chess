#pragma once
#include <iostream>
using namespace std;


template <typename T>
struct Element {
    T value;
    Element* previous;
    Element* next;
};

template <typename T>
class List {

    Element<T>* front;
    Element<T>* rear;
public:
    List();
    ~List();
    List(const List& list);
    void insert(const T& value, int index);
    bool isEmpty() const;
    int size() const;
    Element<T>* elementAt(int index);
    void initialPush(const T& value);
    void pushFront(const T& value);
    void pushRear(const T& value);
    void removeAt(int index);
    void print() const;
    void rangeDestructor(int from, int to);
    void operator= (const List& list);
    T& operator[] (int index);
    void operator += (const List& list);
    void swap(int first, int second);
    void sort();
    bool contains(int value) const;
    void reverse();
    void filter(int value);
};

template <typename T>
List<T>::List() {
    front = nullptr;
    rear = nullptr;
}

template <typename T>
List<T>::List(const List<T>& list) {
    front = nullptr;
    rear = nullptr;
    *this = list;
}


template <typename T>
List<T>::~List() {
    Element<T>* first = rear;
    if (!isEmpty())
    {
        while (first != nullptr)
        {
            Element<T>* elementForDeletion = first;
            first = first->next;
            delete elementForDeletion;
        }
    }

    this->front = nullptr;
    this->rear = nullptr;
    return;
}

/*
template <typename T>
void List<T>::rangeDestructor(int from, int to)
{
    if (from < 0 || to < 0 || from > size() || to > size())
    {
        throw runtime_error("Invalid index!");
    }
    if (this->isEmpty())
    {
        return;
    }
    //int elementsToRemove = to - from;
    List<T> newList;
    Element<T>* back = rear;
    int count{ 0 };
    while (back != nullptr)
    {
        if (count < from || count > to )
        {
            Element<T>* elementToPush = back;
            newList.pushOtprede(elementToPush->value);
        }
        back = back->next;
        count++;
    }
    Element<T>* backCopy = rear;
    while (backCopy != nullptr)
    {
        Element<T>* elementForDeletion = backCopy;
        backCopy = backCopy->next;
        delete elementForDeletion;
    }
    *this = newList;
}
*/

template <typename T>
void List<T>::rangeDestructor(int from, int to) {
    if (from < 0 || to < 0 || from > size() || to > size() || to <= from)
    {
        throw runtime_error("Invalid index!");
    }
    if (this->isEmpty())
    {
        return;
    }

    if (from > 0 && to < size() - 1)
    {
        Element<T>* elementFrom = elementAt(from);
        Element<T>* elementTo = elementAt(to);
        elementFrom->previous->next = elementTo->next;
        elementTo->next->previous = elementFrom->previous;

        Element<T>* elementFollowingTo = elementTo->next;
        while (elementFrom != elementFollowingTo)
        {
            Element<T>* elementForDeletion = elementFrom;
            elementFrom = elementFrom->next;
            delete elementForDeletion;
        }

    }
    else if (from == 0 && to < size() - 1)
    {
        Element<T>* elementFrom = elementAt(from);
        Element<T>* elementTo = elementAt(to);
        rear = elementTo->next;
        rear->previous = nullptr;
        Element<T>* elementFollowingTo = elementTo->next;
        while (elementFrom != elementFollowingTo)
        {
            Element<T>* elementForDeletion = elementFrom;
            elementFrom = elementFrom->next;
            delete elementForDeletion;
        }
    }
    else if (from > 0 && to == size() - 1)
    {
        Element<T>* elementFrom = elementAt(from);
        Element<T>* elementTo = elementAt(to);
        front = elementFrom->previous;
        front->next = nullptr;
        Element<T>* elementFollowingTo = elementTo->next;
        while (elementFrom != elementFollowingTo)
        {
            Element<T>* elementForDeletion = elementFrom;
            elementFrom = elementFrom->next;
            delete elementForDeletion;
        }
    }
    else if (from == 0 && to == size() - 1)
    {
        Element<T>* first = rear;
        if (!isEmpty())
        {
            while (first != nullptr)
            {
                Element<T>* elementForDeletion = first;
                first = first->next;
                delete elementForDeletion;
            }
        }

        this->front = nullptr;
        this->rear = nullptr;

    }




}

template <typename T>
bool List<T>::isEmpty() const {
    return front == nullptr && rear == nullptr;
}

template <typename T>
Element<T>* List<T>::elementAt(int index) {
    if (isEmpty() || index >= size() || index < 0)
    {
        return nullptr;
    }
    Element<T>* ptr = rear;
    int count{ 0 };
    while (count != index)
    {
        ptr = ptr->next;
        count++;
    }
    return ptr;
}

template <typename T>
T& List<T>::operator[] (int index)
{
    if (elementAt(index) != nullptr)
        return elementAt(index)->value;
    else throw runtime_error("NULLPTR");
}

template <typename T>
int List<T>::size() const {
    int count{ 0 };
    Element<T>* ptr = rear;
    while (ptr != nullptr)
    {
        ptr = ptr->next;
        count++;
    }
    return count;
}

template <typename T>
void List<T>::insert(const T& value, int index)
{
    Element<T>* successor = elementAt(index);
    if (successor == nullptr) {
        throw runtime_error("greshka.");
    }
    Element<T>* newElement = new Element<T>;
    newElement->value = value;
    newElement->next = successor;
    newElement->previous = successor->previous;
    if (successor->previous != nullptr)
    {
        successor->previous->next = newElement;
    }
    else {
        rear = newElement;
    }
    successor->previous = newElement;
}

template <typename T>
void List<T>::print() const {
    if (isEmpty())
    {
        return;
    }
    Element<T>* zaden = rear;
    bool flag = false;
    while (zaden != nullptr)
    {
        if (flag)
            cout << ", ";
        cout << zaden->value;
        zaden = zaden->next;
        flag = true;
    }
}

template <typename T>
void List<T>::initialPush(const T& value)
{
    Element<T>* novodupe = new Element<T>;
    novodupe->value = value;
    front = novodupe;
    rear = front;
    novodupe->next = nullptr;
    novodupe->previous = nullptr;
}

template <typename T>
void List<T>::pushFront(const T& value)
{
    if (isEmpty())
    {
        initialPush(value);
        return;
    }
    Element<T>* dupe = front;
    Element<T>* novodupe = new Element<T>;
    novodupe->value = value;
    novodupe->next = nullptr;
    novodupe->previous = dupe;
    dupe->next = novodupe;
    front = novodupe;
}

template <typename T>
void List<T>::pushRear(const T& value)
{

    if (isEmpty())
    {
        initialPush(value);
        return;
    }
    Element<T>* dupe = rear;
    Element<T>* novodupe = new Element<T>;
    novodupe->value = value;
    novodupe->next = dupe;
    novodupe->previous = nullptr;
    dupe->previous = novodupe;
    rear = novodupe;
}

template <typename T>
void List<T>::removeAt(int index)
{
    Element<T>* elementToRemove = elementAt(index);
    if (isEmpty())
    {
        throw runtime_error("nothing to remove");
    }
    if (index == 0)
    {
        rear = elementToRemove->next;//dob
        elementToRemove->next->previous = nullptr;
        delete elementToRemove;
    }
    else if (index == (size() - 1))
    {
        front = elementToRemove->previous;//samo che sa na obratno rear ->
        elementToRemove->previous->next = nullptr;
        delete elementToRemove;
    }
    else {
        elementToRemove->previous->next = elementToRemove->next;
        elementToRemove->next->previous = elementToRemove->previous;
        delete elementToRemove;
    }
}

template <typename T>
void List<T>::operator= (const List& list)
{
    if (this == &list) {
        return;
    }

    Element<T>* first = rear;
    if (!isEmpty()) //pri purvata iteraciq triem null mai nie veche sme promenili first da e first->next, no kogato imame
        //samo 1 element tova shte e nullptr
    {
        while (first != nullptr)
        {
            Element<T>* elementForDeletion = first;
            first = first->next;
            delete elementForDeletion;
        }
        this->front = nullptr;
        this->rear = nullptr;
    }
    if (list.isEmpty())
    {
        this->front = nullptr;
        this->rear = nullptr;
        return;
    }
    first = list.rear;
    while (first != list.front->next)
    {
        pushFront(first->value);
        first = first->next;
    }
}


template <typename T>
void List<T>::swap(int first, int second)
{
    T help = elementAt(first)->value;
    elementAt(first)->value = elementAt(second)->value;
    elementAt(second)->value = help;
}


template <typename T>
void List<T>::sort()
{
    for (int i{ 0 }; i < size(); i++)
    {
        for (int j{ i }; j < size(); j++)
        {
            if (elementAt(i)->value > elementAt(j)->value)
            {
                swap(i, j);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
void List<T>::operator += (const List& list)
{
    Element<T>* ptr = list.rear;
    Element<T>* ptrFront = list.front;
    while (ptr != ptrFront->next)
    {
        T value = ptr->value;
        pushFront(value);
        ptr = ptr->next;
    }
    return;

}

template <typename T>
bool List<T>::contains(int value) const
{
    Element<T>* ptr = rear;
    while (ptr != nullptr)
    {
        if (ptr->value == value)
            return true;
        ptr = ptr->next;

    }
    return false;
}



template <typename T>
void List<T>::reverse()
{

    int front = size() - 1;
    int rear = 0;
    while (front != rear) {
        for (int i{ 0 }; i < size() / 2; i++) {

            swap(front, rear);
            front--;
            rear++;
        }
    }
}

template <typename T>
void List<T>::filter(int value)
{
    if (!contains(value))
    {
        return;
    }
    Element<T>* ptr = rear;
    while (ptr != nullptr)
    {
        Element<T>* elementForDeletion = ptr;
        if (ptr->value == value)
        {
            delete elementForDeletion;
        }
        ptr = ptr->next;

    }

}

template <typename T>
List<T> split(List<T> list)
{
    List<T> listCopy = list;
    int pivotElement = list.size() / 2;

    int initialSize = list.size();
    if (initialSize == 1)
    {
        List<T> result;
        result.pushFront(listCopy.elementAt(0)->value);
        return result;
    }
    if (initialSize == 0)
    {
        List<T> result;
        return result;
    }
    List<T> pivotList;
    T pivotValue = listCopy.elementAt(pivotElement)->value;
    pivotList.pushFront(pivotValue);
    list.removeAt(pivotElement);
    List<T> leftPart;
    List<T> rightPart;
    Element<T>* ptr = list.elementAt(0);
    while (ptr != nullptr)
    {
        if (ptr->value < pivotValue)
        {
            leftPart.pushFront(ptr->value);
        }
        else {
            rightPart.pushFront(ptr->value);
        }
        ptr = ptr->next;
    }
    List<T> newLeft;
    newLeft = split(leftPart);
    List<T> newRight;
    newRight = split(rightPart);
    newLeft += pivotList;
    newLeft += newRight;
    return newLeft;
}




/*
329 6 14
return l + p + r //[123469]
l 3 2 14
    return l + p + r // [1234]
    l : 1
    p: 2
    r : 34 //split([3,4]) -> [3,4]
        l: 3
        p: 4
        r: -
p 6
r 9
*/