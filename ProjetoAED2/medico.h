// medico.h

#ifndef MEDICO_H
#define MEDICO_H

#include <stdbool.h>
#include "paciente.h"

#define MAX_MEDICOS 50

typedef struct {
    int id;
    char nome[MAX_NOME];
    int especialidade_id;
    int horas_trabalhadas;
} Medico;

typedef struct {
    Medico* medicos;
    int tamanho;
    int capacidade;
} HeapMinimoMedicos;

Medico medicos[MAX_MEDICOS];
int num_medicos;

// Funções relacionadas ao mdico
HeapMinimoMedicos* criarHeapMinimo(int capacidade);
void trocarMedicos(Medico* a, Medico* b);
void heapifyMinimo(HeapMinimoMedicos* heap, int i);
void inserirMedicoHeapMinimo(HeapMinimoMedicos* heap, Medico medico);
Medico extrairMinimo(HeapMinimoMedicos* heap);
void destruirHeapMinimo(HeapMinimoMedicos* heap);
int comparaMedicoHoras(const void* a, const void* b);

#endif // MEDICO_H
