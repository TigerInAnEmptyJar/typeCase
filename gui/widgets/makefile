LIBNAME=widgets
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
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I/usr/include/QtGui -I$(ROOTINCLUDE)
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated 
LIBS = 
HDR =include/q3dinputdialog.h \
	include/q3Dinput.h \
	include/qintegerinputdialog.h \
	include/qnumberinputdialog.h \
	include/qstringinputdialog.h \
	include/qvectorchangedialog.h \
	include/qsinglewidgetdialog.h \
	include/qmywizard.h \
	include/stackwidget.h

SRC =src/q3dinputdialog.cpp \
	src/q3Dinput.cpp \
	src/qintegerinputdialog.cpp \
	src/qnumberinputdialog.cpp \
	src/qstringinputdialog.cpp \
	src/qvectorchangedialog.cpp \
	src/qsinglewidgetdialog.cpp \
	src/stackwidget.cpp \
	src/qmywizard.cpp 

OBJS =obj/q3dinputdialog$(PCCODE).o \
	obj/q3Dinput$(PCCODE).o \
	obj/qintegerinputdialog$(PCCODE).o \
	obj/qnumberinputdialog$(PCCODE).o \
	obj/qstringinputdialog$(PCCODE).o \
	obj/qvectorchangedialog$(PCCODE).o \
	obj/qsinglewidgetdialog$(PCCODE).o \
	obj/stackwidget$(PCCODE).o \
	obj/qmywizard$(PCCODE).o

MOCSRC =obj/moc_q3dinputdialog$(PCCODE).cpp \
	obj/moc_q3Dinput$(PCCODE).cpp \
	obj/moc_qintegerinputdialog$(PCCODE).cpp \
	obj/moc_qnumberinputdialog$(PCCODE).cpp \
	obj/moc_qstringinputdialog$(PCCODE).cpp \
	obj/moc_qvectorchangedialog_moc$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_qsinglewidgetdialog$(PCCODE).cpp \
	obj/moc_stackwidget$(PCCODE).cpp \
	obj/moc_qmywizard$(QT_VERSION)$(PCCODE).cpp 


MOCOBJ =obj/moc_q3dinputdialog$(PCCODE).o \
	obj/moc_q3Dinput$(PCCODE).o \
	obj/moc_qintegerinputdialog$(PCCODE).o \
	obj/moc_qnumberinputdialog$(PCCODE).o \
	obj/moc_qstringinputdialog$(PCCODE).o \
	obj/moc_qvectorchangedialog_moc$(QT_VERSION)$(PCCODE).o \
	obj/moc_qsinglewidgetdialog$(PCCODE).o \
	obj/moc_stackwidget$(PCCODE).o \
	obj/moc_qmywizard$(QT_VERSION)$(PCCODE).o 


###

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCOBJ) 
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGETA)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCOBJ) 
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ) #$(UIOBJS)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGET1)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1)

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
	rm -f $(TARGET)
	rm -f obj/*$(QT_VERSION).h

PACKDIR=$(KTOFPACK)/gui/widgets
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


