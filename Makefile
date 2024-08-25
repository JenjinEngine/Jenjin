.PHONY: clean run

run:
	[ -d build ] || cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	[ -d build ] || exit 1

	cmake --build ./build && ./build/pong/pong

clean:
	rm build -rf
