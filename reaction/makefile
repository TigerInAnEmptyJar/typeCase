LIBNAME=reaction
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I../geometry/include -I../container/include -I../shapes/include -I../basic/include -I../fit/include
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra
LIBS = 
HDR =include/Rpp_dpi+.h \
	include/Rppelastic.h \
	include/Rpp_pkl_pkppi.h \
	include/RthreeParticleDecay.h \
	include/reactions.h \
	include/RtwoParticleDecay.h 

SRC =src/Rpp_dpi+.cpp \
	src/Rppelastic.cpp \
	src/Rpp_pkl_pkppi.cpp \
	src/RthreeParticleDecay.cpp \
	src/RtwoParticleDecay.cpp

OBJS =obj/RtwoParticleDecay$(PCCODE).o \
	obj/Rppelastic$(PCCODE).o \
	obj/Rpp_dpi+$(PCCODE).o \
	obj/Rpp_pkl_pkppi$(PCCODE).o \
	obj/RthreeParticleDecay$(PCCODE).o

all: $(TARGETA) $(TARGET)

$(TARGETA): $(OBJS) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGETA)
	rm -f $(KTOFPACK)/lib/$(TARGETA)
	ar cru $(TARGETA) $(OBJS) $(MOCOBJ)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGETA)

$(TARGET):  $(OBJS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
	rm -f $(TARGET) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGET1)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGET1)

cleanLibs:
	rm -f $(TARGET) $(TARGETA) $(TARGET1)
	rm -f $(KTOFPACK)/lib/$(TARGET) $(KTOFPACK)/lib/$(TARGETA) $(KTOFPACK)/lib/$(TARGET1)

obj/%$(PCCODE).o:src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

PACKDIR=$(KTOFPACK)/reaction
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)

