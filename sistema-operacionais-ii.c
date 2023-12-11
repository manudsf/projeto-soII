#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int inicio;
    int tamanho;
} EspacoLivre;

void inicializarEspacoLivre(EspacoLivre *espaco, int inicio, int tamanho) {
    espaco->inicio = inicio;
    espaco->tamanho = tamanho;
}

void mostrarListaEspacosLivres(EspacoLivre *espacosLivres, int numEspacosLivres) {
    for (int i = 0; i < numEspacosLivres; i++) {
        printf("\033[1mREG%d\033[0m", i + 1);
        if (i < numEspacosLivres - 1) {
            printf(" - ");
        }
    }
    printf("\n");
}

void removerRegiao(EspacoLivre *espacosLivres, int *numEspacosLivres, int posicao) {
    for (int i = posicao; i < *numEspacosLivres - 1; i++) {
        espacosLivres[i] = espacosLivres[i + 1];
    }
    (*numEspacosLivres)--;
}

// Algoritmo Best Fit
int bestFit(EspacoLivre *espacosLivres, int *numEspacosLivres, int tamanhoRequisicao) {
    int melhorIndice = -1;
    int melhorDiferenca = __INT_MAX__;

    for (int i = 0; i < *numEspacosLivres; i++) {
        if (espacosLivres[i].tamanho >= tamanhoRequisicao) {
            int diferenca = espacosLivres[i].tamanho - tamanhoRequisicao;
            if (diferenca < melhorDiferenca) {
                melhorDiferenca = diferenca;
                melhorIndice = i;
            }
        }
    }

    return melhorIndice;
}

// Algoritmo Worst Fit
int worstFit(EspacoLivre *espacosLivres, int *numEspacosLivres, int tamanhoRequisicao) {
    int piorIndice = -1;
    int piorDiferenca = -1;

    for (int i = 0; i < *numEspacosLivres; i++) {
        if (espacosLivres[i].tamanho >= tamanhoRequisicao) {
            int diferenca = espacosLivres[i].tamanho - tamanhoRequisicao;
            if (diferenca > piorDiferenca) {
                piorDiferenca = diferenca;
                piorIndice = i;
            }
        }
    }

    return piorIndice;
}

// Algoritmo First Fit
int firstFit(EspacoLivre *espacosLivres, int *numEspacosLivres, int tamanhoRequisicao) {
    for (int i = 0; i < *numEspacosLivres; i++) {
        if (espacosLivres[i].tamanho >= tamanhoRequisicao) {
            return i;
        }
    }

    return -1;
}

// Algoritmo Next Fit
int nextFit(EspacoLivre *espacosLivres, int *numEspacosLivres, int tamanhoRequisicao, int *ultimoIndice) {
    for (int i = *ultimoIndice + 1; i < *numEspacosLivres; i++) {
        if (espacosLivres[i].tamanho >= tamanhoRequisicao) {
            *ultimoIndice = i;
            return i;
        }
    }

    // Se não encontrar a partir do último índice, busca novamente a partir do início
    for (int i = 0; i <= *ultimoIndice; i++) {
        if (espacosLivres[i].tamanho >= tamanhoRequisicao) {
            *ultimoIndice = i;
            return i;
        }
    }

    return -1;
}

int main() {
    const int maxEspacosLivres = 12;
    const int numSolicitacoes = 10;

    EspacoLivre espacosLivres[maxEspacosLivres];
    int numEspacosLivres = maxEspacosLivres;

    int enderecoInicial = 0;
    for (int i = 0; i < numEspacosLivres; i++) {
        int tamanho = rand() % 100 + 1;
        inicializarEspacoLivre(&espacosLivres[i], enderecoInicial, tamanho);
        enderecoInicial += tamanho;
    }

    printf("Memória Inicial:\n");
    mostrarListaEspacosLivres(espacosLivres, numEspacosLivres);
    printf("\n");

    int ultimoIndiceNextFit = 0;

    // Executar sequência de requisições
    for (int i = 0; i < numSolicitacoes; i++) {
        int tamanhoRequisicao = rand() % 100 + 1;
        printf("Solicitação %d [%d]:\n", i + 1, tamanhoRequisicao);

        int indiceAlocadoBestFit = bestFit(espacosLivres, &numEspacosLivres, tamanhoRequisicao);
        int indiceAlocadoWorstFit = worstFit(espacosLivres, &numEspacosLivres, tamanhoRequisicao);
        int indiceAlocadoFirstFit = firstFit(espacosLivres, &numEspacosLivres, tamanhoRequisicao);
        int indiceAlocadoNextFit = nextFit(espacosLivres, &numEspacosLivres, tamanhoRequisicao, &ultimoIndiceNextFit);

        if (indiceAlocadoBestFit != -1) {
            printf("Best Fit: Memória alocada com sucesso em \033[1mREG%d\033[0m\n", indiceAlocadoBestFit + 1);
            espacosLivres[indiceAlocadoBestFit].inicio += tamanhoRequisicao;
            espacosLivres[indiceAlocadoBestFit].tamanho -= tamanhoRequisicao;
            if (espacosLivres[indiceAlocadoBestFit].tamanho == 0) {
                removerRegiao(espacosLivres, &numEspacosLivres, indiceAlocadoBestFit);
            }
        } else {
            printf("Best Fit: Solicitação não pode ser atendida\n");
        }

        if (indiceAlocadoWorstFit != -1) {
            printf("Worst Fit: Memória alocada com sucesso em \033[1mREG%d\033[0m\n", indiceAlocadoWorstFit + 1);
            espacosLivres[indiceAlocadoWorstFit].inicio += tamanhoRequisicao;
            espacosLivres[indiceAlocadoWorstFit].tamanho -= tamanhoRequisicao;
            if (espacosLivres[indiceAlocadoWorstFit].tamanho == 0) {
                removerRegiao(espacosLivres, &numEspacosLivres, indiceAlocadoWorstFit);
            }
        } else {
            printf("Worst Fit: Solicitação não pode ser atendida\n");
        }

        if (indiceAlocadoFirstFit != -1) {
            printf("First Fit: Memória alocada com sucesso em \033[1mREG%d\033[0m\n", indiceAlocadoFirstFit + 1);
            espacosLivres[indiceAlocadoFirstFit].inicio += tamanhoRequisicao;
            espacosLivres[indiceAlocadoFirstFit].tamanho -= tamanhoRequisicao;
            if (espacosLivres[indiceAlocadoFirstFit].tamanho == 0) {
                removerRegiao(espacosLivres, &numEspacosLivres, indiceAlocadoFirstFit);
            }
        } else {
            printf("First Fit: Solicitação não pode ser atendida\n");
        }

        if (indiceAlocadoNextFit != -1) {
            printf("Next Fit: Memória alocada com sucesso em \033[1mREG%d\033[0m\n", indiceAlocadoNextFit + 1);
            espacosLivres[indiceAlocadoNextFit].inicio += tamanhoRequisicao;
            espacosLivres[indiceAlocadoNextFit].tamanho -= tamanhoRequisicao;
            if (espacosLivres[indiceAlocadoNextFit].tamanho == 0) {
                removerRegiao(espacosLivres, &numEspacosLivres, indiceAlocadoNextFit);
            }
        } else {
            printf("Next Fit: Solicitação não pode ser atendida\n");
        }

        printf("\n");
    }

    printf("\033[1mEncaixe perfeito\033[0m\n");

    return 0;
}
