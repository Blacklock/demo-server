cmake_minimum_required(VERSION 3.16.2)
#cmake_policy(SET CMP0048 OLD) # plog warnings?
project(
	DemoServer 
	VERSION 1.0.0 
	DESCRIPTION "Real-time UDP game server"
	LANGUAGES CXX
)

### Settings

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_EXTENSIONS OFF)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	include(CTest) # only in debug?
endif()

### Dependencies

include(FetchContent)
FetchContent_Declare(
	pugixml
	GIT_REPOSITORY https://github.com/zeux/pugixml.git
	GIT_TAG	v1.10
)
FetchContent_Declare(
	plog
	GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
	GIT_TAG 1.1.5 # previous working: 1.1.0, new 1.1.5
)
FetchContent_MakeAvailable(pugixml plog)

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_ARCHITECTURE -x64)
#set(Boost_LIB_PREFIX lib)
find_package(
	Boost
	1.71.0 EXACT
	REQUIRED
	COMPONENTS thread
)

### Other

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
