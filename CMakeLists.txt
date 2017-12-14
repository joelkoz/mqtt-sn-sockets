cmake_minimum_required(VERSION 3.7)
project(mqtt-sn-sockets)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DRH_PLATFORM=RH_PLATFORM_RP -D__RASPBERRY_PI_)
add_definitions(-DOWN_ADDRESS=0x03)
if (EXISTS ${project_name}/lib/arduino-linux-abstraction/CMakeLists.txt)
	add_subdirectory(lib/arduino-linux-abstraction/)
endif()

include_directories(RadioHeadRpi/)
include_directories(lib/)
include_directories(lib/arduino-linux-abstraction/src)

set(RADIOHEAD_SRC_FILES
        RadioHeadRpi/RH_RF95.h
        RadioHeadRpi/RH_RF95.cpp
        RadioHeadRpi/RHReliableDatagram.h
        RadioHeadRpi/RHReliableDatagram.cpp
        RadioHeadRpi/RHutil/)

set(EXAMPLE_LIB_SRC_FILES
	lib/SocketInterface.h
	lib/global_defines.h
	lib/mqttsn_messages.h
	lib/LoggerInterface.h
	lib/LoggerInterface.cpp
	lib/LinuxLogger.h
	lib/LinuxLogger.cpp
	lib/MqttSnMessageHandler.h
	lib/MqttSnMessageHandler.cpp)




add_library(lib-rh96-socket ${RADIOHEAD_SRC_FILES} RF95Socket.h RF95Socket.cpp)
target_link_libraries(lib-rh96-socket arduino-linux-abstraction wiringPi)

add_library(example-lib-rh96-socket ${EXAMPLE_LIB_SRC_FILES})
target_link_libraries(example-lib-rh96-socket lib-rh96-socket wiringPi)

