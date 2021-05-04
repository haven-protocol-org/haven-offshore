FROM ubuntu:20.04 as builder

ARG ZMQ_VERSION=v4.3.1
ARG CPPZMQ_VERSION=v4.3.0

RUN apt-get update \
 && DEBIAN_FRONTEND="noninteractive" apt-get --no-install-recommends --yes install \
    libcurl4-openssl-dev \
    build-essential \
    cmake pkg-config \
    libboost-all-dev \
    libssl-dev \ 
    libunbound-dev \
    libsodium-dev \
    liblzma-dev \
    libldns-dev \
    libexpat1-dev \
    doxygen \
    graphviz \ 
    libpgm-dev \ 
    libudev-dev \
    git \
    patch \
    ca-certificates \
    libtool \
&& apt-get autoclean

COPY . /tmp
WORKDIR /tmp

RUN git clone https://github.com/zeromq/libzmq.git -b ${ZMQ_VERSION} \
 && cd libzmq \
 && ./autogen.sh \
 && ./configure --enable-static --disable-shared \
 && make -j$(nproc) \
 && make install \
 && ldconfig

RUN git clone https://github.com/zeromq/cppzmq.git -b ${CPPZMQ_VERSION} \
 && cd cppzmq \
 && mv *.hpp /usr/local/include

RUN ./build-haven.sh release-static -j$(nproc)

FROM ubuntu:20.04 as runner

RUN useradd -ms /bin/bash haven

USER haven
WORKDIR /home/haven
COPY --from=builder /tmp/monero/build/release/bin/ /home/haven/





