@echo off
set year=%date:~10,4%
set month=%date:~4,2%
set day=%date:~7,2%
set folder="..\..\Adventure_Builds\Adventure_%year%%month%%day%"
set inc=1

:again
rem append a 0 if needed.
if 10 GTR %inc% (
	set "incf=0%inc%"
) else (
	set incf=%inc%
)

if EXIST %folder%_%incf% (
set /a "inc=%inc%+1"
goto again
)

set folder=%folder%_%incf%

echo Building package... %folder%

mkdir %folder%

if exist .\DX9_Debug\log.txt del .\DX9_Debug\log.txt
if exist .\DX11_Debug\log.txt del .\DX11_Debug\log.txt
if exist .\DX9_Release\log.txt del .\DX9_Release\log.txt
if exist .\DX11_Release\log.txt del .\DX11_Release\log.txt

xcopy ".\DX9_Debug" "%folder%\DX9_Debug\" /e
xcopy ".\DX11_Debug" "%folder%\DX11_Debug\" /e
xcopy ".\DX9_Release" "%folder%\DX9_Release\" /e
xcopy ".\DX11_Release" "%folder%\DX11_Release\" /e
xcopy ".\assets_common" "%folder%\assets_common\" /e
xcopy ".\assets_tt" "%folder%\assets_tt\" /e
xcopy ".\documentation" "%folder%\documentation\" /e
xcopy "..\extensions\Debug\*.dll" "%folder%\extensions\Debug\" /e
xcopy "..\extensions\Release\*.dll" "%folder%\extensions\Release\" /e
xcopy "..\output\v140_Debug\Adventure\*.*" "%folder%\DX9_Debug\" /e
xcopy "..\output\v140_Debug\Adventure\*.*" "%folder%\DX11_Debug\" /e
xcopy "..\output\v140_Release\Adventure\*.*" "%folder%\DX9_Release\" /e
xcopy "..\output\v140_Release\Adventure\*.*" "%folder%\DX11_Release\" /e


QFindReplace.exe "%folder%\DX9_Debug\setup.xml" "%folder%\DX9_Debug\setup.xml" "../../extensions/" "../extensions/"
QFindReplace.exe "%folder%\DX11_Debug\setup.xml" "%folder%\DX11_Debug\setup.xml" "../../extensions/" "../extensions/"
QFindReplace.exe "%folder%\DX9_Release\setup.xml" "%folder%\DX9_Release\setup.xml" "../../extensions/" "../extensions/"
QFindReplace.exe "%folder%\DX11_Release\setup.xml" "%folder%\DX11_Release\setup.xml" "../../extensions/" "../extensions/"

git log -1 > "%folder%\git_log.txt"

echo "Build complete!"

pause
