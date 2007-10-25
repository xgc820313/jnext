///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT database plugins 
///////////////////////////////////////////////////////////////////

function Database( strType )
{
	var self = this;
	self.m_strType = strType;
	
    self.open = function( strConnectString )
	{
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "Open " + strConnectString );
		var arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
    }

	self.query = function( strLine )
	{
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "Query " + strLine );
		var arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
	}
	
	self.getRow = function()
	{
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "GetRow" );
		var arParams = strVal.split( " " );
		if ( arParams[ 0 ] == "LastRow" )
		{
			return null;
		}
		
		if ( arParams[ 0 ] != "NewRow" )
		{			
			return "Error";
		}
		
		var nStart = arParams[ 0 ].length + arParams[ 1 ].length + 2; 
		var strExp = "var arRow = " + strVal.substr( nStart );
		eval( strExp );
		return arRow;
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
		if ( !g_JNEXTDispatcher.require( self.m_strType ) )
		{
			return false;
		}
		
        self.m_strObjId = g_JNEXTDispatcher.createObject( self.m_strType );
        if ( self.m_strObjId == "" )
        {
            alert( "error initializing " + self.m_strType );
            return false;
        }
		
        g_JNEXTDispatcher.registerEvents( self );
	}
	
	self.m_strObjId = "";
	self.init();
}

///////////////////////////////////////////////////////////////////

function SQLite3()
{
	var self = this;
	self.m_database = new Database( "SQLite3" );
	self.open	= self.m_database.open;
	self.query	= self.m_database.query;
	self.getRow = self.m_database.getRow;
	self.close	= self.m_database.close;
	self.getId	= self.m_database.getId;
	self.init	= self.m_database.init;
}

