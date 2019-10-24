#!/bin/sh
docker || apt-get install docker docker.io -y
sudo dockerd
sudo docker build -t lib-coap-cpp .
sudo docker container create --name coap-container
sudo docker run -v /var/www:/var/www -p 80:80 -t coap-container
sudo docker exec -i -t coap-container bash
