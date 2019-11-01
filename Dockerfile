FROM ubuntu:16.04
COPY ./src /opt/lib-coap-cpp/src
COPY ./examples /opt/lib-coap-cpp/examples
COPY ./include /opt/lib-coap-cpp/include
COPY ./tests /opt/lib-coap-cpp/tests
COPY ./scripts /opt/lib-coap-cpp/scripts
RUN apt-get update -y
RUN	apt-get install git cmake g++ make -y
RUN cd /opt
RUN git clone https://github.com/wolfSSL/wolfssl.git
RUN git clone https://github.com/google/googletest.git
RUN mkdir lib-coap-cpp/build && cd lib-coap-cpp/build
RUN cmake ..
RUN make
ENTRYPOINT ["/bin/bash"]
