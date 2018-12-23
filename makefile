all:
	make --directory=geometry -f makefile 
	make --directory=fit -f makefile 
	make --directory=eventpainter -f makefile 
	make --directory=parameter -f makefile 
	make --directory=shapes -f makefile 
	make --directory=container -f makefile 
	make --directory=basic -f makefile 
	make --directory=reaction -f makefile 
	make --directory=algorithms -f makefile 
	make --directory=analyser -f makefile
	make --directory=code -f makefile 
	make --directory=qt-utils -f makefile 

clean:
	make --directory=geometry -f makefile clean
	make --directory=fit -f makefile clean
	make --directory=eventpainter -f makefile clean
	make --directory=parameter -f makefile clean
	make --directory=shapes -f makefile clean
	make --directory=container -f makefile clean
	make --directory=basic -f makefile clean
	make --directory=reaction -f makefile clean
	make --directory=algorithms -f makefile clean
	make --directory=analyser -f makefile clean
	make --directory=code -f makefile clean
	make --directory=qt-utils -f makefile clean

PARAMETER:
	make --directory=parameter -f makefile 
SHAPES:
	make --directory=shapes -f makefile 
CONTAINER:
	make --directory=container -f makefile 
BASIC:
	make --directory=basic -f makefile 
REACTION:
	make --directory=reaction -f makefile 
ALGORITHMS:
	make --directory=algorithms -f makefile 
ANALYSER:
	make --directory=analyser -f makefile
GEOMETRY:
	make --directory=geometry -f makefile
FIT:
	make --directory=fit -f makefile
PAINTER:
	make --directory=eventpainter -f makefile
CODE:
	make --directory=code -f makefile 
DOCUMENTATION:
	make --directory=documentation -f makefile 
QT-UTILS:
	make --directory=qt-utils -f makefile 

INSTALL:
	make --directory=geometry -f makefile INSTALL
	make --directory=fit -f makefile INSTALL
	make --directory=parameter -f makefile INSTALL
	make --directory=shapes -f makefile INSTALL
	make --directory=container -f makefile INSTALL
	make --directory=basic -f makefile INSTALL
	make --directory=reaction -f makefile INSTALL
	make --directory=algorithms -f makefile INSTALL
	make --directory=analyser -f makefile INSTALL
	make --directory=eventpainter -f makefile INSTALL
	make --directory=code -f makefile INSTALL
	make --directory=qt-utils -f makefile INSTALL


HEADER:
	make --directory=geometry -f makefile INSTALL_HEADER
	make --directory=fit -f makefile INSTALL_HEADER
	make --directory=parameter -f makefile INSTALL_HEADER
	make --directory=shapes -f makefile  INSTALL_HEADER
	make --directory=container -f makefile  INSTALL_HEADER
	make --directory=basic -f makefile  INSTALL_HEADER
	make --directory=reaction -f makefile  INSTALL_HEADER
	make --directory=analyser -f makefile INSTALL_HEADER
	make --directory=eventpainter -f makefile INSTALL_HEADER
	make --directory=code -f makefile INSTALL_HEADER
	make --directory=qt-utils -f makefile INSTALL_HEADER


FIRST:
	make --directory=geometry -f makefile
	make --directory=geometry -f makefile INSTALL_HEADER
	make --directory=geometry -f makefile INSTALL
	make --directory=fit -f makefile
	make --directory=fit -f makefile INSTALL_HEADER
	make --directory=fit -f makefile INSTALL
	make --directory=parameter -f makefile
	make --directory=parameter -f makefile INSTALL_HEADER
	make --directory=parameter -f makefile INSTALL
	make --directory=shapes -f makefile
	make --directory=shapes -f makefile INSTALL_HEADER
	make --directory=shapes -f makefile INSTALL
	make --directory=container -f makefile INSTALL_HEADER
	make --directory=basic -f makefile INSTALL_HEADER
	make --directory=reaction -f makefile INSTALL_HEADER
	make --directory=container -f makefile
	make --directory=container -f makefile INSTALL
	make --directory=basic -f makefile
	make --directory=basic -f makefile INSTALL
	make --directory=reaction -f makefile
	make --directory=reaction -f makefile INSTALL
	make --directory=algorithms -f makefile
	make --directory=algorithms -f makefile INSTALL_HEADER
	make --directory=algorithms -f makefile INSTALL
	make --directory=analyser -f makefile
	make --directory=analyser -f makefile INSTALL_HEADER
	make --directory=analyser -f makefile INSTALL
	make --directory=eventpainter -f makefile
	make --directory=eventpainter -f makefile INSTALL_HEADER
	make --directory=eventpainter -f makefile INSTALL
	make --directory=code -f makefile 
	make --directory=code -f makefile INSTALL_HEADER
	make --directory=code -f makefile INSTALL
	make --directory=qt-utils -f makefile 
	make --directory=qt-utils -f makefile INSTALL_HEADER
	make --directory=qt-utils -f makefile INSTALL
	make --directory=gui/utils -f makefile
	make --directory=documentation -f makefile INSTALL_HEADER
	make --directory=documentation -f makefile LIBRARY


COMMANDLINE:
	make --directory=cl-typeCase -f makefile STATIC
