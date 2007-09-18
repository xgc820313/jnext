#include "SharedLib.h"

SharedLib::SharedLib( void )
{
	m_hDLL = NULL;
}

SharedLib::~SharedLib()
{
	Unload();
}

#ifdef _WINDOWS
///////////////////////////////////////////////////////////////////////////

bool SharedLib::Load( const string & strName )
{
	m_hDLL = LoadLibrary( strName.c_str() );
	return m_hDLL != NULL;
}

void SharedLib::Unload( void )
{
	if ( m_hDLL != NULL )
	{
		FreeLibrary( m_hDLL );
		m_hDLL = NULL;
	}
}

string SharedLib::GetLibExt( void )
{
	return ".DLL";
}

bool SharedLib::GetLibFunc( const string & strName, SharedLibFunc & func )
{
	if ( m_hDLL == NULL )
	{
		return false;
	}

	FARPROC funcp = GetProcAddress( m_hDLL, strName.c_str() );
	if ( funcp == NULL )
	{
		return false;
	}

	func = (SharedLibFunc)funcp;
	return true;
}

string SharedLib::GetSystemError( void )
{
    string strError;
    int nErrorCode = GetLastError();
    LPTSTR s;
    if ( ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, nErrorCode, 0, ( LPTSTR ) &s, 0, NULL ) )
    {
        strError = s;
    }
    else
    {
		const int nMAX = 20;
		char szBuf[ nMAX ];
        sprintf_s( szBuf, nMAX, "Error code %d", nErrorCode );
		strError = szBuf;
    }
	
	return strError;
}


#else
///////////////////////////////////////////////////////////////////////////

bool SharedLib::Load( const string & strName )
{
	if ( m_hDLL != NULL )
	{
		return true;
	}

	#if defined(P_OPENBSD)
	m_hDLL = dlopen( strName.c_str(), RTLD_NOW);
	#else
	m_hDLL = dlopen((const char *)strName.c_str(), RTLD_NOW);
	#endif

	return (m_hDLL != NULL);
}

void SharedLib::Unload( void )
{
	if ( m_hDLL != NULL )
	{
		dlclose( m_hDLL );
		m_hDLL = NULL;
	}
}

string SharedLib::GetLibExt( void )
{
	return ".so";
}

bool SharedLib::GetLibFunc( const string & strName, SharedLibFunc & func )
{
	if ( m_hDLL == NULL )
	{
		return false;
	}

	#ifdef P_OPENBSD
	void * funcp = dlsym(m_hDLL, strName.c_str() );
	#else
	void * funcp = dlsym(m_hDLL, (const char *)strName.c_str() );
	#endif

	if ( p == NULL )
	{
		return false;
	}
	
	func = (SharedLibFunc)funcp;
	return TRUE;
}

string SharedLib::GetSystemError( void )
{
	string strError = "Unknown";
    char* szError;
    if (( szError = dlerror() ) != NULL )
    {
        strError = szError;
    }
	
	return strError;
}

#endif
