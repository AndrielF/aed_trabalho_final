#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "head.h" 


int main() {
    printf("\n--- Algoritmos de Ordenação: ---");
    printf("\n|    1 - Merge Sort            |");
    printf("\n|    2 - Insertion Sort        |");
    printf("\n|    3 - Selection Sort        |");
    printf("\n|    4 - Quick Sort            |");
    printf("\n|    5 - Shell Sort            |");   
    printf("\n|                              |");      
    printf("\n--- Algoritmos de Busca: -------");
    printf("\n|    6 - Busca Sequencial      |");
    printf("\n|    7 - Busca Binária         |\n");
    printf("--------------------------------\n");

    printf("\n\tOlá Telma! ;-)\nQual algoritmo de ordenação ou busca gostaria de utilizar?\n(informe somente o número) R: ");

    char algoritmo[20];
    int escolha, tipo_busca;
    char buffer[20];
    fgets(buffer, 20, stdin);
    if (sscanf(buffer, "%d", &escolha) != 1) {
        printf("\033[31mEntrada inválida!\n");
        return 0;
    }

    switch (escolha) {
        case 1:
            printf("\033[38;2;255;165;0mMerge Sort\n");
            strcpy(algoritmo, "mergeSort");
            break;
        case 2:
            printf("\033[38;2;255;165;0mInsertion Sort\n");
            strcpy(algoritmo, "insertionSort");
            break;
        case 3:
            printf("\033[38;2;255;165;0mSelection Sort\n");
            strcpy(algoritmo, "selectionSort");
            break;
        case 4:
            printf("\033[38;2;255;165;0mQuick Sort\n");
            strcpy(algoritmo, "quickSort");
            break;
        case 5:
            printf("\033[38;2;255;165;0mShell Sort\n");
            strcpy(algoritmo, "shellSort");
            break;
        case 6:
            tipo_busca = 1;
            break;
        case 7:
            tipo_busca = 2;
            strcpy(algoritmo, "mergeSort");
            break;
        default:
            printf("\033[31mOpção inválida!\n");
            return 0;
    }

    // Ler diretório
    DIR *d = opendir("/conjunto_dados_atualizado");
    if (d == NULL) {
        perror("Erro ao abrir o diretório");
        return 1;
    }

    // Criar diretório
    char dirpath[1024];
    sprintf(dirpath, "/dados_ordenados/%s", algoritmo);
    struct stat st;
    if (stat("/dados_ordenados/", &st) == -1) {
        if (mkdir("/dados_ordenados/", 0700) == -1) {
            perror("Erro ao criar diretório de saída");
            return 1;
        }
    }
    if (stat(dirpath, &st) == -1) {
        if (mkdir(dirpath, 0700) == -1) {
            perror("Erro ao criar diretório de saída");
            return 1;
        }
    }
    clock_t start, end;
    
    // Ler cada arquivo
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        char *filename = dir->d_name;
        if (strstr(filename, ".csv")) {
            if (tipo_busca != 1) {
                printf("\n\033[34mSorting file: \033[36m%s \033[38;2;255;165;0m with %s\n", filename, algoritmo);
            } else {
                printf("\n\033[34mSearching file: \033[36m%s \033[38;2;255;165;0m\n", filename);
            }
            char path[200];
            sprintf(path, "/conjunto_dados_atualizado/%s", filename);
            FILE *fp = fopen(path, "r");
            if (fp == NULL) {
                printf("Error opening file %s\n", filename);
                continue;
            }

            // Ler o arquivo linha por linha
            char *line = NULL;
            size_t len = 0;
            ssize_t read;
            struct Data data[10000];
            int dataIndex = 0;
            while ((read = getline(&line, &len, fp)) != -1) {
                char *token = strtok(line, ";");
                data[dataIndex].ID = atoi(token);
                data[dataIndex].restOfLine = strdup(line + strlen(token) + 1);
                dataIndex++;
            }

            // algoritmo de ordenação
            int swaps = 0;
            int comparisons = 0;
            double duration = 0;
            start = clock();

            if (strcmp(algoritmo, "insertionSort") == 0) {
                insertionSort(data, dataIndex, &comparisons, &swaps);
            } 
            else if (strcmp(algoritmo, "selectionSort") == 0) {
                selectionSort(data, dataIndex, &comparisons, &swaps);
            } 
            else if (strcmp(algoritmo, "quickSort") == 0) {
                quickSort(data, 0, dataIndex - 1, &comparisons, &swaps);
            } 
            else if (strcmp(algoritmo, "shellSort") == 0) {
                shellSort(data, dataIndex, &comparisons, &swaps);
            } 
            else if (strcmp(algoritmo, "mergeSort") == 0) {
                mergeSort(data, 0, dataIndex - 1, &comparisons, &swaps);
            }

            end = clock();
            duration = ((double) (end - start)) / CLOCKS_PER_SEC;
            if (tipo_busca != 1) {
                printf("\033[32mSorted %s \033[35m with  %d comparisons \033[38;5;216m and %d swaps \033[33m in %f seconds\n", filename, comparisons, swaps, duration);
            }

            // Busca sequencial
            if (tipo_busca == 1) {
                int valor;
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);

                int pos = busca_sequencial(data, dataIndex, valor);
                if (pos != -1) {
                    printf("\033[1;32mO valor %d foi encontrado na posição %d\n", valor, pos);
                } else {
                    printf("\033[1;31mO valor %d não foi encontrado\n", valor);
                }
            }

            if (tipo_busca == 2) {
                int valor;
                printf("\nDigite o valor a ser buscado: ");
                scanf("%d", &valor);

                int pos = binarySearch(data, 0, dataIndex, valor, &comparisons);
                if (pos != -1) {
                    printf("\033[1;32mO valor %d foi encontrado na posição %d\n", valor, pos);
                } else {
                    printf("\033[1;31mO valor %d não foi encontrado\n", valor);
                }
            }

            // Escreve o dado ordenado para o novo arquivo
            char outputPath[200];
            sprintf(outputPath, "/dados_ordenados/%s/%s", algoritmo, filename);
            FILE *outputFile = fopen(outputPath, "w");
            for (int i = 0; i < dataIndex; i++) {
                if (i == 0) { // se é a primeira linha, escreva o cabeçalho
                    fprintf(outputFile, "ID;%s", data[i].restOfLine);
                } else {
                    fprintf(outputFile, "%d;%s", data[i].ID, data[i].restOfLine);
                }
            }

            // Fecha o arquivo e libera a memória
            fclose(fp);
            fclose(outputFile);
            free(line);
            for (int i = 0; i < dataIndex; i++) {
                free(data[i].restOfLine);
            }
        }
    }
    closedir(d);
    printf("\n\n\t\033[1;32mTodos os arquivos foram ordenados com sucesso!\n\n\n\n");
    return 0;
}
