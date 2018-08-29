# Script to copy The .ino to a cpp

#Function I found online to delete any file/ folder.
function Remove-Any-File-Force ($Target) {
    if ( Test-Path -Path "$Target" ){
        & $env:SystemRoot\System32\ATTRIB.exe -S -H -R "$Target" >$null 2>$null
    } else {
        return
    }
    $TargetAttributes = (Get-Item -Path $Target -Force).Attributes.ToString()
    if ($TargetAttributes -match "ReparsePoint") {
        if ($TargetAttributes -match "Archive") {
            Remove-Item -Path "$Target" -Force
        } else {
            try {
                & $env:SystemRoot\System32\cmd.exe /c rmdir /Q "$Target" >$null 2>$null
            } catch {
                try {
                    [io.directory]::Delete("$Target")
                } catch {
                    Remove-Item -Path "$Target" -Force
                }
            }
        }
    } else {
        if ($TargetAttributes -match "Directory") {
            Remove-Item -Path "$Target" -Force -Recurse
        } else {
            Remove-Item -Path "$Target" -Force
        }
    }
}

$wrongprojdir="D:\Users\ryanbaas\OneDrive - Signalysis, Inc\Documents\mygit"
# Set directory VARIABLES:

$projdir=Get-Location # This should work, but everytime I use it, it brings in the wrong present location. it sends me "D:\Users\ryanbaas\OneDrive - Signalysis, Inc\Documents\mygit"

If ($projdir -In $wrongprojdir)
{
  $projdir="D:\mygit\public-github\arduino\Arduino-TTL-Serial-DIO" # Manuel Override (uncomment this line)
}

Write-Host "The active project directory is: $projdir"

$piolibdir="$projdir\lib\"
$piosrcdir="$projdir\src\"
$sketchdir="$projdir\srcino\"
$sketchlibdir="$projdir\srcino\Libraries\"

# Tell them where the libraries are getting copied to.
Write-Host "Copping The Libraries to Desitination: $sketchlibdir"

# Remove The Libraries Folder in Sketch directory so we can copy into it
Remove-Any-File-Force ($Sketchlibdir)

# Copy Libraries form pio lib to Arduino Libraries folder:
Copy-Item -Recurse $piolibdir -Destination $sketchlibdir

# Copy src\src.cpp to srcino\srcino.ino
Get-ChildItem -Path $sketchdir -Include *.ino -Recurse | Remove-Item  # Remove it first
Copy-Item "$piosrcdir\src.cpp" -Destination "$sketchdir\srcino.ino"   # Copy it to a new name.

#CRLF DONE!
Write-Host "Contents of src\src.cpp are now copied to srcino\srcino.ino"
Write-Host Done!
