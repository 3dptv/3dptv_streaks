# Microsoft Developer Studio Project File - Name="CamCal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CamCal - Win32 Debug_Alexandros
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CamCal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CamCal.mak" CFG="CamCal - Win32 Debug_Alexandros"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CamCal - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CamCal - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CamCal - Win32 Release_Alexandros" (based on "Win32 (x86) Application")
!MESSAGE "CamCal - Win32 Debug_Alexandros" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CamCal - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\National Instruments\NI-IMAQ\include" /I "C:\Software\IMSLNT\CNL\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 imaq.lib cmath.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib"

!ELSEIF  "$(CFG)" == "CamCal - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\National Instruments\NI-IMAQ\include" /I "C:\Software\IMSLNT\CNL\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imaq.lib cmath.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "CamCal - Win32 Release_Alexandros"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CamCal___Win32_Release_Alexandros"
# PROP BASE Intermediate_Dir "CamCal___Win32_Release_Alexandros"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CamCal___Win32_Release_Alexandros"
# PROP Intermediate_Dir "CamCal___Win32_Release_Alexandros"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\National Instruments\NI-IMAQ\include" /I "C:\Software\IMSLNT\CNL\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\National Instruments\NI-IMAQ\include" /I "C:\Program Files\IMSL\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imaq.lib cmath.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib"
# ADD LINK32 imaq.lib cmath.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Program Files\IMSL\lib"

!ELSEIF  "$(CFG)" == "CamCal - Win32 Debug_Alexandros"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CamCal___Win32_Debug_Alexandros"
# PROP BASE Intermediate_Dir "CamCal___Win32_Debug_Alexandros"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CamCal___Win32_Debug_Alexandros"
# PROP Intermediate_Dir "CamCal___Win32_Debug_Alexandros"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\National Instruments\NI-IMAQ\include" /I "C:\Program Files\IMSL\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\National Instruments\NI-IMAQ\include" /I "C:\Program Files\IMSL\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imaq.lib cmath.lib opengl32.lib glu32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Program Files\IMSL\lib"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 imaq.lib cmath.lib opengl32.lib glu32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Program Files\IMSL\lib"
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "CamCal - Win32 Release"
# Name "CamCal - Win32 Debug"
# Name "CamCal - Win32 Release_Alexandros"
# Name "CamCal - Win32 Debug_Alexandros"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cal_3d.cpp
# End Source File
# Begin Source File

SOURCE=.\calibration.cpp
# End Source File
# Begin Source File

SOURCE=.\CamCal.cpp
# End Source File
# Begin Source File

SOURCE=.\CamCal.rc
# End Source File
# Begin Source File

SOURCE=.\CamCalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ccd_parDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\dist_look_up.cpp
# End Source File
# Begin Source File

SOURCE=.\Get_3D_DialogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\img_proc.cpp
# End Source File
# Begin Source File

SOURCE=.\io_functions.cpp
# End Source File
# Begin Source File

SOURCE=.\lin_calibration.cpp
# End Source File
# Begin Source File

SOURCE=.\matrix_op.cpp
# End Source File
# Begin Source File

SOURCE=.\plot_functions.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\vrml_generator.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\MFC\Include\AFX.H"
# End Source File
# Begin Source File

SOURCE=.\cal_3d.h
# End Source File
# Begin Source File

SOURCE=.\calibration.h
# End Source File
# Begin Source File

SOURCE=.\CamCal.h
# End Source File
# Begin Source File

SOURCE=.\CamCalDlg.h
# End Source File
# Begin Source File

SOURCE=.\ccd_parDlg.h
# End Source File
# Begin Source File

SOURCE=.\dist_look_up.h
# End Source File
# Begin Source File

SOURCE=.\Get_3D_DialogDlg.h
# End Source File
# Begin Source File

SOURCE=.\img_proc.h
# End Source File
# Begin Source File

SOURCE=.\io_functions.h
# End Source File
# Begin Source File

SOURCE=.\lin_calibration.h
# End Source File
# Begin Source File

SOURCE=.\matrix_op.h
# End Source File
# Begin Source File

SOURCE=.\parameters.h
# End Source File
# Begin Source File

SOURCE=.\plot_functions.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\structures.h
# End Source File
# Begin Source File

SOURCE=.\vrml_generator.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CamCal.ico
# End Source File
# Begin Source File

SOURCE=.\res\CamCal.rc2
# End Source File
# Begin Source File

SOURCE="..\Calibration Files\Test1_Mai\Camr\points.3D"
# End Source File
# End Group
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE="..\Calibration Files\Test1_Mai\Camr\points.wrl"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
