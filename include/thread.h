/* 
 * File:   thread.h
 * Author: András Ignéczi
 *
 * Created on 2013. augusztus 20., 22:47
 */

#if _MSC_VER >= 1000
#	pragma once
#endif

#ifndef THREAD_H
#define	THREAD_H

#ifndef WIN32
#include <thread>
#else

#include <windows.h>

namespace std
{
	/*
	 ****************************
	 ********* std::ref *********
	 ****************************
	*/
	template<class T>
	T& ref( T& o )
	{
		return o;
	}

	/*
	 ****************************
	 ********* std::Caller ******
	 ****************************
	 */
	class Caller
	{
	public:
		virtual void Call() = 0;
		HANDLE m_Handle;
	};

	/*
	 ***********************************
	 ******** std::ThreadStorage *******
	 ***********************************
	*/
	template<class T, class Q>
	class ThreadStorage : public Caller
	{
	public:
		ThreadStorage( T& pObj, Q pFunct )
		: m_rObj( pObj )
		, m_pFunct( pFunct )
		{
			
			
		}

		void Call()
		{
			(m_rObj.*m_pFunct)();
		}

		T& m_rObj;
		Q m_pFunct;
	};

	/*
	 ***********************************
	 ******** std::ThreadStorageF ******
	 ***********************************
	*/
	template<class T, class Q>
	class ThreadStorageF : public Caller
	{
	public:
		ThreadStorageF( T pFunct, Q pParam )
		{
			m_pFunct = pFunct;
			m_pParam = pParam;
		}

		void Call()
		{
			m_pFunct( m_pParam );
		}

		T      m_pFunct;
		Q      m_pParam;
	};


	DWORD WINAPI MyThreadFunction( LPVOID lpParam );
	void ErrorHandler(LPTSTR lpszFunction);

	/*
	 ***********************************
	 ************ std::thread **********
	 ***********************************
	*/
	class thread
	{
	public:
		template<class T>
		thread( void (T::*MemberFunction)(), T& Object );

		template<class T>
		thread( void (*Function)( T ), T param );

		template<class T, class Q>
		thread( void (T::*MemberFunction)(), Q& Object );

		void join();
		void detach();
	private:
		HANDLE m_Handle;
	};

	/*
	 ************************************************
	 ************ std::thread constructors **********
	 ************************************************
	*/
	template<class T>
	thread::thread( void (T::*MemberFunction)(), T& Object )
	{
		Caller* pStorage = new ThreadStorage<T&,void (T::*)()>( Object, MemberFunction );
		m_Handle = pStorage->m_Handle = CreateThread( NULL, 0, MyThreadFunction, pStorage, CREATE_SUSPENDED, NULL);  
		
		// if( Handle == NULL)
			//ExitProcess(Data_Of_Thread_2);
		ResumeThread( m_Handle );
	}

	template<class T>
	thread::thread( void (*Function)( T ), T Param )
	{
		Caller* pStorage = new ThreadStorageF<void (*)(T), T>( Function, Param );
		m_Handle = pStorage->m_Handle = CreateThread( NULL, 0, MyThreadFunction, pStorage, CREATE_SUSPENDED, NULL);  
		// if( Handle == NULL)
			//ExitProcess(Data_Of_Thread_2);
		ResumeThread( m_Handle );
	}

	template<class T, class Q>
	thread::thread( void (T::*MemberFunction)(), Q& Object )
	{
		Caller* pStorage = new ThreadStorage<Q&,void (T::*)()>( Object, MemberFunction );
		m_Handle = pStorage->m_Handle = CreateThread( NULL, 0, MyThreadFunction, pStorage, CREATE_SUSPENDED, NULL);  
		// if( Handle == NULL)
			//ExitProcess(Data_Of_Thread_2);
		ResumeThread( m_Handle );
	}
}


#endif // WIN32

#endif // THREAD_H