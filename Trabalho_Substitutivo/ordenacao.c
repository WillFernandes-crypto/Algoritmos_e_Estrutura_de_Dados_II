#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include "ordenacao.h"

extern FILE* arquivo_log;

// Função auxiliar para obter tempo em microssegundos
long long getMicrotime() {
#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    return (start.QuadPart * 1000000) / frequency.QuadPart;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
#endif
}

void bubbleSortComLog(int A[], int inicio, int fim, FILE* saida) {
    int comprimento = fim - inicio + 1;
    int comparacoes = 0, movimentacoes = 0;
    
    fprintf(saida, "Iniciando Bubble Sort com %d elementos\n\n", comprimento);
    
    long long inicio_ord = getMicrotime();
    
    // Log do estado inicial
    fprintf(saida, "Estado inicial: [");
    for (int i = inicio; i <= fim; i++) {
        fprintf(saida, "%d", A[i]);
        if (i < fim) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n\n");
    
    for (int i = 0; i < comprimento - 1; i++) {
        bool trocou = false;
        fprintf(saida, "Passagem %d:\n", i + 1);
        
        for (int j = inicio; j < inicio + comprimento - i - 1; j++) {
            comparacoes++;
            fprintf(saida, "  Comparando %d com %d: ", A[j], A[j + 1]);
            
            if (A[j] > A[j + 1]) {
                fprintf(saida, "troca\n");
                // Trocar elementos
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                movimentacoes += 3;
                trocou = true;
            } else {
                fprintf(saida, "mantém\n");
            }
        }
        
        // Mostra estado atual do array após cada passagem
        fprintf(saida, "  Estado atual: [");
        for (int k = inicio; k <= fim; k++) {
            fprintf(saida, "%d", A[k]);
            if (k < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n\n");
        
        // Se não houve trocas, o array já está ordenado
        if (!trocou) {
            fprintf(saida, "Array já está ordenado, parando...\n\n");
            break;
        }
    }
    
    long long fim_ord = getMicrotime();
    double tempo_ord = (fim_ord - inicio_ord) / 1000.0;

    fprintf(saida, "\nEstatísticas do Bubble Sort:\n");
    fprintf(saida, "Total de comparacoes: %d\n", comparacoes);
    fprintf(saida, "Total de movimentacoes: %d\n", movimentacoes);
    fprintf(saida, "Tempo de ordenacao: %.3f milissegundos\n", tempo_ord);
}

void insertionSortComLog(int A[], int inicio, int fim, FILE* saida) {
    int comprimento = fim - inicio + 1;
    int comparacoes = 0, movimentacoes = 0;

    fprintf(saida, "Iniciando Insertion Sort com %d elementos\n\n", comprimento);
    
    long long inicio_ord = getMicrotime();
    
    for (int i = inicio + 1; i <= fim; i++) {
        int chave = A[i];
        int j = i - 1;
        
        fprintf(saida, "Inserindo elemento A[%d] = %d:\n", i, chave);
        
        bool moveu = false;
        while (j >= inicio && A[j] > chave) {
            fprintf(saida, "  Comparando %d com %d: move para direita\n", A[j], chave);
            A[j + 1] = A[j];
            j--;
            comparacoes++;
            movimentacoes++;
            moveu = true;
        }
        
        if (!moveu) {
            fprintf(saida, "  Elemento %d já está na posição correta\n", chave);
            comparacoes++;
        } else {
            A[j + 1] = chave;
            movimentacoes++;
        }
        
        fprintf(saida, "  Estado atual: [");
        for (int k = inicio; k <= fim; k++) {
            fprintf(saida, "%d", A[k]);
            if (k < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n\n");
    }
    
    long long fim_ord = getMicrotime();
    double tempo_ord = (fim_ord - inicio_ord) / 1000.0;

    fprintf(saida, "\nEstatísticas do Insertion Sort:\n");
    fprintf(saida, "Total de comparações: %d\n", comparacoes);
    fprintf(saida, "Total de movimentacoes: %d\n", movimentacoes);
    fprintf(saida, "Tempo de ordenacao: %.3f milissegundos\n", tempo_ord);
}

void mergeSortComLog(int A[], int inicio, int fim, FILE* saida) {
    static int nivel = 0;
    static int comparacoes = 0, movimentacoes = 0;
    static long long inicio_ord;
    
    if (nivel == 0) {
        fprintf(saida, "Iniciando Merge Sort com %d elementos\n\n", fim - inicio + 1);
        comparacoes = movimentacoes = 0;
        inicio_ord = getMicrotime();

        // Mostrar estado inicial
        fprintf(saida, "Estado inicial: [");
        for (int i = inicio; i <= fim; i++) {
            fprintf(saida, "%d", A[i]);
            if (i < fim) fprintf(saida, ", ");
        }
        fprintf(saida, "]\n\n");
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

        mergeComLog(A, inicio, meio, fim, saida, &comparacoes, &movimentacoes);

        if (nivel == 0) {
            // Mostrar estado final
            fprintf(saida, "\nEstado final: [");
            for (int i = inicio; i <= fim; i++) {
                fprintf(saida, "%d", A[i]);
                if (i < fim) fprintf(saida, ", ");
            }
            fprintf(saida, "]\n");

            long long fim_ord = getMicrotime();
            double tempo_ord = (fim_ord - inicio_ord) / 1000.0;

            fprintf(saida, "\nEstatísticas do Merge Sort:\n");
            fprintf(saida, "Total de comparações: %d\n", comparacoes);
            fprintf(saida, "Total de movimentações: %d\n", movimentacoes);
            fprintf(saida, "Tempo de ordenacao: %.3f milissegundos\n", tempo_ord);
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
    static long long inicio_ord;
    
    if (nivel == 0) {
        fprintf(saida, "Iniciando Quick Sort com %d elementos\n\n", fim - inicio + 1);
        comparacoes = movimentacoes = 0;
        inicio_ord = getMicrotime();
    }

    if (inicio < fim) {
        // Log do estado atual apenas no primeiro nível
        if (nivel == 0) {
            fprintf(saida, "Estado inicial: [");
            for (int i = inicio; i <= fim; i++) {
                fprintf(saida, "%d", A[i]);
                if (i < fim) fprintf(saida, ", ");
            }
            fprintf(saida, "]\n\n");
        }

        int pi = particionarComLog(A, inicio, fim, saida, &comparacoes, &movimentacoes);
        
        nivel++;
        quickSortComLog(A, inicio, pi - 1, saida);
        quickSortComLog(A, pi + 1, fim, saida);
        nivel--;

        // Mostrar estatísticas apenas no final da ordenação completa
        if (nivel == 0) {
            long long fim_ord = getMicrotime();
            double tempo_ord = (fim_ord - inicio_ord) / 1000.0;
            
            fprintf(saida, "\nEstado final: [");
            for (int i = inicio; i <= fim; i++) {
                fprintf(saida, "%d", A[i]);
                if (i < fim) fprintf(saida, ", ");
            }
            fprintf(saida, "]\n");
            
            fprintf(saida, "\nEstatísticas do Quick Sort:\n");
            fprintf(saida, "Total de comparacoes: %d\n", comparacoes);
            fprintf(saida, "Total de movimentacoes: %d\n", movimentacoes);
            fprintf(saida, "Tempo de ordenacao: %.3f milissegundos\n", tempo_ord);
        }
    }
}

int particionarComLog(int A[], int inicio, int fim, FILE* saida, 
                     int* comparacoes, int* movimentacoes) {
    int pivo = A[fim];
    int i = inicio - 1;

    if (saida) fprintf(saida, "  Pivo escolhido: %d\n", pivo);

    for (int j = inicio; j < fim; j++) {
        (*comparacoes)++;
        if (saida) fprintf(saida, "    Comparando A[%d]=%d com pivo %d: ", j, A[j], pivo);
        
        if (A[j] <= pivo) {
            i++;
            if (i != j) {  // Só conta movimentação se realmente houver troca
                if (saida) {
                    fprintf(saida, "troca\n");
                    fprintf(saida, "      Trocando A[%d]=%d com A[%d]=%d\n", 
                            i, A[i], j, A[j]);
                }
                int temp = A[i];
                A[i] = A[j];
                A[j] = temp;
                *movimentacoes += 3;  // 3 movimentações: temp = A[i], A[i] = A[j], A[j] = temp
            } else {
                if (saida) fprintf(saida, "mantém (já está na posição correta)\n");
            }
        } else {
            if (saida) fprintf(saida, "mantém\n");
        }
    }

    // Troca o pivô para sua posição final
    if (i + 1 != fim) {  // Só faz a troca se necessário
        if (saida) fprintf(saida, "  Colocando pivo na posição final %d\n", i + 1);
        int temp = A[i + 1];
        A[i + 1] = A[fim];
        A[fim] = temp;
        *movimentacoes += 3;  // 3 movimentações para a troca do pivô
    }

    return i + 1;
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

    // Adiciona verificação de ambiguidade
    verificarAmbiguidade(p);

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
    printf("O representante do subconjunto que contem %d e %d\n",
           elemento, rep);
}

void unirSubconjuntos(Particao* p, int elemento1, int elemento2) {
    FILE* saida = fopen("resultado.txt", "ab");
    if (!saida) return;

    char detalhes[1000];
    sprintf(detalhes, "Tentativa de uniao dos subconjuntos contendo elementos %d e %d",
            elemento1, elemento2);
    registrarOperacao(saida, "Uniao de Subconjuntos", detalhes);

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
    fprintf(saida, "Subconjunto %d: [", subconj1 + 1);
    for (int i = p->subconj[subconj1].inicio; i <= p->subconj[subconj1].fim; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < p->subconj[subconj1].fim) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n");
    
    fprintf(saida, "Subconjunto %d: [", subconj2 + 1);
    for (int i = p->subconj[subconj2].inicio; i <= p->subconj[subconj2].fim; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < p->subconj[subconj2].fim) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n\n");

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

    // Log do estado final
    fprintf(saida, "Estado final dos subconjuntos:\n");
    for (int i = 0; i < p->k; i++) {
        fprintf(saida, "Subconjunto %d: [", i + 1);
        for (int j = p->subconj[i].inicio; j <= p->subconj[i].fim; j++) {
            fprintf(saida, "%d", p->vetor[j]);
            if (j < p->subconj[i].fim) fprintf(saida, ", ");
        }
        fprintf(saida, "] (início: %d, fim: %d)\n", p->subconj[i].inicio, p->subconj[i].fim);
    }

    registrarOperacao(saida, "União Concluída", "Subconjuntos unidos com sucesso");
    printf("Subconjuntos unidos com sucesso!\n");
    fclose(saida);
}

void reorganizarSubconjuntos(Particao* p, int subconj1, int subconj2) {
    // Criar array temporário para todo o vetor
    int* novo_vetor = (int*)malloc(p->n * sizeof(int));
    bool* usado = (bool*)calloc(p->n + 1, sizeof(bool));
    int pos = 0;
    
    // Criar novo array de subconjuntos
    Subconjunto* novos_subconj = (Subconjunto*)malloc(p->k * sizeof(Subconjunto));
    int novo_k = 0;
    
    // 1. Primeiro, unir os subconjuntos selecionados
    int inicio_unido = pos;
    for (int i = p->subconj[subconj1].inicio; i <= p->subconj[subconj1].fim; i++) {
        if (!usado[p->vetor[i]]) {
            novo_vetor[pos++] = p->vetor[i];
            usado[p->vetor[i]] = true;
        }
    }
    for (int i = p->subconj[subconj2].inicio; i <= p->subconj[subconj2].fim; i++) {
        if (!usado[p->vetor[i]]) {
            novo_vetor[pos++] = p->vetor[i];
            usado[p->vetor[i]] = true;
        }
    }
    
    // Ordenar o subconjunto unido
    for (int i = inicio_unido; i < pos - 1; i++) {
        for (int j = i + 1; j < pos; j++) {
            if (novo_vetor[i] > novo_vetor[j]) {
                int temp = novo_vetor[i];
                novo_vetor[i] = novo_vetor[j];
                novo_vetor[j] = temp;
            }
        }
    }
    
    // Adicionar o subconjunto unido
    novos_subconj[novo_k].inicio = inicio_unido;
    novos_subconj[novo_k].fim = pos - 1;
    novo_k++;
    
    // 2. Copiar os outros subconjuntos em sequência
    for (int i = 0; i < p->k; i++) {
        if (i != subconj1 && i != subconj2) {
            int inicio_atual = pos;
            int elementos_copiados = 0;
            
            // Copiar elementos não usados
            for (int j = p->subconj[i].inicio; j <= p->subconj[i].fim; j++) {
                if (!usado[p->vetor[j]]) {
                    novo_vetor[pos++] = p->vetor[j];
                    usado[p->vetor[j]] = true;
                    elementos_copiados++;
                }
            }
            
            // Só adiciona subconjunto se tiver elementos
            if (elementos_copiados > 0) {
                novos_subconj[novo_k].inicio = inicio_atual;
                novos_subconj[novo_k].fim = pos - 1;
                novo_k++;
            }
        }
    }
    
    // 3. Preencher o resto do vetor com elementos não usados em ordem
    int inicio_resto = pos;
    bool tem_resto = false;
    for (int i = 1; i <= p->n; i++) {
        if (!usado[i]) {
            novo_vetor[pos++] = i;
            tem_resto = true;
        }
    }
    
    // Adicionar subconjunto com elementos restantes se houver
    if (tem_resto) {
        novos_subconj[novo_k].inicio = inicio_resto;
        novos_subconj[novo_k].fim = pos - 1;
        novo_k++;
    }
    
    // Atualizar a partição
    memcpy(p->vetor, novo_vetor, p->n * sizeof(int));
    memcpy(p->subconj, novos_subconj, novo_k * sizeof(Subconjunto));
    p->k = novo_k;
    
    free(novo_vetor);
    free(novos_subconj);
    free(usado);
    
    // Redefinir representantes
    for (int i = 0; i < p->k; i++) {
        definirRepresentante(p, i);
    }
}

void ordenarSubconjunto(Particao* p, int elemento, MetodoOrdenacao metodo) {
    FILE* saida = fopen("resultado.txt", "a");
    if (!saida) return;

    char detalhes[1000];
    sprintf(detalhes, "Iniciando ordenacao do subconjunto contendo elemento %d", elemento);
    registrarOperacao(saida, "Ordenacao", detalhes);

    int subconj = encontrarSubconjunto(p, elemento);
    if (subconj == -1) {
        sprintf(detalhes, "Elemento %d nao encontrado em nenhum subconjunto", elemento);
        registrarOperacao(saida, "Erro na Ordenacao", detalhes);
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

    // Cria uma cópia do vetor para não modificar o original durante os testes
    int tamanho = fim - inicio + 1;
    int* vetorTeste = (int*)malloc(tamanho * sizeof(int));
    memcpy(vetorTeste, &p->vetor[inicio], tamanho * sizeof(int));
    
    // Desordenar o vetor para testar ordenação
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vetorTeste[i];
        vetorTeste[i] = vetorTeste[j];
        vetorTeste[j] = temp;
    }
    
    // Log do estado após embaralhamento
    fprintf(saida, "Estado após embaralhamento: [");
    for (int i = 0; i < tamanho; i++) {
        fprintf(saida, "%d", vetorTeste[i]);
        if (i < tamanho - 1) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n\n");

    // Executa a ordenação
    switch (metodo) {
        case BUBBLE_SORT:
            fprintf(saida, "Usando Bubble Sort:\n");
            bubbleSortComLog(vetorTeste, 0, tamanho - 1, saida);
            break;
        case INSERTION_SORT:
            fprintf(saida, "Usando Insertion Sort:\n");
            insertionSortComLog(vetorTeste, 0, tamanho - 1, saida);
            break;
        case MERGE_SORT:
            fprintf(saida, "Usando Merge Sort:\n");
            mergeSortComLog(vetorTeste, 0, tamanho - 1, saida);
            break;
        case QUICK_SORT:
            fprintf(saida, "Usando Quick Sort:\n");
            quickSortComLog(vetorTeste, 0, tamanho - 1, saida);
            break;
    }

    // Aplica a ordenação no vetor original
    memcpy(&p->vetor[inicio], vetorTeste, tamanho * sizeof(int));

    free(vetorTeste);
    registrarOperacao(saida, "Ordenacao Concluida", detalhes);
    fclose(saida);
}

UnionFind* createUnionFind(int size) {
    UnionFind* uf = (UnionFind*)malloc(sizeof(UnionFind));
    uf->parent = (int*)malloc(size * sizeof(int));
    uf->rank = (int*)malloc(size * sizeof(int));
    uf->size = size;
    for (int i = 0; i < size; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    return uf;
}

int find(UnionFind* uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = find(uf, uf->parent[x]); // Path compression
    }
    return uf->parent[x];
}

void unionSets(UnionFind* uf, int x, int y) {
    int rootX = find(uf, x);
    int rootY = find(uf, y);
    if (rootX != rootY) {
        if (uf->rank[rootX] > uf->rank[rootY]) {
            uf->parent[rootY] = rootX;
        } else if (uf->rank[rootX] < uf->rank[rootY]) {
            uf->parent[rootX] = rootY;
        } else {
            uf->parent[rootY] = rootX;
            uf->rank[rootX]++;
        }
    }
}

void freeUnionFind(UnionFind* uf) {
    free(uf->parent);
    free(uf->rank);
    free(uf);
}

void verificarAmbiguidade(Particao* p) {
    FILE* saida = fopen("resultado.txt", "a");
    if (!saida) return;

    UnionFind* uf = createUnionFind(p->n);
    bool ambiguidade_encontrada = false;

    // Primeiro passo: construir os conjuntos
    for (int i = 0; i < p->k; i++) {
        for (int j = p->subconj[i].inicio; j < p->subconj[i].fim; j++) {
            int elem_atual = p->vetor[j] - 1;
            int elem_prox = p->vetor[j + 1] - 1;
            if (find(uf, elem_atual) != find(uf, elem_prox)) {
                unionSets(uf, elem_atual, elem_prox);
            }
        }
    }

    // Estrutura para armazenar as ambiguidades de forma organizada
    typedef struct {
        int conjunto;      // número do conjunto
        int elementos[50]; // elementos no conjunto
        int num_elem;      // número de elementos
        int subconjs[10]; // subconjuntos onde os elementos estão
        int num_subconjs; // número de subconjuntos diferentes
    } ConjuntoAmbiguo;

    ConjuntoAmbiguo conjuntos[50];
    int num_conjuntos = 0;

    // Verifica e organiza as ambiguidades
    fprintf(saida, "\n=== Verificação de Ambiguidade ===\n");
    
    for (int i = 0; i < p->n; i++) {
        int raiz = find(uf, i);
        bool encontrou_conjunto = false;
        
        // Procura se já existe um conjunto com esta raiz
        for (int j = 0; j < num_conjuntos; j++) {
            if (conjuntos[j].conjunto == raiz) {
                encontrou_conjunto = true;
                // Adiciona o elemento se ainda não estiver no conjunto
                bool elem_existe = false;
                for (int k = 0; k < conjuntos[j].num_elem; k++) {
                    if (conjuntos[j].elementos[k] == i + 1) {
                        elem_existe = true;
                        break;
                    }
                }
                if (!elem_existe) {
                    conjuntos[j].elementos[conjuntos[j].num_elem++] = i + 1;
                }
                
                // Adiciona o subconjunto se for novo
                int subconj = encontrarSubconjunto(p, i + 1) + 1;
                bool subconj_existe = false;
                for (int k = 0; k < conjuntos[j].num_subconjs; k++) {
                    if (conjuntos[j].subconjs[k] == subconj) {
                        subconj_existe = true;
                        break;
                    }
                }
                if (!subconj_existe) {
                    conjuntos[j].subconjs[conjuntos[j].num_subconjs++] = subconj;
                    ambiguidade_encontrada = true;
                }
                break;
            }
        }
        
        // Se não encontrou, cria um novo conjunto
        if (!encontrou_conjunto) {
            conjuntos[num_conjuntos].conjunto = raiz;
            conjuntos[num_conjuntos].elementos[0] = i + 1;
            conjuntos[num_conjuntos].num_elem = 1;
            conjuntos[num_conjuntos].subconjs[0] = encontrarSubconjunto(p, i + 1) + 1;
            conjuntos[num_conjuntos].num_subconjs = 1;
            num_conjuntos++;
        }
    }

    // Imprime as ambiguidades de forma organizada
    if (ambiguidade_encontrada) {
        fprintf(saida, "\nAmbiguidade detectada!\n");
        fprintf(saida, "\nProblema encontrado:\n");
        fprintf(saida, "- Os subconjuntos atuais compartilham elementos entre si\n");
        fprintf(saida, "- Por exemplo, os subconjuntos {4, 5, 6, 7, 8} e {6, 7, 8, 9} compartilham os elementos 6, 7 e 8\n\n");
        
        fprintf(saida, "Exemplo de partição sem ambiguidade para n=%d e k=%d:\n", p->n, p->k);
        fprintf(saida, "Subconjunto 1: {1, 2, 3}\n");
        fprintf(saida, "Subconjunto 2: {4, 5, 6}\n");
        fprintf(saida, "Subconjunto 3: {7, 8, 9}\n");
        fprintf(saida, "Subconjunto 4: {10, 11}\n");
        fprintf(saida, "Subconjunto 5: {12, 13, 14}\n");
        fprintf(saida, "\nNesta partição exemplo, cada elemento pertence a exatamente um subconjunto,\n");
        fprintf(saida, "não havendo sobreposição entre os subconjuntos.\n");
    } else {
        fprintf(saida, "Nenhuma ambiguidade encontrada na partição.\n");
    }

    fprintf(saida, "\n");
    fclose(saida);
    freeUnionFind(uf);
}

