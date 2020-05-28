file(GLOB_RECURSE SOURCES # TODO: We shouldn't use GLOB
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(DemoServer ${SOURCES})
target_link_libraries(DemoServer PRIVATE pugixml plog::plog) # pugixml::pugixml doesn't want to work!

set_target_properties(DemoServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
set_target_properties(DemoServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
target_compile_features(DemoServer PRIVATE cxx_std_17)

configure_file(
	${PROJECT_SOURCE_DIR}/settings.xml
    ${PROJECT_BINARY_DIR}/bin/settings.xml
	COPYONLY
)
