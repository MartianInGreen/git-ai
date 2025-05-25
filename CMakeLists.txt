cmake_minimum_required(VERSION 3.14)
project(gia)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch openai-cpp
FetchContent_Declare(
  openai_cpp
  GIT_REPOSITORY https://github.com/olrea/openai-cpp.git
  GIT_TAG main
)
FetchContent_MakeAvailable(openai_cpp)

# Find libcurl from system
find_package(CURL REQUIRED)

# Find nlohmann/json from system
find_package(nlohmann_json REQUIRED)

add_executable(gia src/main.cpp)

# Link everything together
target_link_libraries(gia
  PRIVATE
    openaicpp                       # from FetchContent
    CURL::libcurl                   # from system
    nlohmann_json::nlohmann_json   # from system
)
