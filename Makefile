.PHONY: build examples docker-run install clean

#DOCKER_FILE := dockerfile.ubuntu
#DOCKER_FILE := dockerfile.debian
DOCKER_FILE := dockerfile.fedora

build:
	mkdir -p build/lib
	cd build/lib &&	cmake ../.. && make -j$(shell nproc)

examples:
	mkdir -p build/examples
	cd build/examples && cmake ../../examples/POSIX && make -j$(shell nproc)

docker-build: scripts/docker/$(DOCKER_FILE)
	cd scripts/docker && docker build -t lib-coap-cpp-image --rm -f $(DOCKER_FILE) ../..

docker-run:
	docker run --name=coap-container --rm -i -t lib-coap-cpp-image bash

install:
	mkdir -p /usr/local/include/coapcpp
	cp -R include/* /usr/local/include/coapcpp
	cp build/lib/libcoapcpp.a /usr/local/lib

clean:
	rm -rf build