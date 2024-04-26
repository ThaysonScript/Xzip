#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

struct NoMinHeap {
    char dado;
    unsigned frequencia;
    struct NoMinHeap *esquerda, *direita;
};

struct MinHeap {
    unsigned tamanho;
    unsigned capacidade;
    struct NoMinHeap** array;
};

struct NoMinHeap* novoNo(char dado, unsigned frequencia) {
    struct NoMinHeap* temp = (struct NoMinHeap*)malloc(sizeof(struct NoMinHeap));
    temp->esquerda = temp->direita = NULL;
    temp->dado = dado;
    temp->frequencia = frequencia;
    return temp;
}

struct MinHeap* criarMinHeap(unsigned capacidade) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->tamanho = 0;
    minHeap->capacidade = capacidade;
    minHeap->array = (struct NoMinHeap**)malloc(minHeap->capacidade * sizeof(struct NoMinHeap*));
    return minHeap;
}

void trocarNoMinHeap(struct NoMinHeap** a, struct NoMinHeap** b) {
    struct NoMinHeap* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    int menor = idx;
    int esquerda = 2 * idx + 1;
    int direita = 2 * idx + 2;
    if (esquerda < minHeap->tamanho && minHeap->array[esquerda]->frequencia < minHeap->array[menor]->frequencia)
        menor = esquerda;
    if (direita < minHeap->tamanho && minHeap->array[direita]->frequencia < minHeap->array[menor]->frequencia)
        menor = direita;
    if (menor != idx) {
        trocarNoMinHeap(&minHeap->array[menor], &minHeap->array[idx]);
        minHeapify(minHeap, menor);
    }
}

int ehTamanhoUm(struct MinHeap* minHeap) {
    return (minHeap->tamanho == 1);
}

struct NoMinHeap* extrairMin(struct MinHeap* minHeap) {
    struct NoMinHeap* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->tamanho - 1];
    --minHeap->tamanho;
    minHeapify(minHeap, 0);
    return temp;
}

void inserirMinHeap(struct MinHeap* minHeap, struct NoMinHeap* noMinHeap) {
    ++minHeap->tamanho;
    int i = minHeap->tamanho - 1;
    while (i && noMinHeap->frequencia < minHeap->array[(i - 1) / 2]->frequencia) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = noMinHeap;
}

void construirMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->tamanho - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void imprimirArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

int ehFolha(struct NoMinHeap* raiz) {
    return !(raiz->esquerda) && !(raiz->direita);
}

struct MinHeap* criarConstruirMinHeap(char dado[], int frequencia[], int tamanho) {
    struct MinHeap* minHeap = criarMinHeap(tamanho);
    for (int i = 0; i < tamanho; ++i)
        minHeap->array[i] = novoNo(dado[i], frequencia[i]);
    minHeap->tamanho = tamanho;
    construirMinHeap(minHeap);
    return minHeap;
}

struct NoMinHeap* construirArvoreHuffman(char dado[], int frequencia[], int tamanho) {
    struct NoMinHeap *esquerda, *direita, *topo;
    struct MinHeap* minHeap = criarConstruirMinHeap(dado, frequencia, tamanho);
    while (!ehTamanhoUm(minHeap)) {
        esquerda = extrairMin(minHeap);
        direita = extrairMin(minHeap);
        topo = novoNo('$', esquerda->frequencia + direita->frequencia);
        topo->esquerda = esquerda;
        topo->direita = direita;
        inserirMinHeap(minHeap, topo);
    }
    return extrairMin(minHeap);
}

void imprimirCodigos(struct NoMinHeap* raiz, int arr[], int topo) {
    if (raiz->esquerda) {
        arr[topo] = 0;
        imprimirCodigos(raiz->esquerda, arr, topo + 1);
    }
    if (raiz->direita) {
        arr[topo] = 1;
        imprimirCodigos(raiz->direita, arr, topo + 1);
    }
    if (ehFolha(raiz)) {
        printf("%c: ", raiz->dado);
        imprimirArr(arr, topo);
    }
}

void codigosHuffman(char dado[], int frequencia[], int tamanho) {
    struct NoMinHeap* raiz = construirArvoreHuffman(dado, frequencia, tamanho);
    int arr[MAX_TREE_HT], topo = 0;
    printf("Codigos de Huffman:\n");
    imprimirCodigos(raiz, arr, topo);
}

int main() {
    char dado[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frequencia[] = {5, 9, 12, 13, 16, 45};
    int tamanho = sizeof(dado) / sizeof(dado[0]);
    codigosHuffman(dado, frequencia, tamanho);
    return 0;
}
