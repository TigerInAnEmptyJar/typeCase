all:
	make --directory=utils -f makefile
	make --directory=widgets -f makefile
	make --directory=root_properties -f makefile
	make --directory=parameter_widgets -f makefile
	make --directory=list_widgets -f makefile
	make --directory=io_algorithm_widgets -f makefile
	make --directory=install_wizards -f makefile
	make --directory=typeCase -f makefile
clean:
	make --directory=widgets -f makefile clean
	make --directory=root_properties -f makefile clean
	make --directory=parameter_widgets -f makefile clean
	make --directory=list_widgets -f makefile clean
	make --directory=io_algorithm_widgets -f makefile clean
	make --directory=install_wizards -f makefile clean
	make --directory=typeCase -f makefile clean

INSTALL:
	make --directory=utils -f makefile
	make --directory=widgets -f makefile INSTALL
	make --directory=root_properties -f makefile INSTALL
	make --directory=parameter_widgets -f makefile INSTALL
	make --directory=list_widgets -f makefile INSTALL
	make --directory=io_algorithm_widgets -f makefile INSTALL
	make --directory=install_wizards -f makefile INSTALL
	make --directory=typeCase -f makefile INSTALL

INSTALL_HEADER:
	make --directory=utils -f makefile
	make --directory=widgets -f makefile INSTALL_HEADER
	make --directory=root_properties -f makefile INSTALL_HEADER
	make --directory=parameter_widgets -f makefile INSTALL_HEADER
	make --directory=list_widgets -f makefile INSTALL_HEADER
	make --directory=install_wizards -f makefile INSTALL_HEADER
	make --directory=io_algorithm_widgets -f makefile INSTALL_HEADER

