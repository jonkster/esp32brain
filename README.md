# ESP32 Application framework

This framework can be used to start various projects using ESP32 development boards.

It provides some basic functionality including:
1. a simple terminal/console interface
2. flashing LED system
3. skeleton for adding other functionality

The code was built using a Freenove ESP32 WROOM Development Board:
![Freenove ESP32-WROOM-S3](https://github.com/jonkster/esp32brain/blob/mainbranch/doc/images/freenove_wroom.jpg)

Using other boards will require adjustment.

## Setting up development environment (ESP-IDF)

### ESP-IDF

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

### ESP32Brain

Clone repository.

In the cloned repository:
```
cmake .
```

This will create Makefile and associated omponents.

You can now do things like:
```
make all flash monitor
```


## Housekeeping

To clean up project to raw essential files:
```
distclean.sh
# Following this, you will need to run:
 cmake .
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


