TARGET=typeCase$(PCCODE)
INSTALLER=../../installer.sh
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
LFLAGS = 
INCS = -I. -Iinclude -I$(KTOFPACK) -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I$(QT_INCLUDE)Core -I$(ROOTINCLUDE) -I$(KTOFPACK)/gui/widgets/include -I$(KTOFPACK)/gui/root_properties/include -I$(KTOFPACK)/gui/list_widgets/include -I$(KTOFPACK)/gui/parameter_widgets/include -I$(KTOFPACK)/gui/io_algorithm_widgets/include -I$(KTOFPACK)/gui/install_wizards/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = -L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE) -lfit$(PCCODE) -lalgorithm$(PCCODE) -lbasic$(PCCODE) -leventpainter$(PCCODE) -lcode$(PCCODE) -ldocumentation$(PCCODE) -lanalyser$(PCCODE) -liowids$(PCCODE) -llistwids$(PCCODE) -lparamwids$(PCCODE) -lreaction$(PCCODE) -lrootprop$(PCCODE) -lwidgets$(PCCODE) -linstallation$(PCCODE) -lqt-util$(PCCODE) $(QT_LIBS) $(shell $(ROOTSYS)/bin/root-config --libs) -lGQt # -lcode$(PCCODE)
PACKDIR=$(KTOFPACK)/gui/typeCase
HDR =include/typecasemw.h \
	include/helpwidget.h \
	include/analysewidget.h \
	include/orderselectionwidget.h \
	include/addInputAlgoWidget.h \
	include/addOutputAlgoWidget.h \
	include/reactionrecognitionwidget.h \
	include/padparameter.h \
	include/viewselectionwidget.h \
#	include/.h \

SRC =src/main.cpp \
	src/typecasemw.cpp \
	src/helpwidget.cpp \
	src/analysewidget.cpp \
	src/orderselectionwidget.cpp \
	src/reactionrecognitionwidget.cpp \
	src/padparameter.cpp \
	src/viewselectionwidget.cpp \
#	src/.cpp \

OBJS =obj/main$(PCCODE).o \
	obj/typecasemw$(PCCODE).o \
	obj/helpwidget$(PCCODE).o \
	obj/analysewidget$(PCCODE).o \
	obj/orderselectionwidget$(PCCODE).o \
	obj/reactionrecognitionwidget$(PCCODE).o \
	obj/padparameter$(PCCODE).o \
	obj/viewselectionwidget$(PCCODE).o \
#	obj/$(PCCODE).o \

MOCSRC =obj/moc_typecasemw$(PCCODE).cpp \
	obj/moc_helpwidget$(PCCODE).cpp \
	obj/moc_analysewidget$(PCCODE).cpp \
	obj/moc_orderselectionwidget$(PCCODE).cpp \
	obj/moc_reactionrecognitionwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_viewselectionwidget$(QT_VERSION)$(PCCODE).cpp \
#	obj/moc_$(PCCODE).cpp \

MOCOBJ =obj/moc_typecasemw$(PCCODE).o \
	obj/moc_helpwidget$(PCCODE).o \
	obj/moc_analysewidget$(PCCODE).o \
	obj/moc_orderselectionwidget$(PCCODE).o \
	obj/moc_reactionrecognitionwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_viewselectionwidget$(QT_VERSION)$(PCCODE).o \
#	obj/moc_$(PCCODE).o \

###

all: $(TARGETA) $(TARGET)


$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) 
	rm -f $(KTOFPACK)/bin/$(TARGET)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ) #$(UIOBJS)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/bin $(INSTALLER) $(TARGET)

cleanLibs:
	rm -f $(TARGET) 
	rm -f $(KTOFPACK)/bin/$(TARGET) 

obj/%$(PCCODE).o: src/%.cpp src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
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
	rm -f $(MOCOBJ)
	rm -f $(MOCSRC)
	rm -f $(TARGET)
	rm -f obj/*$(QT_VERSION).h


INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/bin $(INSTALLER) $(TARGET) 

INSTALL_HEADER:
#	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


