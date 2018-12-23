LIBNAME=rootprop
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
INSTALLER=../../installer.sh
LINK = $(CDIR)/g++
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I/usr/include/QtGui -I/usr/include/QtCore -I$(ROOTINCLUDE) -I../widgets/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR =include/attributewidget.h \
	include/aattributewidget.h \
	include/colorselectdialog.h \
	include/qcanvaswidget.h \
	include/canvaspwidget.h \
	include/histowidget.h \
	include/stylewidget.h \
	include/dividedialog.h \
	include/colorbutton.h \
#	include/treewidget.h 

SRC =src/attributewidget.cpp \
	src/aattributewidget.cpp \
	src/colorselectdialog.cpp  \
	src/qcanvaswidget.cpp \
	src/canvaspwidget.cpp  \
	src/histowidget.cpp  \
	src/stylewidget.cpp  \
	src/dividedialog.cpp  \
	src/colorbutton.cpp  \
#	src/treewidget.cpp 

UIS =src/attributewidget.ui.h \
	src/aattributewidget.ui.h \
	src/colorselectdialog.ui.h \
	src/canvaspwidget.ui.h \
	src/histowidget.ui.h \
	src/stylewidget.ui.h \
	src/dividedialog.ui.h \
	src/colorbutton.ui.h \
#	src/treewidget.ui.h

OBJS =obj/attributewidget$(PCCODE).o \
	obj/aattributewidget$(PCCODE).o \
	obj/colorselectdialog$(PCCODE).o \
	obj/qcanvaswidget$(PCCODE).o \
	obj/canvaspwidget$(PCCODE).o \
	obj/histowidget$(PCCODE).o \
	obj/stylewidget$(PCCODE).o \
	obj/dividedialog$(PCCODE).o \
	obj/colorbutton$(PCCODE).o \
#	obj/treewidget$(PCCODE).o

UIOBJS=obj/attributewidget$(PCCODE)_ui.o \
	obj/aattributewidget$(PCCODE)_ui.o \
	obj/colorselectdialog$(PCCODE)_ui.o \
	obj/canvaspwidget$(PCCODE)_ui.o \
	obj/histowidget$(PCCODE)_ui.o \
	obj/stylewidget$(PCCODE)_ui.o \
	obj/dividedialog$(PCCODE)_ui.o \
	obj/colorbutton$(PCCODE)_ui.o  \
#	obj/treewidget$(PCCODE)_ui.o

MOCSRC =obj/moc_attributewidget$(PCCODE).cpp \
	obj/moc_aattributewidget$(PCCODE).cpp \
	obj/moc_colorselectdialog$(PCCODE).cpp \
	obj/moc_qcanvaswidget$(PCCODE).cpp \
	obj/moc_canvaspwidget$(PCCODE).cpp \
	obj/moc_histowidget$(PCCODE).cpp \
	obj/moc_stylewidget$(PCCODE).cpp \
	obj/moc_dividedialog$(PCCODE).cpp \
	obj/moc_colorbutton$(PCCODE).cpp \
#	obj/moc_treewidget$(PCCODE).cpp

MOCOBJ =obj/moc_attributewidget$(PCCODE).o \
	obj/moc_aattributewidget$(PCCODE).o \
	obj/moc_colorselectdialog$(PCCODE).o  \
	obj/moc_qcanvaswidget$(PCCODE).o \
	obj/moc_canvaspwidget$(PCCODE).o  \
	obj/moc_histowidget$(PCCODE).o  \
	obj/moc_stylewidget$(PCCODE).o \
	obj/moc_dividedialog$(PCCODE).o \
	obj/moc_colorbutton$(PCCODE).o \
#	obj/moc_treewidget$(PCCODE).o 

###

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGETA)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ)
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

PACKDIR=$(KTOFPACK)/gui/root_properties
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


