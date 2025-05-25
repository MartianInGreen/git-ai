rm -rf CMakeCache.txt CMakeFiles/ gia
cmake CMakeLists.txt -DCMAKE_BUILD_TYPE=Debug
make
chmod +x gia
./gia "$@"