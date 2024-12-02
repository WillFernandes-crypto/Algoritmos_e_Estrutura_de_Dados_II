#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "ordenacao.h"

extern FILE* arquivo_log;

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

// Define o representante de um subconjunto (por padrão, o primeiro elemento)
void definirRepresentante(Particao* p, int subconjunto) {
    if (subconjunto >= 0 && subconjunto < p->k) {
        p->subconj[subconjunto].representante = p->subconj[subconjunto].inicio;
    }
}

// Imprime informações sobre o representante de um elemento
void imprimirRepresentante(Particao* p, int elemento) {
    int subconj = encontrarSubconjunto(p, elemento);
    if (subconj == -1) {
        printf("Elemento %d não encontrado em nenhum subconjunto\n", elemento);
        return;
    }

    int rep = obterRepresentante(p, elemento);
    printf("O representante do subconjunto que contém %d é %d\n",
           elemento, rep);
}

void unirSubconjuntos(Particao* p, int elemento1, int elemento2) {
    FILE* saida = fopen("resultado.txt", "ab");
    if (!saida) return;

    char detalhes[1000];
    sprintf(detalhes, "Tentativa de união dos subconjuntos contendo elementos %d e %d",
            elemento1, elemento2);
    registrarOperacao(saida, "União de Subconjuntos", detalhes);

    // Validação de elementos
    if (elemento1 < 1 || elemento1 > p->n || elemento2 < 1 || elemento2 > p->n) {
        sprintf(detalhes, "Erro: elementos devem estar entre 1 e %d", p->n);
        registrarOperacao(saida, "Erro na União", detalhes);
        printf("%s\n", detalhes);
        fclose(saida);
        return;
    }

    int subconj1 = encontrarSubconjunto(p, elemento1);
    int subconj2 = encontrarSubconjunto(p, elemento2);

    // Log do estado inicial
    fprintf(saida, "Estado inicial dos subconjuntos:\n");
    if (subconj1 != -1) {
        fprintf(saida, "Subconjunto 1 (contém elemento %d): [", elemento1);
        for (int i = p->subconj[subconj1].inicio; i <= p->subconj[subconj1].fim; i++) {
            fprintf(saida, "%d", p->vetor[i]);
            if (i < p->subconj[subconj1].fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n");
    }
    if (subconj2 != -1) {
        fprintf(saida, "Subconjunto 2 (contém elemento %d): [", elemento2);
        for (int i = p->subconj[subconj2].inicio; i <= p->subconj[subconj2].fim; i++) {
            fprintf(saida, "%d", p->vetor[i]);
            if (i < p->subconj[subconj2].fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n");
    }

    if (subconj1 == -1 || subconj2 == -1 || subconj1 == subconj2) {
        sprintf(detalhes, "Não é possível unir os subconjuntos: %s",
                subconj1 == subconj2 ? "Elementos no mesmo subconjunto" : "Elemento(s) não encontrado(s)");
        registrarOperacao(saida, "Erro na União", detalhes);
        printf("%s\n", detalhes);
        fclose(saida);
        return;
    }

    // Reorganiza os subconjuntos
    reorganizarSubconjuntos(p, subconj1, subconj2);

    // Log do resultado
    fprintf(saida, "\nResultado da união:\n");
    fprintf(saida, "Novo subconjunto: [");
    for (int i = p->subconj[subconj1].inicio; i <= p->subconj[subconj1].fim; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < p->subconj[subconj1].fim) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n");

    registrarOperacao(saida, "União Concluída", "Subconjuntos unidos com sucesso");
    printf("Subconjuntos unidos com sucesso!\n");
    fclose(saida);
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
    FILE* saida = fopen("resultado.txt", "ab");
    if (!saida) return;

    // Usa clock_t para maior precisão
    clock_t inicio_ord = clock();
    char detalhes[1000];
    sprintf(detalhes, "Iniciando ordenação do subconjunto contendo elemento %d", elemento);
    registrarOperacao(saida, "Ordenação", detalhes);

    int subconj = encontrarSubconjunto(p, elemento);
    if (subconj == -1) {
        sprintf(detalhes, "Elemento %d não encontrado em nenhum subconjunto", elemento);
        registrarOperacao(saida, "Erro na Ordenação", detalhes);
        printf("%s\n", detalhes);
        fclose(saida);
        return;
    }

    int inicio = p->subconj[subconj].inicio;
    int fim = p->subconj[subconj].fim;

    // Log do estado inicial
    fprintf(saida, "Estado inicial do subconjunto: [");
    for (int i = inicio; i <= fim; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < fim) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n\n");

    // Executa a ordenação com log detalhado
    switch (metodo) {
        case BUBBLE_SORT:
            fprintf(saida, "Usando Bubble Sort:\n");
            bubbleSortComLog(p->vetor, inicio, fim, saida);
            break;
        case INSERTION_SORT:
            fprintf(saida, "Usando Insertion Sort:\n");
            insertionSortComLog(p->vetor, inicio, fim, saida);
            break;
        case MERGE_SORT:
            fprintf(saida, "Usando Merge Sort:\n");
            mergeSortComLog(p->vetor, inicio, fim, saida);
            break;
        case QUICK_SORT:
            fprintf(saida, "Usando Quick Sort:\n");
            quickSortComLog(p->vetor, inicio, fim, saida);
            break;
    }

    // Log do resultado
    fprintf(saida, "\nEstado final do subconjunto: [");
    for (int i = inicio; i <= fim; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < fim) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n");

    registrarOperacao(saida, "Ordenação Concluída", detalhes);
    
    fflush(saida);
    fclose(saida);
}

void bubbleSortComLog(int A[], int inicio, int fim, FILE* saida) {
    int comprimento = fim - inicio + 1;
    int comparacoes = 0, movimentacoes = 0;

    fprintf(saida, "Iniciando Bubble Sort com %d elementos\n\n", comprimento);

    for (int i = 0; i < comprimento - 1; i++) {
        fprintf(saida, "Passagem %d:\n", i + 1);
        bool trocou = false;
        
        for (int j = inicio; j < inicio + comprimento - i - 1; j++) {
            fprintf(saida, "  Comparando A[%d]=%d com A[%d]=%d: ", j, A[j], j+1, A[j+1]);
            comparacoes++;
            
            if (A[j] > A[j + 1]) {
                fprintf(saida, "troca\n");
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                movimentacoes += 3;
                trocou = true;
            } else {
                fprintf(saida, "mantém\n");
            }
        }
        
        fprintf(saida, "Estado após passagem %d: [", i + 1);
        for (int k = inicio; k <= fim; k++) {
            fprintf(saida, "%d", A[k]);
            if (k < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n\n");
        
        if (!trocou) {
            fprintf(saida, "Array já ordenado, parando Bubble Sort\n");
            break;
        }
    }

    fprintf(saida, "\nEstatísticas do Bubble Sort:\n");
    fprintf(saida, "Total de comparações: %d\n", comparacoes);
    fprintf(saida, "Total de movimentações: %d\n", movimentacoes);
}

void insertionSortComLog(int A[], int inicio, int fim, FILE* saida) {
    int comprimento = fim - inicio + 1;
    int comparacoes = 0, movimentacoes = 0;

    fprintf(saida, "Iniciando Insertion Sort com %d elementos\n\n", comprimento);

    for (int i = inicio + 1; i <= fim; i++) {
        int chave = A[i];
        int j = i - 1;
        
        fprintf(saida, "Inserindo elemento A[%d] = %d:\n", i, chave);
        fprintf(saida, "  Comparando com elementos anteriores:\n");
        
        bool moveu = false;
        while (j >= inicio && A[j] > chave) {
            fprintf(saida, "    A[%d]=%d > %d: move para direita\n", j, A[j], chave);
            A[j + 1] = A[j];
            j--;
            comparacoes++;
            movimentacoes++;
            moveu = true;
        }
        
        if (!moveu) {
            fprintf(saida, "  Elemento já está na posição correta\n");
        }
        
        A[j + 1] = chave;
        if (moveu) movimentacoes++;
        
        fprintf(saida, "  Estado atual: [");
        for (int k = inicio; k <= fim; k++) {
            fprintf(saida, "%d", A[k]);
            if (k < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n\n");
    }

    fprintf(saida, "Estatísticas do Insertion Sort:\n");
    fprintf(saida, "Total de comparações: %d\n", comparacoes);
    fprintf(saida, "Total de movimentações: %d\n", movimentacoes);
}

void mergeSortComLog(int A[], int inicio, int fim, FILE* saida) {
    static int nivel = 0;
    static int comparacoes = 0, movimentacoes = 0;
    
    if (nivel == 0) {
        fprintf(saida, "Iniciando Merge Sort com %d elementos\n\n", fim - inicio + 1);
        comparacoes = movimentacoes = 0;
    }

    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        
        // Log da divisão atual
        for (int i = 0; i < nivel; i++) fprintf(saida, "  ");
        fprintf(saida, "Dividindo array [");
        for (int i = inicio; i <= fim; i++) {
            fprintf(saida, "%d", A[i]);
            if (i < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "] em duas partes\n");
        
        nivel++;
        mergeSortComLog(A, inicio, meio, saida);
        mergeSortComLog(A, meio + 1, fim, saida);
        nivel--;

        // Log da combinação
        for (int i = 0; i < nivel; i++) fprintf(saida, "  ");
        fprintf(saida, "Combinando as partes ordenadas:\n");
        fprintf(saida, "  Parte esquerda: [");
        for (int i = inicio; i <= meio; i++) {
            fprintf(saida, "%d", A[i]);
            if (i < meio) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n  Parte direita: [");
        for (int i = meio + 1; i <= fim; i++) {
            fprintf(saida, "%d", A[i]);
            if (i < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n");

        mergeComLog(A, inicio, meio, fim, saida, &comparacoes, &movimentacoes);

        // Log do resultado da combinação
        for (int i = 0; i < nivel; i++) fprintf(saida, "  ");
        fprintf(saida, "Resultado da combinação: [");
        for (int i = inicio; i <= fim; i++) {
            fprintf(saida, "%d", A[i]);
            if (i < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n\n");

        if (nivel == 0) {
            fprintf(saida, "\nEstatísticas do Merge Sort:\n");
            fprintf(saida, "Total de comparações: %d\n", comparacoes);
            fprintf(saida, "Total de movimentações: %d\n", movimentacoes);
        }
    }
}

void mergeComLog(int A[], int inicio, int meio, int fim, FILE* saida, 
                 int* comparacoes, int* movimentacoes) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // Copia os elementos para os arrays temporários
    for (int i = 0; i < n1; i++) {
        L[i] = A[inicio + i];
        (*movimentacoes)++;
    }
    for (int j = 0; j < n2; j++) {
        R[j] = A[meio + 1 + j];
        (*movimentacoes)++;
    }

    int i = 0, j = 0, k = inicio;
    
    // Combina os arrays ordenados
    while (i < n1 && j < n2) {
        (*comparacoes)++;
        fprintf(saida, "    Comparando %d com %d: ", L[i], R[j]);
        
        if (L[i] <= R[j]) {
            fprintf(saida, "escolhe %d da esquerda\n", L[i]);
            A[k] = L[i];
            i++;
        } else {
            fprintf(saida, "escolhe %d da direita\n", R[j]);
            A[k] = R[j];
            j++;
        }
        (*movimentacoes)++;
        k++;
    }

    // Copia os elementos restantes
    while (i < n1) {
        fprintf(saida, "    Copiando elemento restante da esquerda: %d\n", L[i]);
        A[k] = L[i];
        i++;
        k++;
        (*movimentacoes)++;
    }

    while (j < n2) {
        fprintf(saida, "    Copiando elemento restante da direita: %d\n", R[j]);
        A[k] = R[j];
        j++;
        k++;
        (*movimentacoes)++;
    }

    free(L);
    free(R);
}

void quickSortComLog(int A[], int inicio, int fim, FILE* saida) {
    static int nivel = 0;
    static int comparacoes = 0, movimentacoes = 0;
    
    if (nivel == 0) {
        fprintf(saida, "Iniciando Quick Sort com %d elementos\n\n", fim - inicio + 1);
        comparacoes = movimentacoes = 0;
    }

    if (inicio < fim) {
        // Log do estado atual
        for (int i = 0; i < nivel; i++) fprintf(saida, "  ");
        fprintf(saida, "Particionando array [");
        for (int i = inicio; i <= fim; i++) {
            fprintf(saida, "%d", A[i]);
            if (i < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "], pivô = %d\n", A[fim]);

        int pi = particionarComLog(A, inicio, fim, saida, &comparacoes, &movimentacoes);
        
        nivel++;
        quickSortComLog(A, inicio, pi - 1, saida);
        quickSortComLog(A, pi + 1, fim, saida);
        nivel--;

        if (nivel == 0) {
            fprintf(saida, "\nEstatísticas do Quick Sort:\n");
            fprintf(saida, "Total de comparações: %d\n", comparacoes);
            fprintf(saida, "Total de movimentações: %d\n", movimentacoes);
        }
    }
}

int particionarComLog(int A[], int inicio, int fim, FILE* saida, 
                     int* comparacoes, int* movimentacoes) {
    int pivo = A[fim];
    int i = inicio - 1;

    fprintf(saida, "  Pivô escolhido: %d\n", pivo);

    for (int j = inicio; j < fim; j++) {
        (*comparacoes)++;
        fprintf(saida, "    Comparando A[%d]=%d com pivô %d: ", j, A[j], pivo);
        
        if (A[j] <= pivo) {
            i++;
            fprintf(saida, "troca\n");
            if (i != j) {
                fprintf(saida, "      Trocando A[%d]=%d com A[%d]=%d\n", 
                        i, A[i], j, A[j]);
                int temp = A[i];
                A[i] = A[j];
                A[j] = temp;
                (*movimentacoes) += 3;
            }
        } else {
            fprintf(saida, "mantém\n");
        }
    }

    if (i + 1 != fim) {
        fprintf(saida, "  Colocando pivô na posição final %d\n", i + 1);
        int temp = A[i + 1];
        A[i + 1] = A[fim];
        A[fim] = temp;
        (*movimentacoes) += 3;
    }

    return i + 1;
}

