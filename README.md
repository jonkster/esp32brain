# ESP32 Device Framework

I sometimes use an ESP32 board to make a hardware device for controlling
motors, relays, devices etc.  I usually like to have some way to interact with
the application using a terminal like interface.

Rather than cut paste and hack other projects each time I build a new one, I
have made a basic template/framework I can re-use.

It is meant to be used as a simple starting point for various projects using
ESP32 development boards.

In its basic form, it provides some basic functionality including:
1. a simple terminal/console interface
2. a 'proof of life' flashing LED system
3. a multicoloured led alert facility using an onboard RGB led
4. a skeleton for adding other functionality

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

This code assumes a Freenove ESP32 WROOM Development Board:
![Freenove ESP32-WROOM-S3](https://github.com/jonkster/esp32brain/blob/mainbranch/doc/images/freenove_wroom.jpg)

Using other boards will require minor adjustment (eg LED and RGB LED availability/pins)


## Overview of what it does

Assuming the Freenove ESP32-WROOM board is used, once the board is flashed with
the application, the board will flash the LED and you can connect to it via a
serial monitor: eg
```
make monitor
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

alert 
  Flash the alert LED

ps 
  Get information about running tasks

reboot 
  Reboot system

relay_set  nn <0|1>
  Set state of relay
    eg: relay_set 2 1
        set relay #2 on

relay_get  nn
  Get state of relay
    eg:relay_get 2
        get state of relay #2

system 
  Display system information

uname 
  Identify System 

version 
  Get version

esp32-brain> 
```


## Setting up development environment

You need to setup ESP-IDF and then the ESP32Brain framework:

### 1. Install ESP-IDF

Follow instructions at https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html

basically process is:
```
# make sure you have python3
python3 --version

# get ESP-IDF
mkdir -p ~/esp
cd ~/esp

# this was initially developed with esp-idf v5.5.2.  Adjust as required
git clone -b v5.5.2 --recursive https://github.com/espressif/esp-idf.git

cd ~/esp/esp-idf
./install.sh esp32

cd esp-idf/
. $HOME/esp/esp-idf/export.sh

```

### 2. ESP32Brain Framework

Clone this repository.

In the cloned repository:
```
cmake .
```
This will create Makefile and associated components/directories etc that will allow the project to be built.


You can now do things like:
```
make all flash monitor
```
etc


## Building an application based on the framework

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


## Housekeeping

To clean up project to raw essential files:
```
distclean.sh
# Following this, you will need to run:
cmake .
# to recreate needed components to allow project to be built
```

To get a simple generic framework that can be used to build other ESP32 projects, currently use version tagged v0.1 on mainbranch
```
git show -s --format='' --color v0.1
tag v0.1
Tagger: Jonathan Kelly <jonathan.p.h.kelly@gmail.com>

Use this state as start for other ESP32 projects.

If building other similar ESP32 projects, code at this point can be used
as a starting point as it is still pretty generic.
```


