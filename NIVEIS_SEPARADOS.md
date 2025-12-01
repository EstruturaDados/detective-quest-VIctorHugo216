# Detective Quest - Três Níveis Separados

Bem-vindo ao desafio **Detective Quest**! Este projeto implementa três níveis progressivos de um jogo de investigação em C, utilizando estruturas de dados fundamentais.

---

## 📋 Estrutura do Projeto

```
/workspaces/detective-quest-VIctorHugo216/
├── novato.c              # Nível Novato: Árvore Binária de Salas
├── aventureiro.c         # Nível Aventureiro: Árvore de Busca (BST) de Pistas
├── mestre.c              # Nível Mestre: Tabela Hash de Suspeitos
└── README.md             # Este arquivo
```

---

## 🎮 Níveis Disponíveis

### 🌱 **Nível Novato: Mapa da Mansão com Árvore Binária**

**Objetivo:** Explorar a mansão navegando por uma árvore binária de salas.

**Funcionalidades:**
- Estrutura de dados: Árvore binária simples
- Navegação: Esquerda (e), Direita (d), Sair (s)
- O jogador explora até chegar a uma sala-folha (fim do caminho)

**Compilação e Execução:**
```bash
gcc -std=c11 -Wall -Wextra novato.c -o novato
./novato
```

**Mapa da Mansão:**
```
        Hall de Entrada
           /            \
      Biblioteca      Cozinha
       /      \              \
    Sótão  Escritório     Jardim
    /
 Adega
```

**Exemplo de Sessão:**
```
Você está em: Hall de Entrada
Escolha (e/d/s): e
[Você foi para a esquerda]

Você está em: Biblioteca
Escolha (e/d/s): e
[Você foi para a esquerda]

Você está em: Sótão
Escolha (e/d/s): s
[Saindo...]
```

---

### 🛡️ **Nível Aventureiro: Organização de Pistas com Árvore de Busca**

**Objetivo:** Explorar a mansão, encontrar pistas e organizá-las automaticamente em ordem alfabética.

**Funcionalidades:**
- Estrutura de dados: Árvore Binária de Busca (BST)
- Pistas são inseridas automaticamente ao visitar certas salas
- As pistas são armazenadas em ordem alfabética na BST
- Opções: Esquerda (e), Direita (d), Listar pistas (l), Sair (s)

**Salas com Pistas:**
- **Biblioteca**: "Bilhete rasgado"
- **Cozinha**: "Pegada de lama"
- **Sótão**: "Fio de cabelo"
- **Escritório**: "Caneta roubada"
- **Adega**: "Vidro quebrado"
- **Jardim**: "Relógio de bolso"

**Compilação e Execução:**
```bash
gcc -std=c11 -Wall -Wextra aventureiro.c -o aventureiro
./aventureiro
```

**Exemplo de Sessão:**
```
Você está em: Hall de Entrada
Escolha (e/d/l/s): e
[Você foi para a esquerda]

Você está em: Biblioteca
[+] Você encontrou uma pista: 'Bilhete rasgado'
Escolha (e/d/l/s): l

[Pistas encontradas em ordem alfabética: 1]
  * Bilhete rasgado

Escolha (e/d/l/s): d
[Você foi para a direita]

Você está em: Escritório
[+] Você encontrou uma pista: 'Caneta roubada'
Escolha (e/d/l/s): l

[Pistas encontradas em ordem alfabética: 2]
  * Bilhete rasgado
  * Caneta roubada
```

---

### 🏆 **Nível Mestre: Suspeitos e Solução com Tabela Hash**

**Objetivo:** Investigar a mansão, relacionando pistas a suspeitos através de uma tabela hash, e deduzir o culpado.

**Funcionalidades:**
- Estrutura de dados: Tabela Hash com encadeamento
- Cada pista encontrada é automaticamente associada a um suspeito
- Função hash: `h = (h * 31 + char) % 11`
- Opções: Esquerda (e), Direita (d), Listar (l), Mostrar culpado (m), Sair (s)
- Ao final, mostra o suspeito mais citado

**Associações Pista → Suspeito:**
| Pista | Suspeito |
|-------|----------|
| Bilhete rasgado | Sr. Green |
| Pegada de lama | Sra. White |
| Fio de cabelo | Sr. Green |
| Caneta roubada | Sra. Peacock |
| Vidro quebrado | Sr. Mustard |
| Relógio de bolso | Sra. Scarlet |

**Compilação e Execução:**
```bash
gcc -std=c11 -Wall -Wextra mestre.c -o mestre
./mestre
```

**Exemplo de Sessão:**
```
Você está em: Hall de Entrada
Escolha (e/d/l/m/s): e
[Você foi para a esquerda]

Você está em: Biblioteca
[+] Pista encontrada: 'Bilhete rasgado'
[+] Associada a: Sr. Green
Escolha (e/d/l/m/s): e
[Você foi para a esquerda]

Você está em: Sótão
[+] Pista encontrada: 'Fio de cabelo'
[+] Associada a: Sr. Green
Escolha (e/d/l/m/s): m

╔═════════════════════════════════════════╗
║  RESULTADO DA INVESTIGAÇÃO            ║
╚═════════════════════════════════════════╝

[SUSPEITO MAIS CITADO]
Nome: Sr. Green
Pistas associadas: 2

>>> Este é o seu principal suspeito! <<<
```

---

## 🔧 Compilação em Lote

Para compilar todos os três programas de uma vez:

```bash
gcc -std=c11 -Wall -Wextra novato.c -o novato
gcc -std=c11 -Wall -Wextra aventureiro.c -o aventureiro
gcc -std=c11 -Wall -Wextra mestre.c -o mestre
```

Ou usando um script:
```bash
#!/bin/bash
for file in novato aventureiro mestre; do
    gcc -std=c11 -Wall -Wextra "${file}.c" -o "${file}"
    echo "✓ Compilado: ${file}"
done
```

---

## 📚 Estruturas de Dados Utilizadas

### Nível Novato
```c
typedef struct Sala {
    char nome[64];
    struct Sala *esq, *dir;
} Sala;
```

### Nível Aventureiro
```c
typedef struct PistaNode {
    char *texto;
    struct PistaNode *esq, *dir;
} PistaNode;  // Nó da Árvore de Busca
```

### Nível Mestre
```c
typedef struct PistaLista {
    char *pista;
    struct PistaLista *prox;
} PistaLista;

typedef struct SuspeitoEntry {
    char *nome;
    PistaLista *pistas;
    struct SuspeitoEntry *prox;
} SuspeitoEntry;  // Entrada na tabela hash com lista encadeada
```

---

## 🚀 Progressão Recomendada

1. **Comece pelo Novato**: Entenda como navegar a árvore binária
2. **Passe para Aventureiro**: Veja como as pistas são organizadas automaticamente em BST
3. **Finalize com Mestre**: Relacione pistas a suspeitos e deduza o culpado

---

## 💡 Dicas de Jogo

- **Explore completamente**: Visite todas as salas para coletar todas as pistas
- **Use a listagem**: Nos níveis Aventureiro e Mestre, use 'l' para revisar o que descobriu
- **Procure padrões**: Veja quais suspeitos aparecem mais frequentemente nas pistas

---

## ✨ Melhorias Futuras

- Adicionar limpeza de memória (`free()`) para evitar vazamentos
- Implementar mais salas e pistas
- Salvar/carregar estado do jogo
- Adicionar dificuldade variável
- Interface gráfica (SDL/ncurses)

---

## 📝 Notas Técnicas

- Todos os programas compilam com `-std=c11 -Wall -Wextra`
- Utilizam alocação dinâmica de memória
- BST mantém pistas em ordem alfabética
- Tabela hash usa função simples com encadeamento para colisões
- Sem bibliotecas externas além de stdio, stdlib e string

---

**Desenvolvido para o desafio Detective Quest - Enigma Studios**
