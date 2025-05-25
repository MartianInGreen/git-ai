rm -rf CMakeCache.txt CMakeFiles/
cmake CMakeLists.txt -DCMAKE_BUILD_TYPE=Debug
make
chmod +x gia
./gia "$@"