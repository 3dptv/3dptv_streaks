# Microsoft Developer Studio Project File - Name="Stereo2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Stereo2 - Win32 Debug_Office
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Stereo2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Stereo2.mak" CFG="Stereo2 - Win32 Debug_Office"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Stereo2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Stereo2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Stereo2 - Win32 Debug_Office" (based on "Win32 (x86) Application")
!MESSAGE "Stereo2 - Win32 Release_Office" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Stereo2 - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\National Instruments\Ni-imaq\include" /I "C:\Software\IMSLNT\Cnl\include" /I "C:\Program Files\Logical Vision\h" /I "C:\Program Files\Logical Vision\G\h" /I "C:\Software\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 imaq.lib cmath.lib wObj.lib wSegment.lib wMorpho.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib" /libpath:"C:\Program Files\Logical Vision\lib"

!ELSEIF  "$(CFG)" == "Stereo2 - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\National Instruments\Ni-imaq\include" /I "C:\Software\IMSLNT\Cnl\include" /I "C:\Program Files\Logical Vision\h" /I "C:\Program Files\Logical Vision\G\h" /I "C:\Software\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imaq.lib cmath.lib wObj.lib wSegment.lib wMorpho.lib Glu32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib" /libpath:"C:\Program Files\Logical Vision\lib" /libpath:"C:\Software\dll"

!ELSEIF  "$(CFG)" == "Stereo2 - Win32 Debug_Office"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Stereo2___Win32_Debug_Office"
# PROP BASE Intermediate_Dir "Stereo2___Win32_Debug_Office"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Stereo2___Win32_Debug_Office"
# PROP Intermediate_Dir "Stereo2___Win32_Debug_Office"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\National Instruments\Ni-imaq\include" /I "C:\Software\IMSLNT\Cnl\include" /I "C:\Program Files\Logical Vision\h" /I "C:\Program Files\Logical Vision\G\h" /I "C:\Software\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\National Instruments\Ni-imaq\include" /I "C:\Program Files\IMSL\include" /I "C:\Program Files\Logical Vision\h" /I "C:\Program Files\Logical Vision\G\h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imaq.lib cmath.lib wObj.lib wSegment.lib wMorpho.lib Glu32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib" /libpath:"C:\Program Files\Logical Vision\lib" /libpath:"C:\Software\dll"
# ADD LINK32 imaq.lib cmath.lib cstat.lib Glu32.lib wObj.lib wSegment.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Program Files\IMSL\lib" /libpath:"C:\Program Files\Logical Vision\lib" /libpath:"C:\Software\dll"

!ELSEIF  "$(CFG)" == "Stereo2 - Win32 Release_Office"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stereo2___Win32_Release_Office"
# PROP BASE Intermediate_Dir "Stereo2___Win32_Release_Office"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stereo2___Win32_Release_Office"
# PROP Intermediate_Dir "Stereo2___Win32_Release_Office"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\National Instruments\Ni-imaq\include" /I "C:\Software\IMSLNT\Cnl\include" /I "C:\Program Files\Logical Vision\h" /I "C:\Program Files\Logical Vision\G\h" /I "C:\Software\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\National Instruments\Ni-imaq\include" /I "C:\Program Files\IMSL\include" /I "C:\Software\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imaq.lib cmath.lib wObj.lib wSegment.lib wMorpho.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Software\IMSLNT\CNL\lib" /libpath:"C:\Program Files\Logical Vision\lib"
# ADD LINK32 imaq.lib cmath.lib cstat.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files\National Instruments\Ni-imaq\lib\Msvc" /libpath:"C:\Program Files\IMSL\lib"

!ENDIF 

# Begin Target

# Name "Stereo2 - Win32 Release"
# Name "Stereo2 - Win32 Debug"
# Name "Stereo2 - Win32 Debug_Office"
# Name "Stereo2 - Win32 Release_Office"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\dspline.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\matrix_op.cpp
# End Source File
# Begin Source File

SOURCE=.\path.cpp
# End Source File
# Begin Source File

SOURCE=.\PathList.cpp
# End Source File
# Begin Source File

SOURCE=.\plot.cpp
# End Source File
# Begin Source File

SOURCE=.\spline.cpp
# End Source File
# Begin Source File

SOURCE=.\splinelist.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Stereo2.cpp
# End Source File
# Begin Source File

SOURCE=.\Stereo2.rc
# End Source File
# Begin Source File

SOURCE=.\Stereo2Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\vrml_generator.cpp
# End Source File
# Begin Source File

SOURCE=.\Write_out.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\matrix_op.h
# End Source File
# Begin Source File

SOURCE=.\parameters.h
# End Source File
# Begin Source File

SOURCE=.\path.h
# End Source File
# Begin Source File

SOURCE=.\pathlist.h
# End Source File
# Begin Source File

SOURCE=.\plot.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\spline.h
# End Source File
# Begin Source File

SOURCE=.\splinelist.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Stereo2.h
# End Source File
# Begin Source File

SOURCE=.\Stereo2Dlg.h
# End Source File
# Begin Source File

SOURCE=.\vrml_generator.h
# End Source File
# Begin Source File

SOURCE=.\write_out.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Stereo2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Stereo2.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\axes.wrl
# End Source File
# Begin Source File

SOURCE=.\bench.wrl
# End Source File
# Begin Source File

SOURCE=.\box.wrl
# End Source File
# Begin Source File

SOURCE=.\delta.wrl
# End Source File
# Begin Source File

SOURCE=.\feeder.wrl
# End Source File
# Begin Source File

SOURCE=.\floor.wrl
# End Source File
# Begin Source File

SOURCE=.\lights.wrl
# End Source File
# Begin Source File

SOURCE=.\splinelist.asp
# End Source File
# Begin Source File

SOURCE=.\streaks.3D
# End Source File
# Begin Source File

SOURCE=.\streaks.wrl
# End Source File
# Begin Source File

SOURCE=.\windtunnel.wrl
# End Source File
# End Target
# End Project
