@REM
@REM To build the installer, you will need the WiX Toolset, available at
@REM http://wixtoolset.org/
@REM
@REM To build the zip distribution, the 7za command line utility (7za.exe) from
@REM the 7-Zip Extra package must be present in this script's folder:
@REM http://www.7-zip.org/download.html
@REM

@ECHO OFF
TITLE 3RVXGaiden Installer Builder
ECHO "   _____ ______     ____  _______   "
ECHO "  |___ /|  _ \ \   / /\ \/ /___ /   "
ECHO "    |_ \| |_) \ \ / /  \  /  |_ \   "
ECHO "   ___) |  _ < \ V /   /  \ ___) |  "
ECHO "  |____/|_| \_\ \_/   /_/\_\____/   "
ECHO "                                    "

ECHO This script will generate a 3RVXGaiden installer.
set /p version= "Enter 3RVXGaiden Version (X.X.X): "
ECHO.

ECHO Setting Version: %version%
ECHO.

ECHO Setting PATH...
ECHO ON

del "../Release/Settings.xml"

set PATH=%PATH%;%WIX%\bin
heat dir ../Release -gg -srd -cg 3RVXGaidenComponents -dr INSTALLFOLDER -out 3RVXGaidenComponents.wxs
candle -pedantic -dProductVersion=%version%.0 ./3RVXGaiden.wxs ./3RVXGaidenComponents.wxs
light -b ../Release -ext WixUIExtension ./3RVXGaiden.wixobj ./3RVXGaidenComponents.wixobj -o 3RVXGaiden-%version%.msi
del 3RVXGaiden.wixobj 3RVXGaiden.wixpdb 3RVXGaidenComponents.wxs 3RVXGaidenComponents.wixobj

@ECHO OFF
ECHO.
ECHO.
IF EXIST "7za.exe" (
	ECHO Creating .zip distribution...
	ECHO.
	ECHO ON
	del 3RVXGaiden-%version%.zip
	copy "..\Assets\EmptySettings.xml" "..\Release\Settings.xml"
	7za a -tzip -mx9 3RVXGaiden-%version%.zip ../Release/*
)

@ECHO OFF
ECHO.

PAUSE
