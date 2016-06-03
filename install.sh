#!/bin/bash

echo installing ykush command...
sudo cp -f bin/ykush /usr/bin

echo installing shared libraries...
sudo cp -f ykushcmd/linux/libhidapi-hidraw.so /usr/lib/libhidapi-hidraw.so
sudo cp -f ykushcmd/linux/libhidapi-hidraw.so /usr/lib/libhidapi-hidraw.so.0

sudo cp -f ykushcmd/linux/libhidapi-libusb.so /usr/lib/libhidapi-libusb.so
sudo cp -f ykushcmd/linux/libhidapi-libusb.so /usr/lib/libhidapi-libusb.so.0

