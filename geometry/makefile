CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LIBNAME=geometry
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
LFLAGS = -shared -Wl,-soname,$(TARGET1)
INCS = -I. -Iinclude 
CFLAGS= -c -g -O -Werror -Wall -fPIC #-Wextra 
LIBS = 


HDR =include/definitions.h \
	include/Eparticles.h \
	include/geometry.h \
	include/string_utilities.h \

SRC =src/Eparticles.cpp \
	src/geomObj.cpp \
	src/lLine2D.cpp \
	src/lLine3D.cpp \
	src/LU_Decomposition.cpp \
	src/matrix3D.cpp \
	src/matrixNxM.cpp \
	src/Momentum4D.cpp \
	src/plane3D.cpp \
	src/Point2D.cpp \
	src/Point3D.cpp \
	src/Point4D.cpp \
	src/Point.cpp \
	src/projector.cpp \
	src/sLine2D.cpp \
	src/sLine3D.cpp \
	src/string_utilities.cpp \
	src/Vector2D.cpp \
	src/Vector3D.cpp \
	src/Vector4D.cpp \
	src/Vector.cpp \
	src/matrix_decomposition.c \
	src/matrix_inversion.c
OBJS =obj/Eparticles$(PCCODE).o \
	obj/geomObj$(PCCODE).o \
	obj/lLine2D$(PCCODE).o \
	obj/lLine3D$(PCCODE).o \
	obj/LU_Decomposition$(PCCODE).o \
	obj/matrix3D$(PCCODE).o \
	obj/matrixNxM$(PCCODE).o \
	obj/Momentum4D$(PCCODE).o \
	obj/plane3D$(PCCODE).o \
	obj/Point2D$(PCCODE).o \
	obj/Point3D$(PCCODE).o \
	obj/Point4D$(PCCODE).o \
	obj/Point$(PCCODE).o \
	obj/projector$(PCCODE).o \
	obj/sLine2D$(PCCODE).o \
	obj/sLine3D$(PCCODE).o \
	obj/string_utilities$(PCCODE).o \
	obj/Vector2D$(PCCODE).o \
	obj/Vector3D$(PCCODE).o \
	obj/Vector4D$(PCCODE).o \
	obj/Vector$(PCCODE).o \
	obj/matrix_decomposition$(PCCODE).o \
	obj/matrix_inversion$(PCCODE).o



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
	$(CCC) $(CFLAGS) -std=c++11 $(INCS) -c "$<" -o "$@"
obj/%$(PCCODE).o: src/%.c
	$(CCC) $(CFLAGS) -x c $(INCS) -c "$<" -o "$@"

PACKDIR=$(KTOFPACK)/$(LIBNAME)

INSTALL: $(TARGET)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

