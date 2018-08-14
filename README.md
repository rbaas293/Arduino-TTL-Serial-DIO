## __RS-232 Serial Commands for Arduino DIO & PWM__
This repository holds the code, libraries, and README created for use with a out of the box arduino uno/Mega.

### General Contributing information.
* I currently have my enviorment set up using `Atom` as my text editor. Through Atom I am using The `Platformio-ide` package to upload to arduino boards(`apm install Platformio-ide`). PlatformIO is a greate package that has support for countless embedded boards. It also includes `linter` for a C++ Debuger and `LLVM (aka Clang)` for intelligent autocompleation.
  - I will be including commands to install this enviorment soon.
* The working source file for our main is located in `src\src.cpp`. Please do all editing in this file.
* The `lib\` directory is whre all user created or third party libraries that are not part of PlatformIOs library database go. The Library Dependency Finder (or `LDF`) will search this folder at each build time.
* At varius times, it becomes handy to upload to our board using the `Arduino IDE`. This is where the directory `srcino\` comes into play. This directory is designed to be a `Arduino Sketch` folder, so its contents include a `src\srcino.ino` file and the subdirectory `srcino\Libraries\` Folder. If you need to use the arduino IDE, ~~Run the `cpp-to-ino` script~~, or you must copy the contents `src\src.cpp` to `srcino\srcino.ino` and copy the subdirectories of `lib\` to `srcino\libraries` (in order to make sure you are using the most up to date libraries).


## Command Info and Examples
### 1. Set Output

#### Description:
This command is used to set an output channel to a specified state. The state can be either `HIGH` or `LOW`.

#### Format: `SOUT,<OUTPUT NUMBER>,<DESIRED STATE>`

#### Example:

__1.__ To SET an individual output `HIGH`.
* __Expected input command:__
```
SOUT,3,1
```
* __Expected response:__
```
SOUT,3,1
ACK,1
```

### 2. Toggle Output

#### Description:
This command is used to toggle an output channel `HIGH` or `LOW`, depending on the outputs current state.

#### Format: `TOUT,<OUTPUT NUMBER>`

#### Example:

__1.__ To toggle an individual output:
* __Expected input command:__
```
TOUT,2
```
* __Expected response:__
```
TOUT,2
ACK,1
```

### 3. Pulse Output

#### Description:
This command is used to pulse an output channel `HIGH` for a specified time period less than 10,000ms.

#### Format: `POUT,<OUTPUT NUMBER>,<PULSETIME>`

#### Example:

__1.__ To pulse an individual output for 58ms:
* __Expected input command:__
```
POUT,5,58
```
* __Expected response:__
```
POUT,5,58
ACK
```

### 4. Get Pin State(s)

#### Description:
This command is meant for individual output state queries or to display a table with each output and its corrisponding state.

#### Format: `STAT,<OUTPUT NUMBER>` or `STAT`

#### Examples:

__1.__ To query an individual output:
* __Expected input command:__
```
STAT,1
```
* __Expected response:__
```
STAT,1
ACK,<OUTPUT 1 PIN STATE>
```


__2.__ To query all 5 outputs, simply send:  `STAT`
* __Expected input command:__
      STAT
* __Expected response:__
```
  STAT,ACK

  Output | State
  -------|-------
    O1   |   0
    O2   |   1
    O3   |   0
    O4   |   0
    O5   |   0
```

### 5. Query Scan Rate

#### Description:
This command is used to print the scan rate of the previous microprocessor loop.

#### Format: `SRT?`


### 6. Debug

#### Description:
This command is used to print debuging statments to the serial monitor. It can be very usefull to troubleshooting the firmware.

#### Format: `DBUG`
