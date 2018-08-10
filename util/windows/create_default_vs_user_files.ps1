$ErrorActionPreference = "Stop"

$ProjectParentDir="../../MercuryTests"
$SourceFile="source/projectname.vcxproj.user"

echo "Copying file $SourceFile user settings to Mercury Test projects..."

Get-ChildItem -Path $ProjectParentDir -Directory | Where-Object { $_ -match '^[0-9]+tests' } |
    ForEach-Object {
        Get-ChildItem -Path $_.FullName -Directory | Where-Object { $_ -match '[0-9]test' } |
            ForEach-Object {
                Get-ChildItem -Path $_.FullName -File | Where-Object { $_ -match 'vcxproj$' } |
                    ForEach-Object {
                        $DestFile = $_.FullName + ".user"

                        echo "...$DestFile"
                        copy $SourceFile $DestFile
                    }
            }
    }

echo "Done."


