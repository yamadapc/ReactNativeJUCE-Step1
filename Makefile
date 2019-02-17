debug: FORCE
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
	cd build && cmake --build .
	./build/tests

xcode: FORCE
	mkdir -p xcode
	cd xcode && cmake .. -GXcode

FORCE:
