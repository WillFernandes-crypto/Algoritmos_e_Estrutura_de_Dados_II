Este trabalho extra consiste em implementar os algoritmos de ordenação que vimos (bublesort, insertionsort, mergesort, quicksort), além dos algoritmos sobre conjuntos disjuntos (Representante e União).
A entrada consiste em uma partição de k>1 subconjuntos de um vetor de tamanho n contendo os elementos 1 a n.

O valor de n, bem como a partição deve ser lida como entrada através de um arquivo.

A cada passo, podemos determinar quem é o representante de cada conjunto que contém um elemento qualquer dos n dados como entrada, efetuar a união de dois subconjuntos contendo dois elementos quaisquer de 1 a n ou ordenar os elementos de um subconjunto qualquer dentre estes, podendo escolher o(s) método(s) de ordenação (note que podemos escolher executar a ordenação para o mesmo subconjunto de elementos de formas distintas).

A saída deve ser o estado final do vetor depois de executar todas as operações e o tempo gasto para executar todas as operações.