; Inno Setup script for installing JNEXT on Windows
; Copyright © Optimistec Ltd.
; Supports installing JNEXT plugins on Firefox, Opera, Safari and Netscape Navigator
;
; License for JNEXT (including this script) is either MPL , GPL or LGPL
; See attached MPL 1.1 license for details.

[Setup]
AppName=JNEXT 1.0.7
AppVerName=JNEXT 1.0.7
AppPublisher=Optimistec Ltd.
AppPublisherURL=http://www.jnext.org
AppSupportURL=http://www.jnext.org
AppUpdatesURL=http://www.jnext.org
DefaultDirName={pf}\Optimistec
DefaultGroupName=JNEXT
LicenseFile=MPL-1.1.txt
OutputDir=JNEXT-Output
OutputBaseFilename=JNEXT-1.0.7
WizardImageFile=jnext-left.bmp
WizardSmallImageFile=smallimg.bmp
BackSolid=no
BackColor=$000000
BackColor2=$0000FF

[Tasks]
; NOTE: The following entry contains English phrases ("Create a desktop icon" and "Additional icons"). You are free to translate them into another language if required.
; Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Files]
Source: "..\src\framework-npruntime\Release\npjnext.dll"; DestDir:"{%JNEXT_}{code:getDir|p_firefox}"; Flags: ignoreversion; Check: appExists( 'firefox' );
Source: "..\src\framework-npruntime\Release\npjnext.dll"; DestDir:"{%JNEXT_}{code:getDir|p_opera}"; Flags: ignoreversion; Check: appExists( 'opera' );
Source: "..\src\framework-npruntime\Release\npjnext.dll"; DestDir:"{%JNEXT_}{code:getDir|p_safari}"; Flags: ignoreversion; Check: appExists( 'safari' );
Source: "..\src\framework-npruntime\Release\npjnext.dll"; DestDir:"{%JNEXT_}{code:getDir|p_navigator}"; Flags: ignoreversion; Check: appExists( 'navigator' );
Source: "..\src\plugins\sockets\Release\Sockets.dll"; DestDir:"{%JNEXT_}{code:getDir|e_firefox}"; Flags: ignoreversion; Check: appExists( 'firefox' );
Source: "..\src\plugins\sockets\Release\Sockets.dll"; DestDir:"{%JNEXT_}{code:getDir|e_opera}"; Flags: ignoreversion; Check: appExists( 'opera' );
Source: "..\src\plugins\sockets\Release\Sockets.dll"; DestDir:"{%JNEXT_}{code:getDir|e_safari}"; Flags: ignoreversion; Check: appExists( 'safari' );
Source: "..\src\plugins\sockets\Release\Sockets.dll"; DestDir:"{%JNEXT_}{code:getDir|e_navigator}"; Flags: ignoreversion; Check: appExists( 'navigator' );
Source: "..\src\plugins\filereader\Release\filereader.dll"; DestDir:"{%JNEXT_}{code:getDir|e_firefox}"; Flags: ignoreversion; Check: appExists( 'firefox' );
Source: "..\src\plugins\filereader\Release\filereader.dll"; DestDir:"{%JNEXT_}{code:getDir|e_opera}"; Flags: ignoreversion; Check: appExists( 'opera' );
Source: "..\src\plugins\filereader\Release\filereader.dll"; DestDir:"{%JNEXT_}{code:getDir|e_safari}"; Flags: ignoreversion; Check: appExists( 'safari' );
Source: "..\src\plugins\filereader\Release\filereader.dll"; DestDir:"{%JNEXT_}{code:getDir|e_navigator}"; Flags: ignoreversion; Check: appExists( 'navigator' );
Source: "..\src\plugins\database\sqlite3\Release\SQLite3.dll"; DestDir:"{%JNEXT_}{code:getDir|e_firefox}"; Flags: ignoreversion; Check: appExists( 'firefox' );
Source: "..\src\plugins\database\sqlite3\Release\SQLite3.dll"; DestDir:"{%JNEXT_}{code:getDir|e_opera}"; Flags: ignoreversion; Check: appExists( 'opera' );
Source: "..\src\plugins\database\sqlite3\Release\SQLite3.dll"; DestDir:"{%JNEXT_}{code:getDir|e_safari}"; Flags: ignoreversion; Check: appExists( 'safari' );
Source: "..\src\plugins\database\sqlite3\Release\SQLite3.dll"; DestDir:"{%JNEXT_}{code:getDir|e_navigator}"; Flags: ignoreversion; Check: appExists( 'navigator' );
Source: ".\auth.txt"; DestDir:"{%JNEXT_}{code:getDir|e_firefox}"; Flags: ignoreversion; Check: appExists( 'firefox' );
Source: ".\auth.txt"; DestDir:"{%JNEXT_}{code:getDir|e_opera}"; Flags: ignoreversion; Check: appExists( 'opera' );
Source: ".\auth.txt"; DestDir:"{%JNEXT_}{code:getDir|e_safari}"; Flags: ignoreversion; Check: appExists( 'safari' );
Source: ".\auth.txt"; DestDir:"{%JNEXT_}{code:getDir|e_navigator}"; Flags: ignoreversion; Check: appExists( 'navigator' );

Source: "..\src\framework-ActiveX\Release\jnext.ocx"; DestDir: "{app}\JNEXT\ActiveX\"; Flags: ignoreversion regserver
Source: "..\src\plugins\sockets\Release\Sockets.dll"; DestDir:"{app}\JNEXT\ActiveX\jnext\"; Flags: ignoreversion
Source: "..\src\plugins\filereader\Release\filereader.dll"; DestDir:"{app}\JNEXT\ActiveX\jnext\"; Flags: ignoreversion
Source: "..\src\plugins\database\sqlite3\Release\SQLite3.dll"; DestDir:"{app}\JNEXT\ActiveX\jnext\"; Flags: ignoreversion
Source: ".\auth.txt"; DestDir:"{app}\JNEXT\ActiveX\jnext\"; Flags: ignoreversion
Source: ".\JNEXT.ico"; DestDir:"{app}\JNEXT\"; Flags: ignoreversion
Source: "..\js-samples\simple\*"; DestDir: "{app}\JNEXT\samples\"; Flags: recursesubdirs ignoreversion
Source: ".\ReadMe.html"; DestDir: "{app}\JNEXT\samples\"; Flags: ignoreversion

[Icons]
Name: "{userdesktop}\JNEXT"; Filename: "{app}\JNEXT\samples\"; WorkingDir: "{app}\JNEXT\samples\";  IconFilename: "{app}\JNEXT\JNEXT.ico"
Name: "{group}\JNEXT samples"; Filename: "{app}\JNEXT\samples\"; WorkingDir: "{app}\JNEXT\samples\"
;Name: "{group}\JNEXT Help"; Filename: "{app}\JNEXT\JNEXT.chm"; WorkingDir: "{app}\JNEXT\"
Name: "{group}\Uninstall JNEXT"; Filename: "{uninstallexe}"

[Code]

//------------------------------------------------------------------------------------
function getDir( strBrowser: String ) : String;
var
    strRetVal, strPath, strLeft, strRight: String;
    nStrLen : Integer;
begin
    nStrLen := Length( strBrowser );
    strLeft := strBrowser;
    Delete(strLeft, 3, nStrLen);
    strRight := Copy( strBrowser, 3, nStrLen);

    case strRight of
      'firefox':
          begin
            RegQueryStringValue(HKLM,
              'SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\firefox.exe',
              'Path',
               strRetVal);
            strRetVal := strRetVal + '\plugins\';
          end;
      'navigator':
          begin
            RegQueryStringValue(HKLM,
              'SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\navigator.exe',
              'Path',
               strRetVal);
            strRetVal := strRetVal + '\plugins\';
          end;
      'opera':
          begin
            RegQueryStringValue(HKLM,
              'SOFTWARE\Clients\Mail\Opera\shell\open\command',
              '',
               strRetVal);
            nStrLen := Length( strRetVal );
            strRetVal := Copy( strRetVal, 2, nStrLen-2);
            strRetVal := ExtractFilePath( strRetVal );
            strRetVal := strRetVal + 'programs\plugins\';
         end;
      'safari':
          begin
            RegQueryStringValue(HKLM,
              'SOFTWARE\Clients\StartMenuInternet\Safari.exe\shell\open\command',
              '',
               strRetVal);
            nStrLen := Length( strRetVal );
            strRetVal := Copy( strRetVal, 2, nStrLen-2);
            strRetVal := ExtractFilePath( strRetVal );
            strRetVal := strRetVal + 'programs\plugins\';
         end;
    end;
    
    if strLeft = 'e_' then
    begin
      strRetVal := strRetVal + 'jnext\';
    end;

    Result := strRetVal;
end;

function appExists( strBrowser: String ) : Boolean;
var
  bExists: Boolean;
begin
  bExists := Length( getDir( 'p_' + strBrowser ) ) > 0;
  Result := bExists;
end;

