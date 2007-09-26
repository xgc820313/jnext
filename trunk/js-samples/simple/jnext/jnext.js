///////////////////////////////////////////////////////////////////
// Define native bridge object objJSExt (either plugin or ActiveX) 
///////////////////////////////////////////////////////////////////
var objBody = document.getElementsByTagName("body")[0];         
var objDiv = document.createElement('div');
var strHTML;

if (window.ActiveXObject)
{
	strHTML = '<object id="objJSExt" width="0" height="0" classid="CLSID:C802F39D-BF85-427a-A334-77E501DB62E9" codebase="Js2n.ocx"></object>';
	strHTML += '<script language="JavaScript" for="objJSExt" EVENT="Js2nEvent( strEvent )">JNEXT_callback_native2js(strEvent)</script>';
}
else
{
	var strAddSrc = "";
	if ( navigator.userAgent.indexOf("Safari") != -1 )
	{
		strAddSrc = 'src="./safari.foo"';
	}
	strHTML = '<embed "' + strAddSrc + ' type="application/JNEXT-scriptable-plugin" width="1" height="1">';
}

objDiv.innerHTML = strHTML;
objBody.appendChild(objDiv);

///////////////////////////////////////////////////////////////////
// This is the main JavaScript framework for JNEXT
///////////////////////////////////////////////////////////////////

function JNEXT_EventDispatcher()
{
	var self = this;
	var m_bFirstRequire = true;

	self.m_arEvents = new Object();

	self.require = function( strLibrary )
	{
        var strCmd;
        var strVal;
        var arParams;
        
		if  ( m_bFirstRequire )
		{
            strCmd = "userAgent " + navigator.userAgent;
            strVal = objJSExt.sendCmd( strCmd );
            arParams = strVal.split( " " );
            if ( arParams[ 0 ] != "Ok" )
            {
                return false;
            }
            self.m_bFirstRequire = false;
		}
		
        strCmd = "Require " + strLibrary;
		strVal = objJSExt.sendCmd( strCmd );
		arParams = strVal.split( " " );
		if ( arParams[ 0 ] != "Ok" )
		{
			alert( strVal );
			return false;
		}

		return true;
	}

    self.createObject = function( strObjName )
    {
        var strCmd;
        var strVal;
        var arParams;
        strVal = objJSExt.sendCmd( "CreateObject " + strObjName );
        arParams = strVal.split( " " );
        if ( arParams[ 0 ] != "Ok" )
        {
            alert( strVal );
            return "";
        }
        return arParams[ 1 ];
    }

    self.invoke = function( strObjId, strMethod, strParams )
    {
        var strCmd = "InvokeMethod " + strObjId + " " + strMethod;
        if ( typeof(strParams) != "undefined" )
        {
            strCmd += " " + strParams; 
        }
        return objJSExt.sendCmd( strCmd );
    }
    
	self.registerEvents = function( objNotify )
	{
		var strId = objNotify.getId();
		self.m_arEvents[ strId ] = objNotify;
	}
	
	self.unregisterEvents = function( objNotify )
	{
		var strId = objNotify.getId();
		delete self.m_arEvents[ strId ];
	}
	
	self.processEvent = function( strId, strEvent )
	{
		var objNotify = self.m_arEvents[ strId ];
		objNotify.onEvent( strEvent );
	}
}

var g_JNEXTDispatcher = new JNEXT_EventDispatcher();

///////////////////////////////////////////////////////////////////
// JNEXT native to JavaScript callback function
///////////////////////////////////////////////////////////////////
function JNEXT_callback_native2js( strVal )
{
	var arParams	= strVal.split( " " );
	var strObjId	= arParams[ 0 ];
	var strEvent	= strVal.substring( strObjId.length + 1 );
	g_JNEXTDispatcher.processEvent( strObjId, strEvent ); 
}
