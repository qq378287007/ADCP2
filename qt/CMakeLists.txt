set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${DIR_NAME})

find_package(Qt5 REQUIRED COMPONENTS Widgets)
link_libraries(Qt5::Widgets)

file(GLOB ALL_FILE "*.h" "*.cpp")
add_executable(${DIR_NAME} WIN32 ${ALL_FILE})

target_include_directories(${DIR_NAME} PRIVATE D:/soft/MATLAB/R2021b/extern/include)
target_link_directories(${DIR_NAME} PRIVATE D:/soft/MATLAB/R2021b/extern/lib/win64/mingw64)
target_link_libraries(${DIR_NAME} libmat libmx) #libmex libeng
