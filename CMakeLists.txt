### CMake

cmake_minimum_required(VERSION 3.16)
project(DemoServer 
	VERSION 1.0.0 
	DESCRIPTION "Real-time UDP game server"
	LANGUAGES CXX
)

### Settings

# cmake_policy(SET CMP0048 OLD) # suppress plog warning

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	include(CTest) # only in debug?
endif()

### Dependencies

include(FetchContent)
# set(EXTERNAL_INSTALL_LOCATION "${CMAKE_BINARY_DIR}/external")

FetchContent_Declare(
	pugixml
	GIT_REPOSITORY https://github.com/zeux/pugixml.git
	GIT_TAG	v1.10
	# CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)
FetchContent_Declare(
	plog
	GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
	GIT_TAG 1.1.5 # previous working: 1.1.0, new 1.1.5
    # CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)
FetchContent_MakeAvailable(pugixml plog)

### Other

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()


### Source groups (TODO) (can we make them automatically follow the folder structure?)

