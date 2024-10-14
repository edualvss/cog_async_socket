cmake_minimum_required(VERSION 3.16)

project(cog_challenge_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cog_challenge_server main.cpp
    connectionhandler.h connectionhandler.cpp
    server.h server.cpp
    setup.h setup.cpp
    filewriter.h filewriter.cpp)

include(GNUInstallDirs)
install(TARGETS cog_challenge_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Boost 1.86.0 REQUIRED system)
add_library(boost_asio INTERFACE)

target_link_libraries(cog_challenge_server PRIVATE Boost::headers Boost::system)
