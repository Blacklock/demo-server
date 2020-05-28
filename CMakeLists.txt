### CMake

cmake_minimum_required(VERSION 3.16)
project(DemoServer 
	VERSION 1.0 
	DESCRIPTION "Real-time UDP game server"
	LANGUAGES CXX
)

### Settings

cmake_policy(SET CMP0048 OLD) # suppress plog warning

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

### Dependencies

include(FetchContent)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

FetchContent_Declare(
	pugixml
	GIT_REPOSITORY https://github.com/zeux/pugixml.git
	GIT_TAG	v1.10
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)
FetchContent_Declare(
	plog
	GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
	GIT_TAG 1.1.5 # previous working: 1.1.0, new 1.1.5
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)
FetchContent_MakeAvailable(pugixml plog)

### Link

file(GLOB_RECURSE SOURCES # TODO: We shouldn't use GLOB
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(DemoServer ${SOURCES})
target_link_libraries(DemoServer PRIVATE pugixml plog::plog) # pugixml::pugixml doesn't want to work!

target_compile_features(DemoServer PRIVATE cxx_std_17)

### Source groups (TODO) (can we make them automatically follow the folder structure?)



### vvv
### old
### vvv

#include_directories(${EXTERNAL_INSTALL_LOCATION}/include) # TODO: don't use global
#link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

#add_library(pugixml STATIC IMPORTED)
#add_library(plog STATIC IMPORTED)
#add_executable(DemoServer ${SOURCES})
#target_link_libraries(DemoServer PUBLIC pugixml plog)
# add_dependencies(DemoServer pugixmlProj plogProj) # redundant

# TODO: source_group here

# include(ExternalProject)

# ExternalProject_Add(pugixmlProj
    # GIT_REPOSITORY https://github.com/zeux/pugixml.git # ... /tree/v1.10
	# GIT_TAG "v1.10"
    # CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
# )
# ExternalProject_Add(plogProj
    # GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git # ... /tree/v1.10
	# GIT_TAG "1.1.5" # previous working: 1.1.0, new 1.1.5
    # CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
# )
