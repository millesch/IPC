#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <time.h>
#include <stdlib.h>

#define BUF_SIZE 2009
TCHAR szName[]=TEXT("FoodSec");

int obterPeso(int peso) {
    int variacao = peso * 0.01;
    int aleatoriedade = 1 + rand() % variacao;
    return peso - variacao / 2 + aleatoriedade;
}

void contabilizarItem(LPCTSTR buffer) {
    int peso = obterPeso(5000);
    int contagem;
    mempcpy(&contagem, buffer + sizeof(char), sizeof(int));
    mempcpy(buffer + 9 + (contagem * sizeof(int)), &peso, sizeof(int));
    contagem++;
    mempcpy(buffer + 1,&contagem, sizeof(int));
}

void atualizarContador (LPCTSTR buffer) {
    system("cls");
    int contador;
    memcpy(&contador, buffer + 1, sizeof(int));
    printf("Contador: %d", contador);
}

int main() {

    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // read/write access
            FALSE,                 // do not inherit the name
            szName);               // name of mapping object

    if (hMapFile == NULL)
    {
        _tprintf(TEXT("Could not open file mapping object (%d).\n"),
                 GetLastError());
        return 1;
    }

    pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
                                  FILE_MAP_ALL_ACCESS,  // read/write permission
                                  0,
                                  0,
                                  BUF_SIZE);

    if (pBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
                 GetLastError());

        CloseHandle(hMapFile);

        return 1;
    }

    clock_t dois_segundos = -(CLOCKS_PER_SEC * 2);

    char parado;
    while(1) {
        if((clock() - dois_segundos) / CLOCKS_PER_SEC >= 2) {
            mempcpy(&parado, pBuf, sizeof(char));
            if(!parado) {
                contabilizarItem(pBuf);
                atualizarContador(pBuf);
            }
            dois_segundos = clock();
        }
    }

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return 0;
}