#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <time.h>
#include <sys/time.h>

#define BUF_SIZE 2009
TCHAR szName[]=TEXT("FoodSec");

void resetarContador(LPCTSTR buffer) {
    int contador = 0;
    memcpy(buffer + 1, &contador, sizeof(int));
}

void atualizarPesoTotal(LPCTSTR buffer) {
    char cTrue = 1;
    memcpy(buffer, &cTrue, sizeof(char));

    int pesoTotalAtual;
    memcpy(&pesoTotalAtual, buffer + 5, sizeof(int));

    for(int i = 0; i < 500; i++) {
        int aux;
        memcpy(&aux, buffer + 9 + (i * sizeof(int)), sizeof(int));
        pesoTotalAtual += aux;
    }

    memcpy(buffer + 5, &pesoTotalAtual, sizeof(int));

    resetarContador(buffer);

    char cFalse = 0;
    memcpy(buffer, &cFalse, sizeof(char));
}

void checarContador (LPCTSTR buffer) {
    int contador;
    memcpy(&contador, buffer + 1, sizeof(int));

    if(contador >= 499) {
        system("cls");

        struct timeval stop, start;
        gettimeofday(&start, NULL);

        atualizarPesoTotal(buffer);

        gettimeofday(&stop, NULL);

        printf("Pesagem realizada em %d microssegundos", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);

        Sleep(10000);
    }
}

int main() {

    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // use paging file
            NULL,                    // default security
            PAGE_READWRITE,          // read/write access
            0,                       // maximum object size (high-order DWORD)
            BUF_SIZE,                // maximum object size (low-order DWORD)
            szName);                 // name of mapping object

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

    char parado = 0;
    memcpy(pBuf, &parado, sizeof(char));
    int contagem = 0;
    memcpy(pBuf + 1, &contagem, sizeof(int));
    int pesoTotal = 0;
    memcpy(pBuf + 5, &pesoTotal, sizeof(int));

    while(1) {
        checarContador(pBuf);

        system("cls");

        memcpy(&contagem, pBuf + 1, sizeof(int));
        printf("Contagem: %d\n", contagem);
        memcpy(&pesoTotal, pBuf + 5, sizeof(int));
        printf("Peso Total: %d\n", pesoTotal);

        for(int i = 0; i < contagem; i++) {
            int aux;
            mempcpy(&aux, pBuf + 9 + (i * sizeof(int)), sizeof(int));
            printf("%d: ", i);
            printf("%d\n", aux);
        }

        Sleep(100);
    }

    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);

    return 0;
}
