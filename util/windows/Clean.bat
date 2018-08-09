cd ..\..\

REM Delete remporary files
del *.ncb /s
del *.user /s
del *.filters /s
del *.suo /s /a

REM Delete junctions and symbolic links
del /s Unify

pause