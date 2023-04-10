import _thread
import time
import random
import math

parado = False
contagem = 0
pesoTotal = 0
pesos = [0 for i in range(500)]


def obterPeso(peso):
    variacao = peso * 0.01
    aleatoriedade = random.random() % variacao
    return math.trunc(peso - variacao / 2 + aleatoriedade)


def contabilizarItem(nomeThread, peso):
    global parado
    global contagem
    global pesos

    parado = True

    pesos[contagem] = obterPeso(peso)

    print("%s: %s - %s" % (nomeThread, contagem, pesos[contagem]))

    contagem += 1

    parado = False


def sensorEsteira(nomeThread, frequencia, peso):
    global parado
    global contagem
    global pesos

    timer = time.time()
    while (True):
        if (time.time() - timer >= frequencia):
            if (not parado):
                contabilizarItem(nomeThread, peso)
            timer = time.time()


def atualizarPesoTotal():
    global parado
    global contagem
    global pesoTotal

    parado = True

    for i in pesos:
        pesoTotal += i

    contagem = 0

    parado = False


def checarContador():
    global contagem
    global pesoTotal

    if (contagem >= 50):
        inicio = time.time()
        atualizarPesoTotal()
        fim = time.time()
        print("Controlador: Peso Total: %s" % pesoTotal)
        print("Controlador: Pesagem realizada em %s microssegundos" %
              ((fim - inicio) * 1000000))


def main():

    try:
        _thread.start_new_thread(sensorEsteira, ("Sensor Esteira 1", 2, 5000))
        _thread.start_new_thread(sensorEsteira, ("Sensor Esteira 2", 1, 2000))
    except:
        print("Erro ao inicializar as threads")

    while (True):
        checarContador()
        time.sleep(0.1)


main()
