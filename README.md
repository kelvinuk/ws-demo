# Boost Websocket C++ Client 

A small program that demonstrate how to call Molly API from C++.


## Structure and Design

This program provides two major functions (boost_https_login and boost_mo_ws_client) 
The boost_https_login function implements the SSL HTTP post request whereas
The boost_mo_ws_client function implements the SSL Websocket client connection.
They both use Asio asynchronis mode. This approach is targeted for C++ coroutine, non-blocking environmnt where software scalbility/flexibility is important.
Compare with traditional multithread programming, locking /semaphore resources are not needed.


An Event Handler is introduced and it can be passed to these functions for future customization / enhancement. 


## Demo Quick start

To spin up docker, docker compose command can be used. Multi stage Docker file is provided.
```
  make start-all
```

## Development Quick start

Please use CLion or VsCode IDE for development. Current development OS is ubuntu 24.
CMake is the default build tool (Ninja can be re-enabled in IDE if CLion is used). (Bazel can be used if node js server simulator is needed) 

```
  make build-local
```