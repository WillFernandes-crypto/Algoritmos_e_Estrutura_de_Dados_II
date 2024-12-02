B-TREE-DELETE(x, k)
i ← 1
while (i ≤ n[x] and keyi[x] < k)
    do i ← i + 1
if leaf[x]  // nó folha, caso 1
    then if i ≤ n[x] and k = keyi[x]
        then delete keyi[x]
    else  // chave k não encontrada
else  // nó interno
    if i ≤ n[x] and k = keyi[x]
        then y ← Ci[x]  // filho à esquerda de x
             z ← Ci+1[x]  // filho à direita de x
             // y tem pelo menos t chaves, caso 2a
             if n[y] ≥ t
                 then k' ← LAST(y)
                      keyi[x] ← k'
                      B-TREE-DELETE(y, k')
             // z tem pelo menos t chaves, caso 2b
             elseif n[z] ≥ t
                 then k' ← FIRST(z)
                      keyi[x] ← k'
                      B-TREE-DELETE(z, k')
             else  // tanto y como z têm t − 1 chaves, caso 2c
                 MERGE(x, i, y, z)
                 B-TREE-DELETE(y, k)
else  // k ≠ keyi[x] para todo i, caso 3
    w ← Ci[x]
    we ← nil  // irmão à esquerda de Ci[x]
    wd ← nil  // irmão à direita de Ci[x]
    if i > 1  // Ci tem irmão à esquerda
        then we ← Ci-1[x]
    if i ≤ n[x]  // Ci tem irmão à direita
        then wd ← Ci+1[x]
    if n[w] = t - 1  // Ci tem t − 1 chaves e irmão com pelo menos t chaves, caso 3a
        then if we ≠ nil and n[we] ≥ t
            then  // move a chave i − 1 de x para w
                 // move a última chave de Ci-1[x] para x
        elseif wd ≠ nil and n[wd] ≥ t
            then  // move a chave i de x para w
                 // move a primeira chave de Ci+1[x] para x
        else  // os irmãos de Ci têm t − 1 chaves, caso 3b
            if we ≠ nil
                then  // intercala w com we e a chave i − 1 de x
            elseif wd ≠ nil
                then  // intercala w com wd e a chave i de x
    // atualiza x
B-TREE-DELETE(Ci[x], k)