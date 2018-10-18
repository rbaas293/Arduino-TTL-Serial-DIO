* **NOTE** as of 10/4/18 This repository has moved to GitLab as private. I may choose to mirror it here in the future. 

## __RS-232 Serial Commands for Arduino DIO & PWM__
This repository holds the code, libraries, and README created for use with a out of the box arduino uno.

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
