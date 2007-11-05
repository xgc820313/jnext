///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT database plugins 
///////////////////////////////////////////////////////////////////

function DBQuery( objDB, strQueryId )
{
	var self = this;
	self.m_strQueryId = strQueryId;
	self.m_objDB      = objDB;

	self.getColNames = function()
	{
		var strVal = self.m_objDB.getColNames( self.m_strQueryId );
		var arParams = strVal.split( " " );
		if ( arParams[ 0 ] == "Error" )
		{
			return null;
		}

		var nStart = arParams[ 0 ].length + arParams[ 1 ].length + 2; 
		var strExp = "var arRow = " + strVal.substr( nStart );
		eval( strExp );
		return arRow;
	}
	
	self.getColTypes = function()
	{
		var strVal = self.m_objDB.getColTypes( self.m_strQueryId );
		var arParams = strVal.split( " " );
		if ( arParams[ 0 ] == "Error" )
		{
			return null;
		}

		var nStart = arParams[ 0 ].length + arParams[ 1 ].length + 2; 
		var strExp = "var arRow = " + strVal.substr( nStart );
		eval( strExp );
		return arRow;
	}
	
	self.getRow = function()
	{
		var strVal = self.m_objDB.getRow( self.m_strQueryId );
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
		self.m_objDB.closeQuery( self.m_strQueryId );
	}	
}

///////////////////////////////////////////////////////////////////

function Database( strType )
{
	var self = this;
	self.m_strType = strType;
	
    self.open = function( strConnectString )
	{
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "Open " + strConnectString );
		var arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" );
    }

	self.query = function( strLine )
	{
		strLine = strLine.replace(/^\s*/, ''); // trim leading whitespace
		strLine = strLine.replace(/\s*$/, ''); // trim trailing whitespace
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "Query " + strLine );
		var arParams = strVal.split( " " );
		if ( arParams[ 0 ] == "Error" )
		{
			return null;
		}

		if ( arParams[ 0 ] == "Ok" )
		{
			return true;
		}
		
		if ( arParams[ 0 ] != "NewQuery" )
		{
			return false;
		}
		
		// initialize query with the returned query id
		var objQuery = new DBQuery( self, arParams[ 2 ] );
		return objQuery;
	}
		
	self.closeQuery = function( strQueryId )
	{
		return g_JNEXTDispatcher.invoke( self.m_strObjId, "CloseQuery " + strQueryId );
	}
	
	self.getRow = function( strQueryId )
	{
		return g_JNEXTDispatcher.invoke( self.m_strObjId, "GetRow " + strQueryId );
	}
	
	self.getColNames = function( strQueryId )
	{
		return g_JNEXTDispatcher.invoke( self.m_strObjId, "GetColNames " + strQueryId );
	}
	
	self.getColTypes = function( strQueryId )
	{
		return g_JNEXTDispatcher.invoke( self.m_strObjId, "GetColTypes " + strQueryId );
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

