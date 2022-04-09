
find_package(Qt5 COMPONENTS Quick Core Network)
find_package(Qt5QuickCompiler)

tc_add_executable(typeCase-gui
  HEADERS
    parameter_models/AllData.hpp
    parameter_models/RunModel.hpp
    parameter_models/DetectorModel.hpp
    parameter_models/MaterialModel.hpp
    parameter_models/AlgorithmModel.hpp
    parameter_models/ShapeModel.hpp
    parameter_models/ValuesModel.hpp
  SOURCES
    main.cpp
    parameter_models/AllData.cpp
    parameter_models/RunModel.cpp
    parameter_models/DetectorModel.cpp
    parameter_models/MaterialModel.cpp
    parameter_models/AlgorithmModel.cpp
    parameter_models/ShapeModel.cpp
    parameter_models/ValuesModel.cpp
    qml/qml.qrc
  DEPENDS
    Qt5::Quick
    Qt5::Core
    Qt5::Network
    TypeCase::ParameterIO
    TypeCase::Factory
    TypeCase::Shape
)
set_target_properties(typeCase-gui PROPERTIES AUTORCC ON)

#qt5_use_modules(typeCase-gui Quick Core Network)

#target_link_libraries(typeCase-gui
#  libparameterIO
#  libfactory
#  libshape
#)
