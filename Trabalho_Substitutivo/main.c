#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"

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

void salvarResultado(Particao* p, double tempo_total) {
    FILE* saida = fopen("resultado.txt", "wb");
    if (!saida) {
        printf("Erro ao criar arquivo de saída!\n");
        return;
    }

    // Adiciona BOM UTF-8
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    fwrite(bom, sizeof(unsigned char), 3, saida);

    // Cabeçalho com informações gerais
    fprintf(saida, "=== Resultado da Execução ===\n\n");
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
    printf("\nResultado salvo em 'resultado.txt'\n");
}

int main() {

    clock_t inicio = clock();  // Inicia contagem do tempo

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
    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Imprime e salva os resultados
    printf("\nTempo total de execução: %.6f segundos\n", tempo_total);
    salvarResultado(p, tempo_total);

    liberarParticao(p);
    return 0;
}
