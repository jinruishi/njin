
# Packages
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Stb REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)
find_package(glslang CONFIG REQUIRED)
add_library(glslang INTERFACE)
target_link_libraries(glslang INTERFACE glslang::glslang
                      glslang::glslang-default-resource-limits
                      glslang::SPIRV
                      glslang::SPVRemapper)
find_package(RapidJSON CONFIG REQUIRED)

# C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Resource directories
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shader")
add_definitions(-DSHADER_DIR="${SHADER_DIR}")

set(TEXTURE_DIR "${CMAKE_SOURCE_DIR}/texture")
add_definitions(-DTEXTURE_DIR="${TEXTURE_DIR}")
message(STATUS "texture directory: ${TEXTURE_DIR}")

# Subdirectories
add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(util)
add_subdirectory(vulkan)

# main executable
set(SOURCES util/src/file.cpp)
add_executable(njin main.cpp ${SOURCES})
target_include_directories(njin PUBLIC util/include ${Stb_INCLUDE_DIR})
target_link_libraries(njin PRIVATE
                      glm::glm
                      vulkan
                      Vulkan::Vulkan
                      SDL3::SDL3
                      glslang
                      math
                      core)

# dev executable
add_executable(dev dev.cpp)
target_link_libraries(dev PRIVATE util core)

# test executable
set(TEST_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test")
add_executable(test test.cpp)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain
                      math_test
                      util_test
                      core_test)
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})


# copy schemas
set(SCHEMA_DIR ${CMAKE_SOURCE_DIR}/schema)
add_custom_target(copy_schemas
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SCHEMA_DIR} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/schema"
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SCHEMA_DIR} "${TEST_OUTPUT_DIR}/schema"
                  COMMENT "Copying ${SCHEMA_DIR} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_dependencies(test copy_schemas)


# copy test inputs
set(TEST_INPUT_DIR ${CMAKE_SOURCE_DIR}/test_input)
add_custom_target(copy_test_inputs
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEST_INPUT_DIR} ${TEST_OUTPUT_DIR}
                  COMMENT "Copying ${TEST_INPUT_DIR} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_dependencies(test copy_test_inputs)

