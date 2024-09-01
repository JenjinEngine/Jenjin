.PHONY: clean build run build-release

NINJA := $(shell which ninja 2>/dev/null || echo "")

run:
	make build
	./build/pong/pong

build-release:
	if [ "$(NINJA)" = "" ]; then \
		[ -d build ] || cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" --parallel 32; \
	else \
		[ -d build ] || cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release; \
	fi

	[ -d build ] || exit 1
	cmake --build ./build -j 32

build:
	if [ "$(NINJA)" = "" ]; then \
		[ -d build ] || cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" --parallel 32; \
	else \
		[ -d build ] || cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug; \
	fi

	[ -d build ] || exit 1
	cmake --build ./build -j 32

clean:
	rm -rf build

