#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include "estruturas_de_dados/abb.h"

#define DINHEIRO_INICIAL 100
typedef struct {
    char nome[30];
    float dinheiro;
    int dia_atual;
    Arvore estoque;
} Jogador;
void inicializa_estoque(Jogador **j){
	(*j)->estoque=inicializa_arvore();
	inserir(&(*j)->estoque,(tp_item_arvore){7,0});
	inserir(&(*j)->estoque,(tp_item_arvore){3,0});
	inserir(&(*j)->estoque,(tp_item_arvore){1,0});
	inserir(&(*j)->estoque,(tp_item_arvore){5,0});
	inserir(&(*j)->estoque,(tp_item_arvore){0,0});
	inserir(&(*j)->estoque,(tp_item_arvore){2,0});
	inserir(&(*j)->estoque,(tp_item_arvore){4,0});
	inserir(&(*j)->estoque,(tp_item_arvore){6,0});
	inserir(&(*j)->estoque,(tp_item_arvore){9,0});
	inserir(&(*j)->estoque,(tp_item_arvore){8,0});
	inserir(&(*j)->estoque,(tp_item_arvore){11,0});
	inserir(&(*j)->estoque,(tp_item_arvore){10,0});
	inserir(&(*j)->estoque,(tp_item_arvore){12,0});
	return;
}
Jogador* criar_novo_save ()
{
    Jogador* j = (Jogador*)malloc(sizeof(Jogador));
    
    inicializa_estoque(&j);
    
    j->dinheiro = DINHEIRO_INICIAL;
    j->dia_atual = 1;

    // Pode pedir para ele digitar o nome dele ou sla

    return j;
}

// Passe o ponteiro para o arquivo de save ou NULL caso vá criar um novo save
Jogador* inicializa_jogador (FILE* arquivo)
{
    #ifdef SEM_ARQUIVO_EXTERNO
        return criar_novo_save();
    #else

    if (arquivo == NULL)
    {
        return criar_novo_save();
    }
    else
    {
        // Aqui chama a função para carregar um save a partir do arquivo passado como parâmetro
        return criar_novo_save();
    }

    #endif
}

Jogador* destruir_jogador (Jogador* j)
{
    destruir_arvore(j->estoque);
    free(j);
    j = NULL;
    return j;
}

// Cria o arquivo que vai salvar os dados em disco
int inicializa_player_save(){

    FILE* pPlayer;
    #ifndef _VSCODE
    pPlayer = fopen("player.txt","w");
    #else
    pPlayer = fopen("../player.txt","w");
    #endif

    if(pPlayer==NULL){
        printf("ERRO: Nao foi possivel criar player.txt!\n");
        return 0;
    }
    fprintf(pPlayer,"Dia %d\n",0);
    fprintf(pPlayer,"Dinheiro %.2f$\n", (float)DINHEIRO_INICIAL);
    fprintf(pPlayer,"Hambúgueres: 0\n");
    fprintf(pPlayer, "------------------------\n");


    fclose(pPlayer);

    return 1;
}

// Atualiza os dados em disco
void atualiza_player_save(Jogador* J, int quant){

    FILE* pPlayer;
    
    #ifndef _VSCODE
    pPlayer = fopen("player.txt","a");
    #else
    pPlayer = fopen("../player.txt","a");
    #endif

    fprintf(pPlayer,"Dia %d\n",J->dia_atual);
    fprintf(pPlayer,"Dinheiro %.2f$\n", J->dinheiro);
    fprintf(pPlayer,"Hambúgueres: %d\n",quant);
    fprintf(pPlayer, "------------------------\n");

    fclose(pPlayer);
}

#endif