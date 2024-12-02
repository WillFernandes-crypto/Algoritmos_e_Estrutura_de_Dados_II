#ifndef ESCALA_H
#define ESCALA_H

#include <stdio.h>
#include "paciente.h"
#include "medico.h"
#include "heap.h"

// Protótipos das funções de escala
void escalaConsultas(FILE* fp, HeapPrioridade* heap, int num_salas, int num_medicos);
void escalaConsultasRetorno(FILE* fp, HeapRetorno* heapRetorno, int num_salas, int num_medicos);

#endif // ESCALA_H
