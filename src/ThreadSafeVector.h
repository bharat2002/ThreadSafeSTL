/*
 * ThreadSafeVector.h
 *
 *  Created on: Oct 19, 2024
 *      Author: bharat
 */

#ifndef THREADSAFEVECTOR_H_
#define THREADSAFEVECTOR_H_

#include<cstring>
#include <iostream>
#include <mutex>
using namespace std;

template <typename Types>
struct Node
{
	void* m_pnext;
	Types m_data;
};

template <typename T>
class ThreadSafeVector
{
private:
	size_t m_nSize;
	size_t m_nCapacity;
	size_t m_nAllocation;
	mutex m_cs;
	T* m_Array = nullptr;
	T* m_pEnd=nullptr;
public:
	ThreadSafeVector();
	ThreadSafeVector(size_t a_nSize);
	ThreadSafeVector(size_t a_nSize,size_t a_nAllocation);
	bool push_back(T a_Data);
	T pop_back();
	void resize(long a_nSize);
	long size();
	T& at(size_t a_nPos);
	bool isEmpty();
	T& erase(long a_nIndex)
	T& operator[](const long a_nSize);
	T& front();
	T& back();
	void insert(long a_nIndex, T a_Data);


};


template <typename T>
ThreadSafeVector<T>::ThreadSafeVector()
    : m_Array(nullptr), m_nSize(0), m_nCapacity(0), m_nAllocation(10)
{
	m_pEnd = new T;
}

template <typename T>
ThreadSafeVector<T>::ThreadSafeVector(size_t a_nSize)
    : m_nSize(0), m_nCapacity(a_nSize), m_nAllocation(10)
{
	m_pEnd = new T;
	m_Array = new T[a_nSize];
}

template <typename T>
ThreadSafeVector<T>::ThreadSafeVector(size_t a_nSize, size_t a_nAllocation)
    : m_nSize(0), m_nCapacity(a_nSize), m_nAllocation(a_nAllocation)
{
	m_pEnd = new T;
	m_Array = new T[a_nSize];
}


template <typename T>
T& ThreadSafeVector<T>::at(size_t a_nPos)
{
	if(a_nPos<m_nSize || a_nPos>=0)
	{
		return m_Array[a_nPos];
	}
	else
	{
		throw std::out_of_range("Index Out of Bonds");
	}
}

template <typename T>
bool ThreadSafeVector<T>::push_back(T a_Data)
{
	if(m_nSize == m_nCapacity)
	{
		T* m_NewArray = new T(m_nCapacity+m_nAllocation);
		memcpy(m_NewArray, m_Array, sizeof(T)*m_nSize);
		delete[] m_Array;
		m_Array = m_NewArray;
		m_NewArray = nullptr;
		m_nCapacity+=m_nAllocation;
	}
		m_Array[m_nSize++]=a_Data;
		return true;
}

template <typename T>
T ThreadSafeVector<T>::pop_back()
{
	if(m_nSize==0)
	{
		throw std::out_of_range("Out of Range");
	}
	else
	{
		return m_Array[--m_nSize];
	}
}

template <typename T>
long ThreadSafeVector<T>::size()
{
	return (long)(this->m_nSize);
}

template <typename T>
void ThreadSafeVector<T>::resize(long a_nSize)
{
	if(a_nSize<m_nCapacity)
	{
		m_nSize=a_nSize;
		return;
	}

	T* lpTempArray = new T[a_nSize+m_nAllocation];
	memcpy(lpTempArray, m_Array, sizeof(T)*m_nSize);
	delete[] m_Array;
	m_Array = lpTempArray;
	lpTempArray = nullptr;
	m_nSize = a_nSize;
	m_nCapacity = a_nSize+m_nAllocation;
	return;
}

template <typename T>
T& ThreadSafeVector<T>::operator[](const long a_nSize)
{
	return m_Array[a_nSize];
}

template <typename T>
T& ThreadSafeVector<T>::front()
{
	return this->at(0);
}

template <typename T>
T& ThreadSafeVector<T>::back()
{
	return this->at(m_nSize-1);
}

template <typename T>
void ThreadSafeVector<T>::insert(long a_nIndex, T a_Data)
{
	if(a_nIndex>=m_nSize || a_nIndex<0)
	{
		throw std::out_of_range("Index Out of Bonds");
	}
	else
	{
		this->at(a_nIndex) = a_Data
	}
}

template<typename T>
T& ThreadSafeVector<T>::erase(long a_nIndex)
{
	if(a_nIndex>=m_nSize || a_nIndex<0)
	{
		throw std::out_of_range("Index Out of Bonds");
	}
	else
	{
		memcpy(m_Array+(a_nIndex-1),m_Array+a_nIndex, sizeof(T)*(m_nSize-a_nIndex-1));
	}

}
#endif /* THREADSAFEVECTOR_H_ */
