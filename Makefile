.PHONY: build lib examples docker-run install clean

TARGET := POSIX
#TARGET := RASPBERRY-PI-3
#TARGET := STM32H747I-DISCO

#DOCKER_FILE := dockerfile.ubuntu
#DOCKER_FILE := dockerfile.debian
DOCKER_FILE := dockerfile.fedora

build: lib examples

lib:
	mkdir -p build/lib
ifeq ($(TARGET),POSIX)
	cd build/lib &&	cmake ../.. && make -j$(shell nproc || echo 2)
else
	cd build/lib &&	make -f ../../scripts/gmake/$(TARGET)/library.mk -j$(shell nproc || echo 2)
	make -f ../../scripts/gmake/$(TARGET)/tests.mk -j$(shell nproc || echo 2)
endif

examples:
	mkdir -p build/examples
ifeq ($(TARGET),POSIX)
	cd build/examples && cmake ../../examples/POSIX && make -j$(shell nproc || echo 2)
else
	cd build/examples && make -f ../../examples/$(TARGET)/Makefile -j$(shell nproc || echo 2)
endif

docker-build: scripts/docker/$(DOCKER_FILE)
	cd scripts/docker && docker build -t lib-coap-cpp-image --rm -f $(DOCKER_FILE) ../..

docker-run:
	docker run --name=coap-container --rm -i -t lib-coap-cpp-image bash

install:
ifeq ($(TARGET),POSIX)
	mkdir -p /usr/local/include/coapcpp
	cp -R include/* /usr/local/include/coapcpp
	cp build/lib/libcoapcpp.a /usr/local/lib
endif

clean:
	rm -rf build