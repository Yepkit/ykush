#!/bin/bash

dest=/usr/local

echo installing ykush command...
sudo cp -f bin/ykushcmd $dest/bin

echo installing shared libraries...
sudo cp -f ykushcmd/linux/libhidapi-hidraw.so $dest/lib/libhidapi-hidraw.so.0
sudo ln -fs libhidapi-hidraw.so.0 $dest/lib/libhidapi-hidraw.so

sudo cp -f ykushcmd/linux/libhidapi-libusb.so $dest/lib/libhidapi-libusb.so.0
sudo ln -fs libhidapi-libusb.so.0 $dest/lib/libhidapi-libusb.so

