FROM ubuntu:24.04

RUN apt-get update && apt-get install -y build-essential cmake libhdf5-dev libtbb-dev

WORKDIR /app

COPY CMakeLists.txt .
COPY src/ ./src/

# rodando o projeto com otimizações habilitadas e usando todos os núcleos disponíveis para a compilação
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j


# A execução será passada como argumento ao comando `docker run`, então não definimos um CMD padrão aqui.
# docker run --rm knn ./build/knn_seq para rodar a versão sequencial
# docker run --rm knn ./build/knn_par para rodar a versão paralela

# CMD ["./build/knn_seq"]