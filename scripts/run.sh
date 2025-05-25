rm -rf CMakeCache.txt CMakeFiles/ gia
cmake CMakeLists.txt
make
chmod +x gia
./gia "$@"