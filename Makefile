PROG_SOURCE = yktrl.cpp
SOURCE += commandParser.cpp
SOURCE += ykushxs/ykushxs.cpp
SOURCE += ykush/ykush.cpp
SOURCE += ykush2/ykush2.cpp
SOURCE += ykush3/ykush3.cpp
SOURCE += yk_usb_device.cpp
SOURCE += help/ykush_help.cpp
SOURCE += utils/command_parser.cpp
SOURCE += utils/string2val.cpp
SOURCE += usbhid/usbhid.cpp


SOURCE_FULL = $(addprefix src/, $(SOURCE))
PROG_SOURCE_FULL = $(addprefix src/, $(PROG_SOURCE))
OBJS = $(SOURCE_FULL:.cpp=.o)
PROG_OBJ = $(PROG_SOURCE_FULL:.cpp=.o)

DEFINES += _LINUX_
DEFINES += _LIBUSB_

COMPILE_FLAGS += $(addprefix -D, $(DEFINES))

CUR_PATH = $(shell echo $(PWD))
INCLUDEPATHS = $(addprefix -I$(CUR_PATH)/, $(dir $(SOURCE_FULL)) libusb )
LOADPATHS = 
LIBS = -lusb-1.0
CXX ?= g++
 


ykushcmd : $(PROG_OBJ) $(OBJS)
	$(CXX) $(CXXFLAGS) $(COMPILE_FLAGS) -o bin/$@ $(PROG_OBJ) $(OBJS) $(LIBS) $(LDFLAGS)

$(PROG_OBJ) :  %.o : %.cpp
	$(CXX) $(CXXFLAGS) $(COMPILE_FLAGS) $(INCLUDEPATHS) -c $< -o $@

$(OBJS) : %.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) $(COMPILE_FLAGS) $(INCLUDEPATHS) -c $< -o $@


clean :
	rm -f bin/yktrl $(OBJS) $(PROG_OBJ)

