#include "thread.h"

namespace std
{
	DWORD WINAPI MyThreadFunction( LPVOID lpParam )
	{
		Caller* pCaller = (Caller*)lpParam;
		pCaller->Call();
		CloseHandle( pCaller->m_Handle );
		delete pCaller;	
		return 0;
	}

	void thread::join()
	{
		WaitForSingleObject( m_Handle, INFINITE );
	}

	void thread::detach()
	{
	}

}

