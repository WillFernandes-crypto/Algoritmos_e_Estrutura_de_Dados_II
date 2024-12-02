// medico.c

#include "medico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HeapMinimoMedicos* criarHeapMinimo(int capacidade) {
    HeapMinimoMedicos* heap = (HeapMinimoMedicos*)malloc(sizeof(HeapMinimoMedicos));
    heap->medicos = (Medico*)malloc(capacidade * sizeof(Medico));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

void trocarMedicos(Medico* a, Medico* b) {
    Medico temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyMinimo(HeapMinimoMedicos* heap, int i) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < heap->tamanho && heap->medicos[esquerda].horas_trabalhadas < heap->medicos[menor].horas_trabalhadas)
        menor = esquerda;

    if (direita < heap->tamanho && heap->medicos[direita].horas_trabalhadas < heap->medicos[menor].horas_trabalhadas)
        menor = direita;

    if (menor != i) {
        trocarMedicos(&heap->medicos[i], &heap->medicos[menor]);
        heapifyMinimo(heap, menor);
    }
}

void inserirMedicoHeapMinimo(HeapMinimoMedicos* heap, Medico medico) {
    if (heap->tamanho == heap->capacidade) {
        fprintf(stderr, "Heap cheio\n");
        return;
    }

    heap->tamanho++;
    int i = heap->tamanho - 1;
    heap->medicos[i] = medico;

    while (i != 0 && heap->medicos[(i-1)/2].horas_trabalhadas > heap->medicos[i].horas_trabalhadas) {
        trocarMedicos(&heap->medicos[i], &heap->medicos[(i-1)/2]);
        i = (i-1)/2;
    }
}

Medico extrairMinimo(HeapMinimoMedicos* heap) {
    if (heap->tamanho <= 0) {
        Medico medico_vazio = {0};
        return medico_vazio;
    }

    if (heap->tamanho == 1) {
        heap->tamanho--;
        return heap->medicos[0];
    }

    Medico raiz = heap->medicos[0];
    heap->medicos[0] = heap->medicos[heap->tamanho-1];
    heap->tamanho--;
    heapifyMinimo(heap, 0);

    return raiz;
}

void destruirHeapMinimo(HeapMinimoMedicos* heap) {
    free(heap->medicos);
    free(heap);
}

int comparaMedicoHoras(const void* a, const void* b) {
    const Medico* medicoA = (const Medico*)a;
    const Medico* medicoB = (const Medico*)b;
    // Ordenação decrescente por horas
    if (medicoB->horas_trabalhadas != medicoA->horas_trabalhadas) {
        return medicoB->horas_trabalhadas - medicoA->horas_trabalhadas;
    }
    // Em caso de empate, ordenar por nome
    return strcmp(medicoA->nome, medicoB->nome);
}
