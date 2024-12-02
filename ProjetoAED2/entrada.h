#ifndef ENTRADA_H
#define ENTRADA_H

#include "paciente.h"
#include "medico.h"
#include "hospital.h"

// Protótipos das funções de entrada
void lerArquivoEntrada(const char* arquivo, int* num_pacientes, int* num_salas,
                       int* num_especialidades, int* num_medicos);

#endif // ENTRADA_H
