///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT FileReader plugin 
///////////////////////////////////////////////////////////////////

function FileReader()
{
    var self = this;
    self.open = function( strPath )
	{
		// Open the file specified by strPath
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "Open", strPath );
		arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
    }

	self.readLine = function( strLine )
	{
		// Read a line from the file that was previously opened
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "ReadLine" );
		arParams = strVal.split( " " );
		if ( arParams[ 0 ] == "EOF" )
		{
			return null;
		}
		
		return strVal.substr( 3 );
	}

	self.getPathSeparator = function()
	{
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "GetPathSep" );
		arParams = strVal.split( " " );
		return arParams[ 1 ];
	}
	
	self.close = function()
	{
		strRes = g_JNEXTDispatcher.invoke( self.m_strObjId, "Close" );		
		strRes = g_JNEXTDispatcher.invoke( self.m_strObjId, "Dispose" );
		g_JNEXTDispatcher.unregisterEvents( self );
	}
	
	self.getId = function()
	{
		return self.m_strObjId; 
	}

	self.init = function()
	{
		// The line below loads the shared library named FileReader.dll or FileReader.so
		// from the jnext directory if it has not already been loaded
		if ( !g_JNEXTDispatcher.require( "FileReader" ) )
		{
			return false;
		}
		
		// The line below creates a new FileReader object 
        self.m_strObjId = g_JNEXTDispatcher.createObject( "FileReader" );
        if ( self.m_strObjId == "" )
        {
            alert( "error initializing FileReader" );
            return false;
        }
		
		// This registers this FileReader object to receive any asyncronous events
		// (unused in the FileReader sample - see the sockets sample for more details)
        g_JNEXTDispatcher.registerEvents( self );
	}
	
	self.m_strObjId = "";
	self.init();
}
