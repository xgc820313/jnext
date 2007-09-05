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

#ifndef NATIVELOGIC_H
#define NATIVELOGIC_H

#include <ptlib.h>

class PURLPermissions
{
	struct tUrl2Auth : public PObject
	{
		tUrl2Auth( const PString& strURL, const PString& strAuth )
		{
			m_strURL			= strURL;
			m_strPermissions	= strAuth;
		}
		PString m_strURL;
		PString m_strPermissions;
	};

public:

	void Add( const PString& strURL, const PString& strPermissions )
	{
		tUrl2Auth* pAuth = new tUrl2Auth( strURL, strPermissions );
		m_arPermissions.Append( pAuth );
	}

	bool Find( const PString& strURL, const PString& strLibrary )
	{
		PString strSaveAuth	= "";
		int nMaxLenURLc		= 0;
		int nLenURL			= strURL.GetSize();
		int nSize			= m_arPermissions.GetSize();

		for (int i=0; i<nSize; i++)
		{
			tUrl2Auth auth  = m_arPermissions[ i ];
			PString strURLc	= auth.m_strURL;
			PString strAuth = auth.m_strPermissions;
			int nLenURLc = strURLc.GetSize();
			if ( nLenURLc > nLenURL )
			{
				continue;
			}
			if ( nLenURLc > nMaxLenURLc )
			{
				nMaxLenURLc = nLenURLc;
				strSaveAuth = strAuth;
			}
		}

		PString strLocate = ":" + strLibrary + ":";
		if ( strSaveAuth.Find( strLocate ) == P_MAX_INDEX &&
			 strSaveAuth.Find( ":*:" ) == P_MAX_INDEX )
		{
			return false;
		}

		return true;
	}

private:
	PArray<tUrl2Auth> m_arPermissions;
};

class tNativeLogic
{
public:
	bool Init( const PString& strURL, const PString& strPluginsPath );
	PString InvokeFunction( const PString& strFunction );

private:
	PString GetObjectId( void );
	PString GetSysErrMsg( void );

private:
	PString				m_strPath;
	PString				m_strURL;
	PAtomicInteger		m_nObjId;
	PURLPermissions		m_Permissions;
};

#endif
