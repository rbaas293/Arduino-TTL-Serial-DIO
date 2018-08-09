# Script to copy The .ino to a cpp

# Set directory VARIABLES:
$piolibdir="$env:pwd\lib"
$piosrcdir="$env:pwd\src"
$sketchdir="$env:pwd\srcino"
$sketchlibdir="$env:pwd\srcino\Libraries"

# Copy Libraries form lib to arduino sketch lib folder:
cp $piolibdir $sketchlibdir

# Copy src\src.cpp to srcino\srcino.ino
cp "$piosrcdir\src.cpp" $sketchdir

mv "$sketchdir\src.cpp" "$sketchdir\srcino.ino"
