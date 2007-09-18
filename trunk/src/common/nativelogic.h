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
* Revision 1.2  2007/08/31 16:14:58  Amnon David
*       Removed dependency on PWLib - based everything on STL. Reduced footprint by 500%       
*
* Revision 1.1  2007/08/31 16:14:58  Amnon David
* Initial revision
*/

#ifndef NATIVELOGIC_H
#define NATIVELOGIC_H

#include "SharedLib.h"

#include <map>
#include <string>
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////

class tURLPermissions
{
	struct tUrl2Auth
    {
        tUrl2Auth( const string& strURL, const string& strAuth )
        {
            m_strURL   = strURL;
            m_strPermissions = strAuth;
        }
        string m_strURL;
        string m_strPermissions;
    };

public:
    void Add( const string& strURL, const string& strPermissions );
    bool Find( const string& strURL, const string& strLibrary );

private:
    vector<tUrl2Auth> m_arPermissions;
};

////////////////////////////////////////////////////////////////////////////////////////
typedef void ( *SendPluginEv )( const char* szEvent );
typedef char*( *SetEventFunc )( SendPluginEv funcPtr );
typedef char*( *InvokeFunc )( const char* szCommand );
////////////////////////////////////////////////////////////////////////////////////////

class tInvokeMethod
{
private:
    tInvokeMethod( void )
    {
    }

public:
    tInvokeMethod( InvokeFunc pInvokefunc )
    {
        m_pInvokeFunc = pInvokefunc;
    }

    InvokeFunc m_pInvokeFunc;
};
////////////////////////////////////////////////////////////////////////////////////////

typedef std::map<string, SharedLib *>		StringToLibMap_T;
typedef std::map<string, tInvokeMethod *>	StringToMethodMap_T;

class tNativeLogic
{
public:
    bool Init( const string& strURL, const string& strPluginsPath );
    string InvokeFunction( const string& strFunction );
    ~tNativeLogic();

private:
    string GetObjectId( void );
    string GetSysErrMsg( void );
	bool   ReadAuthFile( void );
    void   Cleanup( void );

private:
    string                  m_strPath;
    string                  m_strURL;
    string                  m_strUserAgent;
    int                     m_nObjId;
    tURLPermissions	        m_Permissions;
    StringToLibMap_T		m_File2DynaLink;
    StringToMethodMap_T     m_Class2Invoke;
    StringToMethodMap_T		m_ObjID2Invoke;
};

#endif
