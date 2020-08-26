[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{1BCE119D-BFDB-4B89-837E-B1DADBC3101B}
AppName="Call of Duty 2"
AppVersion="1.2"
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher="Activision Publishing"
DefaultDirName={commonpf32}\Call of Duty 2
DefaultGroupName=Call of Duty 2
OutputDir=C:\Users\Drago\COD\OUTPUT
OutputBaseFilename=setup
SetupIconFile=C:\Users\Drago\COD\Call of Duty 2\cod2.ico
TimeStampsInUTC=True
ShowLanguageDialog=auto
LanguageDetectionMethod=locale
DiskSpanning=True
DiskSliceSize=735000000
AllowUNCPath=False
Compression=lzma2/ultra64
WizardImageFile=C:\Users\Drago\COD\OUTPUT\cod2-left.bmp
WizardSmallImageFile=C:\Users\Drago\COD\OUTPUT\cod2-top.bmp
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
Source: "..\Call of Duty 2\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: CallofDuty
Source: "..\pb-cod2\*"; DestDir: "{app}\pb"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Punkbuster

[Run]
Filename: "{src}\DirectX\DXSETUP.exe"; Description:"Установить DirectX"; Flags: postinstall runascurrentuser;
Filename: "{app}\pb\pbsvc.exe"; Description:"Установить Punkbuster"; Flags: postinstall nowait runascurrentuser; Components: Punkbuster



[Ini]
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "ExtVersion"; String: "1.2";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "IntVersion"; String: "8.1";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Language"; String: "255";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Branch"; String: "0.0";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "QA"; String: "4141";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Platform"; String: "PC";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Product"; String: "Call of Duty(R) 2";

[Components]
Name: "CallofDuty"; Description: "Call of Duty 2"; Types: compact custom full
Name: "Punkbuster"; Description: "PB Anticheat"; Types: custom full

[Icons]
Name: "{group}\Документация"; Filename: "{app}\Docs"; Flags: foldershortcut
Name: "{group}\Call of Duty 2"; Filename: "{app}\cod2sp_s.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod2.ico";
Name: "{group}\Call of Duty 2 - Multiplayer"; Filename: "{app}\cod2mp_s.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod2.ico";
Name: "{group}\Поддержка Call of Duty"; Filename: "https://stars-alliance.ru"
Name: "{group}\Удалить Call of Duty"; Filename: "{uninstallexe}"; WorkingDir: "{app}"; IconFilename: "{app}\cod2.ico"
Name: "{commondesktop}\Call of Duty 2"; Filename: "{app}\cod2sp_s.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod2.ico"; Tasks: desktopicon
Name: "{commondesktop}\Call of Duty 2 - Multiplayer"; Filename: "{app}\cod2mp_s.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod2.ico"; Tasks: desktopicon

[Registry]
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "codkey"; ValueData: "63EGJ5LGWZW53QX52CED"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "Language"; ValueData: "rus"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "Version"; ValueData: "1.0"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "EXEString"; ValueData: "{app}\cod2sp_s.exe"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "MultiEXEString"; ValueData: "{app}\cod2mp_s.exe"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "QA"; ValueData: "14.0"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "InstallDrive"; ValueData: "V:\"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "UninstallString"; ValueData: "{uninstallexe}"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty 2; ValueType: string; ValueName: "StartMenuFolder"; ValueData: "Call of Duty 2"; Flags: uninsdeletekey;
