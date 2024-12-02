#ifndef HEAP_H
#define HEAP_H

#include "paciente.h"

// Estruturas
typedef struct {
    Paciente* pacientes;
    int tamanho;
    int capacidade;
} HeapPrioridade;

typedef struct {
    Paciente* pacientes;
    int tamanho;
    int capacidade;
} HeapRetorno;

HeapRetorno* heapRetorno;

// Protótipos das funções
HeapPrioridade* criarHeap(int capacidade);
void destruirHeap(HeapPrioridade* heap);
void inserirPaciente(HeapPrioridade* heap, Paciente paciente);
Paciente extrairMaximo(HeapPrioridade* heap);
void heapify(HeapPrioridade* heap, int i);

HeapRetorno* criarHeapRetorno(int capacidade);
void destruirHeapRetorno(HeapRetorno* heap);
void inserirPacienteRetorno(HeapRetorno* heap, Paciente paciente);
Paciente extrairMaximoRetorno(HeapRetorno* heap);
void heapifyRetorno(HeapRetorno* heap, int i);

#endif // HEAP_H
