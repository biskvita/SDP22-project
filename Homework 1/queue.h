#pragma once

#include <cassert>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

///////////////////////////////

template <typename T>
class Queue
{
private:
    T* data;
    size_t capacity;
    size_t first;
    size_t last;

    void resize();
public:
    Queue();
    ~Queue();
    
    Queue(const Queue<T>& other);
    Queue<T>& operator=(const Queue<T>& other);
    
    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    void push(const T& add);
    void pop();

    size_t size() const;
    bool empty() const;

    T& operator[] (size_t i);
};