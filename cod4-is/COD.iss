[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{0F0AE9AB-CEA0-4EEE-8B71-02D642BAED8F}
AppName="Call of Duty"
AppVersion="1.5"
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher="Activision Publishing"
DefaultDirName={commonpf32}\Call of Duty
DefaultGroupName=Call of Duty
OutputDir=C:\Users\Drago\COD\OUTPUT
OutputBaseFilename=setup
SetupIconFile=C:\Users\Drago\COD\Call of Duty-UO\coduo.ico
TimeStampsInUTC=True
ShowLanguageDialog=auto
LanguageDetectionMethod=locale
DiskSpanning=True
DiskSliceSize=735000000
AllowUNCPath=False
Compression=lzma2/ultra64
WizardImageFile=C:\Users\Drago\COD\OUTPUT\cod-left.bmp
WizardSmallImageFile=C:\Users\Drago\COD\OUTPUT\cod-top.bmp
WizardStyle=modern
CloseApplications=True
RestartApplications=False
UninstallDisplayIcon={uninstallexe}
PrivilegesRequired=admin
AllowNoIcons=True
DisableWelcomePage=False
InternalCompressLevel=ultra64

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Call of Duty\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: CallofDuty
Source: "..\Call of Duty-UO\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: UO
Source: "..\pb-cod1\*"; DestDir: "{app}\pb"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Punkbuster

[Run]
Filename: "{src}\DirectX\DXSETUP.exe"; Description:"Установить DirectX"; Flags: postinstall runascurrentuser;
Filename: "{app}\pb\pbsvc.exe"; Description:"Установить Punkbuster"; Flags: postinstall nowait runascurrentuser; Components: Punkbuster

[Registry]
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "InstallDrive"; ValueData: "V:"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "Language"; ValueData: "RU"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "StartMenuFolder"; ValueData: "Call of Duty"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "Version"; ValueData: "1.5"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "pb"; ValueData: "1"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty"; ValueType: string; ValueName: "codkey"; ValueData: "WUUEWX38XEYE8PWW1D32"; Flags: uninsdeletekey
;UO
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty United Offensive"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletekey; Components: UO
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty United Offensive"; ValueType: string; ValueName: "key"; ValueData: "RTWJDKHG4GJKUHJKED34"; Flags: uninsdeletekey; Components: UO
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty United Offensive"; ValueType: string; ValueName: "InstallDrive"; ValueData: "V:\"; Flags: uninsdeletekey; Components: UO
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty United Offensive"; ValueType: string; ValueName: "Language"; ValueData: "134"; Flags: uninsdeletekey; Components: UO
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty United Offensive"; ValueType: string; ValueName: "StartMenuFolder"; ValueData: "Call of Duty - United Offensive"; Flags: uninsdeletekey; Components: UO
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty United Offensive"; ValueType: string; ValueName: "Version"; ValueData: "1.51"; Flags: uninsdeletekey; Components: UO

[Ini]
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "ExtVersion"; String: "1.5";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "IntVersion"; String: "5.3";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Language"; String: "255";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Branch"; String: "0.0";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "QA"; String: "3602";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Platform"; String: "PC";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Product"; String: "Call of Duty";

[Components]
Name: "CallofDuty"; Description: "Call of Duty"; Types: compact custom full
Name: "UO"; Description: "United Offesive Expansion pack"; Types: custom full
Name: "Punkbuster"; Description: "PB Anticheat"; Types: custom full

[Icons]
Name: "{group}\Документация"; Filename: "{app}\Docs"; Flags: foldershortcut
Name: "{group}\Call of Duty"; Filename: "{app}\codsp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod.ico";
Name: "{group}\Call of Duty - Multiplayer"; Filename: "{app}\codmp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod.ico";
Name: "{group}\Call of Duty United Offensive"; Filename: "{app}\coduosp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\coduo.ico"; Components: UO
Name: "{group}\Call of Duty United Offensive - Multiplayer"; Filename: "{app}\coduomp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\coduo.ico"; Components: UO
Name: "{group}\Поддержка Call of Duty"; Filename: "https://stars-alliance.ru"
Name: "{group}\Удалить Call of Duty"; Filename: "{uninstallexe}"; WorkingDir: "{app}"; IconFilename: "{app}\cod.ico"
Name: "{commondesktop}\Call of Duty"; Filename: "{app}\codsp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod-128.ico"; Tasks: desktopicon
Name: "{commondesktop}\Call of Duty - Multiplayer"; Filename: "{app}\codmp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod-128.ico"; Tasks: desktopicon
Name: "{commondesktop}\Call of Duty United Offensive"; Filename: "{app}\coduosp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\coduo-128.ico"; Tasks: desktopicon; Components: UO
Name: "{commondesktop}\Call of Duty United Offensive - Multiplayer"; Filename: "{app}\coduomp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\coduo-128.ico"; Tasks: desktopicon; Components: UO
