FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

enable_testing() # probably unneeded

set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # ?

file(GLOB_RECURSE TEST_FILES # TODO: We shouldn't use GLOB
	"${PROJECT_SOURCE_DIR}/tests/*.h"
	"${PROJECT_SOURCE_DIR}/tests/*.cpp"
)

add_executable(Tests ${TEST_FILES})
target_link_libraries(Tests PRIVATE gmock gmock_main pugixml plog::plog) # also: gtest gtest_main

set_target_properties(Tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin/tests/")
set_target_properties(Tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin/tests/")
target_compile_features(Tests PRIVATE cxx_std_17)

# vs gtest adapter works when opening the built project sln
# add_test(
#     NAME GTests
#     COMMAND DemoServerTests --config $<CONFIGURATION> --exe $<TARGET_FILE:Tests>
# )

