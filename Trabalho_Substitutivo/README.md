# Algoritmos de Ordenação e Conjuntos Disjuntos

Este projeto implementa algoritmos de ordenação e operações com conjuntos disjuntos, permitindo manipular e ordenar subconjuntos de elementos.

## 📹 Vídeo Explicativo
Para uma explicação detalhada do código e demonstração das funcionalidades, assista ao vídeo:
[Explicação do Código - YouTube](https://www.youtube.com/watch?v=ZPXSHdjV8Sk&authuser=1)

## 🎯 Objetivos
- Implementar algoritmos de ordenação clássicos
- Trabalhar com conjuntos disjuntos
- Manipular partições de elementos
- Medir e comparar desempenho de algoritmos

## 🛠️ Funcionalidades

### Algoritmos de Ordenação
- Bubble Sort
- Insertion Sort
- Merge Sort
- Quick Sort

### Operações com Conjuntos
- Determinar representante de um conjunto
- Unir dois subconjuntos
- Ordenar elementos de um subconjunto
- Verificar ambiguidade entre subconjuntos

## 📋 Requisitos
- Compilador C (GCC recomendado)
- Sistema operacional: Windows/Linux
- Arquivo de entrada (dados.csv)

## 📥 Formato do Arquivo de Entrada (dados.csv)
O arquivo de entrada deve seguir o seguinte formato:
```
n,k
inicio1,fim1
inicio2,fim2
...
iniciok,fimk
```

### Estrutura do Arquivo
- **Primeira linha**: contém dois números separados por vírgula
  - `n`: tamanho total do vetor (elementos de 1 a n)
  - `k`: quantidade de subconjuntos na partição inicial

- **Linhas seguintes**: k linhas contendo pares de números
  - `inicio_i`: índice inicial do subconjunto i (base 0)
  - `fim_i`: índice final do subconjunto i (inclusive)

### Exemplo Atual (dados.csv)
```
14,6 # n=14 elementos, k=6 subconjuntos
0,3 # Subconjunto 1: elementos nos índices 0 a 3
7,9 # Subconjunto 2: elementos nos índices 7 a 9
4,6 # Subconjunto 3: elementos nos índices 4 a 6
1,2 # Subconjunto 4: elementos nos índices 1 a 2
5,8 # Subconjunto 5: elementos nos índices 5 a 8
3,7 # Subconjunto 6: elementos nos índices 3 a 7
```

### Como Modificar o Arquivo
1. **Alterar tamanho do vetor**:
   - Modifique o primeiro número (n) na primeira linha
   - Exemplo: mudar de 14 para 20 para trabalhar com elementos de 1 a 20

2. **Alterar número de subconjuntos**:
   - Modifique o segundo número (k) na primeira linha
   - Adicione ou remova linhas de subconjuntos conforme necessário

3. **Modificar subconjuntos**:
   - Cada linha após a primeira define um subconjunto
   - Formato: índice_inicial,índice_final
   - Os índices devem estar entre 0 e n-1

### Regras e Restrições
- O valor de n deve ser positivo
- k deve ser maior que 1
- Os índices não podem ser negativos
- índice_final deve ser maior ou igual a índice_inicial
- Os índices não podem ultrapassar n-1

### Exemplo de Modificação
Para testar com um vetor menor:

```
10,4
0,2
3,5
6,7
8,9
```

Para testar com subconjuntos maiores:

```
20,3
0,6
7,13
14,19
```

### Observações
- Certifique-se de que o arquivo esteja na mesma pasta do executável
- Use vírgula como separador (sem espaços)
- Mantenha uma linha para cada subconjunto
- Evite linhas em branco ao final do arquivo
