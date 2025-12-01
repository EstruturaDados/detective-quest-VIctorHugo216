#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest - Nível Novato
// Mapa da Mansão com Árvore Binária
// O jogador explora a mansão navegando pela árvore de salas

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

void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao[8];
    
    printf("\n=== BEM-VINDO À MANSÃO ===\n");
    printf("Iniciando exploração (digite 'e' esquerda, 'd' direita, 's' sair)\n\n");
    
    while (atual) {
        printf(">>> Você está em: %s\n", atual->nome);
        
        if (!atual->esq && !atual->dir) {
            printf("[Chegou a uma sala-folha. Fim do caminho.]\n");
            break;
        }
        
        printf("Escolha (e/d/s): ");
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        
        if (opcao[0] == 's') {
            printf("[Saindo...]\n");
            break;
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
            printf("[Opção inválida. Use e/d/s.]\n\n");
        }
    }
    
    printf("\nFim da exploração. Obrigado por jogar!\n");
}

void montarMapa(Sala **raiz) {
    // Criação fixa da árvore de salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sótão");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");
    Sala *adega = criarSala("Adega");
    Sala *salao = criarSala("Salão de Baile");

    // Estrutura da árvore binária
    //           Hall
    //          /    \
    //      Bib        Cozinha
    //      / \            \
    //   Sotão Escrit.    Jardim
    //   /
    // Adega
    //
    // E Salão como ramificação

    hall->esq = biblioteca;
    hall->dir = cozinha;
    
    biblioteca->esq = sotao;
    biblioteca->dir = escritorio;
    
    cozinha->dir = jardim;
    
    sotao->esq = adega;

    *raiz = hall;
}

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  DETECTIVE QUEST - NÍVEL NOVATO      ║\n");
    printf("║  Mapa da Mansão com Árvore Binária    ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    Sala *mapa = NULL;
    montarMapa(&mapa);
    
    if (!mapa) {
        printf("Erro ao criar o mapa!\n");
        return 1;
    }
    
    explorarSalas(mapa);
    
    return 0;
}
