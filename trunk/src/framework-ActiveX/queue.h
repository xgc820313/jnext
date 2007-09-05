#ifndef QUEUE_H
#define QUEUE_H

#include <afxtempl.h>

template <class T> class Queue {
    public:
       //----------------
       // Queue
       //----------------
       Queue( void )
	  {
	   m_handles[SemaphoreIndex] = ::CreateSemaphore(NULL,  // no security attributes
	                                               0,     // initial count
	                                               200, // max count
					               NULL); // anonymous

	   m_handles[StopperIndex] = ::CreateEvent(NULL,        // no security attributes
				                 TRUE,        // manual reset
				                 FALSE,       // initially non-signaled
				                 NULL);       // anonymous
	   
	   ::InitializeCriticalSection(&m_lock);
	  } // Queue

       //----------------
       // ~Queue
       //----------------
       ~Queue()
	  {
	   ::CloseHandle(m_handles[SemaphoreIndex]);
	   ::CloseHandle(m_handles[StopperIndex]);
	   ::DeleteCriticalSection(&m_lock);
	  } // ~Queue

       //----------------
       // AddTail
       //----------------
       BOOL AddTail(const T& p)
	  { 
	   BOOL result;
	   ::EnterCriticalSection(&m_lock);
	   m_queue.AddTail(p);
	   result = ::ReleaseSemaphore(m_handles[SemaphoreIndex], 1, NULL);
	   if(!result)
	      { /* failed */
	       // caller can use ::GetLastError to determine what went wrong
	       m_queue.RemoveTail();
	      } /* failed */
	   ::LeaveCriticalSection(&m_lock);
	   return result;
	  } // AddTail

       //----------------
       // RemoveHead
       //----------------
       T RemoveHead()
	  { 
	   T result;
	   
	   switch(::WaitForMultipleObjects(2, m_handles, FALSE, INFINITE))
	      { /* decode */
	       case StopperIndex:   // shut down thread
		  ::ExitThread(0);  // kill thread
		  return result;      // return keeps C compiler happy

	       case SemaphoreIndex: // semaphore
		  ::EnterCriticalSection(&m_lock);
		  result = m_queue.RemoveHead();
		  ::LeaveCriticalSection(&m_lock);
		  return result;

	       case WAIT_TIMEOUT: // not implemented
	       default:
		  ASSERT(FALSE); // impossible condition
		  return result;
	      } /* decode */
	  } // RemoveHead

       //----------------
       // shutdown
       //----------------
       void shutdown()
	  {
	   ::SetEvent(m_handles[StopperIndex]);
	  } // shutdown

	  BOOL IsEmpty( void )
	  {
		::EnterCriticalSection(&m_lock);
		BOOL bEmpty = m_queue.IsEmpty();
		::LeaveCriticalSection(&m_lock);
		return bEmpty;
	  }
    protected:
       enum {StopperIndex, SemaphoreIndex};
       HANDLE m_handles[2];
       CRITICAL_SECTION m_lock;
       CList<T, T> m_queue;
};

#endif
