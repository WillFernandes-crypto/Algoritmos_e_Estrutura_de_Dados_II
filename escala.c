#include "escala.h"
#include "hospital.h"
#include "agendamento.h"

void escalaConsultas(FILE* fp, HeapPrioridade* heap, int num_salas, int num_medicos) {
    int semana = 1;
    while (heap->tamanho > 0) {
        fprintf(fp, "SEMANA %d\n", semana);
        fprintf(fp, "--------\n\n");

        for (int dia = 0; dia < DIAS_POR_SEMANA; dia++) {
            fprintf(fp, "DIA %d\n", dia + 1);
            fprintf(fp, "-------\n\n");

            for (int hora = HORARIO_INICIO; hora < HORARIO_FIM; hora++) {
                if (hora >= HORARIO_ALMOCO && hora < HORARIO_RETORNO_ALMOCO) {
                    fprintf(fp, "%02d:00 - HORÁRIO DE ALMOÇO\n", hora);
                    continue;
                }

                fprintf(fp, "%02d:00\n", hora);

                AlocacaoDiaria alocacoes[MAX_SALAS] = {0};
                bool algum_atendimento = false;

                for (int sala = 0; sala < num_salas && heap->tamanho > 0; sala++) {
                    Paciente paciente = extrairMaximo(heap);

                    if (verificarFalta(&paciente)) {
                        if (paciente.removido_por_faltas) {
                            fprintf(fp, "Sala %d: VAZIA - Paciente %s removido da fila\n",
                                    sala + 1, paciente.nome);
                        } else {
                            fprintf(fp, "Sala %d: VAZIA - Paciente %s faltou\n",
                                    sala + 1, paciente.nome);
                        }
                    } else {
                        int medico_id = encontrarMedicoDisponivel(&paciente, alocacoes, num_salas, num_medicos);
                        if (medico_id >= 0) {
                            alocacoes[sala].alocado = true;
                            alocacoes[sala].paciente = paciente.id;
                            alocacoes[sala].medico = medico_id;
                            alocacoes[sala].sala = sala;
                            medicos[medico_id].horas_trabalhadas += 1;

                            agendarRetorno(&paciente, medico_id, dia + 1, semana);

                            fprintf(fp, "Sala %d: Dr(a). %s atende %s - %s\n",
                                    sala + 1,
                                    medicos[medico_id].nome,
                                    paciente.nome,
                                    especialidades[medicos[medico_id].especialidade_id].nome);

                            if (paciente.retorno_agendado) {
                                fprintf(fp, "        Retorno agendado para semana %d, dia %d, às %02d:00, Sala %d\n",
                                        paciente.semana_retorno,
                                        paciente.dia_retorno,
                                        paciente.hora_retorno,
                                        paciente.sala_retorno);
                            } else {
                                fprintf(fp, "        Não foi possível agendar retorno nos próximos 30 dias úteis\n");
                            }
                            inserirPacienteRetorno(heapRetorno, paciente);
                            algum_atendimento = true;
                        } else {
                            fprintf(fp, "Sala %d: VAZIA - Sem médico disponível\n", sala + 1);
                        }
                    }
                }

                if (!algum_atendimento && heap->tamanho == 0) break;
                fprintf(fp, "\n");
            }
        }
        semana++;
    }
}

void escalaConsultasRetorno(FILE* fp, HeapRetorno* heapRetorno, int num_salas, int num_medicos) {
    int semana_atual = 1;
    int max_semanas = 7; // Considerando até 30 dias úteis

    while (semana_atual <= max_semanas && heapRetorno->tamanho > 0) {
        fprintf(fp, "SEMANA %d\n", semana_atual);
        fprintf(fp, "--------\n\n");

        for (int dia = 0; dia < DIAS_POR_SEMANA; dia++) {
            fprintf(fp, "DIA %d\n", dia + 1);
            fprintf(fp, "-------\n");

            for (int hora = HORARIO_INICIO; hora < HORARIO_FIM; hora++) {
                if (hora >= HORARIO_ALMOCO && hora < HORARIO_RETORNO_ALMOCO) {
                    fprintf(fp, "\nHorário: %02d:00 - HORÁRIO DE ALMOÇO\n", hora);
                    continue;
                }

                fprintf(fp, "\nHorário: %02d:00\n", hora);
                fprintf(fp, "----------------\n");

                HeapRetorno* temp_heap = criarHeapRetorno(MAX_PACIENTES);
                bool algum_atendimento = false;

                while (heapRetorno->tamanho > 0) {
                    Paciente paciente = extrairMaximoRetorno(heapRetorno);

                    if (verificarFalta(&paciente)) {
                        fprintf(fp, "Sala %d: VAZIA - Paciente %s faltou ao retorno (nova prioridade: %d)\n",
                                paciente.sala_retorno, paciente.nome, paciente.prioridade);
                        if (!paciente.removido_por_faltas) {
                            agendarRetorno(&paciente, paciente.medico_anterior_id, dia, semana_atual);
                            inserirPacienteRetorno(temp_heap, paciente);
                        }
                        continue;
                    }

                    int medico_id = paciente.medico_anterior_id;
                    medicos[medico_id].horas_trabalhadas += 1;

                    fprintf(fp, "Sala %d: Dr(a). %s atende %s - %s (Retorno)\n",
                            paciente.sala_retorno,
                            medicos[medico_id].nome,
                            paciente.nome,
                            especialidades[medicos[medico_id].especialidade_id].nome);

                    algum_atendimento = true;
                }

                while (temp_heap->tamanho > 0) {
                    Paciente p = extrairMaximoRetorno(temp_heap);
                    inserirPacienteRetorno(heapRetorno, p);
                }
                destruirHeapRetorno(temp_heap);

                if (!algum_atendimento && heapRetorno->tamanho == 0) break;
            }
            fprintf(fp, "\n");
        }
        semana_atual++;
    }
}
