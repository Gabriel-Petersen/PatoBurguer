#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include "estruturas_de_dados/abb.h"

typedef struct{
    int id;
    int quantidade;
} quant_bur;

#define QTD_ING_INICIAL 4
typedef struct {
    float dinheiro;
    int dia_atual;
    quant_bur itens_vendidos[10];
    Arvore estoque;
} Jogador;
void inicializa_estoque(Jogador **j){
	(*j)->estoque=inicializa_arvore();
	inserir(&(*j)->estoque,(tp_item_arvore){7,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){3,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){1,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){5,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){0,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){2,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){4,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){6,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){9,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){8,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){11,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){10,QTD_ING_INICIAL});
	inserir(&(*j)->estoque,(tp_item_arvore){12,QTD_ING_INICIAL});
	return;
}

Jogador* inicializa_jogador (int dinheiro_inicial)
{
    Jogador* j = (Jogador*)malloc(sizeof(Jogador));
    
    inicializa_estoque(&j);
    
    j->dinheiro = dinheiro_inicial;
    j->dia_atual = 1;

    for(int i=0;i<10;i++){
        j->itens_vendidos[i].id = i;
        j->itens_vendidos[i].quantidade = 0;
    };

    return j;
}

Jogador* destruir_jogador (Jogador* j)
{
    destruir_arvore(j->estoque);
    free(j);
    j = NULL;
    return j;
}

// Cria o arquivo que vai salvar os dados em disco
int inicializa_player_save(int dinheiro_inicial){

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
    fprintf(pPlayer,"Dinheiro %.2f$\n", (float)dinheiro_inicial);
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