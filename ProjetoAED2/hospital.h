// hospital.h

#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdbool.h>
#include "paciente.h"
#include "medico.h"

#define MAX_SALAS 20
#define DIAS_POR_SEMANA 5
#define HORARIO_INICIO 8
#define HORARIO_ALMOCO 12
#define HORARIO_RETORNO_ALMOCO 14
#define HORARIO_FIM 18
#define MAX_ESPECIALIDADES 10

typedef struct {
    int id;
    bool ocupada;
} Sala;

typedef struct {
    int id;
    char nome[MAX_NOME];
    int num_medicos;
} Especialidade;

typedef struct {
    int medico_id;
    int sala;
    int hora;
    int dia;
    int semana;
    bool ocupado;
} Agendamento;

Especialidade especialidades[MAX_ESPECIALIDADES];
Sala salas[MAX_SALAS];

// Funções relacionadas ao hospital
bool verificarDisponibilidadeSala(int sala, int dia, int semana, int hora);
bool existeConflitoHorario(int medico_id, int dia, int semana, int hora);
void registrarAgendamento(int medico_id, int sala, int dia, int semana, int hora);
void calcularDataRetorno(int dia_atual, int semana_atual, int* dia_retorno, int* semana_retorno);

#endif // HOSPITAL_H
