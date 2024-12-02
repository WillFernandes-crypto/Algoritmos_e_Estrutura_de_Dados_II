#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

#include "paciente.h"
#include "medico.h"
#include "hospital.h"

typedef struct {
    bool alocado;
    int paciente;
    int medico;
    int sala;
} AlocacaoDiaria;

// Protótipos das funções de agendamento
void agendarRetorno(Paciente* paciente, int medico_id, int dia_atual, int semana_atual);
bool verificarPrazoRetorno(Paciente paciente, int dia_atual, int semana_atual);
int encontrarMedicoDisponivel(Paciente* paciente, AlocacaoDiaria* alocacoes, int num_salas, int num_medicos);
bool verificarDisponibilidadeMedico(int medico_id, AlocacaoDiaria* alocacoes, int num_salas);

#endif // AGENDAMENTO_H
