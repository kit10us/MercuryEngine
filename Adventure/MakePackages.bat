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

del .\DX9\log.txt
del .\DX11\log.txt

xcopy ".\DX9" "%folder%\DX9\" /e
xcopy ".\DX11" "%folder%\DX11\" /e
xcopy ".\assets_common" "%folder%\assets_common\" /e
xcopy ".\documentation" "%folder%\documentation\" /e
xcopy "..\extensions\*.dll" "%folder%\extensions\" /e
xcopy "..\renderers\*.dll" "%folder%\renderers\" /e
xcopy "..\output\v140_Debug\Adventure\*.*" "%folder%\DX11\" /e
xcopy "..\output\v140_Debug\Adventure\*.*" "%folder%\DX9\" /e


QFindReplace.exe "%folder%\DX11\setup.xml" "%folder%\DX11\setup.xml" "../../extensions/" "../extensions/"

QFindReplace.exe "%folder%\DX11\setup.xml" "%folder%\DX11\setup.xml" "../../renderers/" "../renderers/"

QFindReplace.exe "%folder%\DX9\setup.xml" "%folder%\DX9\setup.xml" "../../extensions/" "../extensions/"

QFindReplace.exe "%folder%\DX9\setup.xml" "%folder%\DX9\setup.xml" "../../renderers/" "../renderers/"

git log -1 > "%folder%\git_log.txt"

echo "Build complete!"

pause
