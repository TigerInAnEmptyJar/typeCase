LIBNAME=listwids
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
INSTALLER=../../installer.sh
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I$(QT_INCLUDE)Core -I$(ROOTINCLUDE) -I../widgets/include -I../parameter_widgets/include -I../root_properties/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR =include/algorithmwidget.h \
	include/algorithmorderwidget.h \
	include/shapewidget.h \
	include/detectorwidget.h \
	include/materialwidget.h \
	include/databasiswidget.h \
	include/paintwidget.h \
	include/setupwidget.h

SRC =src/algorithmwidget.cpp \
	src/algorithmorderwidget.cpp \
	src/shapewidget.cpp \
	src/detectorwidget.cpp \
	src/materialwidget.cpp \
	src/databasiswidget.cpp \
	src/paintwidget.cpp \
	src/setupwidget.cpp

OBJS =obj/algorithmwidget$(PCCODE).o \
	obj/algorithmorderwidget$(PCCODE).o \
	obj/shapewidget$(PCCODE).o \
	obj/detectorwidget$(PCCODE).o \
	obj/materialwidget$(PCCODE).o \
	obj/databasiswidget$(PCCODE).o \
	obj/paintwidget$(PCCODE).o \
	obj/setupwidget$(PCCODE).o

MOCSRC =obj/moc_algorithmwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_algorithmorderwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_shapewidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_detectorwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_materialwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_databasiswidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_paintwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_setupwidget$(PCCODE).cpp

MOCOBJ =obj/moc_algorithmwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_algorithmorderwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_shapewidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_detectorwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_materialwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_databasiswidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_paintwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_setupwidget$(PCCODE).o

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

PACKDIR=$(KTOFPACK)/gui/list_widgets
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


