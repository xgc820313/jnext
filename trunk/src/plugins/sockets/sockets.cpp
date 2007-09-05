// sockets.cpp : Defines the entry point for the DLL application.
//
#include <ptlib.h>
/////////////////////////////////
// For SocketClient
#include <ptlib/sockets.h>
#include <ptlib/socket.h>
#include <ptlib/tcpsock.h>
/////////////////////////////////



// var strRes = Call("CreateObject ClientSocket");
// extract strObjId from strRes
// strRes = Call("InvokeMethod " + strObjId + " Connect 127.0.0.1 1320" );
// strRes = Call("InvokeMethod " + strObjId + " SendLine Hello World!!!" );
// strRes = Call("InvokeMethod " + strObjId + " Close" );
// strRes = Call("InvokeMethod " + strObjId + " Dispose" );

const char* szERROR			= "Error ";
const char* szOK			= "Ok ";
const char* szDISPOSE		= "Dispose";
const char* szINVOKE		= "InvokeMethod";
const char* szCREATE		= "CreateObj";

const char* szCLIENTSOCK	= "ClientSocket";
const char* szSENDLINE		= "SendLine";
const char* szCONNECT		= "Connect";
const char* szCLOSE			= "Close";

class JSExt : public PObject
{
public:
	virtual PString InvokeMethod( const PString& strCommand ) = 0;
};

class ClientSocketThread : public PThread, public JSExt
{
	enum tReadMode
	{
		eTextLine,
		eRawBytes
	};

	PCLASSINFO(ClientSocketThread, PThread);

public:
	PString InvokeMethod( const PString& strCommand );
    ClientSocketThread( PString strObjId );
	bool Connect( const PString& strAddr, WORD wPort );
	bool WriteLine( PString strLine );
	void Close( void );

protected:
	ClientSocketThread( void );

private:
	void NotifySocketEvent( const char* szEvent );
	void ProcessLine( const PString& strLine );
	void ProcessChar( int nChar );
    void Main();
	virtual ~ClientSocketThread();

private:
	PSyncPoint		m_syncPoint;
	PTCPSocket* 	m_pSocket;
	bool			m_bConnected;
	PString			m_strLine;
	bool			m_bLastWasCR;
	tReadMode		m_eReadMode;
	PString			m_strObjId;
	PString			m_strRemoteAddr;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%% Functions exported by this DLL
//%% Should always be only SetEventFunc and InvokeFunction
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// g++ requires extern "C" otherwise the names of SetEventFunc and InvokeFunction
// are mangled C++ style. MS Visual Studio doesn't seem to care though.
extern "C"
{
typedef void (*SendPluginEv)( const char* szEvent );
SendPluginEv SendPluginEvent;
char* SetEventFunc(SendPluginEv funcPtr);
char* InvokeFunction( const char* szCommand );
}

char* SetEventFunc(SendPluginEv funcPtr)
{
	static char* szObjList = (char*)szCLIENTSOCK;
	SendPluginEvent = funcPtr;
	return szObjList;
}

char						g_szRetVal[ 512 ];
PMutex						g_invokeMutex;
PDictionary<PString,JSExt>	g_ID2Obj;

char* InvokeFunction( const char* szCommand )
{
	PWaitAndSignal m( g_invokeMutex );
	PString strFullCommand = szCommand;
	PStringArray arParams = strFullCommand.Tokenise( " " );
	PString strCommand	= arParams[ 0 ];
	if ( strCommand == szCREATE )
	{
		PString strClassName	= arParams[ 1 ];
		PString strObjId		= arParams[ 2 ];
		JSExt* pJSExt = (JSExt*)g_ID2Obj.GetAt( strObjId );
		if ( pJSExt != NULL )
		{
			strcpy( g_szRetVal, szERROR );
			strcat( g_szRetVal, strObjId );
			strcat( g_szRetVal, " :Object already exists." );
			return g_szRetVal;
		}
		if ( strClassName == szCLIENTSOCK )
		{
			pJSExt = new ClientSocketThread( strObjId );
		}
		else
		{
			strcpy( g_szRetVal, szERROR );
			strcat( g_szRetVal, strClassName );
			strcat( g_szRetVal, " :Unknown object type." );
			return g_szRetVal;
		}

		g_ID2Obj.SetAt( strObjId, pJSExt );
		strcpy( g_szRetVal, szOK );
		strcat( g_szRetVal, strObjId );
		return g_szRetVal;
	}
	else
	if ( strCommand == szINVOKE )
	{
		PString strObjId		= arParams[ 1 ];
		PString strMethod		= arParams[ 2 ];

		JSExt* pJSExt = (JSExt*)g_ID2Obj.GetAt( strObjId );
		if ( pJSExt == NULL )
		{
			strcpy( g_szRetVal, szERROR );
			strcat( g_szRetVal, strObjId );
			strcat( g_szRetVal, " :No object found for id." );
			return g_szRetVal;
		}

		PINDEX nLoc = strFullCommand.Find( strObjId );
		if ( nLoc == P_MAX_INDEX )
		{
			strcpy( g_szRetVal, szERROR );
			strcat( g_szRetVal, strObjId );
			strcat( g_szRetVal, " :Internal InvokeMethod error" );
			return g_szRetVal;
		}

		if ( strMethod == szDISPOSE )
		{
			g_ID2Obj.RemoveAt( strObjId );
			strcpy( g_szRetVal, szOK );
			strcat( g_szRetVal, strObjId );
			return g_szRetVal;
		}

		int nSuffixLoc = nLoc + strObjId.GetLength();
		PString strInvoke = strFullCommand.Mid( nSuffixLoc ).LeftTrim();
		PString strRetVal = pJSExt->InvokeMethod( strInvoke );
		strcpy( g_szRetVal, strRetVal );
		return g_szRetVal;
	}

	strcpy( g_szRetVal, szERROR );
	strcat( g_szRetVal, strCommand );
	strcat( g_szRetVal, " :Unknown command" );
	return g_szRetVal;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class PWLibProcess : public PProcess
{
    PCLASSINFO(PWLibProcess, PProcess)
  public:
    PWLibProcess();

    void Main();
};



PWLibProcess::PWLibProcess()
  : PProcess("pwlibproc", "jsex",
             1, 1 )
{
}

void PWLibProcess::Main()
{
  // Empty function
}

PWLibProcess g_pwlibProcess;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifdef _WINDOWS
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    return TRUE;
}
#endif

/////////////////////////////////////////////////////


ClientSocketThread::ClientSocketThread( void )
 : PThread(1000,NoAutoDeleteThread)
{
}

ClientSocketThread::ClientSocketThread( PString strObjId )
 : PThread(1000,NoAutoDeleteThread)
{
	m_strObjId		= strObjId;
	m_pSocket		= new PTCPSocket();
	m_bConnected	= false;
	m_bLastWasCR	= false;
	m_eReadMode		= eTextLine;
	Resume(); // start running this thread as soon as the thread is created.
}

PString ClientSocketThread::InvokeMethod( const PString& strFullCommand )
{
	PString strRetVal;
	PStringArray arParams	= strFullCommand.Tokenise( " " );
	PString strCommand		= arParams[ 0 ];

	if ( strCommand == szCONNECT )
	{
		if ( m_bConnected )
		{
			strRetVal = szERROR + m_strObjId + ":Already connected";
			return strRetVal;
		}
		PString strAddr		= arParams[ 1 ];
		PString strPort		= arParams[ 2 ];
		WORD wPort = (WORD) strPort.AsInteger();
		if ( wPort < 1 )
		{
			strRetVal = szERROR + m_strObjId + " :Invalid port";
		}

		if ( Connect( strAddr, wPort ) )
		{
			strRetVal = szOK + m_strObjId;
		}
		else
		{
			strRetVal = szERROR + m_strObjId;
		}
	}
	else
	if ( strCommand == szCLOSE )
	{
		if ( !m_bConnected )
		{
			strRetVal = szERROR + m_strObjId + ":Not connected";
			return strRetVal;
		}
		Close();
		strRetVal = szOK + m_strObjId;
		return strRetVal;

	}
	else
	if ( strCommand == szSENDLINE )
	{
		if ( !m_bConnected )
		{
			strRetVal = szERROR + m_strObjId + ":Not connected";
			return strRetVal;
		}
		int nCommandLen = strCommand.GetLength();
		if ( strFullCommand.GetLength() - nCommandLen < 2 )
		{
			strRetVal = szERROR + m_strObjId + ":Invalid parameter";
			return strRetVal;
		}

		PString strSuffix = strFullCommand.Mid( nCommandLen + 1 );

		if ( WriteLine( strSuffix ) )
		{
			strRetVal = szOK + m_strObjId;
		}
		else
		{
			strRetVal = szERROR + m_strObjId;
		}
	}
	else
	{
		strRetVal = szERROR + m_strObjId + " :Unknown method";
	}

	return strRetVal;
}

void ClientSocketThread::Close( void )
{
	if ( !m_bConnected )
	{
		return;
	}
	m_pSocket->Close();
	m_bConnected = false;
}

bool ClientSocketThread::Connect( const PString& strAddr, WORD wPort )
{
	if ( m_bConnected )
	{
		return false;
	}
	m_pSocket->SetPort( wPort );
	m_strRemoteAddr = strAddr;
	m_syncPoint.Signal();
	return true;
}

void ClientSocketThread::ProcessChar( int nChar )
{
	if ( m_eReadMode == eTextLine )
	{
		if ( nChar == 13 )
		{
			m_bLastWasCR = true;
			ProcessLine( m_strLine );
			m_strLine = "";
			return;
		}

		if ( nChar == 10 )
		{
			if ( m_bLastWasCR )
			{
				return;
			}

			ProcessLine( m_strLine );
			m_strLine = "";
			return;
		}

		m_strLine += (char)nChar;
	}
}

bool ClientSocketThread::WriteLine( PString strLine )
{
	strLine += "\r\n";
	return m_pSocket->WriteString( strLine );
}

void ClientSocketThread::ProcessLine( const PString& strLine )
{
	PString strEvent = "OnLine " + strLine;
	NotifySocketEvent( strEvent );
}

void ClientSocketThread::NotifySocketEvent( const char* szEvent )
{
	PString strEvent = szEvent;
	PString strSockEvent = m_strObjId + " " + strEvent;
	SendPluginEvent( (const char *)strSockEvent );
}

void ClientSocketThread::Main()
{
	m_syncPoint.Wait();
	m_bConnected = m_pSocket->Connect( m_strRemoteAddr );

	if ( !m_bConnected )
	{
		NotifySocketEvent( "ConnectError" );
		return;
	}

	NotifySocketEvent( "Connected" );

	for (;;)
	{
		int nChar = m_pSocket->ReadChar();
	
		if ( nChar == -1 )
		{
			break;
		}

		ProcessChar( nChar );
	}

	NotifySocketEvent( "Close" );
}

ClientSocketThread::~ClientSocketThread()
{
	Close();
	delete m_pSocket;
	m_pSocket = NULL;
}
