TARGET=cl-typeCase$(PCCODE)
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=moc
LFLAGS =
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(ROOTINCLUDE) -I$(QT_INCLUDE) -I$(BOOSTINCLUDE) -I$(BOOST_ROOT)
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra 
MYLIBS = -L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lbasic$(PCCODE) -lreaction$(PCCODE) -lalgorithm$(PCCODE) -lanalyser$(PCCODE) -lfit$(PCCODE) -lgeometry$(PCCODE) -leventpainter$(PCCODE) 
STATICLIBS =$(KTOFPACK)/lib/analyser$(PCCODE).a \
	$(KTOFPACK)/lib/eventpainter$(PCCODE).a \
	$(KTOFPACK)/lib/algorithm$(PCCODE).a \
	$(KTOFPACK)/lib/reaction$(PCCODE).a \
	$(KTOFPACK)/lib/basic$(PCCODE).a \
	$(KTOFPACK)/lib/container$(PCCODE).a \
	$(KTOFPACK)/lib/basic$(PCCODE).a \
	$(KTOFPACK)/lib/shape$(PCCODE).a \
	$(KTOFPACK)/lib/parameter$(PCCODE).a \
	$(KTOFPACK)/lib/fit$(PCCODE).a \
	$(KTOFPACK)/lib/geometry$(PCCODE).a

LIBS=$(MYLIBS) $(QT_LIBS) -L$(BOOSTLIB) -lboost_program_options $(shell root-config --libs) #-L$(QTDIR)/lib -lqt-mt
#LIBS=$(MYLIBS) -L/usr/lib64 -lQtCore 

HDR =include/loggingClass.h
SRC =src/analysis.cpp
OBJS =obj/analysis$(PCCODE).o
MOCSRC =obj/moc_loggingClass$(PCCODE).cpp
MOCOBJ =obj/moc_loggingClass$(PCCODE).o
MORELIB=$(KTOFPACK)/lib/libparameter$(PCCODE).so $(KTOFPACK)/lib/libshape$(PCCODE).so $(KTOFPACK)/lib/libcontainer$(PCCODE).so $(KTOFPACK)/lib/libbasic$(PCCODE).so $(KTOFPACK)/lib/libreaction$(PCCODE).so $(KTOFPACK)/lib/libalgorithm$(PCCODE).so $(KTOFPACK)/lib/libanalyser$(PCCODE).so $(KTOFPACK)/lib/libfit$(PCCODE).so $(KTOFPACK)/lib/libgeometry$(PCCODE).so $(KTOFPACK)/lib/libeventpainter$(PCCODE).so

all: $(TARGET)


$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ) $(MYLIBS)
	$(LINK) $(LFLAGS) $(OBJS) $(MOCOBJ) $(LIBS) -o $(TARGET) 

STATIC:  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ) $(STATICLIB)
	$(LINK) $(LFLAGS) $(OBJS) $(MOCOBJ) $(BOOSTLIB)/libboost_program_options.a $(STATICLIBS) $(QT_LIBS) $(shell root-config --libs) -o $(TARGET) 

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/moc_%$(PCCODE).o: obj/moc_%$(PCCODE).cpp include/%.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: include/%.h
	$(MOC) -o "$@" "$<"

"obj/moc_%$(PCCODE).o": "obj/moc_%$(PCCODE).cpp" "include/%.h"
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
"obj/moc_%$(PCCODE).cpp": "include/%.h"
	$(MOC) -o "$@" "$<"

clean:
	rm -f $(OBJS)
	rm -f $(MOCOBJ)
	rm -f $(MOCSRC)
	rm -f $(TARGET)

PACKDIR=$(PWD)/cl-typeCase
INSTALL: $(TARGET)
	-PACKDIR=$(PACKDIR) TODIR=$(PWD)/bin ../installer.sh $(TARGET)



