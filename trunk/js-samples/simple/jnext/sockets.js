///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT sockets plugin 
///////////////////////////////////////////////////////////////////

function AsyncLineSocket()
{
    var self = this;
    self.connect = function( strAddress, nPort )
	{
        var strAddr = strAddress + " " + nPort; 
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "Connect", strAddr );
		arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
    }

	self.sendLine = function( strLine )
	{
		var strVal = g_JNEXTDispatcher.invoke( self.m_strObjId, "SendLine", strLine );
		arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
	}
	
	self.close = function()
	{
		strRes = g_JNEXTDispatcher.invoke( self.m_strObjId, "Close" );		
		strRes = g_JNEXTDispatcher.invoke( self.m_strObjId, "Dispose" );
		g_JNEXTDispatcher.unregisterEvents( self );
	}
		
	self.onEvent = function( strData )
	{
		var arData = strData.split( " " );
		var strEventDesc = arData[ 0 ];
		switch ( strEventDesc )
		{
			case "ConnectError":
			{
				self.onConnectError();
				break;
			}
			
			case "Connected":
			{
				self.onConnected();
				break;
			}
			
			case "Close":
			{
				self.onClose();
				break;
			}
			
			case "OnLine":
			{
				var strLine = strData.substring( strEventDesc.length + 1 );
				self.onLine( strLine );
				break;
			}
		}
	}
	
	self.getId = function()
	{
		return self.m_strObjId; 
	}

	self.init = function()
	{
		if ( !g_JNEXTDispatcher.require( "Sockets" ) )
		{
			return false;
		}
        self.m_strObjId = g_JNEXTDispatcher.createObject( "ClientSocket" );
        if ( self.m_strObjId == "" )
        {
            alert( "error initializing ClientSocket" );
            return false;
        }
        g_JNEXTDispatcher.registerEvents( self );
	}

	self.onConnected = function()
	{
		alert( "onConnected" );
	}
	
	self.onConnectError = function()
	{
		alert( "onConnectError" );
	}
	
	self.onLine = function( strLine )
	{
		alert( "onLine:" + strLine );
	}
	
	self.onClose = function()
	{
		alert( "onClose" );
	}
	
	self.m_strObjId = "";
	self.init();
}
