LIBNAME=installation
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
INSTALLER=../../installer.sh
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK) -I$(KTOFPACK)/include -I$(KTOFPACK)/documentation/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I$(QT_INCLUDE)Core -I$(KTOFPACK)/gui/parameter_widgets/include -I$(ROOTINCLUDE) -I$(KTOFPACK)/gui/root_properties/include 
CFLAGS= -c -g -Werror -Wall -Wextra -fPIC
LIBS = $(GEOObj) 
HDR =include/algorithmwizard.h \
	include/shapewizard.h \
#	include/.h \

SRC =src/algorithmwizard.cpp \
	src/shapewizard.cpp \
#	src/.cpp \

OBJS =obj/algorithmwizard$(PCCODE).o \
	obj/shapewizard$(PCCODE).o \
#	obj/.o \

MOCSRC =obj/moc_algorithmwizard$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_shapewizard$(QT_VERSION)$(PCCODE).cpp \
#	obj/$(PCCODE).cpp \

MOCOBJ =obj/moc_algorithmwizard$(QT_VERSION)$(PCCODE).o \
	obj/moc_shapewizard$(QT_VERSION)$(PCCODE).o \
#	obj/$(PCCODE).o \

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCOBJ)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCOBJ)
	-rm -f $(TARGET) $(TARGET0) $(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(MOCOBJ) $(LIBS)
	-ln -s $(TARGET) $(TARGET1)

obj/%$(PCCODE).o: src/%.cpp 
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
	rm -f $(TARGET)

PACKDIR=$(KTOFPACK)/gui/install_wizards
INSTALL: $(TARGET)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGET1) $(TARGETA) 
	-ln -s $(TARGET) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(PWD)/include $(INSTALLER) $(HDR)


