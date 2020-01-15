#!/bin/sh

set -evu

#git submodule init
#git submodule update

mkdir -p build
cd build && cmake .. && make

# docker images
# docker image rm <image_name>
# docker ps -a
# docker rm <container_name>
# docker create --name coap-container -p 80:80 lib-coap-cpp-image
# docker run lib-coap-cpp-image coap-container
# docker exec -i -t coap-container bash

#docker || sudo apt install docker docker.io -y
#sudo dockerd
#sudo docker build -t lib-coap-cpp .
#sudo docker container create --name coap-container
#sudo docker run -v /var/www:/var/www -p 80:80 -t coap-container
#sudo docker exec -i -t coap-container bash
