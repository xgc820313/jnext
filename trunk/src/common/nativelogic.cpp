/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * Copyright (c) 2007 Optimistec Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
/*
* Revision 1.1  2007/08/31 16:14:58  amnond
* Initial revision
*/
////////////////////////////////////////////////////////////
// nativelogic.cpp : js2n framework /  command dispatcher for plugins
//

#ifndef _WINDOWS
#include "binreloc.h"
#endif

#include <stdio.h>
#include "nativelogic.h"

string& trim( string& str )
{
    // Whitespace characters
    char whspc[] = " \t\r\n\v\f";

    // Whack off first part
    int pos = str.find_first_not_of( whspc );
    if( pos != string::npos )
    str.replace( 0, pos, "" );

    // Whack off trailing stuff
    pos = str.find_last_not_of( whspc );
    if( pos != string::npos )
    str.replace( pos + 1, str.length() - pos, "" );

    return str;
}

void tokenize(const string& str,const string& delimiters, vector<string>& tokens)
{
	// skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

////////////////////////////////////////////////////////////////////////////////

void tURLPermissions::Add( const string& strURL, const string& strPermissions )
{
    tUrl2Auth auth( strURL, strPermissions );
    m_arPermissions.push_back( auth );
}

bool tURLPermissions::Find( const string& strURL, const string& strLibrary )
{
    string strSaveAuth	= "";
    int nMaxLenURLc		= 0;
    int nLenURL			= strURL.size();
    int nSize			= m_arPermissions.size();

    for ( int i=0; i<nSize; i++ )
    {
        tUrl2Auth auth	= m_arPermissions[ i ];
        string strURLc	= auth.m_strURL;
        string strAuth	= auth.m_strPermissions;
        
        int nPos = strURL.find( strURLc );
        if ( nPos == string::npos || nPos > 0 )
        {
            continue;
        }

        int nLenURLc = strURLc.length();
        if ( nLenURLc > nMaxLenURLc )
        {
            nMaxLenURLc = nLenURLc;
            strSaveAuth = strAuth;
        }
    }

    strSaveAuth = trim( strSaveAuth ); 
    vector<string> arLibraries;
	tokenize( strSaveAuth, ",", arLibraries );
    int nTotal = arLibraries.size();
    for ( int i=0; i<nTotal; i++ )
    {
        string strAuthLib = arLibraries[ i ];
        if ( strLibrary == strAuthLib || strAuthLib == "*" )
        {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
// The implementation of SendEventToJS depends on the method
// used to communicate with the hosting browser - it might be
// implemented as an event via an NPAPI plugin or as an event
// via ActiveX for Internet Explorer host.
////////////////////////////////////////////////////////////////////////////////
extern bool SendEventToJS( const string& strEvent );

//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////

bool tNativeLogic::Init( const string& strURL, const string& strPluginsPath )
{
    m_strURL		= strURL;
    m_strPath		= strPluginsPath;
    m_strUserAgent      = "";
    m_nObjId		= 0;
    return true;
}

void tNativeLogic::Cleanup( void )
{
    StringToLibMap_T::iterator      posLibs;
    StringToMethodMap_T::iterator   posClass;
    StringToMethodMap_T::iterator   posObjId;

    for (posClass = m_Class2Invoke.begin(); posClass != m_Class2Invoke.end(); ++posClass )
    {
        delete posClass->second;
    }
    
    for (posLibs = m_File2DynaLink.begin(); posLibs != m_File2DynaLink.end(); ++posLibs )
    {
        posLibs->second->Unload();
        delete posLibs->second;
    }
}

tNativeLogic::~tNativeLogic( void )
{
    Cleanup();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void SendPluginEvent( const char* szEvent )
{
    string strEvent = szEvent;

    SendEventToJS( strEvent );
}

string tNativeLogic::GetSysErrMsg( void )
{
    string strError = "Unknown";
    // Problem loading
#ifdef _WINDOWS
    int nErrorCode = GetLastError();
    LPTSTR s;
    if ( ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, nErrorCode, 0, ( LPTSTR ) &s, 0, NULL ) )
    {
        strError = s;
    }
    else
    {
        char szBuf[ 20 ];
        _snprintf_s( szBuf, _countof(szBuf), 19, "%d", nErrorCode );
        strError = szBuf;
    }
#else
    char* szError;
    if (( szError = dlerror() ) != NULL )
    {
        strError = szError;
    }
#endif
    return strError;
}

#ifdef _WINDOWS
const string strSEP = "\\";
#else
const string strSEP = "/";
#endif

bool tNativeLogic::ReadAuthFile( void )
{
    string strFileName = m_strPath + "auth.txt";
    FILE* fpPermissions = fopen( strFileName.c_str(), "r" );
    if ( fpPermissions == NULL )
    {
        return false;
    }

    const int nLINELEN = 150;
    char szLine[ nLINELEN ];
    while ( fgets( szLine, nLINELEN, fpPermissions ) )
    {
        string strLine = szLine;
        vector<string> arParams;
        tokenize( strLine, " \t", arParams );
        if ( arParams.size() != 2 )
        {
            continue;
        }
        string strURL          = arParams[ 0 ];
        string strPermissions  = arParams[ 1 ];
        m_Permissions.Add( strURL, strPermissions );
    }
    fclose( fpPermissions );

    return true;
}

string tNativeLogic::InvokeFunction( const string& strFunction )
{
    string strResult = "Ok";
    vector<string> arParams;
	tokenize( strFunction, " ", arParams );
    string strCommand = arParams[ 0 ];
    if ( m_strPath.empty() )
    {
        return "Error Application path not set";
    }

    if ( m_strUserAgent.empty() && strCommand == "userAgent" )
    {
        int nStart = strCommand.size() + 1;
        m_strUserAgent = strFunction.substr( nStart );

#ifdef XP_WIN
		// Windows NPAPI
        m_strPath += strSEP;
		if ( m_strUserAgent.find( "Opera" ) != string::npos )
        {
            m_strPath += "program" + strSEP + "plugins" + strSEP + "jnext" + strSEP;
        }
        else
        {
            m_strPath += "plugins" + strSEP + "jnext" + strSEP;
        }

#else
		// Linux NPAPI or Windows ActiveX
        m_strPath += strSEP + "jnext" + strSEP;

#endif
		ReadAuthFile();
        return strResult;
    }
    else
    if ( m_strUserAgent.empty() )
    {
        return "Error User agent not set";
    }

    if ( strCommand == "Require" )
    {
        // Requested use of a JS extension plugin
        string strLibrary = arParams[ 1 ];

		StringToLibMap_T::iterator r = m_File2DynaLink.find( strLibrary );
		if (r != m_File2DynaLink.end())
        {
            // This JS extension plugin has already been loaded
            return strResult;
        }

        // Check if requests from this URL are allowed to access this library
        if ( !m_Permissions.Find( m_strURL, strLibrary ) )
        {
            return "Error No permission to load: " + strLibrary + " for " + m_strURL;;
        }
        /*
        if ( m_strURL.substr( 0, 7 ) != "file://" )  // for now assume local files are safe
        {
            // Check if requests from this URL are allowed to access this library
            if ( !m_Permissions.Find( m_strURL, strLibrary ) )
            {
                return "Error No permission to load: " + strLibrary + " for " + m_strURL;;
            }
        }
        */

        SharedLib* pSharedLib = new SharedLib();
        string strExt = pSharedLib->GetLibExt();

        string strDynaLib = m_strPath + strLibrary + strExt;
        if ( !pSharedLib->Load( strDynaLib ) )
        {
            delete pSharedLib;
            return "Error Can't find " + strDynaLib + " " + GetSysErrMsg();
        }

        // Get the function to set the callback and the receive the list
        // of objects this JS extension supports
        SetEventFunc setEventFunc;
        if ( !pSharedLib->GetLibFunc( "SetEventFunc", ( SharedLib::SharedLibFunc& ) setEventFunc ) )
        {
            delete pSharedLib;
            return "Error Can't find SetEventFunc." + GetSysErrMsg();
        }

        // Set the callback function this JS extension plugin can invoke
        // whenever an event is generated
        string strObjList = setEventFunc( SendPluginEvent );

        // Get the pointer of the command invocation function for this
        // JS extension plugin
        InvokeFunc invokeFunc;
        if ( !pSharedLib->GetLibFunc( "InvokeFunction", ( SharedLib::SharedLibFunc& ) invokeFunc ) )
        {
            delete pSharedLib;
            return "Error Can't find InvokeFunction:" + GetSysErrMsg();
        }

        // Store the pSharedLib structure in the hash table, when
        // the hash table will be cleared, all the pointers of
        // SharedLib will be deleted, unloading the shared libraries
        // that they represent.

        m_File2DynaLink.insert(StringToLibMap_T::value_type( strLibrary, pSharedLib ));

        // Create an object to store this pointer. This pointer
        // will be deleted when m_Class2Invoke is cleared or destroyed

        tInvokeMethod* pInvokeMethod = new tInvokeMethod( invokeFunc );

        vector<string> arObjects;
		tokenize( strObjList, ",", arObjects );
        int nTotal = arObjects.size();
        for ( int i=0; i<nTotal; i++ )
        {
            string strClassName = arObjects[ i ];
            m_Class2Invoke.insert( StringToMethodMap_T::value_type( strClassName, pInvokeMethod ) );
        }
    }
    else
	if ( strCommand == "CreateObject" )
	{
		string strClassName = arParams[ 1 ];
		StringToMethodMap_T::iterator r = m_Class2Invoke.find( strClassName );
		if ( r == m_Class2Invoke.end() )
		{
			return "Error Can't find " + strClassName;
		}
		tInvokeMethod* pInvokeMethod = r->second;
		string strId = GetObjectId();
		string strExtCommand = "CreateObj " + strClassName + " " + strId;
        string strVal = pInvokeMethod->m_pInvokeFunc(( const char* ) strExtCommand.c_str() );
		if ( strVal.substr( 0, 2 ) != "Ok" )
		{
			return "Error Can't find method " + strExtCommand;
		}

		m_ObjID2Invoke.insert( StringToMethodMap_T::value_type( strId, pInvokeMethod ) );
		string strRet = "Ok " + strId;
		return strRet;
	}
	else
	if ( strCommand == "InvokeMethod" )
	{
		string strObjId = arParams[ 1 ];
		StringToMethodMap_T::iterator r = m_ObjID2Invoke.find( strObjId );
		if ( r == m_ObjID2Invoke.end() )
		{
			return "Error Can't find object for id " + strObjId;
		}

		tInvokeMethod* pInvokeMethod = r->second;

        string strVal = pInvokeMethod->m_pInvokeFunc(( const char* ) strFunction.c_str() );
		return strVal;
	}
    return strResult;
}

string tNativeLogic::GetObjectId( void )
{
    const int nMAX = 20;
    char szBuf[ nMAX ];
    sprintf( szBuf, "%d", ++m_nObjId );
    string strObjId = szBuf;
    return strObjId;
}
