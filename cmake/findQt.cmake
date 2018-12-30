find_path(QT_CONFIG_PATH Qt5Config.cmake HINTS ${QT_DIR}/lib/cmake/Qt5/)
find_package(Qt5 COMPONENTS Core REQUIRED HINTS ${QT_CONFIG_PATH})
