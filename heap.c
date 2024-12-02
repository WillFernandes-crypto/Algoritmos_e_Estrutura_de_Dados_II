#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "paciente.h"

void trocarPacientes(Paciente* a, Paciente* b) {
    Paciente temp = *a;
    *a = *b;
    *b = temp;
}

HeapPrioridade* criarHeap(int capacidade) {
    HeapPrioridade* heap = (HeapPrioridade*)malloc(sizeof(HeapPrioridade));
    if (!heap) {
        fprintf(stderr, "Erro ao alocar memória para o heap\n");
        exit(1);
    }

    heap->pacientes = (Paciente*)malloc(capacidade * sizeof(Paciente));
    if (!heap->pacientes) {
        fprintf(stderr, "Erro ao alocar memória para os pacientes\n");
        free(heap);
        exit(1);
    }

    heap->capacidade = capacidade;
    heap->tamanho = 0;
    return heap;
}

void destruirHeap(HeapPrioridade* heap) {
    free(heap->pacientes);
    free(heap);
}

void inserirPaciente(HeapPrioridade* heap, Paciente paciente) {
    if (heap->tamanho == heap->capacidade) {
        fprintf(stderr, "Heap cheio\n");
        return;
    }

    heap->tamanho++;
    int i = heap->tamanho - 1;
    heap->pacientes[i] = paciente;

    while (i != 0 && heap->pacientes[(i-1)/2].prioridade < heap->pacientes[i].prioridade) {
        trocarPacientes(&heap->pacientes[i], &heap->pacientes[(i-1)/2]);
        i = (i-1)/2;
    }
}

Paciente extrairMaximo(HeapPrioridade* heap) {
    if (heap->tamanho <= 0)
        return pacientes[0]; // Retorna paciente vazio em caso de erro

    if (heap->tamanho == 1) {
        heap->tamanho--;
        return heap->pacientes[0];
    }

    Paciente raiz = heap->pacientes[0];
    heap->pacientes[0] = heap->pacientes[heap->tamanho-1];
    heap->tamanho--;
    heapify(heap, 0);

    return raiz;
}

void heapify(HeapPrioridade* heap, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < heap->tamanho &&
        heap->pacientes[esquerda].prioridade > heap->pacientes[maior].prioridade)
        maior = esquerda;

    if (direita < heap->tamanho &&
        heap->pacientes[direita].prioridade > heap->pacientes[maior].prioridade)
        maior = direita;

    if (maior != i) {
        trocarPacientes(&heap->pacientes[i], &heap->pacientes[maior]);
        heapify(heap, maior);
    }
}

HeapRetorno* criarHeapRetorno(int capacidade) {
    HeapRetorno* heap = (HeapRetorno*)malloc(sizeof(HeapRetorno));
    if (!heap) {
        fprintf(stderr, "Erro ao alocar memória para o heap de retorno\n");
        exit(1);
    }

    heap->pacientes = (Paciente*)malloc(capacidade * sizeof(Paciente));
    if (!heap->pacientes) {
        fprintf(stderr, "Erro ao alocar memória para os pacientes de retorno\n");
        free(heap);
        exit(1);
    }

    heap->capacidade = capacidade;
    heap->tamanho = 0;
    return heap;
}

void destruirHeapRetorno(HeapRetorno* heap) {
    free(heap->pacientes);
    free(heap);
}

void inserirPacienteRetorno(HeapRetorno* heap, Paciente paciente) {
    if (heap->tamanho == heap->capacidade) {
        fprintf(stderr, "Heap de retorno cheio\n");
        return;
    }

    heap->tamanho++;
    int i = heap->tamanho - 1;
    heap->pacientes[i] = paciente;

    while (i != 0 && heap->pacientes[(i-1)/2].prioridade < heap->pacientes[i].prioridade) {
        trocarPacientes(&heap->pacientes[i], &heap->pacientes[(i-1)/2]);
        i = (i-1)/2;
    }
}

Paciente extrairMaximoRetorno(HeapRetorno* heap) {
    if (heap->tamanho <= 0)
        return pacientes[0]; // Retorna paciente vazio em caso de erro

    if (heap->tamanho == 1) {
        heap->tamanho--;
        return heap->pacientes[0];
    }

    Paciente raiz = heap->pacientes[0];
    heap->pacientes[0] = heap->pacientes[heap->tamanho-1];
    heap->tamanho--;
    heapifyRetorno(heap, 0);

    return raiz;
}

void heapifyRetorno(HeapRetorno* heap, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < heap->tamanho &&
        heap->pacientes[esquerda].prioridade > heap->pacientes[maior].prioridade)
        maior = esquerda;

    if (direita < heap->tamanho &&
        heap->pacientes[direita].prioridade > heap->pacientes[maior].prioridade)
        maior = direita;

    if (maior != i) {
        trocarPacientes(&heap->pacientes[i], &heap->pacientes[maior]);
        heapifyRetorno(heap, maior);
    }
}
