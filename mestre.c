#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *s);

// Desafio Detective Quest - Nível Mestre
// Relacionamento de Pistas com Suspeitos via Tabela Hash
// O jogador coleta pistas, que são automaticamente associadas a suspeitos

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

// -------- Tabela Hash para Suspeitos --------
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
    for (int i = 0; i < HASH_SIZE; ++i)
        tabela[i] = NULL;
}

SuspeitoEntry* buscarSuspeitoBucket(SuspeitoEntry *head, const char *nome) {
    for (SuspeitoEntry *p = head; p; p = p->prox)
        if (strcmp(p->nome, nome) == 0) return p;
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
    
    // Adicionar pista à lista do suspeito
    PistaLista *pn = (PistaLista*)malloc(sizeof(PistaLista));
    pn->pista = strdup(pista);
    pn->prox = entry->pistas;
    entry->pistas = pn;
}

void listarAssociacoes(SuspeitoEntry *tabela[]) {
    printf("\n╔═════════════════════════════════════════╗\n");
    printf("║  ASSOCIAÇÃO PISTAS x SUSPEITOS        ║\n");
    printf("╚═════════════════════════════════════════╝\n");
    
    int encontrado = 0;
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (SuspeitoEntry *e = tabela[i]; e; e = e->prox) {
            encontrado = 1;
            printf("\n[%s]\n", e->nome);
            if (!e->pistas) {
                printf("  (Nenhuma pista associada)\n");
            } else {
                for (PistaLista *p = e->pistas; p; p = p->prox)
                    printf("  * %s\n", p->pista);
            }
        }
    }
    
    if (!encontrado) {
        printf("(Nenhum suspeito registrado)\n");
    }
}

void suspeitoMaisCitado(SuspeitoEntry *tabela[]) {
    const char *melhor = NULL;
    int max = 0;
    
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (SuspeitoEntry *e = tabela[i]; e; e = e->prox) {
            int cnt = 0;
            for (PistaLista *p = e->pistas; p; p = p->prox)
                ++cnt;
            
            if (cnt > max) {
                max = cnt;
                melhor = e->nome;
            }
        }
    }
    
    printf("\n╔═════════════════════════════════════════╗\n");
    printf("║  RESULTADO DA INVESTIGAÇÃO            ║\n");
    printf("╚═════════════════════════════════════════╝\n");
    
    if (melhor && max > 0) {
        printf("\n[SUSPEITO MAIS CITADO]\n");
        printf("Nome: %s\n", melhor);
        printf("Pistas associadas: %d\n", max);
        printf("\n>>> Este é o seu principal suspeito! <<<\n");
    } else {
        printf("\nNenhum suspeito foi associado a pistas.\n");
    }
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

// Retorna pista e suspeito associado para esta sala
void obterPistaESuspeito(const char *nomeSala, const char **pista, const char **suspeito) {
    *pista = NULL;
    *suspeito = NULL;
    
    if (strcmp(nomeSala, "Biblioteca") == 0) {
        *pista = "Bilhete rasgado";
        *suspeito = "Sr. Green";
    } else if (strcmp(nomeSala, "Cozinha") == 0) {
        *pista = "Pegada de lama";
        *suspeito = "Sra. White";
    } else if (strcmp(nomeSala, "Sótão") == 0) {
        *pista = "Fio de cabelo";
        *suspeito = "Sr. Green";
    } else if (strcmp(nomeSala, "Escritório") == 0) {
        *pista = "Caneta roubada";
        *suspeito = "Sra. Peacock";
    } else if (strcmp(nomeSala, "Adega") == 0) {
        *pista = "Vidro quebrado";
        *suspeito = "Sr. Mustard";
    } else if (strcmp(nomeSala, "Jardim") == 0) {
        *pista = "Relógio de bolso";
        *suspeito = "Sra. Scarlet";
    }
}

void explorarSalas(Sala *raiz, SuspeitoEntry *tabela[]) {
    Sala *atual = raiz;
    char opcao[8];
    
    printf("\n=== BEM-VINDO À MANSÃO - MODO MESTRE ===\n");
    printf("Investigue a mansão e relacione pistas a suspeitos!\n");
    printf("Digite 'e' (esquerda), 'd' (direita), 'l' (listar), 'm' (mostrar culpado), 's' (sair)\n\n");
    
    while (atual) {
        printf(">>> Você está em: %s\n", atual->nome);
        
        // Verifica se há pista nesta sala
        const char *pista = NULL;
        const char *suspeito = NULL;
        obterPistaESuspeito(atual->nome, &pista, &suspeito);
        
        if (pista && suspeito) {
            printf("[+] Pista encontrada: '%s'\n", pista);
            printf("[+] Associada a: %s\n", suspeito);
            inserirNaHash(tabela, pista, suspeito);
        }
        
        if (!atual->esq && !atual->dir) {
            printf("[Chegou a uma sala-folha. Fim do caminho.]\n");
            break;
        }
        
        printf("Escolha (e/d/l/m/s): ");
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        
        if (opcao[0] == 's') {
            printf("[Saindo...]\n");
            break;
        } else if (opcao[0] == 'l') {
            listarAssociacoes(tabela);
            printf("\n");
            continue;
        } else if (opcao[0] == 'm') {
            suspeitoMaisCitado(tabela);
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
            printf("[Opção inválida. Use e/d/l/m/s.]\n\n");
        }
    }
    
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║  FIM DA EXPLORAÇÃO - INVESTIGAÇÃO    ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    listarAssociacoes(tabela);
    suspeitoMaisCitado(tabela);
    
    printf("\n\nObrigado por jogar!\n");
}

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  DETECTIVE QUEST - NÍVEL MESTRE      ║\n");
    printf("║  Pistas, Suspeitos e Tabela Hash      ║\n");
    printf("╚═══════════════════════════════════════╝\n");
    
    Sala *mapa = NULL;
    montarMapa(&mapa);
    
    if (!mapa) {
        printf("Erro ao criar o mapa!\n");
        return 1;
    }
    
    SuspeitoEntry *tabela[HASH_SIZE];
    inicializarHash(tabela);
    
    explorarSalas(mapa, tabela);
    
    return 0;
}
