# Script to copy The .ino to a cpp

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
Get-ChildItem -Path $sketchlibdir -Directory -Recurse | Remove-Item -Recurse -Force -Exclude .git\

Copy-Item -Recurse $piolibdir -Destination $sketchlibdir

# Copy src\src.cpp to srcino\srcino.ino
Get-ChildItem -Path $sketchdir -Include *.ino -Recurse | Remove-Item
Copy-Item "$piosrcdir\src.cpp" -Destination "$sketchdir\srcino.ino"

#Move-Item -Path "$sketchdir\src.cpp" -Destination "$sketchdir\srcino.ino"
