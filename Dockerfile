FROM ubuntu:18.04
RUN apt-get update -y
RUN	apt-get install git cmake g++ make -y
RUN cd /opt
RUN git clone --recursive https://github.com/PetroShevchenko/lib-coap-cpp.git
RUN git clone https://github.com/google/googletest.git
#RUN mkdir /opt/lib-coap-cpp/build && cd /opt/lib-coap-cpp/build
#RUN cmake ..
#RUN make
ENTRYPOINT ["/bin/bash"]
