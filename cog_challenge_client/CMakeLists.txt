cmake_minimum_required(VERSION 3.16)

project(cog_challenge_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cog_challenge_client main.cpp)

include(GNUInstallDirs)
install(TARGETS cog_challenge_client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Boost 1.86.0 REQUIRED system)
add_library(boost_asio INTERFACE)

target_link_libraries(cog_challenge_client PRIVATE Boost::headers Boost::system)
