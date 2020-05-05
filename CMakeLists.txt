cmake_minimum_required(VERSION 3.6)
project(mqtt-sn-sockets)

set(CMAKE_CXX_STANDARD 11)

#add_definitions(-DRH_PLATFORM=RH_PLATFORM_RP -D__RASPBERRY_PI_)
#add_definitions(-DRH_PLATFORM=RH_PLATFORM_RPI)
#add_definitions(-DOWN_ADDRESS=0x03)
#if (EXISTS ${project_name}/lib/arduino-linux-abstraction/CMakeLists.txt)
#	add_subdirectory(lib/arduino-linux-abstraction/)
#endif()
#target_include_directories(lib-rh96-socket PRIVATE RadioHeadRpi)
#target_include_directories(example-lib-rh96-socket PRIVATE RadioHeadRpi)

#include_directories(RadioHeadRpi/)
include_directories(lib/)
#include_directories(lib/arduino-linux-abstraction/src)

set(RH_RF95_RPI_SRC_FILES
        RadioHeadRpi/RH_RF95.h
        RadioHeadRpi/RH_RF95.cpp
        RadioHeadRpi/RHReliableDatagram.h
        RadioHeadRpi/RHReliableDatagram.cpp
	RadioHeadRpi/RHDatagram.h
	RadioHeadRpi/RHDatagram.cpp
	RadioHeadRpi/RHGenericDriver.h
	RadioHeadRpi/RHGenericDriver.cpp
	RadioHeadRpi/RHGenericSPI.h
	RadioHeadRpi/RHGenericSPI.cpp
	RadioHeadRpi/RHLinuxSPI.h
	RadioHeadRpi/RHLinuxSPI.cpp
#        RadioHeadRpi/RHutil/RasPi.cpp
)

set(EXAMPLE_LIB_SRC_FILES
	lib/SocketInterface.h
	lib/global_defines.h
	lib/mqttsn_messages.h
	lib/LoggerInterface.h
	lib/LoggerInterface.cpp
#	lib/LinuxLogger.h
#	lib/LinuxLogger.cpp
	lib/MqttSnMessageHandler.h
	lib/MqttSnMessageHandler.cpp
)




add_library(lib-rh96-socket ${RH_RF95_RPI_SRC_FILES} RF95Socket.h RF95Socket.cpp)
target_compile_definitions(lib-rh96-socket PRIVATE -DRH_PLATFORM=RH_PLATFORM_RPI)
target_include_directories(lib-rh96-socket PRIVATE RadioHeadRpi RadioHeadRpi/RHutil)
target_link_libraries(lib-rh96-socket wiringPi)

add_library(example-lib-rh96-socket ${EXAMPLE_LIB_SRC_FILES} RF95Socket.h RF95Socket.cpp )
target_compile_definitions(example-lib-rh96-socket PRIVATE -DRH_PLATFORM=RH_PLATFORM_RPI)
target_include_directories(example-lib-rh96-socket PRIVATE RadioHeadRpi RadioHeadRpi/RHutil)

target_link_libraries(example-lib-rh96-socket lib-rh96-socket wiringPi)


set(RH_NRF24_RPI_SRC_FILES
	RadioHead/RHutil/RasPi.h
	RadioHead/RHutil/RasPi.cpp
	RadioHead/RH_NRF24.cpp
	RadioHead/RHReliableDatagram.cpp
	RadioHead/RHDatagram.cpp
	RadioHead/RHHardwareSPI.cpp
	RadioHead/RHNRFSPIDriver.cpp
	RadioHead/RHGenericDriver.cpp
	RadioHead/RHGenericSPI.cpp
        )

add_library(lib-rh-nrf24-socket ${RH_NRF24_RPI_SRC_FILES} RF95Socket.h RF95Socket.cpp)
target_compile_definitions(lib-rh-nrf24-socket PRIVATE -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY)
target_include_directories(lib-rh-nrf24-socket PRIVATE RadioHead RadioHead/RHutil)
target_link_libraries(lib-rh-nrf24-socket bcm2835)

add_library(example-lib-rh-nrf24-socket ${EXAMPLE_LIB_SRC_FILES})
target_compile_definitions(example-lib-rh-nrf24-socket PRIVATE -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY)
target_include_directories(example-lib-rh-nrf24-socket PRIVATE RadioHead RadioHead/RHutil)
target_link_libraries(example-lib-rh-nrf24-socket lib-rh-nrf24-socket bcm2835)
 

set(RH_DATAGRAM_SRC_FILES
    RadioHeadRpi/RHReliableDatagram.h
    RadioHeadRpi/RHReliableDatagram.cpp
	RadioHeadRpi/RHDatagram.h
	RadioHeadRpi/RHDatagram.cpp
	RadioHeadRpi/RHGenericDriver.h
	RadioHeadRpi/RHGenericDriver.cpp
	RadioHeadRpi/RH_Serial.cpp
	RadioHeadRpi/RHCRC.cpp
    RadioHeadRpi/RHutil/RasPi.cpp
    RadioHeadRpi/RHutil/HardwareSerial.cpp
	RHReliableDatagramSocket.h
	RHReliableDatagramSocket.cpp
)

add_library(lib-rh-datagram-socket ${RH_DATAGRAM_SRC_FILES})
target_compile_definitions(lib-rh-datagram-socket PRIVATE -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY)
target_include_directories(lib-rh-datagram-socket PRIVATE RadioHeadRPi RadioHeadRPi/RHutil)
target_link_libraries(lib-rh-datagram-socket bcm2835)
