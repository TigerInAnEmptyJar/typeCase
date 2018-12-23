LIBNAME=paramwids
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
INSTALLER=../../installer.sh
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I$(QT_INCLUDE)Core -I$(ROOTINCLUDE) -I../widgets/include -I../root_properties/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR =include/algorithmdisplay.h \
	include/shapedisplay.h \
	include/detectordisplay.h \
	include/materialdisplay.h \
	include/elementdisplay.h \
	include/reactiondisplay.h \
	include/beamtimedisplay.h \
	include/rundisplay.h \
	include/paintdisplay.h

SRC =src/algorithmdisplay.cpp \
	src/shapedisplay.cpp \
	src/detectordisplay.cpp \
	src/materialdisplay.cpp \
	src/elementdisplay.cpp \
	src/reactiondisplay.cpp \
	src/beamtimedisplay.cpp \
	src/rundisplay.cpp \
	src/paintdisplay.cpp

OBJS =obj/algorithmdisplay$(PCCODE).o \
	obj/shapedisplay$(PCCODE).o \
	obj/detectordisplay$(PCCODE).o \
	obj/materialdisplay$(PCCODE).o \
	obj/elementdisplay$(PCCODE).o \
	obj/reactiondisplay$(PCCODE).o \
	obj/beamtimedisplay$(PCCODE).o \
	obj/rundisplay$(PCCODE).o \
	obj/paintdisplay$(PCCODE).o

MOCSRC =obj/moc_algorithmdisplay$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_shapedisplay$(PCCODE).cpp \
	obj/moc_detectordisplay$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_materialdisplay$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_elementdisplay$(PCCODE).cpp \
	obj/moc_reactiondisplay$(PCCODE).cpp \
	obj/moc_beamtimedisplay$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_rundisplay$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_paintdisplay$(PCCODE).cpp

MOCOBJ =obj/moc_algorithmdisplay$(QT_VERSION)$(PCCODE).o \
	obj/moc_shapedisplay$(PCCODE).o \
	obj/moc_detectordisplay$(QT_VERSION)$(PCCODE).o \
	obj/moc_materialdisplay$(QT_VERSION)$(PCCODE).o \
	obj/moc_elementdisplay$(PCCODE).o \
	obj/moc_reactiondisplay$(PCCODE).o \
	obj/moc_beamtimedisplay$(QT_VERSION)$(PCCODE).o \
	obj/moc_rundisplay$(QT_VERSION)$(PCCODE).o \
	obj/moc_paintdisplay$(PCCODE).o

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
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ) #$(UIOBJS)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGET1)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1)

obj/%$(PCCODE).o: src/%.cpp src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/ui_%$(PCCODE).o: src/%.ui.h
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

PACKDIR=$(KTOFPACK)/gui/parameter_widgets
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


