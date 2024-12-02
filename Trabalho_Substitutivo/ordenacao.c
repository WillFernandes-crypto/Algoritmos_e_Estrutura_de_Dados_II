#include <stdio.h>
#include <stdlib.h>
#include "ordenacao.h"

void bubbleSort(int A[], int inicio, int fim) {
    int comprimento = fim - inicio + 1;

    for (int i = 0; i < comprimento - 1; i++) {
        for (int j = inicio; j < inicio + comprimento - i - 1; j++) {
            if (A[j] > A[j + 1]) {
                // Trocar elementos
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int A[], int inicio, int fim) {
    for (int i = inicio + 1; i <= fim; i++) {
        int chave = A[i];
        int j = i - 1;

        while (j >= inicio && A[j] > chave) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = chave;
    }
}

void merge(int A[], int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = A[inicio + i];
    for (int j = 0; j < n2; j++)
        R[j] = A[meio + 1 + j];

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        A[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        A[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortRecursivo(int A[], int inicio, int fim) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        mergeSortRecursivo(A, inicio, meio);
        mergeSortRecursivo(A, meio + 1, fim);
        merge(A, inicio, meio, fim);
    }
}

void mergeSort(int A[], int inicio, int fim) {
    mergeSortRecursivo(A, inicio, fim);
}

int particionar(int A[], int inicio, int fim) {
    int pivo = A[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (A[j] <= pivo) {
            i++;
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }

    int temp = A[i + 1];
    A[i + 1] = A[fim];
    A[fim] = temp;

    return i + 1;
}

void quickSortRecursivo(int A[], int inicio, int fim) {
    if (inicio < fim) {
        int pi = particionar(A, inicio, fim);
        quickSortRecursivo(A, inicio, pi - 1);
        quickSortRecursivo(A, pi + 1, fim);
    }
}

void quickSort(int A[], int inicio, int fim) {
    quickSortRecursivo(A, inicio, fim);
}

void imprimirSubconjunto(int A[], int inicio, int fim) {
    printf("Subconjunto {");
    for (int i = inicio; i <= fim; i++) {
        printf("%d", A[i]);
        if (i < fim) printf(", ");
    }
    printf("}\n");
}

Particao* lerParticaoDeArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return NULL;
    }

    Particao* p = (Particao*)malloc(sizeof(Particao));

    // Lê n e k da primeira linha (formato: n,k)
    if (fscanf(arquivo, "%d,%d\n", &p->n, &p->k) != 2) {
        printf("Erro ao ler n e k do arquivo\n");
        free(p);
        fclose(arquivo);
        return NULL;
    }

    // Adicionar validação para garantir que k > 1
    if (p->k <= 1) {
        printf("Erro: k deve ser maior que 1\n");
        free(p->vetor);
        free(p->subconj);
        free(p);
        fclose(arquivo);
        return NULL;
    }

    // Aloca e inicializa o vetor principal
    p->vetor = (int*)malloc(p->n * sizeof(int));
    for (int i = 0; i < p->n; i++) {
        p->vetor[i] = i + 1;  // preenche com nmeros de 1 a n
    }

    // Aloca espao para os subconjuntos
    p->subconj = (Subconjunto*)malloc(p->k * sizeof(Subconjunto));

    // Lê os subconjuntos (formato: inicio,fim)
    for (int i = 0; i < p->k; i++) {
        if (fscanf(arquivo, "%d,%d\n", &p->subconj[i].inicio, &p->subconj[i].fim) != 2) {
            printf("Erro ao ler subconjunto %d\n", i+1);
            free(p->vetor);
            free(p->subconj);
            free(p);
            fclose(arquivo);
            return NULL;
        }
    }

    // Define os representantes
    for (int i = 0; i < p->k; i++) {
        definirRepresentante(p, i);
    }

    fclose(arquivo);
    return p;
}

void liberarParticao(Particao* p) {
    if (p) {
        free(p->vetor);
        free(p->subconj);
        free(p);
    }
}

void imprimirParticao(Particao* p) {
    printf("Vetor completo: ");
    for (int i = 0; i < p->n; i++) {
        printf("%d ", p->vetor[i]);
    }
    printf("\n\nSubconjuntos:\n");
    for (int i = 0; i < p->k; i++) {
        printf("Subconjunto %d: ", i+1);
        imprimirSubconjunto(p->vetor, p->subconj[i].inicio, p->subconj[i].fim);
    }
}

// Encontra em qual subconjunto est um elemento
int encontrarSubconjunto(Particao* p, int elemento) {
    // elemento-1 porque o vetor comea em 0 mas os elementos so de 1 a n
    for (int i = 0; i < p->k; i++) {
        if (elemento-1 >= p->subconj[i].inicio &&
            elemento-1 <= p->subconj[i].fim) {
            return i;
        }
    }
    return -1; // elemento no encontrado
}

// Obtém o representante do subconjunto que contém o elemento
int obterRepresentante(Particao* p, int elemento) {
    int subconj = encontrarSubconjunto(p, elemento);
    if (subconj == -1) return -1;

    return p->vetor[p->subconj[subconj].representante];
}

// Define o representante de um subconjunto (por padro, o primeiro elemento)
void definirRepresentante(Particao* p, int subconjunto) {
    if (subconjunto >= 0 && subconjunto < p->k) {
        p->subconj[subconjunto].representante = p->subconj[subconjunto].inicio;
    }
}

// Imprime informações sobre o representante de um elemento
void imprimirRepresentante(Particao* p, int elemento) {
    int subconj = encontrarSubconjunto(p, elemento);
    if (subconj == -1) {
        printf("Elemento %d no encontrado em nenhum subconjunto\n", elemento);
        return;
    }

    int rep = obterRepresentante(p, elemento);
    printf("O representante do subconjunto que contm %d : %d\n",
           elemento, rep);
}

void unirSubconjuntos(Particao* p, int elemento1, int elemento2) {
    // Adicionar validação de elementos
    if (elemento1 < 1 || elemento1 > p->n || elemento2 < 1 || elemento2 > p->n) {
        printf("Erro: elementos devem estar entre 1 e %d\n", p->n);
        return;
    }
    
    int subconj1 = encontrarSubconjunto(p, elemento1);
    int subconj2 = encontrarSubconjunto(p, elemento2);

    if (subconj1 == -1 || subconj2 == -1 || subconj1 == subconj2) {
        printf("N no possvel unir os subconjuntos\n");
        return;
    }

    // Reorganiza os subconjuntos
    reorganizarSubconjuntos(p, subconj1, subconj2);

    printf("Subconjuntos unidos com sucesso!\n");
}

void reorganizarSubconjuntos(Particao* p, int subconj1, int subconj2) {
    // Determina o incio e fim do novo subconjunto unido
    int novoInicio = p->subconj[subconj1].inicio < p->subconj[subconj2].inicio ?
                     p->subconj[subconj1].inicio : p->subconj[subconj2].inicio;

    int novoFim = p->subconj[subconj1].fim > p->subconj[subconj2].fim ?
                  p->subconj[subconj1].fim : p->subconj[subconj2].fim;

    // Atualiza o primeiro subconjunto para incluir ambos
    p->subconj[subconj1].inicio = novoInicio;
    p->subconj[subconj1].fim = novoFim;

    // Remove o segundo subconjunto movendo os outros para frente
    for (int i = subconj2; i < p->k - 1; i++) {
        p->subconj[i] = p->subconj[i + 1];
    }

    // Diminui o nmero de subconjuntos
    p->k--;

    // Redefine o representante do subconjunto unido
    definirRepresentante(p, subconj1);
}

void ordenarSubconjunto(Particao* p, int elemento, MetodoOrdenacao metodo) {
    int subconj = encontrarSubconjunto(p, elemento);
    if (subconj == -1) {
        printf("Elemento no encontrado em nenhum subconjunto\n");
        return;
    }

    int inicio = p->subconj[subconj].inicio;
    int fim = p->subconj[subconj].fim;

    switch (metodo) {
        case BUBBLE_SORT:
            bubbleSort(p->vetor, inicio, fim);
            printf("Subconjunto ordenado usando Bubble Sort\n");
            break;

        case INSERTION_SORT:
            insertionSort(p->vetor, inicio, fim);
            printf("Subconjunto ordenado usando Insertion Sort\n");
            break;

        case MERGE_SORT:
            mergeSort(p->vetor, inicio, fim);
            printf("Subconjunto ordenado usando Merge Sort\n");
            break;

        case QUICK_SORT:
            quickSort(p->vetor, inicio, fim);
            printf("Subconjunto ordenado usando Quick Sort\n");
            break;

        default:
            printf("Mtodo de ordenao no reconhecido\n");
    }
}
