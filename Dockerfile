FROM ubuntu:24.04

RUN apt-get update && apt-get install -y build-essential cmake libhdf5-dev

WORKDIR /app

COPY CMakeLists.txt .
COPY src/ ./src/

RUN cmake -S . -B build && cmake --build build

CMD ["./build/knn_project"]