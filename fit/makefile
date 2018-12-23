CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LIBNAME=fit
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
LFLAGS =  -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include 
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lgeometry$(PCCODE) 

HDR =include/fittingRoutines.h

SRC =src/kinfit.cpp \
	src/lineFit.cpp \
	src/vertexFit.cpp
OBJS =obj/kinfit$(PCCODE).o \
	obj/lineFit$(PCCODE).o \
	obj/vertexFit$(PCCODE).o

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGETA)

$(TARGET):  $(OBJS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGET1)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1)

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -c "$<" -o "$@"
obj/%$(PCCODE).o: src/%.c
	$(CCC) $(CFLAGS) -x c $(INCS) -c "$<" -o "$@"

PACKDIR=$(KTOFPACK)/$(LIBNAME)

INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

