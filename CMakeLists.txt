cmake_minimum_required(VERSION 3.14)
project(gia)
set(CMAKE_CXX_STANDARD 20)

# Set debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# Find libcurl from system
find_package(CURL REQUIRED)

# Find nlohmann/json from system
find_package(nlohmann_json REQUIRED)

# Add openai-cpp as an interface library (header-only)
add_library(openaicpp INTERFACE)
target_include_directories(openaicpp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/openai-cpp/include
)

add_executable(gia 
    src/main.cpp
    src/lib/help.cpp
    src/lib/ai.cpp
)

# Add include directory
target_include_directories(gia PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link everything together
target_link_libraries(gia
  PRIVATE
    openaicpp                       # header-only library
    CURL::libcurl                   # from system
    nlohmann_json::nlohmann_json   # from system
)
