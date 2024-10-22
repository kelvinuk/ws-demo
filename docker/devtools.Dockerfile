FROM ubuntu:24.04 AS base

WORKDIR /app

RUN apt update \
  && apt install -y \
        bash \
        curl \
        git \
        python3

#RUN apt-get install -y vim git wget sudo curl

#USER node
#RUN npm install --global yarn

# Dev build image
##############################################################################
FROM base AS build
USER root

# Install build tools
RUN apt install -y \
    build-essential \
    git \
    gcc \
    g++ \
    libboost-all-dev \
    libssl-dev \
    cmake \
    ninja-build

# Copy sources and configuration
COPY --chown=demo:demo .. .
COPY ../CMakeLists.txt .

# Build application
RUN mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . --target all -j 10


# Create final production Image
##############################################################################
FROM base AS production
USER root

ARG USER_ID=2000
ARG GROUP_ID=2000

WORKDIR /app

# Create runtime user
RUN addgroup demo --gid ${GROUP_ID} \
  && useradd -ms /bin/bash demo -g demo \
  && mkdir -p /usr/local/bin \
  && mkdir -p /app/server
    
# Copy runtime script
# COPY ./scripts/*.sh .

# Install runtime dependencies
RUN apt install -y \
  libboost-all-dev \
  libssl-dev
# boost1.80-program_options=1.80.0-r3 \
# libstdc++=12.2.1_git20220924-r4

# Copy built binary from build image
COPY --chown=demo:demo --from=build \
  ./app/build/ws_demo \
  /app

COPY ./docker-entrypoint.sh /app/docker-entrypoint.sh
RUN chmod +x docker-entrypoint.sh

# Define entrypoint  
ENTRYPOINT ["/app/docker-entrypoint.sh"]
#EXPOSE 8126

CMD ["client"]
# CMD ["/bin/sh"]
