
build-docker:
	docker compose -f docker-compose.yml build --no-cache --build-arg BUILD_MODE=1

build-local:
	cmake --build ./cmake-build-debug --target all -j 10

start-all:
	docker compose -f docker-compose.yml build --build-arg BUILD_MODE=1
	docker compose up ws-docker

start:
	docker compose up ws-docker

