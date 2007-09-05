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
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
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

#include "nativelogic.h"
#include <ptlib.h>
#include <ptlib/pprocess.h>

class PInvokeMethod;

PDictionary<PString,PDynaLink>		g_File2DynaLink;
PDictionary<PString,PInvokeMethod>	g_Class2Invoke;
PDictionary<PString,PInvokeMethod>	g_ObjID2Invoke;

////////////////////////////////////////////////////////////////////////////////
// The implementation of SendEventToJS depends on the method
// used to communicate with the hosting browser - it might be 
// implemented as an event via an NPAPI plugin or as an event
// via ActiveX for Internet Explorer host.
////////////////////////////////////////////////////////////////////////////////
extern bool SendEventToJS( const PString& strEvent );

////////////////////////////////////////////////////////////////////////////////
// The following is required in order to use PWLib from within a shared library
////////////////////////////////////////////////////////////////////////////////
//______________________________________________________________________________
class PWLibProcess : public PProcess
{
    PCLASSINFO(PWLibProcess, PProcess)
  public:
    PWLibProcess();

    void Main();
};

PWLibProcess::PWLibProcess()
  : PProcess("pwlibproc", "js2n",
             1, 1 )
{
	// The objects will already be deleted by g_Class2Invoke destructor
	g_ObjID2Invoke.DisallowDeleteObjects();
}

void PWLibProcess::Main()
{
  // Empty function
}

PWLibProcess g_pwlibProcess;
//______________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////

bool tNativeLogic::Init( const PString& strURL, const PString& strPluginsPath )
{
	m_strURL = strURL;
	m_nObjId.SetValue( 0 );
	m_strPath = strPluginsPath;
	PTextFile filePermissions;
	PString strFileName = m_strPath + "auth.txt";
	if ( filePermissions.Open( strFileName ) )
	{
		PString strLine;
		while ( filePermissions.ReadLine( strLine ) )
		{
			PStringArray arParams = strLine.Tokenise( " \t", FALSE );
			if ( arParams.GetSize() != 2 )
			{
				continue;
			}
			PString strURL			= arParams[ 0 ];
			PString strPermissions	= arParams[ 1 ];
			m_Permissions.Add( strURL, strPermissions );
		}
		filePermissions.Close();
	}
    return TRUE;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void SendPluginEvent( const char* szEvent )
{
	PString strEvent = szEvent;

	SendEventToJS( strEvent );
}
//////////////////////////////////////////////////////////////////////
typedef void (*SendPluginEv)( const char* szEvent );
typedef char* (*SetEventFunc)(SendPluginEv funcPtr);
typedef char* (*InvokeFunc)(const char* szCommand);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PInvokeMethod : public PObject
{
private:
	PInvokeMethod( void )
	{
	}

public:
	PInvokeMethod( InvokeFunc pInvokefunc )
	{
		m_pInvokeFunc	= pInvokefunc;
	}

	InvokeFunc	m_pInvokeFunc;
};

PString tNativeLogic::GetSysErrMsg( void )
{
	PString strError = "Unknown";
	// Problem loading 
#ifdef _WINDOWS
	int nErrorCode = GetLastError();
	LPTSTR s;
	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL, nErrorCode, 0, (LPTSTR)&s, 0, NULL))
	{
			strError = s;
	}
	else
	{
			strError = psprintf( "Error code %d", nErrorCode ); 
	}
#else
	char* szError;
	if ((szError = dlerror()) != NULL)
	{
		strError = szError;
	}
#endif
	return strError;
}

PString tNativeLogic::InvokeFunction( const PString& strFunction )
{
	PString strResult = "Ok";
	PStringArray arParams = strFunction.Tokenise( " " );
	PString strCommand = arParams[ 0 ];
	if ( strCommand == "Require" )
	{
		// Requested use of a JS extension plugin
		PString strLibrary = arParams[ 1 ];

		if ( g_File2DynaLink.GetAt( strLibrary ) != NULL )
		{
			// This JS extension plugin has already been loaded
			return strResult;
		}

		if ( m_strURL.Left( 7 ) != "file://" ) // for now assume local files are safe
		{
			// Check if requests from this URL are allowed to access this library
			if ( m_Permissions.Find( m_strURL, strLibrary ) )
			{
				return "Error No permission to load: " + strLibrary + " for " + m_strURL;;
			}
		}

		PDynaLink* pDynaLink = new PDynaLink();
		PString strExt = pDynaLink->GetExtension();

		PString strDynaLib = m_strPath + strLibrary + strExt;
		if ( !pDynaLink->Open( strDynaLib ) )
		{
			delete pDynaLink;
			return "Error Can't find " + strDynaLib + " " + GetSysErrMsg();
		}

		// Get the function to set the callback and the receive the list
		// of objects this JS extension supports
		SetEventFunc setEventFunc;
		if ( !pDynaLink->GetFunction( "SetEventFunc", (PDynaLink::Function&)setEventFunc ) )
		{
			delete pDynaLink;
			return "Error Can't find SetEventFunc." + GetSysErrMsg();
		}

		// Set the callback function this JS extension plugin can invoke
		// whenever an event is generated
		PString strObjList = setEventFunc( SendPluginEvent );

		// Get the pointer of the command invocation function for this
		// JS extension plugin
		InvokeFunc invokeFunc;
		if ( !pDynaLink->GetFunction( "InvokeFunction", (PDynaLink::Function&)invokeFunc ) )
		{
			delete pDynaLink;
			return "Error Can't find InvokeFunction:" + GetSysErrMsg();
		}

		// Store the pDynaLink structure in the hash table, when
		// the hash table will be cleared, all the pointers of
		// PDynaLink will be deleted, unloading the shared libraries
		// that they represent.
		g_File2DynaLink.SetAt( strLibrary, pDynaLink );

		// Create an object to store this pointer. This pointer
		// will be deleted when g_Class2Invoke is cleared or destroyed

		PInvokeMethod* pInvokeMethod = new PInvokeMethod( invokeFunc );

		PStringArray arObjects = strObjList.Tokenise( "," );
		int nTotal = arObjects.GetSize();
		for (int i=0; i<nTotal; i++)
		{
			PString strClassName = arObjects[ i ];
			g_Class2Invoke.SetAt( strClassName, pInvokeMethod );
		}
	}
	else
	if ( strCommand == "CreateObject" )
	{
		PString strClassName = arParams[ 1 ];
		PInvokeMethod* pInvokeMethod = g_Class2Invoke.GetAt( strClassName );
		if ( pInvokeMethod == NULL )
		{
			return "Error Can't find " + strClassName;
		}
		PString strId = GetObjectId();
		PString strExtCommand = "CreateObj " + strClassName + " " + strId;
		PString strVal = pInvokeMethod->m_pInvokeFunc( (const char*)strExtCommand );
		if ( strVal.Left( 2 ) != "Ok" )
		{
			return "Error Can't find method " + strExtCommand;
		}

		g_ObjID2Invoke.SetAt( strId, pInvokeMethod ); 
		PString strRet = "Ok " + strId;
		return strRet;
	}
	else
	if ( strCommand == "InvokeMethod" )
	{
		PString strObjId = arParams[ 1 ];
		PInvokeMethod* pInvokeMethod = g_ObjID2Invoke.GetAt( strObjId ); 
		if ( pInvokeMethod == NULL )
		{
			return "Error Can't find object for id " + strObjId;
		}
		PString strVal = pInvokeMethod->m_pInvokeFunc( (const char*)strFunction );
		return strVal;
	}
	return strResult;
}

PString tNativeLogic::GetObjectId( void )
{
	PString strObjId;
	strObjId.sprintf( "%i", ++m_nObjId );
	return strObjId;
}
