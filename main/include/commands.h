#ifndef _COMMANDS_H_
#define _COMMANDS_H_

int uname(int argc, char **argv);

int version(int argc, char **argv);

int systemStats(int argc, char **argv);

int setRelay(int argc, char **argv);

int getRelay(int argc, char **argv);

int tasksInfo(int argc, char **argv);

int reboot(int argc, char **argv);

int alertLed(int argc, char **argv);

#endif
