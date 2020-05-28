cd ..
cmake -S. -B_builds/Debug -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Debug
cmake --build _builds/Debug
pause
