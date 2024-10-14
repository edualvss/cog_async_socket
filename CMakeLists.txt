cmake_minimum_required(VERSION 3.16)
project(async_socket)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server cog_server/main.cpp
    cog_server/connectionhandler.h cog_server/connectionhandler.cpp
    cog_server/server.h cog_server/server.cpp
    cog_server/setup.h cog_server/setup.cpp
    cog_server/filewriter.h cog_server/filewriter.cpp)

include(GNUInstallDirs)
install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_executable(client cog_client/main.cpp)
install(TARGETS client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Boost 1.86.0 REQUIRED system)
add_library(boost INTERFACE)

target_link_libraries(server PRIVATE Boost::headers Boost::system)
target_link_libraries(client PRIVATE Boost::headers Boost::system)
