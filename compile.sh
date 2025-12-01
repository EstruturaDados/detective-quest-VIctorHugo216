#!/bin/bash

# Script de compilação para Detective Quest
# Compila todos os três níveis do desafio

echo "╔════════════════════════════════════════╗"
echo "║  Detective Quest - Compilador          ║"
echo "║  Compilando todos os níveis...         ║"
echo "╚════════════════════════════════════════╝"
echo ""

COMPILER="gcc -std=c11 -Wall -Wextra"
SUCCESS=0
FAILED=0

# Função para compilar
compile_level() {
    local file=$1
    local name=$2
    
    echo -n "Compilando $name... "
    if $COMPILER "$file.c" -o "$file" 2>/dev/null; then
        echo "✓ OK"
        ((SUCCESS++))
    else
        echo "✗ ERRO"
        $COMPILER "$file.c" -o "$file"
        ((FAILED++))
    fi
}

# Compilar cada nível
compile_level "novato" "Nível Novato (Árvore Binária)"
compile_level "aventureiro" "Nível Aventureiro (BST)"
compile_level "mestre" "Nível Mestre (Tabela Hash)"

echo ""
echo "╔════════════════════════════════════════╗"
echo "║  Resultado da Compilação               ║"
echo "╚════════════════════════════════════════╝"
echo "Sucesso: $SUCCESS"
echo "Erros: $FAILED"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "✓ Todos os programas compilados com sucesso!"
    echo ""
    echo "Para executar:"
    echo "  ./novato        - Nível Novato"
    echo "  ./aventureiro   - Nível Aventureiro"
    echo "  ./mestre        - Nível Mestre"
else
    echo "✗ Alguns programas falharam na compilação."
    exit 1
fi
