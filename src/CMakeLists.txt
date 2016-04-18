file(GLOB_RECURSE SOURCES "*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tcpstats ${SOURCES})
target_link_libraries(tcpstats pcap jsoncpp)
