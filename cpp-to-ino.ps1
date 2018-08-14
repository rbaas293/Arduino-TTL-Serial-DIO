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

# Set directory VARIABLES:
$projdir=$PWD

#Set-Location D:
#Set-Location $PWD


$piolibdir="$projdir\lib"
$piosrcdir="$projdir\src"
$sketchdir="$projdir\srcino"
$sketchlibdir="$projdir\srcino\Libraries"
# $foldersToDelete =
# Copy Libraries form lib to arduino sketch lib folder:
#Get-ChildItem -Path $sketchlibdir -Directory -Recurse | Remove-Item -Recurse -Force -Exclude .git\
if (Test-Path $sketchlibdir)
{
  $items = Get-ChildItem -Path $sketchlibdir -Directory -Recurse
  foreach ($item in $items)
  {
    if (Test-Path $item)
    {
      cmd /c rmdir .\$item
    #  Remove-Item $item -Recurse -Force -Confirm:$false
    }
  }
  # Remove-Item $sketchlibdir -Force -Confirm:$false
}
Copy-Item -Recurse $piolibdir -Destination $sketchlibdir

# Copy src\src.cpp to srcino\srcino.ino
Get-ChildItem -Path $sketchdir -Include *.ino -Recurse | Remove-Item
Copy-Item "$piosrcdir\src.cpp" -Destination "$sketchdir\srcino.ino"

#Move-Item -Path "$sketchdir\src.cpp" -Destination "$sketchdir\srcino.ino"
