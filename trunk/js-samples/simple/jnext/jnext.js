///////////////////////////////////////////////////////////////////
// Define native bridge object objJSExt (either plugin or ActiveX) 
///////////////////////////////////////////////////////////////////
var objBody = document.getElementsByTagName("body")[0];         
var objDiv = document.createElement('div');
var strHTML;

if (window.ActiveXObject)
{
	strHTML = '<object id="objJSExt" width="1" height="1" classid="CLSID:C802F39D-BF85-427a-A334-77E501DB62E9" codebase="Js2n.ocx"></object>';
	strHTML += '<script language="JavaScript" for="objJSExt" EVENT="Js2nEvent( strEvent )">JNEXT_callback_native2js(strEvent)</script>';
}
else
{
	strHTML = '<embed id="embed1" type="application/mozilla-npruntime-scriptable-plugin" width="1" height="1">';
}

objDiv.innerHTML = strHTML;
objBody.appendChild(objDiv);

///////////////////////////////////////////////////////////////////
// This is the main JavaScript framework for JNEXT
///////////////////////////////////////////////////////////////////
function JNEXT_EventDispatcher()
{
	var self = this;
	self.m_arEvents = new Object();
	
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

var g_JNEXTeventDispatcher = new JNEXT_EventDispatcher();

///////////////////////////////////////////////////////////////////
// JNEXT native to JavaScript callback function
///////////////////////////////////////////////////////////////////
function JNEXT_callback_native2js( strVal )
{
	var arParams	= strVal.split( " " );
	var strObjId	= arParams[ 0 ];
	var strEvent	= strVal.substring( strObjId.length + 1 );
	g_JNEXTeventDispatcher.processEvent( strObjId, strEvent ); 
}
