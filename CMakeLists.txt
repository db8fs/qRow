cmake_minimum_required(VERSION 2.8)

project(qRow)

cmake_policy(SET CMP0020 NEW)

include( cmake/Boost.cmake )
include( cmake/Qt5.cmake )
include( cmake/OpenCV.cmake )
include( cmake/Samples.cmake)
include( cmake/Database.cmake )

generateTest_Logfiles_QVector( "data/Logfiles/Python/*.log" )

configure_file( "${CMAKE_SOURCE_DIR}/templates/SampleData.h.in" "${CMAKE_BINARY_DIR}/data/SampleData.h" )

include_directories(    "${CMAKE_BINARY_DIR}/data"
						"${CMAKE_SOURCE_DIR}/src/System"
                        "${CMAKE_SOURCE_DIR}/src/Backend"
                        "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/"
                        "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Logfiles"
                        "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model"
						"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower"
                        )

set( TEMPLATE_HEADERS   "${CMAKE_SOURCE_DIR}/templates/SampleData.h.in" )
set( GENERATED_HEADERS  "${CMAKE_BINARY_DIR}/data/SampleData.h" )

qt5_add_resources( UI_RESOURCES src/Frontend/ui.qrc )

set( HEADER_FILES   "${TEMPLATE_HEADERS}"
                    "${GENERATED_HEADERS}"
					"${CMAKE_SOURCE_DIR}/src/System/SystemParsing.h"
					"${CMAKE_SOURCE_DIR}/src/System/SystemLogging.h"
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
                    "${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Waterrower.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Model/AbstractWaterrowerModel.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Model/WaterrowerModelV4.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4Adapter.h"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4/Logging/Logging.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4/Parser/Parser.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4/Parser/IProtocolHandler.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Transceiver/Transceiver.h"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Transceiver/Serial/SerialAdapter.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/FaceDetector.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoFilterOpenCV.h"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoTools.h"
					"${CMAKE_SOURCE_DIR}/src/Frontend/CLI/ArgumentParser.h"
                    )
set( SRC_FILES      "${CMAKE_SOURCE_DIR}/src/System/SystemLogging.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Analysis/WorkoutController.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Logfiles/LogReader.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/SessionModel.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/WorkoutModel.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/WorkoutSession.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/StrokeDetector.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/Stroke.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/StrokeState.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/Phase.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Analysis/Model/RowingData.cpp"
                    "${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Waterrower.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Model/AbstractWaterrowerModel.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Model/WaterrowerModelV4.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4Adapter.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4/Logging/Logging.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Protocol/ProtocolV4/Parser/Parser.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Transceiver/Transceiver.cpp"
					"${CMAKE_SOURCE_DIR}/src/Backend/Waterrower/Transceiver/Serial/SerialAdapter.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/FaceDetector.cpp"
#					"${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoFilterOpenCV.cpp"
#                    "${CMAKE_SOURCE_DIR}/src/Backend/Video/VideoTools.cpp"
					"${CMAKE_SOURCE_DIR}/src/Frontend/CLI/ArgumentParser.cpp"
                    "${CMAKE_SOURCE_DIR}/src/main.cpp" )

add_executable( ${PROJECT_NAME}
                ${UI_RESOURCES}
                ${HEADER_FILES}
                ${SRC_FILES} )

target_link_libraries(  qRow ${QT5_LIBS}
                        ${3rdParty_Boost_LIBS}
#if 0
                        ${3rdParty_OpenCV_LIBS} 
#endif
						)
