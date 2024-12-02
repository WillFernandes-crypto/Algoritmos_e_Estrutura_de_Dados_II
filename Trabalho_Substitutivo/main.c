#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ordenacao.h"

clock_t inicio_global;

void mostrarMenu() {
    printf("\nMenu de Operacoes:\n");
    printf("1. Mostrar particao atual\n");
    printf("2. Encontrar representante de um elemento\n");
    printf("3. Unir dois subconjuntos\n");
    printf("4. Ordenar um subconjunto\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

void menuOrdenacao() {
    printf("\nMetodos de Ordenacao:\n");
    printf("1. Bubble Sort\n");
    printf("2. Insertion Sort\n");
    printf("3. Merge Sort\n");
    printf("4. Quick Sort\n");
    printf("Escolha um metodo: ");
}

void registrarOperacao(FILE* saida, const char* operacao, const char* detalhes) {
    FILE* arquivo = fopen("resultado.txt", "a");
    if (!arquivo) return;

    fprintf(arquivo, "\n=== %s ===\n", operacao);
    fprintf(arquivo, "Timestamp: %.6f segundos\n", 
            (double)(clock() - inicio_global) / CLOCKS_PER_SEC);
    fprintf(arquivo, "%s\n", detalhes);

    fclose(arquivo);
}

void salvarResultado(Particao* p, double tempo_total) {
    // Primeiro, vamos ler o conteúdo existente do arquivo
    FILE* entrada = fopen("resultado.txt", "rb");
    char* log_anterior = NULL;
    long tamanho = 0;
    
    if (entrada) {
        // Obtém o tamanho do arquivo
        fseek(entrada, 0, SEEK_END);
        tamanho = ftell(entrada);
        rewind(entrada);
        
        // Aloca memória e lê o conteúdo existente
        log_anterior = (char*)malloc(tamanho + 1);
        fread(log_anterior, 1, tamanho, entrada);
        log_anterior[tamanho] = '\0';
        fclose(entrada);
    }

    // Agora abre o arquivo para escrita
    FILE* saida = fopen("resultado.txt", "wb");
    if (!saida) {
        printf("Erro ao criar arquivo de saída!\n");
        free(log_anterior);
        return;
    }

    // Adiciona BOM UTF-8
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    fwrite(bom, sizeof(unsigned char), 3, saida);

    // Escreve o cabeçalho do log
    fprintf(saida, "====== Log de Operações ======\n");
    
    // Se existia conteúdo anterior, escreve ele
    if (log_anterior && tamanho > 0) {
        fprintf(saida, "%s", log_anterior);
    }
    
    // Escreve o resultado final
    fprintf(saida, "\n\n====== Resultado Final ======\n");
    fprintf(saida, "Tempo total de execução: %.6f segundos\n", tempo_total);
    fprintf(saida, "Tamanho do vetor (n): %d\n", p->n);
    fprintf(saida, "Número de subconjuntos (k): %d\n\n", p->k);

    // Estado final do vetor
    fprintf(saida, "Estado final do vetor:\n[");
    for (int i = 0; i < p->n; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < p->n - 1) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n\n");

    // Estado final dos subconjuntos
    fprintf(saida, "Estado final dos subconjuntos:\n");
    for (int i = 0; i < p->k; i++) {
        fprintf(saida, "Subconjunto %d: [", i+1);
        for (int j = p->subconj[i].inicio; j <= p->subconj[i].fim; j++) {
            fprintf(saida, "%d", p->vetor[j]);
            if (j < p->subconj[i].fim) fprintf(saida, ", ");
        }
        fprintf(saida, "] (início: %d, fim: %d)\n",
                p->subconj[i].inicio, p->subconj[i].fim);
    }

    fclose(saida);
    free(log_anterior);
    printf("\nResultado salvo em 'resultado.txt'\n");
}

int main() {
    inicio_global = clock();

    Particao* p = lerParticaoDeArquivo("dados.csv");
    if (!p) {
        printf("Erro ao ler arquivo de entrada!\n");
        return 1;
    }

    int opcao;
    do {
        mostrarMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                imprimirParticao(p);
                break;
            }
            case 2: {
                int elemento;
                printf("Digite o elemento para encontrar seu representante: ");
                scanf("%d", &elemento);
                imprimirRepresentante(p, elemento);
                break;
            }
            case 3: {
                int elem1, elem2;
                printf("Digite os dois elementos dos subconjuntos a serem unidos: ");
                scanf("%d %d", &elem1, &elem2);
                unirSubconjuntos(p, elem1, elem2);
                break;
            }
            case 4: {
                int elemento, metodo;
                printf("Digite o elemento do subconjunto a ser ordenado: ");
                scanf("%d", &elemento);
                menuOrdenacao();
                scanf("%d", &metodo);
                ordenarSubconjunto(p, elemento, (MetodoOrdenacao)(metodo - 1));
                break;
            }
            case 0:
                printf("Encerrando programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    // Calcula o tempo total
    clock_t fim = clock();
    double tempo_total = (double)(fim - inicio_global) / CLOCKS_PER_SEC;

    // Imprime e salva os resultados
    printf("\nTempo total de execução: %.6f segundos\n", tempo_total);
    salvarResultado(p, tempo_total);

    liberarParticao(p);
    return 0;
}
