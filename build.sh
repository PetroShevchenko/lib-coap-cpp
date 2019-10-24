#!/bin/sh
docker || apt-get install docker docker.io -y
sudo dockerd
sudo docker build -t lib-coap-cpp - < Dockerfile
sudo docker run -v /var/www:/var/www -p 80:80 -t lib-coap-cpp --name coap-container
sudo docker exec -i -t coap-container bash
