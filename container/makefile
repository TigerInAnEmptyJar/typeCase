LIBNAME=container
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include 
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lgeometry$(PCCODE) -lparameter$(PCCODE) -lshape$(PCCODE)
 #-L/usr/lib64 -lThread -lQt3Support -lQtSql -pthread -lQtXml -L/usr/X11R6/lib64 -lQtNetwork -lQtGui -lpng -lSM -lICE -lXi -lXrender -lXrandr -lXfixes -lXcursor -lXinerama -lfreetype -lfontconfig -lXext -lX11 -lQtCore -lz -lm -lgthread-2.0 -lrt -lglib-2.0 -ldl 

HDR =include/TBase.h \
	include/container.h \
	include/rawHit.h \
	include/calibHit.h \
	include/hitCluster.h \
	include/pixel.h \
	include/cluster.h \
	include/track.h \
	include/event.h \
	include/detector.h \
	include/material.h \
	include/setup.h \
	include/CommonCalibrationParser.h \
	include/container_utilities.h

SRC =src/TBase.cpp \
	src/rawHit.cpp \
	src/calibHit.cpp \
	src/hitCluster.cpp \
	src/pixel.cpp \
	src/cluster.cpp \
	src/track.cpp \
	src/event.cpp \
	src/detector.cpp \
	src/material.cpp \
	src/setup.cpp \
	src/CommonCalibrationParser.cpp \
	src/container_utilities.cpp

OBJS =obj/TBase$(PCCODE).o \
	obj/rawHit$(PCCODE).o \
	obj/calibHit$(PCCODE).o \
	obj/hitCluster$(PCCODE).o \
	obj/pixel$(PCCODE).o \
	obj/cluster$(PCCODE).o \
	obj/track$(PCCODE).o \
	obj/event$(PCCODE).o \
	obj/detector$(PCCODE).o \
	obj/material$(PCCODE).o \
	obj/setup$(PCCODE).o \
	obj/CommonCalibrationParser$(PCCODE).o  \
	obj/container_utilities$(PCCODE).o



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
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

PACKDIR=$(KTOFPACK)/container
INSTALL: $(TARGET)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

