LIBNAME=basic
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=$(QT_DIR)/bin/moc
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I../parameter/include -I../geometry/include -I../container/include -I../shapes/include -I../reaction/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -std=c++11 #-Wextra
LIBS = #-L$(KTOFPACK)/lib -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lgeometry$(PCCODE)
HDR =include/algorithm.h \
	include/eLossAlgorithm.h \
	include/fitAlgorithm.h \
	include/logger.h \
	include/AReactionRecognition.h \
	include/RbaseReaction.h

SRC =src/algorithm.cpp \
	src/eLossAlgorithm.cpp \
	src/fitAlgorithm.cpp \
	src/logger.cpp \
	src/getReaction.cpp \
	src/AReactionRecognition.cpp \
	src/RbaseReaction.cpp

OBJS =obj/algorithm$(PCCODE).o \
	obj/eLossAlgorithm$(PCCODE).o \
	obj/fitAlgorithm$(PCCODE).o \
	obj/logger$(PCCODE).o \
	obj/AReactionRecognition$(PCCODE).o \
	obj/RbaseReaction$(PCCODE).o \
	obj/getReaction$(PCCODE).o 

MOCSRC =obj/moc_logger$(PCCODE).cpp \
	obj/moc_fitAlgorithm$(PCCODE).cpp

MOCOBJ =obj/moc_logger$(PCCODE).o \
	obj/moc_fitAlgorithm$(PCCODE).o
###

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGETA)

$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGET1)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1)

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
"obj/moc_%.o": "obj/moc_%.cpp"
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
"obj/moc_%$(PCCODE).cpp": "include/%.h"
	$(MOC) -o "$@" "$<"
obj/moc_%.o: obj/moc_%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: include/%.h
	$(MOC) -o "$@" "$<"

clean:
	rm -f $(OBJS)
	rm -f $(MOCOBJ)
	rm -f $(MOCSRC)
	rm -f $(TARGET)

PACKDIR=$(KTOFPACK)/basic
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)


