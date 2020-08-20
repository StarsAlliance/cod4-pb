[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{1601D586-CCA6-4C94-A6C1-05E6C0C4FB38}
AppName="Call of Duty 4 - Modern Warfare"
AppVersion="1.7"
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher="Activision Publishing"
DefaultDirName={commonpf32}\Call of Duty 4 - Modern Warfare
DefaultGroupName=Call of Duty 4
OutputDir=C:\Users\Drago\COD\OUTPUT
OutputBaseFilename=setup
SetupIconFile=C:\Users\Drago\COD\Call of Duty 4 - Modern Warfare\cod4.ico
TimeStampsInUTC=True
ShowLanguageDialog=auto
LanguageDetectionMethod=locale
DiskSpanning=True
DiskSliceSize=735000000
AllowUNCPath=False
Compression=lzma2/ultra64
WizardImageFile=C:\Users\Drago\COD\OUTPUT\left.bmp
WizardSmallImageFile=C:\Users\Drago\COD\OUTPUT\top.bmp
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
Source: "..\Call of Duty 4 - Modern Warfare\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: CallofDuty4
Source: "..\pb\*"; DestDir: "{app}\pb"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Punkbuster
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "..\cod4x183.exe"; DestDir: "{app}"; Components: COD4X

[Run]
Filename: "{src}\DirectX\DXSETUP.exe"; Description:"Установить DirectX"; Flags: postinstall runascurrentuser;
Filename: "{app}\cod4x183.exe"; Description:"Установить COD4X"; Flags: postinstall nowait runascurrentuser runhidden; Components: COD4X
Filename: "{app}\pb\pbsvc.exe"; Description:"Установить Punkbuster"; Flags: postinstall nowait runascurrentuser; Components: Punkbuster

[Registry]
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "InstallDrive"; ValueData: "V:\"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "Language"; ValueData: "rus"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "StartMenuFolder"; ValueData: "Call of Duty 4"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "Version"; ValueData: "1.7"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "UninstallString"; ValueData: "{app}\{uninstallexe}"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "EXEStringS"; ValueData: "{app}\iw3sp.exe"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "EXEStringM"; ValueData: "{app}\iw3mp.exe"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "QA"; ValueData: "5.1"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "Techhelp"; ValueData: "{app}\Docs\Help\readme.htm"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "codkey"; ValueData: "QEGS4TJDUTMSLJTPAF2C"; Flags: uninsdeletekey createvalueifdoesntexist
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "PatchLog"; ValueData: "{app}\install.log"; Flags: uninsdeletekey
Root: "HKLM32"; Subkey: "SOFTWARE\Activision\Call of Duty 4"; ValueType: string; ValueName: "Version-SP"; ValueData: "1.6"; Flags: uninsdeletekey

[Ini]
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "ExtVersion"; String: "1.7";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "IntVersion"; String: "28.2";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Language"; String: "255";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "Branch"; String: "0.0";
Filename: "{app}\version.inf"; Section: "Version Info"; Key: "QA"; String: "5112";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Platform"; String: "PC";
Filename: "{app}\version.inf"; Section: "Product Info"; Key: "Product"; String: "Call of Duty 4: Modern Warfare";

[Components]
Name: "CallofDuty4"; Description: "Call of Duty 4"; Types: compact custom full
Name: "Punkbuster"; Description: "PB Anticheat"; Types: custom full
Name: "COD4X"; Description: "COD4X Mod"; Types: custom full

[Icons]
Name: "{group}\Документация"; Filename: "{app}\Docs"; Flags: foldershortcut
Name: "{group}\Call of Duty 4"; Filename: "{app}\iw3sp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod4.ico";
Name: "{group}\Call of Duty 4 MP "; Filename: "{app}\iw3mp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod4.ico";
Name: "{group}\Поддержка Call of Duty 4"; Filename: "https://stars-alliance.ru"
Name: "{group}\Удалить Call of Duty 4"; Filename: "{uninstallexe}"; WorkingDir: "{app}"; IconFilename: "{app}\cod4.ico"
Name: "{commondesktop}\Call of Duty 4 - MP"; Filename: "{app}\iw3mp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod4.ico"; Tasks: desktopicon
Name: "{commondesktop}\Call of Duty 4"; Filename: "{app}\iw3sp.exe"; WorkingDir: "{app}"; IconFilename: "{app}\cod4.ico"; Tasks: desktopicon
