/*
*____________________________________________________________________________
*
* FileReader.cpp
*
* This is a self contained example that illistrates how to write a JNEXT
* extension library.
*
* This library supports one class, named FileReader that can open a file
* given a specified path, read lines from that file and close it.
* 
* The corresponding JavaScript file that wraps the methods in this 
* extension is named FileReader.js and the file named FileReader.html
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

/////////////////////////////////////////////////////////////////////////
// Constants common to FileReader extension
// When writing your own extension, replace these with your
// own relevant constants.
/////////////////////////////////////////////////////////////////////////

// Comma separated list of classes supported by this extension
// In this case only one class is supported, i.e FileReader
const char* szFILEREADER	= "FileReader"; // The only class supported by this plugin

// List of object methods supported by this extension
const char* szOPEN			= "Open";       // Open a file
const char* szREADLINE		= "ReadLine";   // Read a line from a file
const char* szCLOSE			= "Close";      // Close a file
const char* szGETPATHSEP	= "GetPathSep"; // Get OS Path separator

// constants used by this extension
const char* szEOF			= "EOF ";       // return in case of end of file

/////////////////////////////////////////////////////////////////////////
// Definition of the file reader object
/////////////////////////////////////////////////////////////////////////

// Derive your extension class from JSExt (defined in plugin.h)
class FileReader : public JSExt
{
public:
    string InvokeMethod( const string& strCommand ); // Required to implement class methods
    FileReader( const string& strObjId ); // Required to create object and save object id
    bool CanDelete( void ); // Indicate whether object can be deleted by framework
    virtual ~FileReader();

private:
    void Close( void );
    void NotifyEvent( const char* szEvent );
    FileReader( void );

private:
    string			m_strObjId; // unique JNEXT id given to this object
    FILE*			m_fpFile;   // file handle for FileReader object
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
    // in this case it is only "FileReader"

    return (char*)szFILEREADER;
}

JSExt* onCreateObject( const string& strClassName, const string& strObjId )
{
    // Given a class name and identifier, create the relevant object.
    // In this sample, only the FileReader object is valid

    if ( strClassName != szFILEREADER )
    {
        return NULL;
    }

    // Object name is valid - return a new object
    return new FileReader( strObjId );
}

/////////////////////////////////////////////////////////////////////////
// FileReader implementation
/////////////////////////////////////////////////////////////////////////

FileReader::FileReader( void )
{
}

FileReader::FileReader( const string& strObjId )
{
    // Save the object id
    m_strObjId		= strObjId;

    // Perform any other class specific initializations
    m_fpFile		= NULL;
}

bool FileReader::CanDelete( void )
{
    // This object can be deleted by plugin framework
    return true;
}

/////////////////////////////////////////////////////////////////////////
// This method is called as a result from a request from Javascript
// to invoke a method of this particular object
/////////////////////////////////////////////////////////////////////////

string FileReader::InvokeMethod( const string& strFullCommand )
{
    string strRetVal;
    vector<string> arParams;
    g_tokenize( strFullCommand, " ", arParams );
    string strCommand = arParams[ 0 ];

    if ( strCommand == szOPEN )
    {
        // FileReader Open method has been requested
        if ( m_fpFile != NULL )
        {
            strRetVal = szERROR + m_strObjId + ":Already opened file";
            return strRetVal;
        }
        string strPath	= strFullCommand.substr( arParams[ 0 ].size()+1 );
        strPath = g_trim( strPath );

        // open the file in the first param
        m_fpFile = fopen( strPath.c_str(), "r" );

        if ( m_fpFile != NULL )
        {
            strRetVal = szOK + m_strObjId;
        }
        else
        {
            strRetVal = szERROR + m_strObjId;
        }
    }
    else
    if ( strCommand == szGETPATHSEP )
    {
        // Return the path separator for this operating system
        strRetVal = szOK;
#ifdef _WINDOWS
        strRetVal += "\\";
#else
        strRetVal += "/";
#endif
    }
    else
    if ( m_fpFile == NULL )
    {
        strRetVal = szERROR + m_strObjId + ":No file open";
        return strRetVal;
    }
    else
    if ( strCommand == szCLOSE )
    {
        // FileReader Close method has been requested
        Close();
        strRetVal = szOK + m_strObjId;
        return strRetVal;
    }
    else
    if ( strCommand == szREADLINE )
    {
        // FileReader ReadLine method has been requested
        const int nMAXBYTES = 255;
        char szLine[ nMAXBYTES ];
        char* pLine = fgets( szLine, nMAXBYTES, m_fpFile );

        if ( pLine != NULL )
        {
            string strLine = szLine;
            strRetVal = szOK + strLine;
        }
        else
        {
            strRetVal = szEOF;
        }
    }
    else
    {
        strRetVal = szERROR + m_strObjId + " :Unknown method";
    }

    return strRetVal;
}

void FileReader::Close( void )
{
    if ( m_fpFile != NULL )
    {
        fclose( m_fpFile );
    }

    m_fpFile = NULL;
}

/////////////////////////////////////////////////////////////////////////
// Virtual destructor is called by the framework
// whenever Javascript disposes of this object or when the shared
// library unloads from memory
/////////////////////////////////////////////////////////////////////////

FileReader::~FileReader()
{
    // Implement any cleanup code here
    Close();
}

/////////////////////////////////////////////////////////////////////////
// This method is not used in this FileReader example. It should
// be used whenever an asyncronous event should be sent to the
// Javascript JNEXT framework, usually when an event occurs
// on a different worker thread such as data ariving on a socket,
// a thread completing some long task, a state change in the system etc.
/////////////////////////////////////////////////////////////////////////

void FileReader::NotifyEvent( const char* szEvent )
{
    string strEvent = szEvent;
    string strFileEvent = m_strObjId + " " + strEvent;
    SendPluginEvent( strFileEvent.c_str(), m_pContext );
}

