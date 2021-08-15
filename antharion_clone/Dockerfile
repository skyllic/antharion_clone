FROM ubuntu:20.04
RUN apt-get -y update;
RUN apt-get -y install -y cmake g++ libgtest-dev libtbb-dev;
COPY . /simulator
WORKDIR /simulator
RUN mkdir build
RUN cd build
RUN cmake .
RUN make
LABEL Name=theorysimulator