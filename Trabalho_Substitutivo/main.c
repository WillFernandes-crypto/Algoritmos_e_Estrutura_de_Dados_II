#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ordenacao.h"

clock_t inicio_global;
FILE* arquivo_log = NULL;

void abrirArquivoLog() {
    // Abre o arquivo em modo write para começar um novo log
    arquivo_log = fopen("resultado.txt", "w");
    if (arquivo_log) {
        fprintf(arquivo_log, "====== Log de Operações ======\n\n");
        fclose(arquivo_log);
        arquivo_log = NULL;
    }
}

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
    if (!saida) return;

    // Adiciona uma linha em branco antes de cada operação para melhor legibilidade
    fprintf(saida, "\n");
    
    // Cabeçalho da operação com timestamp
    fprintf(saida, "=== %s (%.3f segundos) ===\n", 
            operacao, (double)(clock() - inicio_global) / CLOCKS_PER_SEC);
    
    // Detalhes da operação
    fprintf(saida, "%s\n", detalhes);
}

void salvarResultado(Particao* p, double tempo_total) {
    // Abre o arquivo em modo append
    FILE* saida = fopen("resultado.txt", "a");
    if (!saida) return;

    // Escreve o resultado final
    fprintf(saida, "\n====== Resultado Final ======\n");
    fprintf(saida, "Tempo total de execução: %.6f segundos\n", tempo_total);
    fprintf(saida, "Tamanho do vetor (n): %d\n", p->n);
    fprintf(saida, "Número de subconjuntos (k): %d\n\n", p->k);

    fprintf(saida, "Estado final do vetor:\n[");
    for (int i = 0; i < p->n; i++) {
        fprintf(saida, "%d", p->vetor[i]);
        if (i < p->n - 1) fprintf(saida, ", ");
    }
    fprintf(saida, "]\n\n");

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
}

int main() {
    inicio_global = clock();
    abrirArquivoLog();  // Abre o arquivo de log no início

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

    // Antes de salvar o resultado final, fechar o arquivo de log
    if (arquivo_log) {
        fclose(arquivo_log);
        arquivo_log = NULL;
    }

    clock_t fim = clock();
    double tempo_total = (double)(fim - inicio_global) / CLOCKS_PER_SEC;

    printf("\nTempo total de execução: %.6f segundos\n", tempo_total);
    salvarResultado(p, tempo_total);

    liberarParticao(p);
    return 0;
}
