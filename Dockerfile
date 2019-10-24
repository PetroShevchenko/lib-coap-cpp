FROM ubuntu:16.04
ADD . /app
RUN apt-get update
RUN	apt-get install git cmake g++ make -y
RUN git clone https://github.com/wolfSSL/wolfssl.git
RUN git clone https://github.com/google/googletest.git
ENTRYPOINT ["/bin/bash"]
CMD ["--help"]