// sockets.cpp : Defines the entry point for the DLL application.
//

#include <ptlib.h>
/////////////////////////////////
// For SocketClient
#include <ptlib/sockets.h>
#include <ptlib/socket.h>
#include <ptlib/tcpsock.h>
/////////////////////////////////

#include "../common/plugin.h"

const char* szCLIENTSOCK	= "ClientSocket";

const char* szSENDLINE		= "SendLine";
const char* szCONNECT		= "Connect";
const char* szCLOSE			= "Close";

class ClientSocketThread : public PThread, public JSExt
{
    enum tReadMode
    {
        eTextLine,
        eRawBytes
    };

    PCLASSINFO(ClientSocketThread, PThread);

public:
    string InvokeMethod( const string& strCommand );
    bool CanDelete( void )
    {
        return false; // Thread deletes itself when complete
    }

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

char* onGetObjList( void )
{
    return (char*)szCLIENTSOCK;
}

JSExt* onCreateObject( const string& strClassName, const string& strObjId )
{
    // Given a class name and identifier, create the relevant object.
    // In this sample, only the FileReader object is valid

    if ( strClassName != szCLIENTSOCK )
    {
        return NULL;
    }

    // Object name is valid - return a new object
    return new ClientSocketThread( strObjId );
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PWLibProcess : public PProcess
{
    PCLASSINFO(PWLibProcess, PProcess)
public:
    PWLibProcess();

    void Main();
};


PWLibProcess::PWLibProcess()
        : PProcess("pwlibproc", "jnext",
                   1, 1 )
{
}

void PWLibProcess::Main()
{
    // Empty function
}

PWLibProcess g_pwlibProcess;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

string ClientSocketThread::InvokeMethod( const string& strSentCommand )
{
    PString strFullCommand = strSentCommand;
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
    SendPluginEvent( (const char *)strSockEvent, m_pContext );
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
