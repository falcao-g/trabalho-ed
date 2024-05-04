def crivo(n):
    primos = [True for _ in range(n+1)]
    primos[0] = False  # 0 não é primo
    primos[1] = False  # 1 não é primo
    for i in range(2, n+1):
        if primos[i]:  # se i é primo
            for j in range(i*i, n+1, i):  # marca todos os múltiplos de i como não primos
                primos[j] = False
    return primos


lista = crivo(13000)
for el, i in enumerate(lista):
    if i:
        print(el)
