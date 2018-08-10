# Script to copy The .ino to a cpp

# Set directory VARIABLES:
#$projdir="$env:Onedrive\Documents\mygit\public-github\Arduino\ArduinoTTL-Serial-DIO"

Set-Location D:
Set-Location $projdir


$piolibdir="$projdir\lib"
$piosrcdir="$projdir\src"
$sketchdir="$projdir\srcino"
$sketchlibdir="$projdir\srcino\Libraries"

# Copy Libraries form lib to arduino sketch lib folder:
Copy-Item -Recurse $piolibdir -Destination $sketchlibdir

# Copy src\src.cpp to srcino\srcino.ino
Copy-Item "$piosrcdir\src.cpp" $sketchdir

Move-Item -Path "$sketchdir\src.cpp" -Destination "$sketchdir\srcino.ino"
