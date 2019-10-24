FROM ubuntu:16.04
COPY ./src /usr/local/lib-coap-cpp/src
RUN apt-get update -y
RUN	apt-get install git cmake g++ make -y
RUN git clone https://github.com/wolfSSL/wolfssl.git
RUN git clone https://github.com/google/googletest.git
ENTRYPOINT ["/bin/bash"]
