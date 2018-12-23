LIBNAME=documentation
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
TARGETP=documentation$(PCCODE)
INSTALLER=../installer.sh
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=moc
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(KTOFPACK)/gui/widgets/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I$(ROOTINCLUDE)
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = -L$(KTOFPACK)/lib -lgeometry$(PCCODE) -lqt-util$(PCCODE) # -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR=include/tmyclass.h

SRC=src/tmyclass.cpp \
	src/nametypedoc.cpp \
	src/functionnametypedoc.cpp \
	src/utils.cpp

OBJS=obj/tmyclass$(PCCODE).o \
	obj/nametypedoc$(PCCODE).o \
	obj/functionnametypedoc$(PCCODE).o \
	obj/utils$(PCCODE).o

HDRp=include/doc_classes.h \
	include/doc_display.h \
	include/documentation_mw.h \
	include/doc_selectclassesdialog.h \
	include/doc_display.h \
	include/tmp.h \
	include/doc_update_dialog.h \

SRCp=src/classes.cpp \
	src/doc_selectclassesdialog.cpp \
	src/main.cpp \
	src/doc_display.cpp \
	src/documentation_mw.cpp \
        src/qmake_image_collection.cpp \
	src/doc_update_dialog.cpp \

OBJp=obj/doc_classes$(PCCODE).o \
	obj/doc_display$(PCCODE).o \
	obj/main$(PCCODE).o \
	obj/doc_selectclassesdialog$(PCCODE).o \
	obj/documentation_mw$(PCCODE).o \
        obj/qmake_image_collection$(PCCODE).o \
	obj/doc_update_dialog$(PCCODE).o \

MOCSRC=obj/moc_doc_classes$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_doc_display$(PCCODE).cpp \
	obj/moc_documentation_mw$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_doc_selectclassesdialog$(PCCODE).cpp \
	obj/moc_doc_update_dialog$(QT_VERSION)$(PCCODE).cpp \


MOCOBJ=obj/moc_doc_classes$(QT_VERSION)$(PCCODE).o \
	obj/moc_doc_display$(PCCODE).o \
	obj/moc_doc_selectclassesdialog$(PCCODE).o \
	obj/moc_documentation_mw$(QT_VERSION)$(PCCODE).o \
	obj/moc_doc_update_dialog$(QT_VERSION)$(PCCODE).o \


all: $(TARGETA) $(TARGET) $(TARGETP)

$(TARGETA): $(OBJS) #$(MOCSRC) $(MOCOBJ) #$(UIOBJS)
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGETA)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) #$(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGET1)

$(TARGETP): $(MOCOBJ) $(OBJp) 
	rm -f $(TARGETP) 
	rm -f $(KTOFPACK)/bin/$(TARGETP)
	$(LINK) -m64 -o $(TARGETP) $(OBJp) $(LIBS) $(QT_LIBS) -L$(KTOFPACK)/lib -lwidgets$(PCCODE) -lgeometry$(PCCODE) -ldocumentation$(PCCODE) $(MOCOBJ) 
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/bin $(INSTALLER) $(TARGETP)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1) $(TARGETP)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1) $(KTOFPACK)/lib/$(TARGETP)

"obj/moc_%$(PCCODE).cpp": "include/%.h"
	$(MOC) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: include/%.h
	$(MOC) -o "$@" "$<"
obj/moc_%.o: obj/moc_%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: include/%.h
	$(MOC) -o "$@" "$<"

obj/%$(PCCODE).o: src/%.cpp src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/%$(PCCODE)_ui.o: src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/moc_%.o: obj/moc_%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/moc_%$(PCCODE).cpp: obj/%.h
	$(MOC) -o "$@" "$<"
obj/%$(QT_VERSION).h: include/%.h
	$(HDRMAKER) --$(QT_VERSION) -o=obj "$<"

clean:
	rm -f $(OBJS)
	rm -f $(MOCOBJ)
	rm -f $(MOCSRC)
	rm -f $(TARGETA)
	rm -f $(TARGET1)
	rm -f $(TARGETP)
	rm -f $(OBJp)


PACKDIR=$(KTOFPACK)/documentation
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)

LIBRARY: $(TARGET) $(TARGETA)