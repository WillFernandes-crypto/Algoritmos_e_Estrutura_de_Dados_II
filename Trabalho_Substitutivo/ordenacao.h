#ifndef ALGORITMOS_DE_ORDENACAO_H
#define ALGORITMOS_DE_ORDENACAO_H

// Estrutura para representar um subconjunto
typedef struct {
    int inicio;
    int fim;
    int representante;
} Subconjunto;

// Estrutura para armazenar a partição completa
typedef struct {
    int* vetor;           // vetor com elementos de 1 a n
    int n;                // tamanho total do vetor
    Subconjunto* subconj; // array de subconjuntos
    int k;                // número de subconjuntos
} Particao;

// Funções do bubblesort
void bubbleSort(int A[], int inicio, int fim);
void insertionSort(int A[], int inicio, int fim);
void mergeSort(int A[], int inicio, int fim);
void quickSort(int A[], int inicio, int fim);
void imprimirSubconjunto(int A[], int inicio, int fim);

// Funções para manipulação do arquivo e partição
Particao* lerParticaoDeArquivo(const char* nomeArquivo);
void liberarParticao(Particao* p);
void imprimirParticao(Particao* p);

// Novas funções para trabalhar com representantes
int encontrarSubconjunto(Particao* p, int elemento);
int obterRepresentante(Particao* p, int elemento);
void definirRepresentante(Particao* p, int subconjunto);
void imprimirRepresentante(Particao* p, int elemento);

// Funções para união de subconjuntos
void unirSubconjuntos(Particao* p, int elemento1, int elemento2);
void reorganizarSubconjuntos(Particao* p, int subconj1, int subconj2);

// Funções de ordenação (preparando para outros métodos)
typedef enum {
    BUBBLE_SORT,
    INSERTION_SORT,
    MERGE_SORT,
    QUICK_SORT
} MetodoOrdenacao;

// Função genérica para ordenar usando o método escolhido
void ordenarSubconjunto(Particao* p, int elemento, MetodoOrdenacao metodo);

#endif
