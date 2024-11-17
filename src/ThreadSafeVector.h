/*
 * ThreadSafeVector.h
 *
 *  Created on: Oct 19, 2024
 *      Author: bharat
 */
#ifndef THREADSAFEVECTOR_H_
#define THREADSAFEVECTOR_H_

#include <iostream>
#include <cstddef>
#include "ThreadHeader.h"
#include <stdexcept>
template <typename T>
class ThreadSafeVector {
private:
    size_t m_nSize;
    size_t m_nCapacity;
    size_t m_nAllocation;
    cCriticalSection m_cCriticalSection;
    T* m_Array;
    void resizeInternal(size_t newCapacity);

public:
    ThreadSafeVector();
    explicit ThreadSafeVector(size_t a_nSize);
    ThreadSafeVector(size_t a_nSize, size_t a_nAllocation);
    ~ThreadSafeVector();

    void push_back(const T& a_Data);
    T pop_back();
    void resize(size_t a_nSize);
    size_t size() const;
    T at(size_t a_nPos) const;
    bool isEmpty() const;
    void erase(size_t a_nIndex);
    T front() const;
    T back() const;

    ThreadSafeVector(const ThreadSafeVector&) = delete;
    ThreadSafeVector& operator=(const ThreadSafeVector&) = delete;

    ThreadSafeVector(ThreadSafeVector&& other) noexcept;
    ThreadSafeVector& operator=(ThreadSafeVector&& other) noexcept;
};

// Default constructor
template <typename T>
ThreadSafeVector<T>::ThreadSafeVector()
    : m_nSize(0), m_nCapacity(10), m_nAllocation(10), m_Array(new T[10]) {}

template <typename T>
ThreadSafeVector<T>::ThreadSafeVector(size_t a_nSize)
    : m_nSize(a_nSize), m_nCapacity(a_nSize), m_nAllocation(10), m_Array(new T[a_nSize]) {}

template <typename T>
ThreadSafeVector<T>::ThreadSafeVector(size_t a_nSize, size_t a_nAllocation)
    : m_nSize(a_nSize), m_nCapacity(a_nSize), m_nAllocation(a_nAllocation), m_Array(new T[a_nSize]) {}

template <typename T>
ThreadSafeVector<T>::~ThreadSafeVector() {
	if(true == std::is_pointer<T>::value)
	{
		for(int i=0; i<m_nSize;i++)
		{
			delete (T)m_Array[i];
		}
	}
    delete[] m_Array;
}

template <typename T>
void ThreadSafeVector<T>::resizeInternal(size_t newCapacity) {
    T* newArray = new T[newCapacity];
    memcpy(newArray, m_Array, sizeof(T) * m_nSize);
    delete[] m_Array;
    m_Array = newArray;
    m_nCapacity = newCapacity;
}

template <typename T>
void ThreadSafeVector<T>::push_back(const T& a_Data) {
    LockGuard lock(m_cCriticalSection);
    if (m_nSize == m_nCapacity) {
        resizeInternal(m_nCapacity + m_nAllocation);
    }
    m_Array[m_nSize++] = a_Data;
}

template <typename T>
T ThreadSafeVector<T>::pop_back() {
    LockGuard lock(m_cCriticalSection);
    if (m_nSize == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return m_Array[--m_nSize];
}

template <typename T>
void ThreadSafeVector<T>::resize(size_t a_nSize) {
    LockGuard lock(m_cCriticalSection);
    if (a_nSize > m_nCapacity) {
        resizeInternal(a_nSize + m_nAllocation);
    }
    m_nSize = a_nSize;
}

template <typename T>
size_t ThreadSafeVector<T>::size() const {
    LockGuard lock(m_cCriticalSection);
    return m_nSize;
}

template <typename T>
T ThreadSafeVector<T>::at(size_t a_nPos) const {
    LockGuard lock(m_cCriticalSection);
    if (a_nPos >= m_nSize) {
        throw std::out_of_range("Index out of bounds");
    }
    return m_Array[a_nPos];
}

template <typename T>
bool ThreadSafeVector<T>::isEmpty() const {
    LockGuard lock(m_cCriticalSection);
    return m_nSize == 0;
}

template <typename T>
void ThreadSafeVector<T>::erase(size_t a_nIndex) {
    LockGuard lock(m_cCriticalSection);
    if (a_nIndex >= m_nSize) {
        throw std::out_of_range("Index out of bounds");
    }
    memmove(m_Array + a_nIndex, m_Array + a_nIndex + 1, sizeof(T) * (m_nSize - a_nIndex - 1));
    --m_nSize;
}

template <typename T>
T ThreadSafeVector<T>::front() const {
    LockGuard lock(m_cCriticalSection);
    if (m_nSize == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return m_Array[0];
}

template <typename T>
T ThreadSafeVector<T>::back() const {
    LockGuard lock(m_cCriticalSection);
    if (m_nSize == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return m_Array[m_nSize - 1];
}

template <typename T>
ThreadSafeVector<T>::ThreadSafeVector(ThreadSafeVector&& other) noexcept
    : m_nSize(other.m_nSize), m_nCapacity(other.m_nCapacity),
      m_nAllocation(other.m_nAllocation), m_Array(other.m_Array) {
    other.m_Array = nullptr;
    other.m_nSize = 0;
    other.m_nCapacity = 0;
}


template <typename T>
ThreadSafeVector<T>& ThreadSafeVector<T>::operator=(ThreadSafeVector&& other) noexcept {
    if (this != &other) {
        delete[] m_Array;
        m_nSize = other.m_nSize;
        m_nCapacity = other.m_nCapacity;
        m_nAllocation = other.m_nAllocation;
        m_Array = other.m_Array;

        other.m_Array = nullptr;
        other.m_nSize = 0;
        other.m_nCapacity = 0;
    }
    return *this;
}

#endif /* THREADSAFEVECTOR_H_ */
