#include <iostream>
#include <vector>
#include <string>
#include "H5Cpp.h"

void imprimir_elementos(const std::vector<float>&data, int max_dimensoes, int limit){

    std:: cout << "Imprimindo os primeiros " << limit << " vetores lidos:\n";

    for (hsize_t i = 0; i <limit; i++) {
        std:: cout << "Vetor " << i << ": ";
        for (hsize_t j = 0; j < max_dimensoes; j++) {

            std:: string inicio = (j == 0) ? "[" : "";
            std::string final = (j < max_dimensoes - 1) ? ", " : "]";
            std:: cout << inicio << data[i * max_dimensoes + j] << final;
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
        hsize_t count[2] = {num_vectors, vector_size}; //quantidade de vetores a serem lidos (6000) e tamanho do vetor (dimensão do dataset

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

        std::cout << "Foram lidos " << num_vectors << " vetores.\n";

        imprimir_elementos(data, 10, 20);

    } catch (const H5::Exception& error){
        std:: cerr << "Erro ao abrir o arquivo.\n";
        error.printErrorStack();

        return 1;
    }
}