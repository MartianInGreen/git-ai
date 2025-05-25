rm -rf CMakeCache.txt CMakeFiles/
cmake CMakeLists.txt
make
chmod +x gia
./gia "$@"