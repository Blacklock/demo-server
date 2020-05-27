cmake_minimum_required(VERSION 3.16)
project(Demoserver 
	VERSION 1.0 
	LANGUAGES CXX
	DESCRIPTION "Real-time UDP game server"
	)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

file(GLOB_RECURSE SOURCES 
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

ExternalProject_Add(pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml # ... /tree/v1.10
	GIT_TAG "v1.10"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

ExternalProject_Add(plog
    GIT_REPOSITORY https://github.com/SergiusTheBest/plog # ... /tree/v1.10
	GIT_TAG "1.1.0" # previous working: 1.1.0, new 1.1.5
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(pugixmlLib STATIC IMPORTED)
add_library(plogLib STATIC IMPORTED)
add_executable(DemoServer ${SOURCES})
target_link_libraries(DemoServer pugixmlLib plogLib)
add_dependencies(DemoServer pugixmlLib plogLib)

# set_property(TARGET DemoServer
#     PROPERTY VS_PACKAGE_REFERENCES "pugixml_1.10.0;Plog_1.1.0.1"
# )

# target_include_directories(${PROJECT_NAME}
# 	PRIVATE
# 		${PROJECT_SOURCE_DIR}/external
# 
