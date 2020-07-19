cd ..
cmake -S. -B_builds/Debug -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Debug -DBoost_DEBUG=ON
cmake --build _builds/Debug
pause
