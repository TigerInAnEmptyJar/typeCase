LIBNAME=shape
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=$(TARGET).2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude  -I$(KTOFPACK)/include -I../geometry/include -I../parameter/include 
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra
LIBS =# -L$(KTOFPACK)/lib -lparameter$(PCCODE) -lgeometry$(PCCODE)  #-L/usr/lib64 -lThread -lQt3Support -lQtSql -pthread -lQtXml -L/usr/X11R6/lib64 -lQtNetwork -lQtGui -lpng -lSM -lICE -lXi -lXrender -lXrandr -lXfixes -lXcursor -lXinerama -lfreetype -lfontconfig -lXext -lX11 -lQtCore -lz -lm -lgthread-2.0 -lrt -lglib-2.0 -ldl 

HDR =	include/shapes.h \
	include/baseShape.h \
	include/planeShape.h \
	include/circle.h \
	include/quadrangle.h \
	include/sphericRectangle.h \
	include/volumeShape.h \
	include/wedge.h \
	include/spiral.h \
	include/fiber.h \
	include/ring.h \
	include/hexPrism.h \
	include/cylinder.h \
	include/strawTube.h \
	include/shape_utilities.h
SRC =src/baseShape.cpp \
	src/getShape.cpp \
	src/planeShape.cpp \
	src/quadrangle.cpp \
	src/rectangle.cpp \
	src/triangle.cpp \
	src/sphericRectangle.cpp \
	src/sphericTriangle.cpp \
	src/circle.cpp \
	src/hexagon.cpp \
	src/volumeShape.cpp \
	src/wedge.cpp \
	src/ring.cpp \
	src/spiral.cpp \
	src/fiber.cpp \
	src/hexPrism.cpp \
	src/cylinder.cpp \
	src/strawTube.cpp \
	src/shape_utilities.cpp
OBJS =obj/baseShape$(PCCODE).o \
	obj/getShape$(PCCODE).o \
	obj/planeShape$(PCCODE).o \
	obj/quadrangle$(PCCODE).o \
	obj/rectangle$(PCCODE).o \
	obj/triangle$(PCCODE).o \
	obj/sphericRectangle$(PCCODE).o \
	obj/sphericTriangle$(PCCODE).o \
	obj/circle$(PCCODE).o \
	obj/hexagon$(PCCODE).o \
	obj/volumeShape$(PCCODE).o \
	obj/wedge$(PCCODE).o \
	obj/ring$(PCCODE).o \
	obj/spiral$(PCCODE).o \
	obj/fiber$(PCCODE).o \
	obj/hexPrism$(PCCODE).o \
	obj/cylinder$(PCCODE).o \
	obj/strawTube$(PCCODE).o \
	obj/shape_utilities$(PCCODE).o


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

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) "$<" -o "$@"
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

PACKDIR=$(KTOFPACK)/shapes
INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)


clean:
	rm -f $(OBJS)
	rm -f $(TARGET)



