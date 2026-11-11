# Análise de desempenho da busca kNN sequencial e paralela

## 1. Objetivo

Comparar o desempenho das implementações sequencial e paralela do cálculo de similaridades e da busca dos k vizinhos mais próximos (kNN), utilizando C++17.

## 2. Metodologia

Foi utilizado um dataset HDF5 contendo 200.000 vetores de 1024 dimensões, dos quais foram lidos os primeiros 6000 vetores.

O cálculo da matriz de similaridades considerou 6 vetores de consulta em relação aos 6000 vetores lidos, utilizando o produto escalar como medida de similaridade.

A busca kNN foi realizada para o vetor de índice 0, com k = 3. A versão paralela utilizou políticas de execução `std::execution::par` nas operações de cálculo, construção e ordenação dos candidatos.

Foram realizadas cinco execuções de cada implementação, medindo separadamente o tempo da matriz de similaridades e da busca kNN, em milissegundos. Os tempos não incluem a leitura do arquivo HDF5 nem a impressão dos resultados.

## 3. Resultados

| Etapa | Sequencial (ms) | Paralelo (ms) | Speedup |
|---|---:|---:|---:|
| Matriz de similaridades | 20,251 | 6,185 | 3,27× |
| Busca kNN | 0,530 | 0,509 | 1,04× |
| Total de processamento | 20,781 | 6,695 | 3,10× |

O speedup foi calculado pela razão entre o tempo médio sequencial e o tempo médio paralelo:

**Speedup = Tempo sequencial / Tempo paralelo**

As duas implementações retornaram os mesmos três vizinhos para o vetor 0:

| Vizinho | Índice | Similaridade |
|---|---:|---:|
| 1 | 265 | 0,499931 |
| 2 | 3793 | 0,488349 |
| 3 | 5538 | 0,482943 |

## 4. Discussão

A paralelização apresentou ganhos expressivos no cálculo da matriz de similaridades, reduzindo o tempo médio de 20,251 ms para 6,185 ms, o que corresponde a um speedup de aproximadamente 3,27 vezes.

Na busca kNN, os tempos médios foram próximos: 0,530 ms na versão sequencial e 0,509 ms na paralela. A diferença é pequena em relação à variabilidade observada, não sendo suficiente para demonstrar um ganho consistente nessa etapa.

## 5. Conclusão

Os experimentos indicam que a paralelização foi vantajosa principalmente no cálculo da matriz de similaridades, proporcionando uma redução de aproximadamente 69,5% no tempo dessa etapa.

Considerando as duas etapas medidas, a implementação paralela apresentou speedup médio de aproximadamente 3,10 vezes.

Os resultados sugerem que o paralelismo oferece maiores benefícios na etapa de cálculo das similaridades, enquanto sua vantagem na busca kNN, para a carga de trabalho utilizada, ainda não ficou demonstrada.

Para aprofundar a análise, pode ser interessante aumentar o número de vetores consultados no kNN, verificando se os custos relacionados a execução paralela são compensados conforme a busca do knn aumenta.