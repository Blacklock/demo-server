file(GLOB_RECURSE SRC_FILES # TODO: We shouldn't use GLOB
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(DemoServer ${SRC_FILES})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(DemoServer pugixml plog::plog ${Boost_LIBRARIES}) # not PRIVATE anymore

set_target_properties(DemoServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin/src/")
set_target_properties(DemoServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin/src/")
target_compile_features(DemoServer PRIVATE cxx_std_17)

configure_file(
	"${PROJECT_SOURCE_DIR}/settings.xml"
    "${PROJECT_BINARY_DIR}/bin/src/settings.xml"
)

if(MSVC)
  target_compile_options(DemoServer PRIVATE /W4)
else()
  target_compile_options(DemoServer PRIVATE -Wall -Wextra -pedantic)
endif()
