#include "relatorio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gerarRelatorioFinal(const char* arquivo_saida, HeapPrioridade* heap, HeapRetorno* heapRetorno, int num_salas, int num_medicos) {
    FILE* fp = fopen(arquivo_saida, "w");
    if (!fp) {
        fprintf(stderr, "Erro ao criar arquivo de saída\n");
        return;
    }

    fprintf(fp, "ESCALA DE CONSULTAS\n");
    fprintf(fp, "===================\n\n");
    escalaConsultas(fp, heap, num_salas, num_medicos);

    fprintf(fp, "\nESCALA DE CONSULTAS DE RETORNO\n");
    fprintf(fp, "=============================\n\n");
    escalaConsultasRetorno(fp, heapRetorno, num_salas, num_medicos);

    fprintf(fp, "\nRELATÓRIO DE HORAS TRABALHADAS POR MÉDICO\n");
    fprintf(fp, "========================================\n");

    Medico* medicosOrdenados = (Medico*)malloc(num_medicos * sizeof(Medico));
    memcpy(medicosOrdenados, medicos, num_medicos * sizeof(Medico));
    qsort(medicosOrdenados, num_medicos, sizeof(Medico), comparaMedicoHoras);

    for (int i = 0; i < num_medicos; i++) {
        fprintf(fp, "Dr(a). %s (%s): %d horas\n",
                medicosOrdenados[i].nome,
                especialidades[medicosOrdenados[i].especialidade_id].nome,
                medicosOrdenados[i].horas_trabalhadas);
    }

    free(medicosOrdenados);
    fclose(fp);
}

void gerarRelatorioHorasTrabalhadas(FILE* fp, Medico* medicos, int num_medicos) {
    fprintf(fp, "\nRELATÓRIO DE HORAS TRABALHADAS POR MÉDICO\n");
    fprintf(fp, "========================================\n");

    for (int i = 0; i < num_medicos; i++) {
        fprintf(fp, "Dr(a). %s (%s): %d horas\n",
                medicos[i].nome,
                especialidades[medicos[i].especialidade_id].nome,
                medicos[i].horas_trabalhadas);
    }
}
