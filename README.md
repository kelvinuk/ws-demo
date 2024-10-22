# Boost Websocket C++ Client 

A small program that demonstrates how to call Molly API from C++.


## Structure and Design

This program features two main functions: boost_https_login and boost_mo_ws_client. The boost_https_login function handles SSL HTTP POST requests, while the boost_mo_ws_client function establishes an SSL WebSocket client connection. Both functions utilize Asio's asynchronous mode, which is designed for C++ coroutines in a non-blocking environment where software scalability and flexibility are essential. Unlike traditional multithreading programming, this approach eliminates the need for locking and semaphore resources.

An Event Handler has been introduced, which can be passed to these functions for future customization and enhancement. This event handler can be extended as Application Storage to store token and received data.


## Demo Quick start

To launch Docker, you can use the Docker Compose command. A multi-stage Dockerfile is provided for this purpose.
```
  make start-all
```

## Development Quick start

Please use CLion or VSCode IDE for development. The current development operating system is Ubuntu 24. CMake is the default build tool, although Ninja can be re-enabled in CLion if preferred. Additionally, Bazel can be used if a Node.js server simulator is required.

```
  apt-get install cmake
  apt-get install -y libboost-all-dev libssl-dev
  make build-local
```

## Further Improvement

1. The "sync" key detection can be simpified. Boost json ptree getChild function can be used. The query string should be "data..sync".
2. Unit Tests
3. Node JS Websocket Server (developed) can be added for testing / simulation
4. Postman Test (developed) can be added in the project.
5. Migrate CMake to Bazel
6. Use uWebsocket for high traffic environment
7. A better logging system can be added
