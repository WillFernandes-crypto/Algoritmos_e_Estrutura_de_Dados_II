#include "entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include "hospital.h"

void lerArquivoEntrada(const char* arquivo, int* num_pacientes, int* num_salas,
                       int* num_especialidades, int* num_medicos) {
    FILE* fp = fopen(arquivo, "r");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir arquivo de entrada\n");
        exit(1);
    }

    // Lê primeira linha com números básicos
    if (fscanf(fp, "%d,%d,%d,%d\n", num_pacientes, num_salas,
               num_especialidades, num_medicos) != 4) {
        fprintf(stderr, "Erro ao ler configurações básicas\n");
        exit(1);
    }

    // Lê especialidades
    for (int i = 0; i < *num_especialidades; i++) {
        char linha[256];
        if (!fgets(linha, sizeof(linha), fp)) {
            fprintf(stderr, "Erro ao ler especialidade %d\n", i);
            exit(1);
        }
        sscanf(linha, "%[^,],%d", especialidades[i].nome, &especialidades[i].num_medicos);
        especialidades[i].id = i;
    }

    // Lê médicos
    for (int i = 0; i < *num_medicos; i++) {
        char linha[256];
        if (!fgets(linha, sizeof(linha), fp)) {
            fprintf(stderr, "Erro ao ler médico %d\n", i);
            exit(1);
        }
        sscanf(linha, "%[^,],%d", medicos[i].nome, &medicos[i].especialidade_id);
        medicos[i].id = i;
        medicos[i].horas_trabalhadas = 0;
    }

    // Lê pacientes
    for (int i = 0; i < *num_pacientes; i++) {
        char linha[512];
        if (!fgets(linha, sizeof(linha), fp)) {
            fprintf(stderr, "Erro ao ler paciente %d\n", i);
            exit(1);
        }
        sscanf(linha, "%d,%[^,],%d,%f,%f,%[^,],%[^,],%[^,],%d,%d,%[^\n]",
               &pacientes[i].id,
               pacientes[i].nome,
               &pacientes[i].idade,
               &pacientes[i].peso,
               &pacientes[i].altura,
               pacientes[i].sintomas,
               pacientes[i].medicacoes,
               pacientes[i].telefone,
               &pacientes[i].prioridade,
               &pacientes[i].especialidade_id,
               pacientes[i].diagnostico_preliminar);

        pacientes[i].faltas = 0;
        pacientes[i].atendido = false;
        pacientes[i].precisa_retorno = false;
        pacientes[i].removido_por_faltas = false;
        pacientes[i].aguardando_retorno = false;
        pacientes[i].retorno_agendado = false;
    }

    fclose(fp);

    // Inicializa o contador de médicos por especialidade
    int medicos_por_especialidade[MAX_ESPECIALIDADES] = {0};

    // Conta quantos médicos existem para cada especialidade
    for (int i = 0; i < *num_medicos; i++) {
        if (medicos[i].especialidade_id >= 0 &&
            medicos[i].especialidade_id < *num_especialidades) {
            medicos_por_especialidade[medicos[i].especialidade_id]++;
        } else {
            fprintf(stderr, "Erro: Médico %s com especialidade inválida\n",
                    medicos[i].nome);
            exit(1);
        }
    }

    // Verifica se cada especialidade tem pelo menos um médico
    for (int i = 0; i < *num_especialidades; i++) {
        if (medicos_por_especialidade[i] == 0) {
            fprintf(stderr, "Erro: Especialidade %s não possui nenhum médico\n",
                    especialidades[i].nome);
            exit(1);
        }

        // Verifica se há médicos suficientes conforme especificado
        if (medicos_por_especialidade[i] < especialidades[i].num_medicos) {
            fprintf(stderr, "Aviso: Especialidade %s possui apenas %d médico(s) dos %d especificados\n",
                    especialidades[i].nome,
                    medicos_por_especialidade[i],
                    especialidades[i].num_medicos);
            // Atualiza o número real de médicos da especialidade
            especialidades[i].num_medicos = medicos_por_especialidade[i];
        }
    }
}
