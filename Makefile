.PHONY: build

DOCKER_FILE := dockerfile.ubuntu
#DOCKER_FILE := dockerfile.debian
#DOCKER_FILE := dockerfile.fedora

build:
	mkdir -p build
	cd build &&	cmake .. && make -j$(shell nproc)

docker-build: scripts/docker/$(DOCKER_FILE)
	cd scripts/docker && docker build -t lib-coap-cpp-image --rm -f $(DOCKER_FILE) ../..

clean:
	rm -rf build
