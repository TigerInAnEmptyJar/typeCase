CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LIBNAME=parameter
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lgeometry$(PCCODE) 
#-L/usr/lib64 -lThread -lQt3Support -lQtSql -pthread -lQtXml -L/usr/X11R6/lib64 -lQtNetwork -lQtGui -lpng -lSM -lICE -lXi -lXrender -lXrandr -lXfixes -lXcursor -lXinerama -lfreetype -lfontconfig -lXext -lX11 -lQtCore -lz -lm -lgthread-2.0 -lrt -lglib-2.0 -ldl 

HDR =include/algorithmparameter.h \
	include/baseparameter.h \
	include/beamtimeparameter.h \
	include/detectorparameter.h \
	include/materialparameter.h \
	include/paintparameter.h \
	include/parameterManager.h \
	include/shapeparameter.h
SRC =src/algorithmparameter.cpp \
	src/baseparameter.cpp \
	src/beamtimeparameter.cpp \
	src/detectorparameter.cpp \
	src/materialparameter.cpp \
	src/paintparameter.cpp \
	src/parameterManager.cpp \
	src/shapeparameter.cpp
OBJS = 	obj/baseparameter$(PCCODE).o \
	obj/algorithmparameter$(PCCODE).o \
	obj/beamtimeparameter$(PCCODE).o \
	obj/shapeparameter$(PCCODE).o \
	obj/detectorparameter$(PCCODE).o \
	obj/materialparameter$(PCCODE).o \
	obj/paintparameter$(PCCODE).o \
	obj/parameterManager$(PCCODE).o


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

PACKDIR=$(KTOFPACK)/$(LIBNAME)

INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

