cmake_minimum_required(VERSION 3.10)

project(nanopb-cobs-example)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/nanopb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cobs-c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated/src)

# Set the common source files
set(common_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/src/simple.pb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/src/simple.pb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_encode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_decode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_encode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/pb_decode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cobs-c/cobs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cobs-c/cobs.c
)

# Set the executable source files
set(executable1_sources
    src/encode.c
    ${common_sources}
)

set(executable2_sources
    src/decode.c
    ${common_sources}
)

# Create a custom target to generate the protobufs
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated/src/simple.pb.h
           ${CMAKE_CURRENT_SOURCE_DIR}/generated/src/simple.pb.c
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/generator/nanopb_generator.py
    ARGS -t --c-style -D ${CMAKE_CURRENT_SOURCE_DIR}/generated/src -I${CMAKE_CURRENT_SOURCE_DIR}/src -t ${CMAKE_CURRENT_SOURCE_DIR}/src/simple.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/simple.proto
)

# Create a custom target to depend on the generated protobufs
add_custom_target(generate_protobufs
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generated/src/simple.pb.h
            ${CMAKE_CURRENT_SOURCE_DIR}/generated/src/simple.pb.c
)

# Create the executables
add_executable(${PROJECT_NAME}_encode ${executable1_sources})
add_executable(${PROJECT_NAME}_decode ${executable2_sources})

# Link the executables against the common object file
target_link_libraries(${PROJECT_NAME}_encode ${PROJECT_NAME}_common)
target_link_libraries(${PROJECT_NAME}_decode ${PROJECT_NAME}_common)

# Add a dependency on the generate_protobufs target
add_dependencies(${PROJECT_NAME}_encode generate_protobufs)
add_dependencies(${PROJECT_NAME}_decode generate_protobufs)

# Create a common object file
add_library(${PROJECT_NAME}_common ${common_sources})

# Set the output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
