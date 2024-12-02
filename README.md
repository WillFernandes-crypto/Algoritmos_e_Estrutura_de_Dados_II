# Sistema de Agendamento Hospitalar

## Descrição
Sistema desenvolvido em C para gerenciamento de consultas médicas e retornos em um ambiente hospitalar, com foco em priorização de atendimentos e distribuição eficiente de recursos.

## Funcionalidades Principais

### 1. Gestão de Pacientes
- Cadastro de pacientes com informações completas (nome, idade, peso, altura, etc.)
- Sistema de priorização baseado em gravidade (escala 1-10)
- Controle de faltas e penalizações
- Agendamento automático de retornos

### 2. Gestão de Médicos
- Cadastro de médicos por especialidade
- Controle de horas trabalhadas
- Distribuição equilibrada de carga de trabalho
- Associação médico-paciente para consultas de retorno

### 3. Gestão de Especialidades
- Suporte a múltiplas especialidades médicas
- Controle de número mínimo de médicos por especialidade
- Validação de disponibilidade de especialistas

### 4. Sistema de Agendamento
- Alocação automática de salas
- Controle de horários (8h às 18h, com intervalo para almoço 12h-14h)
- Gerenciamento de consultas iniciais e retornos
- Priorização dinâmica da fila de espera

## Estruturas de Dados Implementadas

### Heaps
1. `HeapPrioridade`: Gerencia a fila de primeira consulta
   - Ordenação por prioridade do paciente
   - Inserção e remoção em O(log n)

2. `HeapRetorno`: Controla a fila de consultas de retorno
   - Priorização específica para retornos
   - Gestão de prazos de retorno (até 30 dias úteis)

### Outras Estruturas
- `Paciente`: Armazena dados completos dos pacientes
- `Medico`: Mantém informações dos médicos e especialidades
- `Sala`: Controle de ocupação das salas de atendimento
- `Agendamento`: Registro de horários e alocações

## Algoritmos Principais

### 1. Escalonamento de Consultas
- Alocação eficiente de médicos e salas
- Verificação de disponibilidade
- Controle de conflitos de horário
- Balanceamento de carga entre médicos

### 2. Gestão de Retornos
- Agendamento automático dentro do prazo
- Manutenção da relação médico-paciente
- Tratamento de faltas e reagendamentos

### 3. Relatórios
- Escala de consultas por semana/dia
- Escala de retornos
- Relatório de horas trabalhadas por médico

## Entrada e Saída

### Arquivo de Entrada (entrada.csv)
- Linha 1: Número de pacientes, salas, especialidades e médicos
- Seção 1: Lista de especialidades e requisitos
- Seção 2: Cadastro de médicos
- Seção 3: Dados dos pacientes

### Arquivo de Saída (relatorio.txt)
- Escala detalhada de consultas
- Escala de retornos
- Relatório de horas trabalhadas

## Validações e Regras de Negócio
- Número de médicos >= número de salas
- Cada especialidade deve ter pelo menos um médico
- Controle de faltas com penalização na prioridade
- Retornos devem ser agendados em até 30 dias úteis
- Preferência pelo mesmo médico em consultas de retorno

## Como Usar
1. Prepare o arquivo `entrada.csv` com os dados necessários
2. Compile o projeto usando um compilador C
3. Execute o programa
4. Verifique o resultado no arquivo `relatorio.txt`

## Limitações e Constantes
- MAX_PACIENTES: 1000
- MAX_SALAS: 20
- MAX_ESPECIALIDADES: 10
- MAX_MEDICOS: 50
- DIAS_POR_SEMANA: 5 (dias úteis)
- Horário de funcionamento: 8h às 18h (intervalo 12h-14h)

## Dependências
- Standard C Library
- Compilador C (GCC recomendado)
