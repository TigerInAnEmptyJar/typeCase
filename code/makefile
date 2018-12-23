LIBNAME=code
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
INSTALLER=../installer.sh
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK) -I$(KTOFPACK)/include -I$(KTOFPACK)/documentation/include -I../parameter/include -I../geometry/include -I.. -I../shapes/include
CFLAGS= -c -g -Werror -Wall -Wextra -fPIC
LIBS = $(GEOObj) 
HDR =include/insertAlgorithm.h \
	include/insertShape.h \
	include/algorithmwriter.h \
	include/shapewriter.h \

SRC =src/insertAlgorithm.cpp \
	src/insertShape.cpp \
	src/algorithmwriter.cpp \
	src/shapewriter.cpp \

OBJS =obj/algorithmwriter$(PCCODE).o \
	obj/shapewriter$(PCCODE).o \
#	obj/insertAlgorithm.o \
#	obj/insertShape.o \


all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS)
	ar cru $(TARGETA) $(OBJS)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP)  
	-rm -f $(TARGET) $(TARGET0) $(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	-ln -s $(TARGET) $(TARGET1)

obj/%.o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

PACKDIR=$(KTOFPACK)/code
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGET1) $(TARGETA) 

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(PWD)/include ../installer.sh $(HDR)


