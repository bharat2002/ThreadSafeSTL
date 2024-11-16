/*
 * ThreadHeader.h
 *
 *  Created on: 16-Nov-2024
 *      Author: bpali
 */

#ifndef THREADHEADER_H_
#define THREADHEADER_H_

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

    #define OS_Windows 0
    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>


#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

    #define OS_Windows 1
    #include <windows.h>
    #include <stdio.h>
    #include <tchar.h>
    #define DIV 1048576
    #define WIDTH 7

#endif


class cCriticalSection {
public:
    cCriticalSection() {
#if defined(_WIN32) || defined(WIN32)
        InitializeCriticalSection(&m_CriticalSection);
#elif defined(__unix__)
        pthread_mutex_init(&m_Mutex, nullptr);
#endif
    }

    ~cCriticalSection() {
#if defined(_WIN32) || defined(WIN32)
        DeleteCriticalSection(&m_CriticalSection);
#elif defined(__unix__)
        pthread_mutex_destroy(&m_Mutex);
#endif
    }

    // Enter the critical section
    void Enter() {
#if defined(_WIN32) || defined(WIN32)
        EnterCriticalSection(&m_CriticalSection);
#elif defined(__unix__)
        pthread_mutex_lock(&m_Mutex);
#endif
    }

    // Leave the critical section
    void Leave() {
#if defined(_WIN32) || defined(WIN32)
        LeaveCriticalSection(&m_CriticalSection);
#elif defined(__unix__)
        pthread_mutex_unlock(&m_Mutex);
#endif
    }

private:
#if defined(_WIN32) || defined(WIN32)
    CRITICAL_SECTION m_CriticalSection;
#elif defined(__unix__)
    pthread_mutex_t m_Mutex;
#endif
};

class LockGuard
{
public:
    explicit LockGuard(cCriticalSection& cs) : m_cs(cs) {
        m_cs.Enter(); // Lock the critical section
    }
    ~LockGuard() {
        m_cs.Leave(); // Unlock the critical section when the LockGuard goes out of scope
    }

private:
    cCriticalSection& m_cs;
};


#endif /* THREADHEADER_H_ */
