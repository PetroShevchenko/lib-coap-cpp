.PHONY: build clean docker-run

#DOCKER_FILE := dockerfile.ubuntu
#DOCKER_FILE := dockerfile.debian
DOCKER_FILE := dockerfile.fedora

build:
	mkdir -p build
	cd build &&	cmake .. && make -j$(shell nproc)

docker-build: scripts/docker/$(DOCKER_FILE)
	cd scripts/docker && docker build -t lib-coap-cpp-image --rm -f $(DOCKER_FILE) ../..

docker-run:
	docker run --name=coap-container --rm -i -t lib-coap-cpp-image bash

clean:
	rm -rf build
