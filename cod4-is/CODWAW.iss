[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{63D569DF-A40C-4632-96FF-B89B6BB1020A}
AppName="Call of Duty: World At War"
AppVersion="1.7"
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher="Activision Publishing"
DefaultDirName={commonpf32}\Call of Duty - World at War
DefaultGroupName=Call of Duty - World at War
OutputDir=C:\Users\Drago\COD\OUTPUT
OutputBaseFilename=setup
SetupIconFile=C:\Users\Drago\COD\Call of Duty - World at War\CoDWaW.ico
TimeStampsInUTC=True
ShowLanguageDialog=auto
LanguageDetectionMethod=locale
DiskSpanning=True
DiskSliceSize=735000000
AllowUNCPath=False
Compression=lzma2/ultra64
WizardImageFile=C:\Users\Drago\COD\OUTPUT\left-waw.bmp
WizardSmallImageFile=C:\Users\Drago\COD\OUTPUT\top-waw.bmp
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
Source: "..\Call of Duty - World at War\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: CallofDuty
Source: "..\pb-waw\*"; DestDir: "{app}\pb"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Punkbuster

[Run]
Filename: "{src}\DirectX\DXSETUP.exe"; Description:"Установить DirectX"; Flags: postinstall runascurrentuser;
Filename: "{app}\pb\pbsvc.exe"; Description:"Установить Punkbuster"; Flags: postinstall nowait runascurrentuser; Components: Punkbuster

[Registry]
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "InstallDrive"; ValueData: "V:\"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "Language"; ValueData: "rus"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "StartMenuFolder"; ValueData: "C:\Users\Drago\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\{group}"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "Version"; ValueData: "1.7"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "UninstallString"; ValueData: "{uninstallexe}"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "EXEStringS"; ValueData: "{app}\CoDWaW.exe"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "EXEStringM"; ValueData: "{app}\CoDWaWmp.exe"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "QA"; ValueData: "6.0"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "Techhelp"; ValueData: "{app}\Docs\Help\readme.htm"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "codkey"; ValueData: "MPC8MD8P78P175DU890F"; Flags: uninsdeletekey;
Root: HKLM32; Subkey: SOFTWARE\Activision\Call of Duty WAW; ValueType: string; ValueName: "PatchLog"; ValueData: "{app}\install.log"; Flags: uninsdeletekey;

[Components]
Name: "CallofDuty"; Description: "Call of Duty - World At War"; Types: compact custom full
Name: "Punkbuster"; Description: "PB Anticheat"; Types: custom full

[Icons]
Name: "{group}\Документация"; Filename: "{app}\Docs"; Flags: foldershortcut
Name: "{group}\Call of Duty - WAW"; Filename: "{app}\CoDWaW.exe"; WorkingDir: "{app}"; IconFilename: "{app}\CoDWaW.ico";
Name: "{group}\Call of Duty - WAW MP"; Filename: "{app}\CoDWaWmp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\CoDWaW.ico";
Name: "{group}\Поддержка Call of Duty - WAW"; Filename: "https://stars-alliance.ru"
Name: "{group}\Удалить Call of Duty - WAW"; Filename: "{uninstallexe}"; WorkingDir: "{app}"; IconFilename: "{app}\CoDWaW.ico"
Name: "{commondesktop}\Call of Duty - WAW"; Filename: "{app}\CoDWaW.exe"; WorkingDir: "{app}"; IconFilename: "{app}\CoDWaW.ico"; Tasks: desktopicon
Name: "{commondesktop}\Call of Duty - WAW MP"; Filename: "{app}\CoDWaWmp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\CoDWaW.ico"; Tasks: desktopicon

[Ini]
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "ExtVersion"; String: "1.7";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "IntVersion"; String: "68.0";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Language"; String: "255";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Branch"; String: "0.0";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "QA"; String: "6448";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Platform"; String: "PC";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Product"; String: "Call of Duty: World at War";
