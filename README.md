# Simple ESP32 Project Framework

I sometimes use an ESP32 board to make a hardware device for controlling
motors, relays, devices etc.  I usually like to have some way to interact with
the application using a terminal like interface.

Rather than cut paste and hack other projects each time I build a new one, I
have made a basic template/framework I can re-use.

It is meant to be used as a simple starting point for various projects using
ESP32 development boards.

This framework uses the Espressif ESP32-IDF environment and is commandline
focused.

No doubt there are many ways to do this (and many will be 'better'), this is
just mine and suits the way I work and the types of projects I build.  It may
or may not be useful for you.

**I have no pretensions about the quality or robustmess of my code, its elegance,
polish or readability.  I specialise in spaghetti.  Feel free to make it clean
and beautiful, criticise it, suggest improvements or throw it away.**

Also it is not meant to be a 'better' example system than that provided by
Espressif which provides a rich set of examples.  My code borrows stuff from
these examples.

It hopefully provides a basic foundation that can be built on for applications
that need console control.

## Overview of what it does

Assuming the project has been configured to match the development board being used, once the board is flashed with
the application, the board will flash the LED and you can connect to it via a
serial monitor: eg
```
idf.py monitor
# or
minicom -D /dev/ttyXXXX --color
# etc
```


On boot, the monitor will display a welcome message and a prompt:
```
------------------------------------
Brain (c)2026 Jon Kelly
Project
 Project Name: switch
 Version: v0.0-5-g9a13f50-dirty
 Compile time: 17:21:36 Jan 31 2026
 IDF Version: v5.5.2
------------------------------------
Ctrl+] to exit monitor
type help for help

esp32-brain> 
```

The basic commands available are similar to the Essprissif example 'CONSOLE'
application (but with some examples of extra functionality added)
```
esp32-brain> help

help  [<string>] [-v <0|1>]
  Print the summary of all registered commands if no arguments are given,
  otherwise print summary of given command.
      <string>  Name of command
  -v, --verbose=<0|1>  If specified, list console commands with given verbose level

ps 
  Get information about running tasks

reboot 
  Reboot system

system 
  Display system information

uname 
  Identify System 

version 
  Get version

esp32-brain> 
```

# Getting it up and running

## To clean up previous builds etc

Assumes you have project code in main/ and a valid CMakeList.txt that defines the project.

This will clean up all the build and configuration info

*NB once done this will require reconfiguration etc (see steps (3)-4-5 below) to allow project to be rebuilt*

```
./distclean
```

## 1. To set up the development environment

Assumes eim installed (Espressif Installation Manager)

```
eim fix
```

## 2. To activate an environment

```
source "/home/jonk/.espressif/tools/activate_idf_v6.1.sh"   #(or whichever version you want)

# you can list known environments using 
eim list
```

## 3. To set the esp target

1. Firstly adjust any board specific configuration in sdkconfig.defaults
2. Build sdkconfig

### To adjust board edit sdkconfig.defaults eg:
```
# this for the FireBeetle 2 ESP32-P4 AI Vision Board
CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y
CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y
```

### To build sdkconfig

First, edit any default values for the board in main/Kconfig.projbuild eg
```
    config BLINK_GPIO
        int "GPIO LED Pin for board"
        default 3
        help
            The GPIO pin that drives the onboard LED
```

then run:

```
idf.py set-target esp32p4   #(or whichever version)
```

*This will build the sdkconfig file.  It will use any default values in the sdkconfig.default file (if it exists)*


## 4. To set/adjust configuration

```
make menuconfig
```


## 5. To generate Makefile

```
cmake .
```

this will allow things like:


# Building/Flashing etc

AT this point you should be able to do things like:

```
make

make flash

make monitor

# etc
```

# Building an application based on the framework

Typically edit the main/commands.c file to add your functionality.

eg
```
...
...
#include "deathray.h"

...
...
int fireMyDeathRay(int argc, char **argv)
{                       
        setUpDeathRay();
        deathRayState_t dr = readDeathRayState();
        if (dr.charged) {
            deathRaySystemFire();
            return 0;
        }
        printf("Sorry, your death ray is not available at this time, please try again later.\n);
        return 1;
}
...
...
```

Then add this as a command to the registerCommands function  in terminal.c
```

	const esp_console_cmd_t commands[] = {
        ...
        ...
		{
			.command = "fdr",
			.help = "Fire The Death Ray",
			.hint = NULL,
			.func = &fireMyDeathRay,
		},
        ...
        ...

```
