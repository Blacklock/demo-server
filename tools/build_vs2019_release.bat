cd ..
cmake -S. -B_builds/Release -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build _builds/Release
pause
