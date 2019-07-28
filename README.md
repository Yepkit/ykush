# YKUSH Command Application


Control application for Yepkit YKUSH Switchable USB Hub boards.


Description
===========

Console application developed to illustrate the programmatic control of YKUSH family boards capabilities.
It executes one command per run, being appropriate to be executed as a console command.
But it can be easily adapted to execute a work-flow with multiple commands and we encourage you to alter it to best fit your needs.

The implementation makes use of libusb for Linux builds and hidapi for Windows.
For Linux we include a build and installation script, `build.sh` and `install.sh` respectively, for building and installing the application. 


Boards Supported
================
- [YKUSH](https://www.yepkit.com/products/ykush)
- [YKUSHXS](https://www.yepkit.com/product/300115/YKUSHXS)
- [YKUSH3](https://www.yepkit.com/product/300110/YKUSH3)


Licensing
=========

The source code is licensed Apache License, Version 2.0. 
Refer to [LICENSE](LICENSE.md) file.


Building
========

The steps for building on Linux and Windows are detailed bellow.


Linux
-----

For Linux `libusb-1.0` must be installed. For Debian based systems run the following.
```
sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev
```
With these dependencies installed, build the application the running the following script.
```
./build.sh
```

After a successful build process you can install the ykush command in the system. To do so, run:
```
sudo ./install.sh
```

After install, the `ykushcmd` command is ready for use.



Windows
-------

To build using MinGW run the following command.

For 32bit:
```
make -f Makefile_win32
```

For 64bit:
```
make -f Makefile_win64
```

After a successful build process the executable file will be created in the `bin\Win32` or `bin\Win64` folder depending if it was the 32 or 64 bit build.



Using it
========

For details on using YKUSHCMD please refer to the [YKUSHCMD Reference Manual](https://www.learn.yepkit.com/reference/ykushcmd-reference-ykush/1/2).











