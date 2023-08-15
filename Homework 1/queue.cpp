#include "queue.h"

template<typename T>
void Queue<T>::resize()
{
    this->capacity *= 2;
    T* temp = new T[this->capacity];

    for (size_t i = this->first; i < this->last; ++i)
    {
        temp[i] = this->data[i];
    }

    std::swap(temp, this->data);
    delete[] temp;
}

template<typename T>
Queue<T>::Queue() : data(new T[1]), capacity(1), first(0), last(0)
{
    
}

template<typename T>
Queue<T>::~Queue()
{
    delete[] data;
}

template<typename T>
Queue<T>::Queue(const Queue<T>& other)
{
    try
    {
        this->data = new T[other.capacity];
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "Memory problem" << '\n';
        throw e;
    }
    
    this->capacity = other.capacity;
    this->last = other.last;
    this->first = other.first;

    for (size_t i = 0; i < other.capacity; ++i)
    {
        this->data[i] = other.data[i];
    }
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other)
{
    Queue<T> temp = other;
    std::swap(this->data, temp.data);
    std::swap(this->first, temp.first);
    std::swap(this->last, temp.last);
    std::swap(this->capacity, temp.capacity);

    return *this;
}

template<typename T>    
T& Queue<T>::front()
{
    return this->data[this->first];
}

template<typename T>
const T& Queue<T>::front() const
{
    return this->data[this->first]; 
}

template<typename T>
T& Queue<T>::back()
{
    assert(this->last > 0);
    return this->data[this->last - 1];
}

template<typename T>
const T& Queue<T>::back() const
{
    assert(this->last > 0);
    return this->data[this->last - 1]; 
}

template<typename T>
void Queue<T>::push(const T& add)
{
   if (this->last == this->capacity)
    {
        resize();
    }

    this->data[this->last] = add;
    ++this->last; 
}

template<typename T>
void Queue<T>::pop()
{
    ++this->first;
}

template<typename T>
size_t Queue<T>::size() const
{
    return this->last - this->first; 
}

template<typename T>
bool Queue<T>::empty() const 
{
    return size() == 0; 
}

template<typename T>
T& Queue<T>::operator[](size_t i) 
{
    if (empty() || i < 0 || i >= size()) 
    {
        throw std::out_of_range("Out of range");
    }

    return this->data[i];
}
