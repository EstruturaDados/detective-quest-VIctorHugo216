#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Garantir protótipo de strdup para compatibilidade ANSI C */
char *strdup(const char *s);

// Versão implementada: Novato, Aventureiro e Mestre

// -------------------- Árvore binária de Salas (Novato) --------------------
typedef struct Sala {
    char nome[64];
    struct Sala *esq, *dir;
} Sala;

Sala* criarSala(const char *nome) {
    Sala *s = (Sala*)malloc(sizeof(Sala));
    if (!s) return NULL;
    strncpy(s->nome, nome, sizeof(s->nome)-1);
    s->nome[sizeof(s->nome)-1] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

// Exploração simples para o nível Novato (somente navegação)
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao[8];
    printf("Iniciando exploração (digite 'e' esquerda, 'd' direita, 's' sair)\n");
    while (atual) {
        printf("Você está em: %s\n", atual->nome);
        if (!atual->esq && !atual->dir) {
            printf("Chegou a uma sala-folha. Fim do caminho.\n");
            break;
        }
        printf("Escolha (e/d/s): ");
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        if (opcao[0] == 's') break;
        else if (opcao[0] == 'e') {
            if (atual->esq) atual = atual->esq;
            else printf("Não há caminho à esquerda.\n");
        } else if (opcao[0] == 'd') {
            if (atual->dir) atual = atual->dir;
            else printf("Não há caminho à direita.\n");
        } else printf("Opção inválida. Use e/d/s.\n");
    }
}

// -------------------- Árvore de Busca para Pistas (Aventureiro) --------------------
typedef struct PistaNode {
    char *texto;
    struct PistaNode *esq, *dir;
} PistaNode;

PistaNode* novoNoPista(const char *texto) {
    PistaNode *n = (PistaNode*)malloc(sizeof(PistaNode));
    if (!n) return NULL;
    n->texto = strdup(texto);
    n->esq = n->dir = NULL;
    return n;
}

void inserirPista(PistaNode **raiz, const char *texto) {
    if (!*raiz) { *raiz = novoNoPista(texto); return; }
    int cmp = strcmp(texto, (*raiz)->texto);
    if (cmp == 0) return; // já existe
    if (cmp < 0) inserirPista(&(*raiz)->esq, texto);
    else inserirPista(&(*raiz)->dir, texto);
}

void listarPistasEmOrdem(PistaNode *raiz) {
    if (!raiz) return;
    listarPistasEmOrdem(raiz->esq);
    printf("- %s\n", raiz->texto);
    listarPistasEmOrdem(raiz->dir);
}

// -------------------- Tabela Hash para Suspeitos (Mestre) --------------------
typedef struct PistaLista {
    char *pista;
    struct PistaLista *prox;
} PistaLista;

typedef struct SuspeitoEntry {
    char *nome;
    PistaLista *pistas;
    struct SuspeitoEntry *prox;
} SuspeitoEntry;

#define HASH_SIZE 11

unsigned int hashFunc(const char *s) {
    unsigned int h = 0;
    while (*s) h = (h * 31u) + (unsigned char)(*s++);
    return h % HASH_SIZE;
}

void inicializarHash(SuspeitoEntry *tabela[]) {
    for (int i = 0; i < HASH_SIZE; ++i) tabela[i] = NULL;
}

SuspeitoEntry* buscarSuspeitoBucket(SuspeitoEntry *head, const char *nome) {
    for (SuspeitoEntry *p = head; p; p = p->prox) if (strcmp(p->nome, nome) == 0) return p;
    return NULL;
}

void inserirNaHash(SuspeitoEntry *tabela[], const char *pista, const char *suspeito) {
    unsigned int idx = hashFunc(suspeito);
    SuspeitoEntry *entry = buscarSuspeitoBucket(tabela[idx], suspeito);
    if (!entry) {
        entry = (SuspeitoEntry*)malloc(sizeof(SuspeitoEntry));
        entry->nome = strdup(suspeito);
        entry->pistas = NULL;
        entry->prox = tabela[idx];
        tabela[idx] = entry;
    }
    // adicionar pista à lista do suspeito
    PistaLista *pn = (PistaLista*)malloc(sizeof(PistaLista));
    pn->pista = strdup(pista);
    pn->prox = entry->pistas;
    entry->pistas = pn;
}

void listarAssociacoes(SuspeitoEntry *tabela[]) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (SuspeitoEntry *e = tabela[i]; e; e = e->prox) {
            printf("Suspeito: %s\n", e->nome);
            for (PistaLista *p = e->pistas; p; p = p->prox) printf("  * %s\n", p->pista);
        }
    }
}

void suspeitoMaisCitado(SuspeitoEntry *tabela[]) {
    const char *melhor = NULL;
    int max = 0;
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (SuspeitoEntry *e = tabela[i]; e; e = e->prox) {
            int cnt = 0;
            for (PistaLista *p = e->pistas; p; p = p->prox) ++cnt;
            if (cnt > max) { max = cnt; melhor = e->nome; }
        }
    }
    if (melhor) printf("Suspeito mais citado: %s (%d pistas)\n", melhor, max);
    else printf("Nenhum suspeito registrado.\n");
}

// -------------------- Jogos/Modos --------------------
void montarMapa(Sala **raiz) {
    // construção fixa e simples
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sótão");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");

    hall->esq = biblioteca; hall->dir = cozinha;
    biblioteca->esq = sotao; biblioteca->dir = escritorio;
    cozinha->dir = jardim; // cozinha sem esquerda

    *raiz = hall;
}

// Nível Novato
void jogarNovato() {
    Sala *mapa = NULL;
    montarMapa(&mapa);
    printf("--- Modo Novato ---\n");
    explorarSalas(mapa);
}

// Nível Aventureiro
void jogarAventureiro() {
    Sala *mapa = NULL;
    montarMapa(&mapa);
    PistaNode *arvorePistas = NULL;
    char opcao[8];
    Sala *atual = mapa;
    printf("--- Modo Aventureiro ---\n");
    printf("Dicas: Biblioteca, Cozinha e Sótão possuem pistas.\n");
    while (atual) {
        printf("Você está em: %s\n", atual->nome);
        // pistas automáticas em algumas salas
        if (strcmp(atual->nome, "Biblioteca") == 0) inserirPista(&arvorePistas, "Bilhete rasgado");
        else if (strcmp(atual->nome, "Cozinha") == 0) inserirPista(&arvorePistas, "Pegada de lama");
        else if (strcmp(atual->nome, "Sótão") == 0) inserirPista(&arvorePistas, "Fio de cabelo");

        printf("Escolha (e/d/l listar pistas/s sair): ");
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        if (opcao[0] == 's') break;
        else if (opcao[0] == 'l') {
            printf("Pistas encontradas (em ordem):\n");
            listarPistasEmOrdem(arvorePistas);
            continue;
        } else if (opcao[0] == 'e') {
            if (atual->esq) atual = atual->esq; else printf("Não há caminho à esquerda.\n");
        } else if (opcao[0] == 'd') {
            if (atual->dir) atual = atual->dir; else printf("Não há caminho à direita.\n");
        } else printf("Opção inválida.\n");
    }
    printf("Fim do modo Aventureiro. Revisando pistas:\n");
    listarPistasEmOrdem(arvorePistas);
}

// Nível Mestre
void jogarMestre() {
    Sala *mapa = NULL;
    montarMapa(&mapa);
    SuspeitoEntry *tabela[HASH_SIZE];
    inicializarHash(tabela);
    char opcao[8];
    Sala *atual = mapa;
    printf("--- Modo Mestre ---\n");
    printf("Cada pista é associada a um suspeito. Use 'l' para listar associações e 'm' para ver o mais citado.\n");
    while (atual) {
        printf("Você está em: %s\n", atual->nome);
        if (strcmp(atual->nome, "Biblioteca") == 0) inserirNaHash(tabela, "Bilhete rasgado", "Sr. Green");
        else if (strcmp(atual->nome, "Cozinha") == 0) inserirNaHash(tabela, "Pegada de lama", "Sra. White");
        else if (strcmp(atual->nome, "Sótão") == 0) inserirNaHash(tabela, "Fio de cabelo", "Sr. Green");
        else if (strcmp(atual->nome, "Escritório") == 0) inserirNaHash(tabela, "Caneta roubada", "Sra. Peacock");

        printf("Escolha (e/d/l listar/m mostrar-mais/s sair): ");
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        if (opcao[0] == 's') break;
        else if (opcao[0] == 'l') {
            listarAssociacoes(tabela);
            continue;
        } else if (opcao[0] == 'm') {
            suspeitoMaisCitado(tabela);
            continue;
        } else if (opcao[0] == 'e') {
            if (atual->esq) atual = atual->esq; else printf("Não há caminho à esquerda.\n");
        } else if (opcao[0] == 'd') {
            if (atual->dir) atual = atual->dir; else printf("Não há caminho à direita.\n");
        } else printf("Opção inválida.\n");
    }
    printf("Fim do modo Mestre. Associação final:\n");
    listarAssociacoes(tabela);
    suspeitoMaisCitado(tabela);
}

int main() {
    int escolha = -1;
    char buf[8];
    while (1) {
        printf("\nDetective Quest - escolha um modo:\n");
        printf("1 - Novato (mapa)\n2 - Aventureiro (pistas BST)\n3 - Mestre (hash de suspeitos)\n0 - Sair\n");
        printf("\nEscolha: ");
        if (!fgets(buf, sizeof(buf), stdin)) break;
        escolha = atoi(buf);
        if (escolha == 0) break;
        if (escolha == 1) jogarNovato();
        else if (escolha == 2) jogarAventureiro();
        else if (escolha == 3) jogarMestre();
        else printf("Opção inválida.\n");
    }
    printf("Obrigado por jogar!\n");
    return 0;
}

