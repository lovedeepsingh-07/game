cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("net")

# cargo build
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CARGO_BUILD_FLAGS "build")
    set(CARGO_PROFILE "debug")
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set(CARGO_BUILD_FLAGS "build --release")
    set(CARGO_PROFILE "release")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CARGO_TARGET "x86_64-pc-windows-gnu")
else ()
	set(CARGO_TARGET "x86_64-unknown-linux-gnu")
endif ()

set(CARGO_CMD cargo ${CARGO_BUILD_FLAGS} --target ${CARGO_TARGET})
set(CARGO_PROFILE_DIR "${CARGO_TARGET}/${CARGO_PROFILE}")
set(CARGO_BUILD_DIR "${CMAKE_BINARY_DIR}/cargo")

# cargo src
set(CARGO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE CARGO_SRC_FILES
	"${CARGO_SRC_DIR}/*.rs"
)

# net source files
set(NET_CXX "${CMAKE_BINARY_DIR}/net.cpp")
set(NET_H "${CMAKE_BINARY_DIR}/net.h")
set(NET_LIB "${CARGO_BUILD_DIR}/${CARGO_PROFILE_DIR}/libnet.a")
set(CXX_H "${CMAKE_BINARY_DIR}/rust/cxx.h")

# cargo build command
add_custom_command(
	OUTPUT ${NET_CXX} ${NET_H} ${NET_LIB}
	COMMAND CARGO_TARGET_DIR=${CARGO_BUILD_DIR} RUSTFLAGS="${RUST_FLAGS}" ${CARGO_CMD}

	COMMAND cp ${CARGO_BUILD_DIR}/cxxbridge/net/src/lib.rs.cc ${NET_CXX}
	COMMAND cp ${CARGO_BUILD_DIR}/cxxbridge/net/src/lib.rs.h ${NET_H}
	COMMAND mkdir -p ${CMAKE_BINARY_DIR}/rust
	COMMAND cp -r ${CARGO_BUILD_DIR}/cxxbridge/rust/cxx.h ${CXX_H}

	DEPENDS ${CARGO_SRC_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# c++ net library
add_library(net STATIC ${NET_CXX})
target_link_libraries(net PUBLIC ${NET_LIB})
target_include_directories(net PUBLIC
	${CMAKE_BINARY_DIR}
)

# cross-compilation
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(net PUBLIC
        ws2_32
        userenv
        advapi32
        bcrypt
    )
endif()

# # cargo test
# add_test(NAME net_test
#     COMMAND cargo test
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# )
