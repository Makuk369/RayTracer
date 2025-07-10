cmake_minimum_required(VERSION 3.30)
project(main)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Tell CMake where to find SDL3
# Na pridanie dalsej lib staci pridat dalsi string oddeleny medzerou
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3" "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3_image" "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3_ttf")

# Find SDL3 package
# Na pridanie dalsej lib staci pridat: find_package(NazovLib REQUIRED CONFIG)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-shared)
find_package(SDL3_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL3_ttf-shared)

# Add .cpp files
file(GLOB_RECURSE BUILD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create game executable
add_executable(${PROJECT_NAME} ${BUILD_FILES})

# Add header files
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Link against SDL3
# # Na pridanie dalsej lib staci pridat: NazovLib::NazovLib
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

# Copy .dll files to build
# Na pridanie dalsej lib staci pridat dalsi string oddeleny medzerou do LIB_DLLS
set(LIB_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3/bin/SDL3.dll" "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3_image/bin/SDL3_image.dll" "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3_ttf/bin/SDL3_ttf.dll")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIB_DLLS}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
