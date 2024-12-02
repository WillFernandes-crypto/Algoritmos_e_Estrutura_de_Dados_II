// paciente.h

#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdbool.h>
#include <time.h>

// Definições de constantes
#define MAX_PACIENTES 1000
#define MAX_NOME 100
#define DIAS_UTEIS_RETORNO 30
#define PRIORIDADE_RETORNO 8
#define PRIORIDADE_MINIMA 1
#define PRIORIDADE_MAXIMA 10
#define REDUCAO_PRIORIDADE_PORCENTAGEM 50
#define REDUCAO_PRIORIDADE_FALTA 2
#define CHANCE_FALTA 5
#define DIAS_POR_SEMANA 5

typedef struct {
    int id;
    char nome[MAX_NOME];
    int idade;
    float peso;
    float altura;
    char sintomas[MAX_NOME];
    char medicacoes[MAX_NOME];
    char telefone[20];
    int prioridade;
    int especialidade_id;
    char diagnostico_preliminar[MAX_NOME];
    bool precisa_retorno;
    int medico_anterior_id;
    int dias_ate_retorno;
    int faltas;
    bool atendido;
    int dia_ultima_consulta;
    int semana_ultima_consulta;
    bool aguardando_retorno;
    int dia_retorno;
    int semana_retorno;
    bool retorno_agendado;
    bool removido_por_faltas;
    int prioridade_original;
    time_t data_falta;
    int hora_retorno;
    bool retorno_realizado;
    int sala_retorno;
} Paciente;

Paciente pacientes[MAX_PACIENTES];

// Funções relacionadas ao paciente
void configurarRetorno(Paciente* paciente, int medico_id);
bool verificarFalta(Paciente* paciente);
int calcularPrioridadeRetorno(Paciente* paciente, int semana_atual, int dia_atual);
int calcularNovaPrioridade(int prioridade_atual);

#endif // PACIENTE_H
