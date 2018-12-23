LIBNAME=iowids
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
INSTALLER=../../installer.sh
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=moc
HDRMAKER=$(KTOFPACK)/gui/utils/header-3-4/header-3-4$(PCCODE)
LFLAGS = -shared -Wl,-soname,$(TARGET0)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I$(QT_INCLUDE)Gui -I$(QT_INCLUDE)Core -I$(ROOTINCLUDE) -I../widgets/include -I../parameter_widgets/include -I../root_properties/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -Wno-deprecated #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR =include/ioalgorithms.h \
	include/atracktreeinputwidget.h \
	include/qalgorithmdefinewidget.h \
	include/agenerateeventswidget.h \
	include/agenerateeventswithdecaywidget.h \
	include/agenerateoutputwidget.h \
	include/ahittreeinputwidget.h \
	include/ahittreeoutputwidget.h \
	include/amultipletreeinputwidget.h \
	include/amultipletreeoutputwidget.h \
	include/areadfromsimpletreewidget.h \
	include/areadfromtadewidget.h \
	include/areadreactionfromrootwidget.h \
	include/awritehistostorootfilewidget.h \

SRC =src/qalgorithmdefinewidget.cpp \
	src/atracktreeinputwidget.cpp \
	src/agenerateeventswidget.cpp \
	src/agenerateeventswithdecaywidget.cpp \
	src/agenerateoutputwidget.cpp \
	src/ahittreeinputwidget.cpp \
	src/ahittreeoutputwidget.cpp \
	src/amultipletreeinputwidget.cpp \
	src/amultipletreeoutputwidget.cpp \
	src/areadfromsimpletreewidget.cpp \
	src/areadfromtadewidget.cpp \
	src/areadreactionfromrootwidget.cpp \
	src/awritehistostorootfilewidget.cpp \

OBJS =obj/qalgorithmdefinewidget$(PCCODE).o \
	obj/atracktreeinputwidget$(PCCODE).o \
	obj/agenerateeventswidget$(PCCODE).o \
	obj/agenerateeventswithdecaywidget$(PCCODE).o \
	obj/agenerateoutputwidget$(PCCODE).o \
	obj/ahittreeinputwidget$(PCCODE).o \
	obj/ahittreeoutputwidget$(PCCODE).o \
	obj/amultipletreeinputwidget$(PCCODE).o \
	obj/amultipletreeoutputwidget$(PCCODE).o \
	obj/areadfromsimpletreewidget$(PCCODE).o \
	obj/areadfromtadewidget$(PCCODE).o \
	obj/areadreactionfromrootwidget$(PCCODE).o \
	obj/awritehistostorootfilewidget$(PCCODE).o \

MOCSRC =obj/moc_qalgorithmdefinewidget$(PCCODE).cpp \
	obj/moc_atracktreeinputwidget$(PCCODE).cpp \
	obj/moc_agenerateeventswidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_agenerateeventswithdecaywidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_agenerateoutputwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_ahittreeinputwidget$(PCCODE).cpp \
	obj/moc_ahittreeoutputwidget$(PCCODE).cpp \
	obj/moc_amultipletreeinputwidget$(PCCODE).cpp \
	obj/moc_amultipletreeoutputwidget$(PCCODE).cpp \
	obj/moc_areadfromsimpletreewidget$(PCCODE).cpp \
	obj/moc_areadfromtadewidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_areadreactionfromrootwidget$(QT_VERSION)$(PCCODE).cpp \
	obj/moc_awritehistostorootfilewidget$(PCCODE).cpp \

MOCOBJ =obj/moc_qalgorithmdefinewidget$(PCCODE).o \
	obj/moc_atracktreeinputwidget$(PCCODE).o \
	obj/moc_agenerateeventswidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_agenerateeventswithdecaywidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_agenerateoutputwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_ahittreeinputwidget$(PCCODE).o \
	obj/moc_ahittreeoutputwidget$(PCCODE).o \
	obj/moc_amultipletreeinputwidget$(PCCODE).o \
	obj/moc_amultipletreeoutputwidget$(PCCODE).o \
	obj/moc_areadfromsimpletreewidget$(PCCODE).o \
	obj/moc_areadfromtadewidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_areadreactionfromrootwidget$(QT_VERSION)$(PCCODE).o \
	obj/moc_awritehistostorootfilewidget$(PCCODE).o \

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

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/%$(PCCODE)_ui.o: src/%.ui.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

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

PACKDIR=$(KTOFPACK)/gui/io_algorithm_widgets
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib $(INSTALLER) $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)






