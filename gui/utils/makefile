INSTALLER=../../installer.sh
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
MOC=$(QT_DIR)/bin/moc
LFLAGS = 
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(QT_INCLUDE) -I../../geometry/include
CFLAGS= -c -g -O -Werror -Wall -fPIC -std=c++11 -Wno-deprecated #-Wextra
LIBS = -L$(KTOFPACK)/lib -lparameter$(PCCODE) -lgeometry$(PCCODE) $(QT_LIBS) $(shell root-config --libs) #-lGQt # -lcode$(PCCODE)
PACKDIR=$(KTOFPACK)/gui/utils

###


header-3-4: header-3-4/header-3-4$(PCCODE).o
	$(LINK) $(LDFLAGS) $(LIBS) -o header-3-4/header-3-4$(PCCODE) header-3-4/header-3-4$(PCCODE).o
header-3-4/header-3-4$(PCCODE).o: header-3-4/header-3-4.cpp
	$(CCC) $(CFLAGS) $(INCS) -o header-3-4/header-3-4$(PCCODE).o header-3-4/header-3-4.cpp

#all: $(TARGETA) $(TARGET)


#$(TARGET):  $(OBJS) $(SUBLIBS) $(OBJCOMP) $(MOCSRC) $(MOCOBJ)
#	rm -f $(TARGET) 
#	rm -f $(KTOFPACK)/bin/$(TARGET)
#	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(MOCOBJ) #$(UIOBJS)
#	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/bin $(INSTALLER) $(TARGET)

#cleanLibs:
#	rm -f $(TARGET) 
#	rm -f $(KTOFPACK)/bin/$(TARGET) 

#obj/%$(PCCODE).o: src/%.cpp src/%.ui.h
#	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
#obj/%$(PCCODE).o: src/%.cpp 
#	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
#obj/ui_%$(PCCODE).o: src/%.ui.h
#	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
#"obj/moc_%.o": "obj/moc_%.cpp"
#	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
#"obj/moc_%$(PCCODE).cpp": "include/%.h"
#	$(MOC) -o "$@" "$<"
#obj/moc_%.o: obj/moc_%.cpp
#	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
#obj/moc_%$(PCCODE).cpp: include/%.h
#	$(MOC) -o "$@" "$<"

#clean:
#	rm -f $(OBJS)
#	rm -f $(MOCOBJ)
#	rm -f $(MOCSRC)
#	rm -f $(TARGET)


#INSTALL: $(TARGET)
#	-ln -s $(TARGET) $(TARGET1)
#	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/bin $(INSTALLER) $(TARGET) 

#INSTALL_HEADER:
#	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include $(INSTALLER) $(HDR)


