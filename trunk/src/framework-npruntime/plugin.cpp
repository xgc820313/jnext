/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

//////////////////////////////////////////////////
//
// CPlugin class implementation
//

#ifdef XP_WIN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable : 4996)
#include <windows.h>
#include <windowsx.h>
#endif

#ifdef XP_MAC
#include <TextEdit.h>
#endif

#ifdef XP_UNIX
#include <string.h>
#endif

#include "plugin.h"
#include "npupp.h"
#include "../common/nativelogic.h"

#define WM_PLUGIN_EVENT	WM_USER + 101
PString			g_strPageURL	= "";
NPP				g_pNPInstance	= NULL;				
NPWindow*		g_pNPWindow		= NULL;
tNativeLogic	g_NativeLogic;

static NPIdentifier sendCmd_id;
static NPObject *sWindowObj;


// Helper class that can be used to map calls to the NPObject hooks
// into virtual methods on instances of classes that derive from this
// class.
class ScriptablePluginObjectBase : public NPObject
{
public:
  ScriptablePluginObjectBase(NPP npp)
    : mNpp(npp)
  {
  }

  virtual ~ScriptablePluginObjectBase()
  {
  }

  // Virtual NPObject hooks called through this base class. Override
  // as you see fit.
  virtual void Invalidate();
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);

public:
  static void _Deallocate(NPObject *npobj);
  static void _Invalidate(NPObject *npobj);
  static bool _HasMethod(NPObject *npobj, NPIdentifier name);
  static bool _Invoke(NPObject *npobj, NPIdentifier name,
                      const NPVariant *args, uint32_t argCount,
                      NPVariant *result);
  static bool _InvokeDefault(NPObject *npobj, const NPVariant *args,
                             uint32_t argCount, NPVariant *result);
  static bool _HasProperty(NPObject * npobj, NPIdentifier name);
  static bool _GetProperty(NPObject *npobj, NPIdentifier name,
                           NPVariant *result);
  static bool _SetProperty(NPObject *npobj, NPIdentifier name,
                           const NPVariant *value);
  static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);

protected:
  NPP mNpp;
};

#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
static NPClass s##_class##_NPClass = {                                        \
  NP_CLASS_STRUCT_VERSION,                                                    \
  ctor,                                                                       \
  ScriptablePluginObjectBase::_Deallocate,                                    \
  ScriptablePluginObjectBase::_Invalidate,                                    \
  ScriptablePluginObjectBase::_HasMethod,                                     \
  ScriptablePluginObjectBase::_Invoke,                                        \
  ScriptablePluginObjectBase::_InvokeDefault,                                 \
  ScriptablePluginObjectBase::_HasProperty,                                   \
  ScriptablePluginObjectBase::_GetProperty,                                   \
  ScriptablePluginObjectBase::_SetProperty,                                   \
  ScriptablePluginObjectBase::_RemoveProperty                                 \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

void
ScriptablePluginObjectBase::Invalidate()
{
}

bool
ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant *args,
                                   uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::InvokeDefault(const NPVariant *args,
                                          uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant *result)
{
  VOID_TO_NPVARIANT(*result);

  return false;
}

bool
ScriptablePluginObjectBase::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  return false;
}

bool
ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
  return false;
}

// static
void
ScriptablePluginObjectBase::_Deallocate(NPObject *npobj)
{
  // Call the virtual destructor.
  delete (ScriptablePluginObjectBase *)npobj;
}

// static
void
ScriptablePluginObjectBase::_Invalidate(NPObject *npobj)
{
  ((ScriptablePluginObjectBase *)npobj)->Invalidate();
}

// static
bool
ScriptablePluginObjectBase::_HasMethod(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->HasMethod(name);
}

// static
bool
ScriptablePluginObjectBase::_Invoke(NPObject *npobj, NPIdentifier name,
                                    const NPVariant *args, uint32_t argCount,
                                    NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->Invoke(name, args, argCount,
                                                       result);
}

// static
bool
ScriptablePluginObjectBase::_InvokeDefault(NPObject *npobj,
                                           const NPVariant *args,
                                           uint32_t argCount,
                                           NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->InvokeDefault(args, argCount,
                                                              result);
}

// static
bool
ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->HasProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_GetProperty(NPObject *npobj, NPIdentifier name,
                                         NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->GetProperty(name, result);
}

// static
bool
ScriptablePluginObjectBase::_SetProperty(NPObject *npobj, NPIdentifier name,
                                         const NPVariant *value)
{
  return ((ScriptablePluginObjectBase *)npobj)->SetProperty(name, value);
}

// static
bool
ScriptablePluginObjectBase::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->RemoveProperty(name);
}



class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
  ScriptablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
  }

  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
  return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
  return name == sendCmd_id;
}

bool
ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args,
                               uint32_t argCount, NPVariant *result)
{
  if (name == sendCmd_id)
  {
	if ( argCount < 1 )
	{
		return false;
	}

	PString strParam = "";
	NPString npStr = NPVARIANT_TO_STRING( args[0] );
	for (unsigned int i=0; i<npStr.utf8length; i++)
	{
		strParam += npStr.utf8characters[ i ];
	}


	PString strResult = g_NativeLogic.InvokeFunction( strParam );

	char* pszName = (char*)NPN_MemAlloc( strResult.GetSize() + 1 );
	strcpy( pszName, strResult );
    STRINGZ_TO_NPVARIANT(pszName, *result);

    return PR_TRUE;
  }

  return PR_FALSE;
}

bool
ScriptablePluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  printf ("ScriptablePluginObject default method called!\n");

  STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

  return PR_TRUE;
}

CPlugin::CPlugin(NPP pNPInstance) :
  m_pNPInstance(pNPInstance),
  m_pNPStream(NULL),
  m_bInitialized(FALSE),
  m_pScriptableObject(NULL)
{
#ifdef XP_WIN
  m_hWnd = NULL;
#endif

  NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &sWindowObj);

  NPIdentifier n = NPN_GetStringIdentifier("foof");

  sendCmd_id = NPN_GetStringIdentifier("sendCmd");

  NPVariant v;
  INT32_TO_NPVARIANT(46, v);

  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

  n = NPN_GetStringIdentifier("document");

  NPVariant arg;
  OBJECT_TO_NPVARIANT(sWindowObj, arg);


  NPObject *myobj =
    NPN_CreateObject(m_pNPInstance,
                     GET_NPOBJECT_CLASS(ScriptablePluginObject));

  n = NPN_GetStringIdentifier("objJSExt");

  OBJECT_TO_NPVARIANT(myobj, v);

  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

  NPN_ReleaseObject(myobj);
  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);

    // Create a "location" identifier.
	NPIdentifier identifier = NPN_GetStringIdentifier( "location" );
	// Declare a local variant value.
	NPVariant variantValue;
	// Get the location property from the window object (which is another object).
	bool b1 = NPN_GetProperty( m_pNPInstance, sWindowObj, identifier, &variantValue );
	// Get a pointer to the "location" object.
	NPObject *locationObj = variantValue.value.objectValue;
	// Create a "href" identifier.
	identifier = NPN_GetStringIdentifier( "href" );
	// Get the location property from the location object.
	bool b2 = NPN_GetProperty( m_pNPInstance, locationObj, identifier, &variantValue );
	g_strPageURL = "";
	NPString npStr = NPVARIANT_TO_STRING( variantValue );
	for (unsigned int i=0; i<npStr.utf8length; i++)
	{
		g_strPageURL += npStr.utf8characters[ i ];
	}
	
	NPN_ReleaseObject(locationObj);
}

CPlugin::~CPlugin()
{
  if (sWindowObj)
    NPN_ReleaseObject(sWindowObj);
  if (m_pScriptableObject)
    NPN_ReleaseObject(m_pScriptableObject);

  sWindowObj = 0;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;
#endif

NPBool CPlugin::init(NPWindow* pNPWindow)
{
  if(pNPWindow == NULL)
    return FALSE;

#ifdef XP_WIN
  m_hWnd = (HWND)pNPWindow->window;
  if(m_hWnd == NULL)
    return FALSE;

  // subclass window so we can intercept window messages and
  // do our drawing to it
  lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);

  // associate window with our CPlugin object so we can access 
  // it in the window procedure
  SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)this);
#endif

  m_Window = pNPWindow;

  m_bInitialized	= TRUE;
  g_pNPInstance		= m_pNPInstance;
  g_pNPWindow		= pNPWindow;
  PString strPluginPath;
#ifdef XP_WIN // NPAPI plugin
	//Get Current Path of exe
	char szFullFileName[ MAX_PATH ];
	GetModuleFileName ( GetModuleHandle(NULL), szFullFileName, MAX_PATH ) ;
	strPluginPath = szFullFileName;
	PINDEX nPos = strPluginPath.FindLast( "\\" );
	strPluginPath = strPluginPath.Mid( 0, nPos + 1 ) + "plugins\\js2n\\";
#else // not Windows - assuming Linux
    BrInitError error;
    if (br_init_lib(&error) == 0 && error != BR_INIT_ERROR_DISABLED)
    {
        printf( "Error - can't init binreloc, code %d\n", error );
    }
	strPluginPath = br_find_exe_dir("/usr/lib/firefox/plugins/");
	strPluginPath += "js2n/";
#endif
  g_NativeLogic.Init( g_strPageURL, strPluginPath );
  return TRUE;
}

bool SendEventToJS( const PString& strEvent )
{
	// UF: This might be called from a different thread than the
	// one that created the plugin UI. We should actually post
	// a platform specific Window message so that the call to
	// the hosting browser of this plugin will be made in the
	// same thread that "holds" the plugin UI (be it an NPAPI
	// plugin or ActiveX plugin)

	// This implementation is specific to NPAPI plugins.
	PString strJSCall = "javascript:callback('" + strEvent + "')"; 
	return (NPN_GetURL( g_pNPInstance, strJSCall,"_self") == NPERR_NO_ERROR );
}

void CPlugin::shut()
{
#ifdef XP_WIN
  // subclass it back
  //SubclassWindow(m_hWnd, lpOldProc);
  m_hWnd = NULL;
#endif

  m_bInitialized = FALSE;
}

NPBool CPlugin::isInitialized()
{
  return m_bInitialized;
}

int16 CPlugin::handleEvent(void* event)
{
#ifdef XP_MAC
  NPEvent* ev = (NPEvent*)event;
  if (m_Window) {
    Rect box = { m_Window->y, m_Window->x,
                 m_Window->y + m_Window->height, m_Window->x + m_Window->width };
    if (ev->what == updateEvt) {
      ::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
    }
  }
#endif
  return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, TRUE);
  UpdateWindow(m_hWnd);
#endif

  if (m_Window) {
    NPRect r =
      {
        (uint16)m_Window->y, (uint16)m_Window->x,
        (uint16)(m_Window->y + m_Window->height),
        (uint16)(m_Window->x + m_Window->width)
      };

    NPN_InvalidateRect(m_pNPInstance, &r);
  }
}

// this will clean the plugin window
void CPlugin::clear()
{
  strcpy(m_String, "");

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, TRUE);
  UpdateWindow(m_hWnd);
#endif
}

void CPlugin::getVersion(char* *aVersion)
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  char*& version = *aVersion;
  version = (char*)NPN_MemAlloc(1 + strlen(ua));
  if (version)
    strcpy(version, ua);
}

NPObject *
CPlugin::GetScriptableObject()
{
  if (!m_pScriptableObject) {
    m_pScriptableObject =
      NPN_CreateObject(m_pNPInstance,
                       GET_NPOBJECT_CLASS(ScriptablePluginObject));
  }

  if (m_pScriptableObject) {
    NPN_RetainObject(m_pScriptableObject);
  }

  return m_pScriptableObject;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
	  /*
    case WM_PAINT:
      {
        // draw a frame and display the string
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        FrameRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
        CPlugin * p = (CPlugin *)GetWindowLong(hWnd, GWL_USERDATA);
        if(p) {
          if (p->m_String[0] == 0) {
            strcpy("foo", p->m_String);
          }
            DrawText(hdc, p->m_String, strlen(p->m_String), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        EndPaint(hWnd, &ps);
      }
      break;
	  */
	  case WM_PLUGIN_EVENT:
	  {
		  break;
	  }
    default:
      break;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif
