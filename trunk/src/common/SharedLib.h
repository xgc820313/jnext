#ifndef SHAREDLIB_H
#define SHAREDLIB_H

//#include "npapi.h" // We'll borrow the Windows / Linux types from here
#ifdef _WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <string>
using namespace std;

class SharedLib
{
public:
	SharedLib( void );
	~SharedLib();
	bool Load( const string & strName );
	void Unload( void );
	string GetLibExt( void );
	typedef void (*SharedLibFunc)( void );
	bool GetLibFunc( const string & strName, SharedLibFunc & func );
	string GetSystemError( void );

private:
#ifdef _WINDOWS
    HINSTANCE	m_hDLL;
#else
	void*		m_hDLL;
#endif
};

#endif
