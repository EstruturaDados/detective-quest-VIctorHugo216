#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *s);

// Desafio Detective Quest - Nível Aventureiro
// Organização de Pistas com Árvore de Busca (BST)
// O jogador explora a mansão, encontra pistas e as organiza automaticamente

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

// -------- Árvore de Busca para Pistas --------
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
    if (!*raiz) {
        *raiz = novoNoPista(texto);
        return;
    }
    
    int cmp = strcmp(texto, (*raiz)->texto);
    if (cmp == 0) return; // Pista já existe
    
    if (cmp < 0)
        inserirPista(&(*raiz)->esq, texto);
    else
        inserirPista(&(*raiz)->dir, texto);
}

void listarPistasEmOrdem(PistaNode *raiz) {
    if (!raiz) return;
    
    listarPistasEmOrdem(raiz->esq);
    printf("  * %s\n", raiz->texto);
    listarPistasEmOrdem(raiz->dir);
}

int contarPistas(PistaNode *raiz) {
    if (!raiz) return 0;
    return 1 + contarPistas(raiz->esq) + contarPistas(raiz->dir);
}

// -------- Mapa e Exploração --------
void montarMapa(Sala **raiz) {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sótão");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");
    Sala *adega = criarSala("Adega");

    hall->esq = biblioteca;
    hall->dir = cozinha;
    
    biblioteca->esq = sotao;
    biblioteca->dir = escritorio;
    
    cozinha->dir = jardim;
    
    sotao->esq = adega;

    *raiz = hall;
}

// Retorna pista se a sala possui uma, NULL caso contrário
const char* obterPistaSala(const char *nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0)
        return "Bilhete rasgado";
    else if (strcmp(nomeSala, "Cozinha") == 0)
        return "Pegada de lama";
    else if (strcmp(nomeSala, "Sótão") == 0)
        return "Fio de cabelo";
    else if (strcmp(nomeSala, "Escritório") == 0)
        return "Caneta roubada";
    else if (strcmp(nomeSala, "Adega") == 0)
        return "Vidro quebrado";
    else if (strcmp(nomeSala, "Jardim") == 0)
        return "Relógio de bolso";
    
    return NULL;
}

void explorarSalas(Sala *raiz, PistaNode **arvorePistas) {
    Sala *atual = raiz;
    char opcao[8];
    
    printf("\n=== BEM-VINDO À MANSÃO - MODO AVENTUREIRO ===\n");
    printf("Procure por pistas enquanto explora! Use 'l' para listar pistas.\n");
    printf("Digite 'e' (esquerda), 'd' (direita), 'l' (listar), 's' (sair)\n\n");
    
    while (atual) {
        printf(">>> Você está em: %s\n", atual->nome);
        
        // Verifica se há pista nesta sala
        const char *pista = obterPistaSala(atual->nome);
        if (pista) {
            printf("[+] Você encontrou uma pista: '%s'\n", pista);
            inserirPista(arvorePistas, pista);
        }
        
        if (!atual->esq && !atual->dir) {
            printf("[Chegou a uma sala-folha. Fim do caminho.]\n");
            break;
        }
        
        printf("Escolha (e/d/l/s): ");
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        
        if (opcao[0] == 's') {
            printf("[Saindo...]\n");
            break;
        } else if (opcao[0] == 'l') {
            int total = contarPistas(*arvorePistas);
            printf("\n[Pistas encontradas em ordem alfabética: %d]\n", total);
            if (total == 0) {
                printf("  (Nenhuma pista encontrada ainda)\n");
            } else {
                listarPistasEmOrdem(*arvorePistas);
            }
            printf("\n");
            continue;
        } else if (opcao[0] == 'e') {
            if (atual->esq) {
                atual = atual->esq;
                printf("[Você foi para a esquerda]\n\n");
            } else {
                printf("[Não há caminho à esquerda!]\n\n");
            }
        } else if (opcao[0] == 'd') {
            if (atual->dir) {
                atual = atual->dir;
                printf("[Você foi para a direita]\n\n");
            } else {
                printf("[Não há caminho à direita!]\n\n");
            }
        } else {
            printf("[Opção inválida. Use e/d/l/s.]\n\n");
        }
    }
    
    int total = contarPistas(*arvorePistas);
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║  FIM DA EXPLORAÇÃO - RESUMO         ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("Total de pistas coletadas: %d\n\n", total);
    
    if (total > 0) {
        printf("Pistas em ordem alfabética:\n");
        listarPistasEmOrdem(*arvorePistas);
    }
    
    printf("\nObrigado por jogar!\n");
}

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  DETECTIVE QUEST - NÍVEL AVENTUREIRO ║\n");
    printf("║  Organização de Pistas com BST        ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    Sala *mapa = NULL;
    montarMapa(&mapa);
    
    if (!mapa) {
        printf("Erro ao criar o mapa!\n");
        return 1;
    }
    
    PistaNode *arvorePistas = NULL;
    explorarSalas(mapa, &arvorePistas);
    
    return 0;
}
