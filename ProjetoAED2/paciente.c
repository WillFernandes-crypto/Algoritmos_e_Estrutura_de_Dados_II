// paciente.c

#include "paciente.h"
#include <stdio.h>
#include <stdlib.h>

void configurarRetorno(Paciente* paciente, int medico_id) {
    paciente->precisa_retorno = true;
    paciente->medico_anterior_id = medico_id;
    paciente->dias_ate_retorno = DIAS_UTEIS_RETORNO;
    paciente->prioridade = PRIORIDADE_RETORNO;
}

bool verificarFalta(Paciente* paciente) {
    int chance = rand() % 100;

    if (chance < CHANCE_FALTA) {
        paciente->faltas++;

        if (paciente->faltas == 1) {
            paciente->prioridade_original = paciente->prioridade;
            paciente->prioridade = calcularNovaPrioridade(paciente->prioridade);

            if (paciente->prioridade == 0) {
                paciente->removido_por_faltas = true;
                fprintf(stderr, "Paciente %s removido por prioridade zero após falta\n",
                        paciente->nome);
                return true;
            }

            paciente->data_falta = time(NULL);
            return true;
        }
        else if (paciente->faltas > 1) {
            paciente->removido_por_faltas = true;
            fprintf(stderr, "Paciente %s removido por excesso de faltas\n",
                    paciente->nome);
            return true;
        }
    }
    return false;
}

int calcularPrioridadeRetorno(Paciente* paciente, int semana_atual, int dia_atual) {
    int dias_totais = (paciente->semana_retorno - semana_atual) * DIAS_POR_SEMANA +
                     (paciente->dia_retorno - dia_atual);

    int prioridade_base = PRIORIDADE_MINIMA + 1;

    if (dias_totais <= 5) {
        return PRIORIDADE_MAXIMA - 1;
    } else if (dias_totais <= 10) {
        return (PRIORIDADE_MAXIMA + prioridade_base) / 2;
    } else {
        return prioridade_base;
    }
}

int calcularNovaPrioridade(int prioridade_atual) {
    int nova_prioridade = (prioridade_atual * (100 - REDUCAO_PRIORIDADE_PORCENTAGEM)) / 100;
    return nova_prioridade > 0 ? nova_prioridade : 0;
}
