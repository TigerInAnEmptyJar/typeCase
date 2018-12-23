LIBNAME=qt-util
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
INSTALLER=../installer.sh
LINK = $(CDIR)/g++
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
LFLAGS =  -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I/usr/include/QtGui -I/usr/include/QtCore -I$(ROOTINCLUDE) #-I../widgets/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR =	include/utilities.h \

SRC =	src/utilities.cpp  \

UIS =

OBJS =	obj/utilities$(PCCODE).o \

UIOBJS=

MOCSRC =

MOCOBJ =

###

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCSRC) $(MOCOBJ) #$(UIOBJS)
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ) #$(UIOBJS)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGETA)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ)# $(UIOBJS)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGET1)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1)

obj/%$(PCCODE).o: src/%.cpp 
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/%$(PCCODE).o: src/%.cpp src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/%$(PCCODE)_ui.o: src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
"obj/moc_%.o": "obj/moc_%.cpp"
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
"obj/moc_%$(PCCODE).cpp": "include/%.h"
	$(MOC) -o "$@" "$<"
obj/moc_%.o: obj/moc_%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: include/%.h
	$(MOC) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: obj/%.h
	$(MOC) -o "$@" "$<"
obj/%$(QT_VERSION).h: include/%.h
	$(HDRMAKER) --$(QT_VERSION) -o=obj "$<"


clean:
	rm -f $(OBJS)
	rm -f $(MOCOBJ)
	rm -f $(MOCSRC)
	rm -f $(TARGET)
	rm -f obj/*$(QT_VERSION).h

PACKDIR=$(KTOFPACK)/qt-utils
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


