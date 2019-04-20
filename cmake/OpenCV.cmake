
function( checkOpenCV )

    set( 3rdParty_OpenCV_DIR "${CMAKE_SOURCE_DIR}/3rdParty/OpenCV/Win32/x86" CACHE STRING "OpenCV SDK root")
    set( OpenCV_DIR "${3rdParty_OpenCV_DIR}/cmake" )
    list( APPEND CMAKE_FIND_ROOT_PATH "${3rdParty_OpenCV_DIR}" )

    find_package(OpenCV REQUIRED core videoio objdetect)

    set( 3rdParty_OpenCV_INCLUDES "${OpenCV_INCLUDE_DIRS}" CACHE STRING "OpenCV Include paths to use")
    set( 3rdParty_OpenCV_LIBS "${OpenCV_LIBS}" CACHE STRING "OpenCV Libs to use")

endfunction()

checkOpenCV()

include_directories( "${3rdParty_OpenCV_INCLUDES}" )
