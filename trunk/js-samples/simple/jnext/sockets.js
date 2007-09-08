///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT sockets plugin 
///////////////////////////////////////////////////////////////////

function AsyncLineSocket()
{
    var self = this;

    self.connect = function( strAddress, nPort )
	{
		var strVal = objJSExt.sendCmd( self.m_strCmd + self.m_strObjId + " Connect " + strAddress + " " + nPort );
		arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
    }

	self.sendLine = function( strLine )
	{
		var strVal = objJSExt.sendCmd( self.m_strCmd + self.m_strObjId + " SendLine " + strLine );
		arParams = strVal.split( " " );
		return ( arParams[ 0 ] == "Ok" )
	}
	
	self.close = function()
	{
		strRes = objJSExt.sendCmd( self.m_strCmd + self.m_strObjId + " Close" );		
		strRes = objJSExt.sendCmd( self.m_strCmd + self.m_strObjId + " Dispose" );
		g_JNEXTeventDispatcher.unregisterEvents( self );
	}
		
	self.onEvent = function( strData )
	{
		var arData = strData.split( " " );
		var strEventDesc = arData[ 0 ];
		switch ( strEventDesc )
		{
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
		var strVal = objJSExt.sendCmd( "Require Sockets" );
		var arParams = strVal.split( " " );
		if ( arParams[ 0 ] != "Ok" )
		{
			return 0;
		}
		strVal = objJSExt.sendCmd( "CreateObject ClientSocket" );
		arParams = strVal.split( " " );
		if ( arParams[ 0 ] != "Ok" )
		{
			return 0;
		}
		self.m_strObjId = arParams[ 1 ];
		g_JNEXTeventDispatcher.registerEvents( self );
	}

	self.onConnected = function()
	{
		alert( "onConnected" );
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
	self.m_strCmd = "InvokeMethod ";
	self.init();
}
