#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Definições de constantes atualizadas
#define MAX_NOME 100
#define MAX_ESPECIALIDADES 10
#define MAX_MEDICOS 50
#define MAX_PACIENTES 1000
#define MAX_SALAS 20
#define DIAS_POR_SEMANA 5
#define HORARIO_INICIO 8
#define HORARIO_ALMOCO 12
#define HORARIO_RETORNO_ALMOCO 14
#define HORARIO_FIM 18
#define CHANCE_FALTA 5
#define DIAS_UTEIS_RETORNO 30
#define REDUCAO_PRIORIDADE_FALTA 2
#define PRIORIDADE_RETORNO 8
#define PRIORIDADE_MAXIMA 10
#define PRIORIDADE_MINIMA 1

// Estruturas
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
} Paciente;

typedef struct {
    int id;
    char nome[MAX_NOME];
    int especialidade_id;
    int horas_trabalhadas;
} Medico;

typedef struct {
    int id;
    char nome[MAX_NOME];
    int num_medicos;
} Especialidade;

typedef struct {
    int id;
    bool ocupada;
} Sala;

typedef struct {
    bool alocado;
    int paciente;
    int medico;
    int sala;
} AlocacaoDiaria;

typedef struct {
    char nome[MAX_NOME];
    int horas;
    int especialidade_id;
} MedicoRelatorio;

typedef struct {
    Paciente* pacientes;
    int tamanho;
    int capacidade;
} HeapPrioridade;

// Protótipos das funções
int comparaMedicoHoras(const void* a, const void* b);
void agendarRetorno(Paciente* paciente, int medico_id, int dia_atual, int semana_atual);
bool verificarPrazoRetorno(Paciente paciente, int dia_atual, int semana_atual);
int encontrarMedicoDisponivel(Paciente* paciente, AlocacaoDiaria* alocacoes,
                             int num_salas, int num_medicos);
bool verificarDisponibilidadeMedico(int medico_id, AlocacaoDiaria* alocacoes, int num_salas);
void ordenarMedicosPorHorasTrabalhadas(void);
void lerArquivoEntrada(const char* arquivo, int* num_pacientes, int* num_salas,
                       int* num_especialidades, int* num_medicos);
void gerarEscala(const char* arquivo_saida, int num_pacientes, int num_salas,
                 int num_especialidades, int num_medicos);
void gerarRelatorioHorasTrabalhadas(FILE* fp, int num_medicos);
Paciente extrairMaximo(HeapPrioridade* heap);
void calcularDataRetorno(int dia_atual, int semana_atual, int* dia_retorno, int* semana_retorno);
bool verificarFalta(Paciente* paciente);
int calcularPrioridadeRetorno(Paciente* paciente, int semana_atual, int dia_atual);
void gerarRelatorioCompleto(FILE* fp, int semana, int num_medicos);

// Variáveis globais
Paciente pacientes[MAX_PACIENTES];
Especialidade especialidades[MAX_ESPECIALIDADES];
Medico medicos[MAX_MEDICOS];
Sala salas[MAX_SALAS];
int num_medicos;

// Funções do Heap
void trocarPacientes(Paciente* a, Paciente* b) {
    Paciente temp = *a;
    *a = *b;
    *b = temp;
}

HeapPrioridade* criarHeap(int capacidade) {
    HeapPrioridade* heap = (HeapPrioridade*)malloc(sizeof(HeapPrioridade));
    if (!heap) {
        fprintf(stderr, "Erro ao alocar memória para o heap\n");
        exit(1);
    }

    heap->pacientes = (Paciente*)malloc(capacidade * sizeof(Paciente));
    if (!heap->pacientes) {
        fprintf(stderr, "Erro ao alocar memória para os pacientes\n");
        free(heap);
        exit(1);
    }

    heap->capacidade = capacidade;
    heap->tamanho = 0;
    return heap;
}

void heapify(HeapPrioridade* heap, int i) {
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < heap->tamanho &&
        heap->pacientes[esquerda].prioridade > heap->pacientes[maior].prioridade)
        maior = esquerda;

    if (direita < heap->tamanho &&
        heap->pacientes[direita].prioridade > heap->pacientes[maior].prioridade)
        maior = direita;

    if (maior != i) {
        trocarPacientes(&heap->pacientes[i], &heap->pacientes[maior]);
        heapify(heap, maior);
    }
}

void inserirPaciente(HeapPrioridade* heap, Paciente paciente) {
    if (heap->tamanho == heap->capacidade) {
        fprintf(stderr, "Heap cheio\n");
        return;
    }

    heap->tamanho++;
    int i = heap->tamanho - 1;
    heap->pacientes[i] = paciente;

    while (i != 0 && heap->pacientes[(i-1)/2].prioridade < heap->pacientes[i].prioridade) {
        trocarPacientes(&heap->pacientes[i], &heap->pacientes[(i-1)/2]);
        i = (i-1)/2;
    }
}

Paciente extrairMaximo(HeapPrioridade* heap) {
    if (heap->tamanho <= 0)
        return pacientes[0]; // Retorna paciente vazio em caso de erro

    if (heap->tamanho == 1) {
        heap->tamanho--;
        return heap->pacientes[0];
    }

    Paciente raiz = heap->pacientes[0];
    heap->pacientes[0] = heap->pacientes[heap->tamanho-1];
    heap->tamanho--;
    heapify(heap, 0);

    return raiz;
}

void destruirHeap(HeapPrioridade* heap) {
    free(heap->pacientes);
    free(heap);
}

// Funções auxiliares
void configurarRetorno(Paciente* paciente, int medico_id) {
    paciente->precisa_retorno = true;
    paciente->medico_anterior_id = medico_id;
    paciente->dias_ate_retorno = DIAS_UTEIS_RETORNO;
    paciente->prioridade = PRIORIDADE_RETORNO;
}

// Função principal de geração de escala
void gerarEscala(const char* arquivo_saida, int num_pacientes, int num_salas,
                 int num_especialidades, int num_medicos) {
    FILE* fp = fopen(arquivo_saida, "w");
    if (!fp) {
        fprintf(stderr, "Erro ao criar arquivo de saída\n");
        return;
    }

    HeapPrioridade* heap = criarHeap(num_pacientes);

    // Inicializar o heap com todos os pacientes
    for (int i = 0; i < num_pacientes; i++) {
        if (!pacientes[i].atendido && !pacientes[i].removido_por_faltas) {
            inserirPaciente(heap, pacientes[i]);
        }
    }

    int semana = 1;
    int pacientes_atendidos = 0;
    bool pacientes_pendentes = true;

    fprintf(fp, "ESCALA DE ATENDIMENTOS SEMANAIS\n");
    fprintf(fp, "================================\n\n");

    while (pacientes_pendentes && pacientes_atendidos < num_pacientes) {
        fprintf(fp, "SEMANA %d\n", semana);
        fprintf(fp, "--------\n\n");

        for (int dia = 0; dia < DIAS_POR_SEMANA; dia++) {
            fprintf(fp, "DIA %d\n", dia + 1);
            fprintf(fp, "-------\n");

            for (int hora = HORARIO_INICIO; hora < HORARIO_FIM; hora++) {
                if (hora == HORARIO_ALMOCO || hora == HORARIO_ALMOCO + 1) {
                    fprintf(fp, "\n%02d:00 - HORÁRIO DE ALMOÇO\n", hora);
                    continue;
                }

                fprintf(fp, "\n%02d:00\n", hora);
                AlocacaoDiaria alocacoes[MAX_SALAS] = {0};
                bool algum_atendimento = false;

                // Tentar alocar pacientes em todas as salas disponíveis
                for (int sala = 0; sala < num_salas && heap->tamanho > 0; sala++) {
                    Paciente paciente = extrairMaximo(heap);

                    // Verificar falta
                    if (verificarFalta(&paciente)) {
                        if (paciente.removido_por_faltas) {
                            fprintf(fp, "Sala %d: VAZIA - Paciente %s removido por excesso de faltas\n",
                                    sala + 1, paciente.nome);
                            pacientes_atendidos++;
                        } else {
                            fprintf(fp, "Sala %d: VAZIA - Paciente %s faltou (nova prioridade: %d)\n",
                                    sala + 1, paciente.nome, paciente.prioridade);
                            inserirPaciente(heap, paciente);
                        }
                        continue;
                    }

                    // Tentar encontrar médico disponível
                    int medico_id = encontrarMedicoDisponivel(&paciente, alocacoes, num_salas, num_medicos);
                    if (medico_id >= 0) {
                        alocacoes[sala].alocado = true;
                        alocacoes[sala].paciente = paciente.id;
                        alocacoes[sala].medico = medico_id;
                        medicos[medico_id].horas_trabalhadas++;

                        fprintf(fp, "Sala %d: Dr(a). %s atende %s - %s\n",
                                sala + 1,
                                medicos[medico_id].nome,
                                paciente.nome,
                                especialidades[medicos[medico_id].especialidade_id].nome);

                        if (!paciente.retorno_agendado && (rand() % 2)) {
                            agendarRetorno(&paciente, medico_id, dia, semana);
                            fprintf(fp, "        Retorno agendado para semana %d, dia %d\n",
                                    paciente.semana_retorno, paciente.dia_retorno);
                            inserirPaciente(heap, paciente);
                        } else {
                            pacientes_atendidos++;
                        }
                        algum_atendimento = true;
                    } else {
                        inserirPaciente(heap, paciente);
                        fprintf(fp, "Sala %d: VAZIA - Sem médico disponível\n", sala + 1);
                    }
                }

                if (!algum_atendimento && heap->tamanho == 0) break;
            }
            fprintf(fp, "\n");
        }
        semana++;
    }

    gerarRelatorioHorasTrabalhadas(fp, num_medicos);
    gerarRelatorioCompleto(fp, semana - 1, num_medicos);
    fprintf(fp, "\nTotal de semanas necessárias: %d\n", semana - 1);

    destruirHeap(heap);
    fclose(fp);
}

// Função principal
int main(int argc, char *argv[]) {
    int num_pacientes, num_salas, num_especialidades, num_medicos;
    const char* arquivo_entrada = "entrada.csv";
    const char* arquivo_saida = "escala.txt";

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

    gerarEscala(arquivo_saida, num_pacientes, num_salas,
                num_especialidades, num_medicos);

    printf("Processamento concluído. Verifique o arquivo '%s'\n", arquivo_saida);
    return 0;
}

void agendarRetorno(Paciente* paciente, int medico_id, int dia_atual, int semana_atual) {
    paciente->precisa_retorno = true;
    paciente->medico_anterior_id = medico_id;
    paciente->dia_ultima_consulta = dia_atual;
    paciente->semana_ultima_consulta = semana_atual;

    // Calcula a data do retorno
    calcularDataRetorno(dia_atual, semana_atual,
                       &paciente->dia_retorno,
                       &paciente->semana_retorno);

    // Define prioridade inicial de retorno
    paciente->prioridade = calcularPrioridadeRetorno(paciente, semana_atual, dia_atual);
    paciente->aguardando_retorno = true;
}

bool verificarPrazoRetorno(Paciente paciente, int dia_atual, int semana_atual) {
    int dias_totais = (semana_atual - paciente.semana_ultima_consulta) * DIAS_POR_SEMANA +
                     (dia_atual - paciente.dia_ultima_consulta);
    return dias_totais <= DIAS_UTEIS_RETORNO;
}

int encontrarMedicoDisponivel(Paciente* paciente, AlocacaoDiaria* alocacoes,
                             int num_salas, int num_medicos) {
    // Se é retorno, tenta alocar o mesmo médico
    if (paciente->aguardando_retorno) {
        if (verificarDisponibilidadeMedico(paciente->medico_anterior_id,
                                         alocacoes, num_salas)) {
            return paciente->medico_anterior_id;
        }
        return -1;
    }

    // Procura médico da especialidade necessária
    for (int i = 0; i < num_medicos; i++) {
        if (medicos[i].especialidade_id == paciente->especialidade_id &&
            verificarDisponibilidadeMedico(i, alocacoes, num_salas)) {
            return i;
        }
    }
    return -1;
}

bool verificarDisponibilidadeMedico(int medico_id, AlocacaoDiaria* alocacoes, int num_salas) {
    for (int i = 0; i < num_salas; i++) {
        if (alocacoes[i].alocado && alocacoes[i].medico == medico_id) {
            return false;
        }
    }
    return true;
}

void ordenarMedicosPorHorasTrabalhadas(void) {
    for (int i = 0; i < num_medicos - 1; i++) {
        for (int j = 0; j < num_medicos - i - 1; j++) {
            if (medicos[j].horas_trabalhadas < medicos[j+1].horas_trabalhadas) {
                Medico temp = medicos[j];
                medicos[j] = medicos[j+1];
                medicos[j+1] = temp;
            }
        }
    }
}

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
            fprintf(stderr, "Erro: Especialidade %s no possui nenhum médico\n",
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

void gerarRelatorioHorasTrabalhadas(FILE* fp, int num_medicos) {
    fprintf(fp, "\nRELATÓRIO DE HORAS TRABALHADAS POR MÉDICO\n");
    fprintf(fp, "========================================\n");

    // Criar array temporário para ordenação
    MedicoRelatorio* relatorio = malloc(num_medicos * sizeof(MedicoRelatorio));
    for (int i = 0; i < num_medicos; i++) {
        strcpy(relatorio[i].nome, medicos[i].nome);
        relatorio[i].horas = medicos[i].horas_trabalhadas;
        relatorio[i].especialidade_id = medicos[i].especialidade_id;
    }

    // Ordenar por horas trabalhadas (decrescente)
    qsort(relatorio, num_medicos, sizeof(MedicoRelatorio), comparaMedicoHoras);

    // Imprimir relatório
    for (int i = 0; i < num_medicos; i++) {
        fprintf(fp, "Dr(a). %s (%s): %d horas\n",
                relatorio[i].nome,
                especialidades[relatorio[i].especialidade_id].nome,
                relatorio[i].horas);
    }

    free(relatorio);
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

bool verificarFalta(Paciente* paciente) {
    // Gera número aleatório entre 0 e 99
    int chance = rand() % 100;

    // 5% de chance de falta (números de 0 a 4)
    if (chance < CHANCE_FALTA) {
        paciente->faltas++;

        if (paciente->faltas > 1) {
            paciente->removido_por_faltas = true;
            return true; // Paciente será removido
        }

        // Primeira falta: reduz prioridade
        paciente->prioridade = paciente->prioridade / REDUCAO_PRIORIDADE_FALTA;
        return true; // Paciente faltou mas continua na fila
    }
    return false; // Paciente não faltou
}

int calcularPrioridadeRetorno(Paciente* paciente, int semana_atual, int dia_atual) {
    int dias_totais = (paciente->semana_retorno - semana_atual) * DIAS_POR_SEMANA +
                     (paciente->dia_retorno - dia_atual);

    // Garante que a prioridade de retorno seja maior que pelo menos uma primeira consulta
    int prioridade_base = PRIORIDADE_MINIMA + 1;

    if (dias_totais <= 5) {  // Última semana
        return PRIORIDADE_MAXIMA - 1;  // Alta, mas não máxima
    } else if (dias_totais <= 10) {
        return (PRIORIDADE_MAXIMA + prioridade_base) / 2;
    } else {
        return prioridade_base;
    }
}

int comparaMedicoHoras(const void* a, const void* b) {
    MedicoRelatorio* m1 = (MedicoRelatorio*)a;
    MedicoRelatorio* m2 = (MedicoRelatorio*)b;
    return m2->horas - m1->horas;  // Ordem decrescente
}

void gerarRelatorioCompleto(FILE* fp, int semana, int num_medicos) {
    fprintf(fp, "\nRESUMO FINAL\n");
    fprintf(fp, "============\n");
    fprintf(fp, "Total de semanas necessárias: %d\n\n", semana);

    fprintf(fp, "RELATÓRIO DE HORAS TRABALHADAS POR MÉDICO\n");
    fprintf(fp, "=======================================\n");

    // Ordenar médicos por horas trabalhadas
    MedicoRelatorio* relatorio = malloc(num_medicos * sizeof(MedicoRelatorio));

    free(relatorio);
}
