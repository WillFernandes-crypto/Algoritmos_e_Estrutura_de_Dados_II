// hospital.c

#include "hospital.h"
#include <stdio.h>
#include <stdlib.h>

Agendamento agendamentos[MAX_SALAS * DIAS_POR_SEMANA * 30]; // 30 dias úteis
int num_agendamentos = 0;

bool verificarDisponibilidadeSala(int sala, int dia, int semana, int hora) {
    for (int i = 0; i < num_agendamentos; i++) {
        if (agendamentos[i].sala == sala &&
            agendamentos[i].dia == dia &&
            agendamentos[i].semana == semana &&
            agendamentos[i].hora == hora) {
            return false;
        }
    }
    return true;
}

bool existeConflitoHorario(int medico_id, int dia, int semana, int hora) {
    for (int i = 0; i < num_agendamentos; i++) {
        if (agendamentos[i].medico_id == medico_id &&
            agendamentos[i].dia == dia &&
            agendamentos[i].semana == semana &&
            agendamentos[i].hora == hora) {
            return true;
        }
    }
    return false;
}

void registrarAgendamento(int medico_id, int sala, int dia, int semana, int hora) {
    agendamentos[num_agendamentos].medico_id = medico_id;
    agendamentos[num_agendamentos].sala = sala;
    agendamentos[num_agendamentos].dia = dia;
    agendamentos[num_agendamentos].semana = semana;
    agendamentos[num_agendamentos].hora = hora;
    agendamentos[num_agendamentos].ocupado = true;
    num_agendamentos++;
}

void calcularDataRetorno(int dia_atual, int semana_atual, int* dia_retorno, int* semana_retorno) {
    int dias_uteis_totais = DIAS_UTEIS_RETORNO;
    int semanas_completas = dias_uteis_totais / DIAS_POR_SEMANA;
    int dias_restantes = dias_uteis_totais % DIAS_POR_SEMANA;

    *semana_retorno = semana_atual + semanas_completas;
    *dia_retorno = dia_atual + dias_restantes;

    // Ajuste para garantir que os dias fiquem entre 1 e 5
    if (*dia_retorno > DIAS_POR_SEMANA) {
        *dia_retorno = (*dia_retorno % DIAS_POR_SEMANA);
        if (*dia_retorno == 0) *dia_retorno = DIAS_POR_SEMANA;
        (*semana_retorno)++;
    }

    // Garantir que nunca retorne 0
    if (*dia_retorno == 0) {
        *dia_retorno = 1;
    }
}
