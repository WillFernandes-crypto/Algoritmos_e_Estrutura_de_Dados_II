#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "paciente.h"
#include "medico.h"
#include "hospital.h"
#include "heap.h"
#include "agendamento.h"
#include "relatorio.h"
#include "escala.h"
#include "entrada.h"

int main(int argc, char *argv[]) {
    int num_pacientes, num_salas, num_especialidades, num_medicos;
    const char* arquivo_entrada = "entrada.csv";
    const char* arquivo_saida = "relatorio.txt";

    srand(time(NULL));

    lerArquivoEntrada(arquivo_entrada, &num_pacientes, &num_salas,
                      &num_especialidades, &num_medicos);

    // Validações conforme especificação
    if (num_medicos < num_salas) {
        fprintf(stderr, "Erro: Número de médicos deve ser maior ou igual ao número de salas\n");
        return 1;
    }

    int medicos_por_especialidade[MAX_ESPECIALIDADES] = {0};
    for (int i = 0; i < num_medicos; i++) {
        medicos_por_especialidade[medicos[i].especialidade_id]++;
    }

    for (int i = 0; i < num_especialidades; i++) {
        if (medicos_por_especialidade[i] == 0) {
            fprintf(stderr, "Erro: Especialidade %s não possui médicos\n",
                    especialidades[i].nome);
            return 1;
        }
    }

    // Inicialização dos heaps
    HeapPrioridade* heap = criarHeap(MAX_PACIENTES);  // Aumentado para MAX_PACIENTES
    heapRetorno = criarHeapRetorno(MAX_PACIENTES);    // Inicialização da variável global

    // Preencher o heap com os pacientes
    for (int i = 0; i < num_pacientes; i++) {
        inserirPaciente(heap, pacientes[i]);
    }

    // Inicializar salas
    for (int i = 0; i < num_salas; i++) {
        salas[i].id = i;
        salas[i].ocupada = false;
    }

    // Gerar relatório
    FILE* fp = fopen(arquivo_saida, "w");
    if (!fp) {
        fprintf(stderr, "Erro ao criar arquivo de saída\n");
        return 1;
    }

    fprintf(fp, "ESCALA DE CONSULTAS\n");
    fprintf(fp, "===================\n\n");
    escalaConsultas(fp, heap, num_salas, num_medicos);

    fprintf(fp, "\nESCALA DE CONSULTAS DE RETORNO\n");
    fprintf(fp, "=============================\n\n");
    escalaConsultasRetorno(fp, heapRetorno, num_salas, num_medicos);

    // Relatório de horas trabalhadas
    fprintf(fp, "\nRELATÓRIO DE HORAS TRABALHADAS POR MÉDICO\n");
    fprintf(fp, "========================================\n\n");

    // Ordenar médicos por horas trabalhadas
    qsort(medicos, num_medicos, sizeof(Medico), comparaMedicoHoras);

    for (int i = 0; i < num_medicos; i++) {
        fprintf(fp, "Dr(a). %s (%s): %d horas\n",
                medicos[i].nome,
                especialidades[medicos[i].especialidade_id].nome,
                medicos[i].horas_trabalhadas);
    }

    fclose(fp);

    // Liberar memória
    destruirHeap(heap);
    destruirHeapRetorno(heapRetorno);

    printf("Processamento concluído. Verifique o arquivo '%s'\n", arquivo_saida);
    return 0;
}
