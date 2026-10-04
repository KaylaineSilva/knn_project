#include <iostream>
#include <vector>
#include <string>
#include <algorithm>  // std::sort
#include <limits>     // std::numeric_limits
#include <utility>    // std::pair
#include "H5Cpp.h"

float similaridade(const float* vetor1, const float* vetor2, std::size_t vector_size){

    //Calculando a similaridade a partir do produto escalar entre os vetores
    float dot_product = 0.0;

    for(size_t i=0; i<vector_size; i++){
        dot_product += vetor1[i] * vetor2[i];
    }

    return dot_product;
}

std::vector<std::pair<int, float>> knn_search(const std::vector<float>& similaridades, int num_vetores, int num_vetores_ler, int k) {
    
    std::vector<std::pair<int, float>> resultado;

    for (int i = 0; i < num_vetores; i++) {

        // Candidatos para os k vizinhos mais próximos do vetor i
        std::vector<std::pair<int, float>> candidatos;

        for (int j = 0; j < num_vetores_ler; j++) {

            if (i == j)
                continue;

            candidatos.push_back(std::make_pair(j, similaridades[i * num_vetores_ler + j]));
        }

        // Maior similaridade primeiro
        std::sort(candidatos.begin(), candidatos.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                return a.second > b.second;
            }
        );

        // Pegar somente os k primeiros
        for (int j = 0; j < k; j++) {
            resultado.push_back(candidatos[j]);
        }
    }

    return resultado;
}

void imprimir_elementos(const std::vector<float>&data, size_t vector_size, int max_dimensoes, int limit){

    std:: cout << "\nImprimindo os primeiros " << limit << " vetores lidos:\n\n";

    for (hsize_t i = 0; i <limit; i++) {
        std:: cout << "Vetor " << i << ": ";
        for (hsize_t j = 0; j < max_dimensoes; j++) {

            std:: string inicio = (j == 0) ? "[" : "";
            std::string final = (j < max_dimensoes - 1) ? ", " : "]";
            std:: cout << inicio << data[i * vector_size + j] << final;
        }
        std:: cout << "\n\n";
    }
}

void imprimir_resultado(const std::vector<std::pair<int, float>>&resultado, int num_vetores, int k){
    std:: cout << "Resultados da busca KNN:\n";

    for (int i = 0; i < num_vetores; i++) {
        std:: cout << "Vetor " << i << ":\n";
        for (int j = 0; j < k; j++) {
            int index = i * k + j;
            std:: cout << "  Vizinho " << j + 1 << ": Vetor " << resultado[index].first
                       << ", Similaridade: " << resultado[index].second << "\n";
        }
    }
}

void imprimir_similary_vector(const std::vector<float>&similarity_vector, int num_vetores, int num_vetores_ler, int limit){
    std:: cout << "Imprimindo a matriz de similaridade:\n";

    int limit_vetores = std::min(num_vetores_ler, limit);

    for (int i = 0; i < num_vetores; i++) {
        for (int j = 0; j < limit_vetores; j++) {
            std:: cout << "(" << i << "," << j << "): " << similarity_vector[i * num_vetores_ler + j] << "\n";
        }
        std:: cout << "\n";
    }
}


int main() {
    //Abrindo arquivo HBF5
    const std::string path = "/data/wikipedia-small/benchmark-dev-wikipedia-bge-m3-small.h5";
    
    try {
        H5:: H5File file(path, H5F_ACC_RDONLY);

        std:: cout << "Arquivo HDF5 aberto com sucesso!\n";

        H5:: DataSet dataset = file.openDataSet("train"); //abrindo o dataset com o nome "train"

        std:: cout << "Dataset 'train' aberto com sucesso!\n";

        H5:: DataSpace dataspace = dataset.getSpace(); //obtendo o espaço de dados do dataset (a organização dimensional dos dados)

        int rank = dataspace.getSimpleExtentNdims(); //obtendo o número de dimensões do dataset
        std:: cout << "Número de dimensões do dataset: " << rank << "\n";

        std:: vector<hsize_t> dims(rank); //criando um vetor para armazenar as dimensões do dataset
        dataspace.getSimpleExtentDims(dims.data(), nullptr); //obtendo as dimensões
        std:: cout << "Dimensões do dataset: ";
        for (int i = 0; i < rank; i++) {
            std:: cout << dims[i] << " ";
        }
        std:: cout << "\n";

        //Lendo os dados do dataset
        const hsize_t num_vectors = 6000;
        const hsize_t vector_size = dims[1];

        std:: vector<float> data(num_vectors * dims[1]); 

        //Como serão lidos apenas os primeiros 6000 vetores, é necessário usar a função selectHyperslab para selecionar apenas a parte do dataset que será lida

        hsize_t offset[2] = {0, 0}; //início da leitura (primeiro vetor)
        hsize_t count[2] = {num_vectors, vector_size}; //quantidade de vetores a serem lidos (6000) e tamanho do vetor (dimensão do dataset)

        dataspace.selectHyperslab(
            H5S_SELECT_SET,
            count,
            offset
        );

        //Também é necessário criar um DataSpace para o buffer de leitura, que terá as mesmas dimensões do dataset selecionado

        //Para mapear na memória, precisa criar outro DataSpace para o buffer de leitura, que terá as mesmas dimensões do dataset selecionado
        H5:: DataSpace memory_space(2, count); //criando um DataSpace para o buffer de leitura

        dataset.read(
            data.data(),                     // onde colocar
            H5::PredType::NATIVE_FLOAT,      // tipo na memória
            memory_space,                    // formato na memória
            dataspace                        // seleção no arquivo
        );

        std::cout << "\nForam lidos " << num_vectors << " vetores.\n";

        imprimir_elementos(data, vector_size, 10, 20);

        std::vector<float>similary_vector(6*6000,0.0f); //vetor para armazenar a similaridade de 6 vetores com os 6000 vetores lidos
        
        for(int i=0; i<6; i++){
            
            for(int j=0; j<6000; j++){
                if(i==j) continue;

                similary_vector[i*6000+j] = similaridade(data.data() + i*vector_size, data.data() + j*vector_size, vector_size);
            }
        }

        int num_vetores_knn = 1;
        int k = 3;


        std::vector<std::pair<int, float>> result = knn_search(similary_vector, num_vetores_knn, 6000, k);

        imprimir_similary_vector(similary_vector, 6, 6000, 10);

        imprimir_resultado(result, num_vetores_knn, k);

    } catch (const H5::Exception& error){
        std:: cerr << "Erro ao abrir o arquivo.\n";
        error.printErrorStack();

        return 1;
    }
}