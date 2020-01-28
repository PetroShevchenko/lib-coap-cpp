.PHONY: build clean docker-run

#DOCKER_FILE := dockerfile.ubuntu
#DOCKER_FILE := dockerfile.debian
DOCKER_FILE := dockerfile.fedora

build:
	mkdir -p build/lib
	cd build/lib &&	cmake ../.. && make -j$(shell nproc)

examples-build:
	mkdir -p build/examples
	cd build/examples && cmake ../../examples/POSIX && make -j$(shell nproc)

docker-build: scripts/docker/$(DOCKER_FILE)
	cd scripts/docker && docker build -t lib-coap-cpp-image --rm -f $(DOCKER_FILE) ../..

docker-run:
	docker run --name=coap-container --rm -i -t lib-coap-cpp-image bash

clean:
	rm -rf build