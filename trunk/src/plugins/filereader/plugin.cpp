#include "plugin.h"

SendPluginEv SendPluginEvent;

string& g_trim( string& str )
{
    // Whitespace characters
    char whspc[] = " \t\r\n\v\f";

    // Whack off first part
    size_t pos = str.find_first_not_of( whspc );
    if( pos != string::npos )
    str.replace( 0, pos, "" );

    // Whack off trailing stuff
    pos = str.find_last_not_of( whspc );
    if( pos != string::npos )
    str.replace( pos + 1, str.length() - pos, "" );

    return str;
}

void g_tokenize(const string& str,const string& delimiters, vector<string>& tokens)
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

char* SetEventFunc(SendPluginEv funcPtr)
{
	static char* szObjList = onGetObjList();
	SendPluginEvent = funcPtr;
	return szObjList;
}

typedef std::map<string, JSExt*>	StringToJExt_T;

const int       nMAXSIZE = 512;
char			g_szRetVal[ nMAXSIZE ];
StringToJExt_T	g_ID2Obj;

char* g_str2static( const string& strRetVal )
{
    strncpy( g_szRetVal, strRetVal.c_str(), nMAXSIZE );
    return g_szRetVal;
}

char* InvokeFunction( const char* szCommand )
{
	string strFullCommand = szCommand;
    vector<string> arParams;
	g_tokenize( strFullCommand, " ", arParams );
	string strCommand	= arParams[ 0 ];
    string strRetVal = szERROR;
	if ( strCommand == szCREATE )
	{
		string strClassName	= arParams[ 1 ];
		string strObjId		= arParams[ 2 ];
		
		StringToJExt_T::iterator r = g_ID2Obj.find( strObjId );
		if ( r == g_ID2Obj.end() )
		{
            strRetVal += strObjId;
            strRetVal += " :Error Can't find object.";
			return g_str2static( strRetVal );
		}

		JSExt* pJSExt = r->second;
		if ( pJSExt != NULL )
		{
            strRetVal += strObjId;
            strRetVal += " :Object already exists.";
			return g_str2static( strRetVal );
		}

        pJSExt = onCreateObject( strClassName, strObjId );
		if ( pJSExt == NULL )
		{
            strRetVal += strObjId;
            strRetVal += " :Unknown object type.";
            strRetVal += strClassName;
			return g_str2static( strRetVal );
		}

		g_ID2Obj.insert( StringToJExt_T::value_type( strObjId, pJSExt ) );

		strRetVal = szOK;
		strRetVal += strObjId;
		return g_str2static( strRetVal );
	}
	else
	if ( strCommand == szINVOKE )
	{
		string strObjId		= arParams[ 1 ];
		string strMethod	= arParams[ 2 ];

		StringToJExt_T::iterator r = g_ID2Obj.find( strObjId );
		if ( r == g_ID2Obj.end() )
		{
            strRetVal += strObjId;
            strRetVal += " :No object found for id.";
			return g_str2static( strRetVal );
		}

        JSExt* pJSExt = r->second;

		size_t nLoc = strFullCommand.find( strObjId );
		if ( nLoc == string::npos )
		{
            strRetVal += strObjId;
            strRetVal += " :Internal InvokeMethod error.";
			return g_str2static( strRetVal );
		}
		
		if ( strMethod == szDISPOSE )
		{
			delete g_ID2Obj[ strObjId ];
			g_ID2Obj.erase( strObjId );
            strRetVal = szOK;
            strRetVal += strObjId;
			return g_str2static( strRetVal );
		}

		size_t nSuffixLoc = nLoc + strObjId.size();
		string strInvoke = strFullCommand.substr( nSuffixLoc );
		strInvoke = g_trim( strInvoke );
		strRetVal = pJSExt->InvokeMethod( strInvoke );
		return g_str2static( strRetVal );
	}

	strRetVal += " :Unknown command ";
	strRetVal += strCommand;
	return g_str2static( strRetVal );
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifdef _WINDOWS
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    return TRUE;
}
#endif

