/*
*____________________________________________________________________________
*
* Database.cpp
*
* This is a self contained example that illistrates how to write a JNEXT
* extension library.
* 
* The corresponding JavaScript file that wraps the methods in this 
* extension is named Database.js and the file named Database.html
* shows how a file is read from JavaScript using this extension.
*
* To create your own extension, simply include the files plugin.cpp and
* plugin.h in your project, and create your own class specific implementation
* following the same guidlines explained in this file.
*
*____________________________________________________________________________
*
* Revision 1.0  2007/09/26 00:55:00  Amnon David
*       First release
*/


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
 * ***** END LICENSE BLOCK *****
 */


////////////////////////////////////////////////////////////////////////////////
// Include plugin.h for the common constants, callback and utility functions
// used by the extension framework
#include "../common/plugin.h"

// SOCI database abstraction definitions
#include "soci.h"
using namespace SOCI;
#include <algorithm>
#include <string>

// The following is required since the tolower on Linux is defined as an inline
// function and the transform STL algorithm cannot accept is as a function pointer
#include <ctype.h>
int mytolower(int c)
{
    return tolower( c );
}

/////////////////////////////////////////////////////////////////////////
// Constants common to Database extension
// When writing your own extension, replace these with your
// own relevant constants.
/////////////////////////////////////////////////////////////////////////


// List of object methods supported by this extension
const char* szOPEN			= "Open";       // Open the Database
const char* szGETROW        = "GetRow";     // Get next row for last query
const char* szQUERY			= "Query";   	// Send an SQL query
const char* szCLOSEQUERY	= "CloseQuery"; // Close an SQL query
const char* szCLOSE			= "Close";      // Close

// constants
const char* szNEWROW        = "NewRow ";  // SQL query result
const char* szLASTROW       = "LastRow ";
/////////////////////////////////////////////////////////////////////////
// Definition of the Database object
/////////////////////////////////////////////////////////////////////////

// SOCI database backend
extern BackEndFactory const&    g_backEnd;
// Comma separated list of classes supported by this extension
extern const char*              g_szDatabase;

/////////////////////////////////////////////////////////////////////////////////////////////

class DBQuery
{
public:
    DBQuery( void );
    ~DBQuery();
    bool Init( Session& sql, const string& strQueryString, string& strErr );
    string GetRow( void );

private:
    SOCI::Row           m_Row;
    SOCI::Statement*    m_pStatement;
};

typedef std::map<string, DBQuery*>     StringToQuery_T;

/////////////////////////////////////////////////////////////////////////////////////////////

class Database : public JSExt
{
public:
    string InvokeMethod( const string& strCommand ); // Required to implement class methods
    Database( const string& strObjId ); // Required to create object and save object id
    bool CanDelete( void ); // Indicate whether object can be deleted by framework
    virtual ~Database();

private:
    void Close( void );
    void NotifyEvent( const char* szEvent );
    Database( void );
    string GetNextQid( void );

private:
    string			    m_strObjId;  // unique JNEXT id given to this object
    SOCI::Session*	    m_pSQL;      // corresponding Database session
    StringToQuery_T     m_id2Query;  // container of queries opened on this database
    int                 m_nCurrQid;  // index of last query id
};

/////////////////////////////////////////////////////////////////////////
// The following two callbacks that have to be implemented. They are
// invoked from plugin.cpp:
//
// onGetObjList:
// =============
// Returns: A comma separated list of classes supported by this JNEXT extension
//
// onCreateObject:
// ===============
// strClassName (input): Name of the class requested to be created
//                       Valid named are those that are returned in onGetObjList
// strObjId     (input): The unique object id for the class
// Returns: A pointer to the created extension object
/////////////////////////////////////////////////////////////////////////

char* onGetObjList( void )
{
    // Return a comma separated list of classes known to this plugin
    // in this case it is "<Database>,<DBQuery>", where <Database> and
    // <DBQuery> depend on which database plugin project this file
    // is being compiled into
    return (char*)g_szDatabase;
}

JSExt* onCreateObject( const string& strClassName, const string& strObjId )
{
    // Given a class name and identifier, create the relevant object.
    if ( strClassName != g_szDatabase )
    {
        return NULL;
    }

    return new Database( strObjId );
}


/////////////////////////////////////////////////////////////////////////
// DBQuery implementation
/////////////////////////////////////////////////////////////////////////

DBQuery::DBQuery( void )
{
    m_pStatement = NULL;
}

DBQuery::~DBQuery()
{
    delete m_pStatement;
}

bool DBQuery::Init( Session& sql, const string& strQueryString, string& strErr )
{
    strErr = "No Error";
    bool bRetVal = true;
    try
    {
        m_pStatement = new Statement((sql.prepare << strQueryString, into(m_Row)));
        if ( m_pStatement == NULL )
        {
            strErr = "Could not allocate statment";
            return false;
        }

        bRetVal = m_pStatement->execute();
        bRetVal = true;
    }
    catch (SOCIError const &e)
    {
        strErr = e.what();
        return false;
    }

    return bRetVal;
}

string DBQuery::GetRow( void )
{
    string strRetVal;
    Statement& st = *m_pStatement;
    if ( st.fetch() )
    {
        char szBuf[80];
        string strRow = "[";
        for ( size_t i=0; i<m_Row.size(); ++i)
        {
            const ColumnProperties& props = m_Row.getProperties(i);
            //std::cout << '<' << props.getName() << '>';
            switch(props.getDataType())
            {
                case eString:
                {
                    strRow += '"';
                    string s = m_Row.get<std::string>(i);
                    for (size_t j=0; j<s.length(); ++j)
                    {
                        if ( s[ j ] == '"' )
                        {
                            strRow += '\\';
                        }
                        strRow += s[ j ];
                    }
                    strRow += '"';
                    break;
                }

                case eDouble:
                {
                    sprintf( szBuf, "%f", m_Row.get<double>(i) );
                    strRow += szBuf;
                    break;
                }

                case eInteger:
                {
                    sprintf( szBuf, "%d", m_Row.get<int>(i) );
                    strRow += szBuf;
                    break;
                }

                case eUnsignedLong:
                {
                    sprintf( szBuf, "%u", m_Row.get<unsigned long>(i) );
                    strRow += szBuf;
                    break;
                }

                case eDate:
                {
                    std::tm when = m_Row.get<std::tm>(i); 
                    strRow += '"';
                    char szTime[ 30 ];
                    strcpy( szTime, asctime(&when) );
                    char* pNL = strchr(szTime, '\n');
                    if ( pNL != NULL )
                    {
                        // silly asctime thing
                        *pNL = '\0';
                    }

                    strRow += szTime;
                    strRow += '"';
                    break;
                }
            } // switch

            if ( i < m_Row.size()-1 )
            {
                strRow += ",";
            }
        } // for loop of row contents
        strRow += "]";
        return strRow;
    } // if fetch
    else
    {
        return szLASTROW;
    }
}


/////////////////////////////////////////////////////////////////////////
// Database implementation
/////////////////////////////////////////////////////////////////////////

Database::Database( void )
{
}

Database::Database( const string& strObjId )
{
    // Save the object id
    m_strObjId		= strObjId;

    // Perform any other class specific initializations
    m_pSQL          = NULL;

    m_nCurrQid      = 1;
}

string Database::GetNextQid( void )
{
    static char szBuf[ 20 ];
    sprintf( szBuf, "%d", m_nCurrQid );
    return szBuf;
}

bool Database::CanDelete( void )
{
    // This object can be deleted by plugin framework
    return true;
}

/////////////////////////////////////////////////////////////////////////
// This method is called as a result from a request from Javascript
// to invoke a method of this particular object
/////////////////////////////////////////////////////////////////////////

string Database::InvokeMethod( const string& strFullCommand )
{
    string strRetVal;
    vector<string> arParams;
    g_tokenize( strFullCommand, " ", arParams );
    string strCommand = arParams[ 0 ];

    if ( strCommand == szOPEN )
    {
        if ( m_pSQL != NULL )
        {
            strRetVal = szERROR + m_strObjId + " :Database already open";
            return strRetVal;
        }
        // Database Open method has been requested
        string strConnectString	= strFullCommand.substr( arParams[ 0 ].size()+1 );
        strConnectString = g_trim( strConnectString );

        // Create the database session
        m_pSQL = new Session( g_backEnd, strConnectString );

        if ( m_pSQL != NULL )
        {
            strRetVal = szOK + m_strObjId;
        }
        else
        {
            strRetVal = szERROR + m_strObjId + " :Can't allocate database";
        }
    }
    else
    if ( strCommand == szCLOSE )
    {
        // Database Close method has been requested
        Close();
        strRetVal = szOK + m_strObjId;
        return strRetVal;
    }
    else
    if ( strCommand == szQUERY )
    {
        // Database query method has been requested

        if ( m_pSQL == NULL )
        {
            strRetVal = szERROR + m_strObjId + " :No open database";
            return strRetVal;
        }

        Session& sql = *m_pSQL;
        string strQueryString	= strFullCommand.substr( arParams[ 0 ].size()+1 );
        vector<string> arParams;
        g_tokenize( strQueryString, " \t", arParams );
        string s = arParams[ 0 ];
        std::transform(s.begin(), s.end(), s.begin(), mytolower);
        if ( s == "select" )
        {
            // this is a SELECT statement
            DBQuery* pQuery = new DBQuery();
            if ( pQuery == NULL )
            {
                strRetVal = szERROR + m_strObjId + " :Can't create query";
                return strRetVal;
            }
            string strErr;
            if ( !pQuery->Init( sql, strQueryString, strErr ) )
            {
                delete pQuery;
                strRetVal = szERROR + m_strObjId + " :" + strErr;
                return strRetVal;
            }
            string strQid = GetNextQid();
            m_id2Query.insert( StringToQuery_T::value_type( strQid, pQuery ) );
        }
        else
        {
            // some other SQL statement
            try
            {
                sql << strQueryString;
            }
            catch (SOCIError const &e)
            {
                string strErr = e.what();
                strRetVal = szERROR + m_strObjId + " :" + strErr;
                return strRetVal;
            }
        }
        strRetVal = szOK + m_strObjId;
        return strRetVal;
    }
    else
    if ( strCommand == szGETROW )
    {
        string strQueryId	= strFullCommand.substr( arParams[ 0 ].size()+1 );
        StringToQuery_T::iterator r = m_id2Query.find( strQueryId );
        if ( r == m_id2Query.end() )
        {
            strRetVal = szERROR + m_strObjId + " :No Query found for id.";
            return strRetVal;
        }

        DBQuery* pDBQuery = r->second;
        strRetVal = szNEWROW + m_strObjId + " " + pDBQuery->GetRow();
        return strRetVal;
    }
    else
    if ( strCommand == szCLOSEQUERY )
    {
        string strQueryId	= strFullCommand.substr( arParams[ 0 ].size()+1 );
        StringToQuery_T::iterator r = m_id2Query.find( strQueryId );
        if ( r == m_id2Query.end() )
        {
            strRetVal = szERROR + m_strObjId + " :No Query found for id.";
            return strRetVal;
        }

        DBQuery* pDBQuery = r->second;
        delete pDBQuery;
        m_id2Query.erase( strQueryId );
        strRetVal = szOK + m_strObjId;
        return strRetVal;
    }
    else
    {
        strRetVal = szERROR + m_strObjId + " :Unknown method";
    }

    return strRetVal;
}

void Database::Close( void )
{
    StringToQuery_T::iterator posMap;

    // delete any queries that have not been closed
    for ( posMap = m_id2Query.begin(); posMap != m_id2Query.end(); ++posMap )
    {
        DBQuery* pDBQuery = posMap->second;
        delete pDBQuery;
    }
    m_id2Query.erase( m_id2Query.begin(), m_id2Query.end() );

    // Delete the corresponding SQL session
    if ( m_pSQL != NULL )
    {
        delete m_pSQL;
    }

    m_pSQL = NULL;
}

/////////////////////////////////////////////////////////////////////////
// Virtual destructor is called by the framework
// whenever Javascript disposes of this object or when the shared
// library unloads from memory
/////////////////////////////////////////////////////////////////////////

Database::~Database()
{
    // Implement any cleanup code here
    Close();
}

/////////////////////////////////////////////////////////////////////////
// This method is not used in this Database example. It should
// be used whenever an asyncronous event should be sent to the
// Javascript JNEXT framework, usually when an event occurs
// on a different worker thread such as data ariving on a socket,
// a thread completing some long task, a state change in the system etc.
/////////////////////////////////////////////////////////////////////////

void Database::NotifyEvent( const char* szEvent )
{
    string strEvent = szEvent;
    string strFileEvent = m_strObjId + " " + strEvent;
    SendPluginEvent( strFileEvent.c_str(), m_pContext );
}

