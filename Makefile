CUR_PATH = $(shell echo $(PWD))
OBJS = $(addprefix ykushcmd/objs/,ykushcmd.o commandParser.o usbcom.o)
LIBS = -lhidapi-libusb -lusb-1.0 -ludev
LOADPATHS = -L$(CUR_PATH)/ykushcmd/linux
PREPROCESSOR_DEFS = -DLINUX
CPP = g++

ykushcmd : $(OBJS)
	$(CPP) $(LOADPATHS) -o bin/ykushcmd $(OBJS) $(LIBS)


ykushcmd/objs/ykushcmd.o : ykushcmd/ykushcmd.cpp ykushcmd/commandParser.h
	$(CPP) $(PREPROCESSOR_DEFS) -c ykushcmd/ykushcmd.cpp -o ykushcmd/objs/ykushcmd.o

ykushcmd/objs/commandParser.o : ykushcmd/commandParser.cpp ykushcmd/commandParser.h ykushcmd/usbcom.h 
	$(CPP) $(PREPROCESSOR_DEFS) -c ykushcmd/commandParser.cpp -o ykushcmd/objs/commandParser.o

ykushcmd/objs/usbcom.o : ykushcmd/usbcom.cpp ykushcmd/linux/hidapi.h ykushcmd/usbcom.h
	$(CPP) $(PREPROCESSOR_DEFS) -c ykushcmd/usbcom.cpp -o ykushcmd/objs/usbcom.o

clean :
	rm -f bin/ykushcmd $(OBJS)

