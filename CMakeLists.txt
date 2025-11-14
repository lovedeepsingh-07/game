cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("game")

set(EXTERNAL_DIR deps)
add_subdirectory(${EXTERNAL_DIR}/fmt)
add_subdirectory(${EXTERNAL_DIR}/flecs)
add_subdirectory(${EXTERNAL_DIR}/enet)
add_subdirectory(${EXTERNAL_DIR}/raylib)
set(IMGUI_SOURCES
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
)
set(RLIMGUI_SOURCES
	${EXTERNAL_DIR}/rlimgui/rlImGui.cpp
)

# ------ game_common ------
set(COMMON_INCLUDE_FILES
	common/include
)
set(COMMON_SRC_FILES
	common/src/common.cpp
)
add_library(${PROJECT_NAME}_common ${COMMON_SRC_FILES})
target_link_libraries(${PROJECT_NAME}_common PUBLIC
	enet
	fmt::fmt
)
target_include_directories(${PROJECT_NAME}_common PUBLIC
	${EXTERNAL_DIR}/enet/include
	${EXTERNAL_DIR}/flatbuffers/include
	${COMMON_INCLUDE_FILES}
)

# ------ game_server ------
set(SERVER_INCLUDE_FILES server/include)
set(SERVER_SRC_FILES
	server/src/main.cpp
)
add_executable(${PROJECT_NAME}_server ${SERVER_SRC_FILES})
target_link_libraries(${PROJECT_NAME}_server PRIVATE
	${PROJECT_NAME}_common
)
target_include_directories(${PROJECT_NAME}_server PRIVATE
	${SERVER_INCLUDE_FILES}
)

# ------ game_client ------
set(CLIENT_INCLUDE_FILES client/include)
set(CLIENT_SRC_FILES
	client/src/observers/_observers.cpp
	client/src/observers/game_quit.cpp
	client/src/observers/scene_manager.cpp
	client/src/scenes/game.cpp
	client/src/scenes/main_menu.cpp
	client/src/systems/render/_render.cpp
	client/src/systems/_systems.cpp
	client/src/systems/global_events.cpp
	client/src/user_interface/game.cpp
	client/src/user_interface/main_menu.cpp
	client/src/app_utils.cpp
	client/src/clay_raylib.cpp
	client/src/components.cpp
	client/src/main.cpp
	client/src/phases.cpp
	client/src/pipelines.cpp
)
add_executable(${PROJECT_NAME}_client ${CLIENT_SRC_FILES} ${IMGUI_SOURCES} ${RLIMGUI_SOURCES})
target_link_libraries(${PROJECT_NAME}_client PRIVATE
	${PROJECT_NAME}_common
	raylib
	flecs::flecs_static
)
target_include_directories(${PROJECT_NAME}_client PRIVATE
	${CLIENT_INCLUDE_FILES}
	${EXTERNAL_DIR}/clay
	${EXTERNAL_DIR}/imgui
	${EXTERNAL_DIR}/rlimgui
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
	target_compile_definitions(${PROJECT_NAME}_server PRIVATE PLATFORM_DESKTOP)
	target_compile_definitions(${PROJECT_NAME}_client PRIVATE PLATFORM_DESKTOP)
endif()
