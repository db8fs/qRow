cmake_minimum_required(VERSION 2.8)

project(qRow)

cmake_policy(SET CMP0020 NEW)

include( cmake/Boost.cmake )
include( cmake/Qt5.cmake )
include( cmake/OpenCV.cmake )
include( cmake/Samples.cmake)
include( cmake/Database.cmake )
include( cmake/Doxygen.cmake )

generateTest_Logfiles_QVector( "data/Logfiles/Python/*.log" )

configure_file( "${CMAKE_SOURCE_DIR}/templates/SampleData.h.in" "${CMAKE_BINARY_DIR}/data/SampleData.h" )

include_directories(    "${CMAKE_BINARY_DIR}/data"
                        "${CMAKE_SOURCE_DIR}/src/Backend"
                        "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/"
                        "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Logfiles"
                        "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model"
                        )

set( TEMPLATE_HEADERS   "${CMAKE_SOURCE_DIR}/templates/SampleData.h.in" )
set( GENERATED_HEADERS  "${CMAKE_BINARY_DIR}/data/SampleData.h" )

qt5_add_resources( UI_RESOURCES src/Frontend/ui.qrc )

set( HEADER_FILES   "${TEMPLATE_HEADERS}"
                    "${GENERATED_HEADERS}"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Logfiles/LogReader.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/WorkoutController.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/LogEntry.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/SessionModel.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/WorkoutModel.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/WorkoutSession.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/StrokeDetector.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/Stroke.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/StrokeState.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/Phase.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/RowingData.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/SerialAdapter.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/WaterrowerController.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/WaterrowerProtocol.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/WaterrowerSerial.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/IProtocolHandler.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/System/Logging.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/System/Parsing.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/FaceDetector.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoFilterOpenCV.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoTools.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/VoiceWidget.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/Engine.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/Waveform.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/WavFile.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/Utils.h"
                    "${CMAKE_SOURCE_DIR}/src/Frontend/CLI/ArgumentParser.h"
                    )
set( SRC_FILES      "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/WorkoutController.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Logfiles/LogReader.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/SessionModel.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/WorkoutModel.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/WorkoutSession.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/StrokeDetector.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/Stroke.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/StrokeState.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/Phase.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/RowingData.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/SerialAdapter.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/WaterrowerController.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/WaterrowerProtocol.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Serial/WaterrowerSerial.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/System/Logging.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/FaceDetector.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoFilterOpenCV.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoTools.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/VoiceWidget.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/Engine.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/Waveform.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/WavFile.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Voice/Utils.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Frontend/CLI/ArgumentParser.cpp"
                    "${CMAKE_SOURCE_DIR}/src/main.cpp"
                    )


add_custom_command( OUTPUT "output.map" COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_BINARY_DIR}/output.map")

#set(
#    CMAKE_EXE_LINKER_FLAGS
#    "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -Map=output.map"
#)

add_executable( ${PROJECT_NAME}
                ${UI_RESOURCES}
                ${HEADER_FILES}
                ${SRC_FILES} 
                ${CMAKE_CURRENT_BINARY_DIR}/output.map
)

set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/output.map")

target_link_libraries(  qRow ${QT5_LIBS}
                        ${3rdParty_Boost_LIBS}
                        ${3rdParty_OpenCV_LIBS} )
