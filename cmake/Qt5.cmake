

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )

find_package( Qt5Widgets )
find_package( Qt5Core ) 
find_package( Qt5Quick )
find_package( Qt5Multimedia )
find_package( Qt5SerialPort )

set(QT5_LIBS Qt5::Widgets Qt5::Core Qt5::Quick Qt5::Multimedia Qt5::SerialPort )
