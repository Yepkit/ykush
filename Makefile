CUR_PATH = $(shell echo $(PWD))
OBJS = $(addprefix ykushcmd/objs/,ykushcmd.o commandParser.o usbcom.o ykushxs.o yk_usb_device.o ykush.o)
LIBS = -lhidapi-libusb -lusb-1.0 -ludev
LOADPATHS = -L$(CUR_PATH)/ykushcmd/linux
INCLUDEPATHS= -I$(CUR_PATH)/ykushcmd/linux -I$(CUR_PATH)/ykushcmd/ykushxs -I$(CUR_PATH)/ykushcmd/ykush -I$(CUR_PATH)/ykushcmd
PREPROCESSOR_DEFS = -DLINUX
CPP = g++

ykushcmd : $(OBJS)
	$(CPP) $(LOADPATHS) -o bin/ykushcmd $(OBJS) $(LIBS)

ykushcmd/objs/ykushcmd.o : ykushcmd/ykushcmd.cpp ykushcmd/commandParser.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c ykushcmd/ykushcmd.cpp -o ykushcmd/objs/ykushcmd.o

ykushcmd/objs/commandParser.o : ykushcmd/commandParser.cpp ykushcmd/commandParser.h ykushcmd/usbcom.h 
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c ykushcmd/commandParser.cpp -o ykushcmd/objs/commandParser.o

ykushcmd/objs/usbcom.o : ykushcmd/usbcom.cpp ykushcmd/linux/hidapi.h ykushcmd/usbcom.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c ykushcmd/usbcom.cpp -o ykushcmd/objs/usbcom.o

ykushcmd/objs/ykushxs.o : ykushcmd/ykushxs/ykushxs.cpp ykushcmd/ykushxs/ykushxs.h ykushcmd/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c ykushcmd/ykushxs/ykushxs.cpp -o ykushcmd/objs/ykushxs.o

ykushcmd/objs/ykush.o : ykushcmd/ykush/ykush.cpp ykushcmd/ykush/ykush.h ykushcmd/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c ykushcmd/ykush/ykush.cpp -o ykushcmd/objs/ykush.o

ykushcmd/objs/yk_usb_device.o : ykushcmd/yk_usb_device.cpp ykushcmd/yk_usb_device.h
	$(CPP) $(PREPROCESSOR_DEFS) $(INCLUDEPATHS) -c ykushcmd/yk_usb_device.cpp -o ykushcmd/objs/yk_usb_device.o


clean :
	rm -f bin/ykushcmd $(OBJS)

