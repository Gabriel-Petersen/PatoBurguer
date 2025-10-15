#include <stdio.h>
#include <stdlib.h>

typedef long double ldouble;
#define DINHEIRO_INICIAL 100
typedef struct {
    char nome[30];
    ldouble dinheiro;
    int dia_atual;
    //RBTree* estoque;
} Jogador;

Jogador* criar_novo_save ()
{
    Jogador* j = (Jogador*)malloc(sizeof(Jogador));
    j->dinheiro = DINHEIRO_INICIAL;
    j->dia_atual = 0;

    // Pode pedir para ele digitar o nome dele ou sla

    return j;
}

// Passe o ponteiro para o arquivo de save ou NULL caso vá criar um novo save
Jogador* inicializa_jogador (FILE* arquivo)
{
    if (arquivo == NULL)
    {
        return criar_novo_save();
    }
    else
    {
        // Aqui chama a função para carregar um save a partir do arquivo passado como parâmetro
        return criar_novo_save();
    }
}

Jogador* destruir_jogador (Jogador* j)
{
    // destruir_arvore (j->estoque);
    free(j);
    j = NULL;
    return j;
}