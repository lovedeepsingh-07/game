BINARY_NAME := "boron"
TOOLCHAIN_FILE := env_var('PWD') + "/.cmake/mingw-w64-toolchain.cmake"
export EM_CACHE := env_var('HOME') + '/.cache/emscripten'

default:
	just -l

setup:
	@nix run .#setup

build:
	@mkdir -p build/windows
	@cmake -G Ninja -S . -B build/windows -DCMAKE_TOOLCHAIN_FILE={{TOOLCHAIN_FILE}} -DCMAKE_SYSTEM_NAME=Windows
	@cmake --build ./build/windows
	@cp build/windows/compile_commands.json build/compile_commands.json

run component: build
	@build/windows/{{BINARY_NAME}}_{{component}}.exe

runw component: build
	@wine build/windows/{{BINARY_NAME}}_{{component}}.exe

gen:
	@mkdir -p common/include/generated net/src/generated
	@flatc -o ./common/include/generated/ ./data/packet.fbs --cpp --filename-suffix _
	@flatc -o ./net/src/generated ./data/packet.fbs --rust --filename-suffix _
