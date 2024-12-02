#include "agendamento.h"
#include <stdio.h>
#include <stdlib.h>
#include "medico.h"

void agendarRetorno(Paciente* paciente, int medico_id, int dia_atual, int semana_atual) {
    paciente->precisa_retorno = true;
    paciente->medico_anterior_id = medico_id;
    paciente->dia_ultima_consulta = dia_atual;
    paciente->semana_ultima_consulta = semana_atual;

    int semana_tentativa = semana_atual + 1;
    int max_semanas = semana_atual + 6; // 30 dias úteis = 6 semanas
    bool agendado = false;

    for (int semana = semana_tentativa; semana <= max_semanas && !agendado; semana++) {
        for (int dia = 1; dia <= DIAS_POR_SEMANA && !agendado; dia++) {
            for (int hora = HORARIO_INICIO; hora < HORARIO_FIM; hora++) {
                if (hora >= HORARIO_ALMOCO && hora < HORARIO_RETORNO_ALMOCO) {
                    continue;
                }

                if (!existeConflitoHorario(medico_id, dia, semana, hora)) {
                    for (int sala = 0; sala < MAX_SALAS; sala++) {
                        if (verificarDisponibilidadeSala(sala + 1, dia, semana, hora)) {
                            paciente->dia_retorno = dia;
                            paciente->semana_retorno = semana;
                            paciente->hora_retorno = hora;
                            paciente->sala_retorno = sala + 1;

                            registrarAgendamento(medico_id, sala + 1, dia, semana, hora);
                            agendado = true;
                            break;
                        }
                    }
                }
                if (agendado) break;
            }
        }
    }

    paciente->prioridade = calcularPrioridadeRetorno(paciente, semana_atual, dia_atual);
    paciente->aguardando_retorno = true;
    paciente->retorno_agendado = agendado;
}

bool verificarPrazoRetorno(Paciente paciente, int dia_atual, int semana_atual) {
    int dias_totais = (semana_atual - paciente.semana_ultima_consulta) * DIAS_POR_SEMANA +
                     (dia_atual - paciente.dia_ultima_consulta);
    return dias_totais <= DIAS_UTEIS_RETORNO;
}

int encontrarMedicoDisponivel(Paciente* paciente, AlocacaoDiaria* alocacoes, int num_salas, int num_medicos) {
    HeapMinimoMedicos* heapEspecialidade = criarHeapMinimo(num_medicos);

    for (int i = 0; i < num_medicos; i++) {
        if (medicos[i].especialidade_id == paciente->especialidade_id) {
            inserirMedicoHeapMinimo(heapEspecialidade, medicos[i]);
        }
    }

    while (heapEspecialidade->tamanho > 0) {
        Medico medico = extrairMinimo(heapEspecialidade);
        if (verificarDisponibilidadeMedico(medico.id, alocacoes, num_salas)) {
            destruirHeapMinimo(heapEspecialidade);
            return medico.id;
        }
    }

    destruirHeapMinimo(heapEspecialidade);
    return -1; // Nenhum médico disponível
}

bool verificarDisponibilidadeMedico(int medico_id, AlocacaoDiaria* alocacoes, int num_salas) {
    for (int i = 0; i < num_salas; i++) {
        if (alocacoes[i].alocado && alocacoes[i].medico == medico_id) {
            return false;
        }
    }
    return true;
}
