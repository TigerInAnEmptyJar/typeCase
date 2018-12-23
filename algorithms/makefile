LIBNAME=algorithm
TARGETA=$(LIBNAME)$(PCCODE).a
TARGET=lib$(LIBNAME)$(PCCODE).so
TARGET0=$(TARGET)
TARGET1=lib$(LIBNAME)$(PCCODE).so.2
CCC = $(CDIR)/g++
LINK = $(CDIR)/g++
LFLAGS = -shared -Wl,-soname,$(TARGET1)
MOC=moc
INCSSTRAW = -I/users/data12/pion/ehrhardt/straw/Straws/include -I/users/scr0_pc85/pion/ehrhardt/minuit-1.7.9_zi42/include/ -I/users/scr0_pc85/pion/ehrhardt/minuit-1.7.9_zi42/include/Minuit 
INCS = -I. -Iinclude -I$(KTOFPACK)/include -I$(ROOTINCLUDE) $(INCSSTRAW) -I$(QT_INCLUDE)
CFLAGS= -c -g -O -Werror -Wall -fPIC # -Wextra
LIBS = #-L$(KTOFPACK)/lib -lgeometry$(PCCODE) -lparameter$(PCCODE) -lshape$(PCCODE) -lcontainer$(PCCODE) -lbasic$(PCCODE) -lreaction$(PCCODE)
HDR =include/AapplyLRC.h \
	include/AAssignHitsToTracks.h \
	include/ABetheBloch.h \
	include/ACalculateTrackProperties.h \
	include/ACalibrationAlgorithm.h \
	include/ACalibrationGeneration.h \
	include/ACalibration.h \
	include/ACMTransformation.h \
	include/ACompareToGIN.h \
	include/AConversion.h \
	include/AFindDecaysInTracks.h \
	include/AGenerateEvents.h \
	include/AGenerateEventsWithDecay.h \
	include/AGenerateOutput.h \
	include/AHistoAlgorithm.h \
	include/AHitClusterSearch.h \
	include/AHitTreeInput.h \
	include/AHitTreeOutput.h \
	include/ATrackTreeInput.h \
	include/ATrackTreeOutput.h \
	include/AHodo3Pixel.h \
	include/AKinFit.h \
	include/AKinkSearch.h \
	include/ALineTrackSearch.h \
	include/AMomentumFromGeometry.h \
	include/AMultipleTreeInput.h \
	include/AMultipleTreeOutput.h \
	include/APixelTreeWriteOut.h \
	include/APromptHistoTracker.h \
	include/APulsHeightCorrection.h \
	include/AReadFromRoot.h \
	include/AReadFromSimpleTree.h \
	include/AReadReactionFromRoot.h \
	include/ATDCcalibration.h \
	include/ATeufelCorrection.h \
	include/ATofPixCorrection.h \
	include/ATofStrawTrackFinder.h \
	include/AVertexFit.h \
	include/AVirtualMachine.h \
	include/AVLineTrackSearch.h \
	include/APixelTracking.h \
	include/AWriteDataToRootFile.h \
	include/AWriteHistogramsToRootFile.h \
	include/BarrelPixel.h \
	include/Calibrations.h \
	include/GetTheShape.h \
	include/HodoPixel.h \
	include/lineFitAlgorithm.h \
	include/MicroCluster.h \
	include/MicroPixel.h \
	include/readFromTade.h \
	include/Ringpixel.h \
	include/TüTrackSearch.h \
	include/TüVSearch.h
SRC =src/AapplyLRC.cpp \
	src/AAssignHitsToTracks.cpp \
	src/ABetheBloch.cpp \
	src/ACalculateTrackProperties.cpp \
	src/ACalibrationAlgorithm.cpp \
	src/ACalibration.cpp \
	src/ACalibrationGeneration.cpp \
	src/ACMTransformation.cpp \
	src/ACompareToGIN.cpp \
	src/AConversion.cpp \
	src/AFindDecaysInTracks.cpp \
	src/AGenerateEvents.cpp \
	src/AGenerateEventsWithDecay.cpp \
	src/AGenerateOutput.cpp \
	src/AHistoAlgorithm.cpp \
	src/AHitClusterSearch.cpp \
	src/AHitTreeInput.cpp \
	src/AHitTreeOutput.cpp \
	src/ATrackTreeInput.cpp \
	src/ATrackTreeOutput.cpp \
	src/AHodo3Pixel.cpp \
	src/AKinFit.cpp \
	src/AKinkSearch.cpp \
	src/ALineTrackSearch.cpp \
	src/AMCCompare.cpp \
	src/AMomentumFromGeometry.cpp \
	src/AMultipleTreeInput.cpp \
	src/AMultipleTreeOutput.cpp \
	src/APixelTreeWriteOut.cpp \
	src/APromptHistoTracker.cpp \
	src/APulsHeightCorrection.cpp \
	src/AReadFromRoot.cpp \
	src/AReadFromSimpleTree.cpp \
	src/AReadReactionFromRoot.cpp \
	src/ATDCcalibration.cpp \
	src/ATeufelCorrection.cpp \
	src/ATofPixCorrection.cpp \
	src/ATofStrawTrackFinder.cpp \
	src/AVertexFit.cpp \
	src/AVirtualMachine.cpp \
	src/AVLineTrackSearch.cpp \
	src/APixelTracking.cpp \
	src/AWriteDataToRootFile.cpp \
	src/AWriteHistogramsToRootFile.cpp \
	src/BarrelPixel.cpp \
	src/Calibrations0.cpp \
	src/Calibrations11.cpp \
	src/Calibrations12.cpp \
	src/Calibrations1.cpp \
	src/Calibrations2.cpp \
	src/Calibrations3.cpp \
	src/Calibrations4.cpp \
	src/Calibrations5.cpp \
	src/Calibrations6.cpp \
	src/Calibrations7.cpp \
	src/Calibrations8.cpp \
	src/GetTheShape.cpp \
	src/HodoPixel.cpp \
	src/lineFitAlgorithm.cpp \
	src/MicroCluster.cpp \
	src/MicroPixel.cpp \
	src/readFromTade.cpp \
	src/Ringpixel.cpp \
	src/TüTrackSearch.cpp \
	src/TüVSearch.cpp 
OBJS =	obj/AConversion$(PCCODE).o \
	obj/AGenerateEvents$(PCCODE).o \
	obj/AGenerateEventsWithDecay$(PCCODE).o \
	obj/AVirtualMachine$(PCCODE).o \
	obj/AWriteDataToRootFile$(PCCODE).o \
	obj/AWriteHistogramsToRootFile$(PCCODE).o \
	obj/AGenerateOutput$(PCCODE).o \
	obj/AReadFromSimpleTree$(PCCODE).o \
	obj/AReadReactionFromRoot$(PCCODE).o \
	obj/AReadFromRoot$(PCCODE).o \
	obj/readFromTade$(PCCODE).o \
	obj/AHitTreeInput$(PCCODE).o \
	obj/AHitTreeOutput$(PCCODE).o \
	obj/ATrackTreeInput$(PCCODE).o \
	obj/ATrackTreeOutput$(PCCODE).o \
	obj/AMultipleTreeInput$(PCCODE).o \
	obj/AMultipleTreeOutput$(PCCODE).o \
	obj/APixelTreeWriteOut$(PCCODE).o \
	obj/ATeufelCorrection$(PCCODE).o \
	obj/ACalibration$(PCCODE).o \
	obj/GetTheShape$(PCCODE).o \
	obj/AHitClusterSearch$(PCCODE).o \
	obj/BarrelPixel$(PCCODE).o \
	obj/AHodo3Pixel$(PCCODE).o \
	obj/HodoPixel$(PCCODE).o \
	obj/MicroCluster$(PCCODE).o \
	obj/MicroPixel$(PCCODE).o \
	obj/Ringpixel$(PCCODE).o \
	obj/TüTrackSearch$(PCCODE).o \
	obj/TüVSearch$(PCCODE).o \
	obj/ALineTrackSearch$(PCCODE).o \
	obj/AVLineTrackSearch$(PCCODE).o \
	obj/APixelTracking$(PCCODE).o \
	obj/AKinkSearch$(PCCODE).o \
	obj/AAssignHitsToTracks$(PCCODE).o \
	obj/AFindDecaysInTracks$(PCCODE).o \
	obj/AapplyLRC$(PCCODE).o \
	obj/ACalculateTrackProperties$(PCCODE).o \
	obj/ABetheBloch$(PCCODE).o \
	obj/ATDCcalibration$(PCCODE).o \
	obj/ACalibrationAlgorithm$(PCCODE).o \
	obj/ACalibrationGeneration$(PCCODE).o \
	obj/ACMTransformation$(PCCODE).o \
	obj/ACompareToGIN$(PCCODE).o \
	obj/AHistoAlgorithm$(PCCODE).o \
	obj/APromptHistoTracker$(PCCODE).o \
	obj/APulsHeightCorrection$(PCCODE).o \
	obj/ATofPixCorrection$(PCCODE).o \
	obj/AVertexFit$(PCCODE).o \
	obj/Calibrations0$(PCCODE).o \
	obj/Calibrations11$(PCCODE).o \
	obj/Calibrations12$(PCCODE).o \
	obj/Calibrations1$(PCCODE).o \
	obj/Calibrations2$(PCCODE).o \
	obj/Calibrations3$(PCCODE).o \
	obj/Calibrations4$(PCCODE).o \
	obj/Calibrations5$(PCCODE).o \
	obj/Calibrations6$(PCCODE).o \
	obj/Calibrations7$(PCCODE).o \
	obj/Calibrations8$(PCCODE).o \
	obj/AKinFit$(PCCODE).o \
	obj/lineFitAlgorithm$(PCCODE).o# \
#	obj/ATofStrawTrackFinder$(PCCODE).o

MOCSRC =obj/moc_AConversion$(PCCODE).cpp \
	obj/moc_AWriteDataToRootFile$(PCCODE).cpp \
	obj/moc_AWriteHistogramsToRootFile$(PCCODE).cpp \
	obj/moc_AGenerateOutput$(PCCODE).cpp \
	obj/moc_AReadFromSimpleTree$(PCCODE).cpp \
	obj/moc_AReadReactionFromRoot$(PCCODE).cpp \
	obj/moc_AReadFromRoot$(PCCODE).cpp \
	obj/moc_readFromTade$(PCCODE).cpp \
	obj/moc_AHitTreeInput$(PCCODE).cpp \
	obj/moc_ATrackTreeInput$(PCCODE).cpp \
	obj/moc_AMultipleTreeInput$(PCCODE).cpp \
	obj/moc_AMultipleTreeOutput$(PCCODE).cpp \
	obj/moc_ATeufelCorrection$(PCCODE).cpp \
	obj/moc_ACalibration$(PCCODE).cpp \
	obj/moc_BarrelPixel$(PCCODE).cpp \
	obj/moc_AapplyLRC$(PCCODE).cpp \
	obj/moc_ATDCcalibration$(PCCODE).cpp \
	obj/moc_ACalibrationGeneration$(PCCODE).cpp \
	obj/moc_AKinFit$(PCCODE).cpp \
	obj/moc_APulsHeightCorrection$(PCCODE).cpp \
	obj/moc_ATofPixCorrection$(PCCODE).cpp \
	obj/moc_AVertexFit$(PCCODE).cpp \
	obj/moc_Calibrations$(PCCODE).cpp \
	obj/moc_lineFitAlgorithm$(PCCODE).cpp

MOCOBJ = obj/moc_AConversion$(PCCODE).o \
	obj/moc_AWriteDataToRootFile$(PCCODE).o \
	obj/moc_AWriteHistogramsToRootFile$(PCCODE).o \
	obj/moc_AGenerateOutput$(PCCODE).o \
	obj/moc_AReadFromSimpleTree$(PCCODE).o \
	obj/moc_AReadReactionFromRoot$(PCCODE).o \
	obj/moc_AReadFromRoot$(PCCODE).o \
	obj/moc_readFromTade$(PCCODE).o \
	obj/moc_AHitTreeInput$(PCCODE).o \
	obj/moc_ATrackTreeInput$(PCCODE).o \
	obj/moc_AMultipleTreeInput$(PCCODE).o \
	obj/moc_AMultipleTreeOutput$(PCCODE).o \
	obj/moc_ATeufelCorrection$(PCCODE).o \
	obj/moc_ACalibration$(PCCODE).o \
	obj/moc_BarrelPixel$(PCCODE).o \
	obj/moc_AapplyLRC$(PCCODE).o \
	obj/moc_ATDCcalibration$(PCCODE).o \
	obj/moc_ACalibrationGeneration$(PCCODE).o \
	obj/moc_AKinFit$(PCCODE).o \
	obj/moc_APulsHeightCorrection$(PCCODE).o \
	obj/moc_ATofPixCorrection$(PCCODE).o \
	obj/moc_AVertexFit$(PCCODE).o \
	obj/moc_Calibrations$(PCCODE).o \
	obj/moc_lineFitAlgorithm$(PCCODE).o

all: $(TARGET) $(TARGETA) 

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

clean:
	rm -f $(OBJS)
	rm -f $(MOCOBJ)
	rm -f $(MOCSRC)
	rm -f $(TARGET)

PACKDIR=$(KTOFPACK)/algorithms
INSTALL: $(TARGET) $(TARGETA)
	-ln -s $(TARGET) $(TARGET1)
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/lib ../installer.sh $(TARGET) $(TARGETA) $(TARGET1)

INSTALL_HEADER:
	-PACKDIR=$(PACKDIR) TODIR=$(KTOFPACK)/include ../installer.sh $(HDR)

obj/ATofStrawTrackFinder$(PCCODE).o: src/ATofStrawTrackFinder.cpp
	$(CCC) $(CFLAGS) $(INCS) -o obj/ATofStrawTrackFinder$(PCCODE).o src/ATofStrawTrackFinder.cpp

obj/%$(PCCODE).o: src/%.cpp
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"

obj/moc_%$(PCCODE).o: obj/moc_%$(PCCODE).cpp include/%.h
	$(CCC) $(CFLAGS) $(INCS) -o "$@" "$<"
obj/moc_%$(PCCODE).cpp: include/%.h
	$(MOC) -o "$@" "$<"



