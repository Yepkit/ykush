# YKUSH Command Application


Control application for Yepkit YKUSH Switchable USB Hub board.


Description
===========

Console application developed to illustrate the programmatic control of YKUSH capabilities.
It executes one command per run, being appropriate to be executed as a console command.
But it can be easily adapted to execute a work-flow with multiple commands and we encourage you to alter it to best fit your needs.

The core of the application is consolidated in two source code files, the *commandParser.cpp* and the *usbcom.cpp*.
The first contains the work-flow control and the second the communication functions.

The implementation makes use of hidapi open-source library which is portable across several operating systems.
We include a Visual Studio solution file for building on Windows. For Linux we include a build and installation script, `build.sh` and `install.sh` respectively, for building and installing the application.

Note that hidapi is not included in the source code package and has to be obtained beforehand. For Linux, if the
`build.sh` script is used, to build the application, it will take care of downloading and building the hidapi library for you. In Windows this step has to
be manually performed by the user.


Licensing
=========

The source code is licensed under MIT license.
Refer to [LICENSE](LICENSE.md) file.


Building
========

The steps for building on Windows and Linux are detailed bellow.

Windows
-------

Before building the application from source on Windows you need to get the [hidapi library](http://www.signal11.us/oss/hidapi/) and build it.
Follow the instructions in the hidapi documentation for building the library in the several systems.

After building the hidapi library you need to include the relevant hidapi files to *ykushcmd* project.

Copy to `ykush\ykushcmd\windows\` the following hidapi library files:
- hidapi.dll
- hidapi.exp
- hidapi.lib


Open the `ykush\ykush.sln` with Microsoft Visual Studio and build the solution.
After a successful build process the executable file will be created in the`ykush\bin\` folder.

The next step is to make the dynamically linked library accessible to executable.
We can install the `hidapi.dll` in the system or ensure that a copy of the file is in the same folder than the `ykush.exe`.


Linux
-----

Before attempting one must install developer package for `libhidapi`. On a Debian
system it is possible with:

```
# apt-get install libhidapi-dev
```

After that build the `ykushcmd`:

```
$ make
```

It can be globally installed as follows:

```
# make install
```

After install, the `ykushcmd` command is ready for use.

OSX
---

To build on OS X `hidapi` must be installed. Brew can do it for you:

```
$ brew install hidapi
```

**NB!** Build scripts have been tested only against `hidapi` installed with brew.

Build the utility:

```
$ make
```

And install it globally:

```
$ make install
```

Using it
========

Windows
-------
Navigate to the `ykush\bin\` folder and run `ykush.exe -h` to print all the available command options a syntax.

Let's look at some command examples.

Assuming that just one YKUSH board is connected to our host, if we wanted to turn **OFF** the downstream port 1 we would issue the following command.
```
ykushcmd.exe -d 1
```

Similarly, if we wanted to turn the downstream 1 back **ON** we would issue the following command.
```
ykushcmd.exe -u 1
```

Assuming that two YKUSH boards are connected to our host and we wanted to turn **OFF** the downstream port 1 of one specific board.
To accomplish this we need to address the command by the YKUSH board **serial number**.
To find out the boards serial number we can issue the following command.
```
ykushcmd.exe -l
```
This will print the serial number of all YKUSH boards attached to the host.
Assuming the our board had the serial number YK1234567, we would issue the following command.
```
ykushcmd.exe -s YK1234567 -d 1
```
This would turn **OFF** the downstream port 1 of the board with the serial number YK12345.


Linux
-----
For Linux the command options and syntax are the same with the slight difference that the executable file does not have the `.exe` extension.
Also depending of your user permissions you may need to precede the command with the *sudo* directive (e.g, `sudo ykushcmd -d 1`).


For more information and resources for the YKUSH board please visit the [yepkit website ykush page](https://www.yepkit.com/products/ykush).
