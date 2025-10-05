#ifndef CARDAPIO_H
#define CARDAPIO_H

#include "hamburgueres.h"

/*
    O cardápio é um vetor de ponteiros para hambúrgueres
    O i-ésimo hamburguer no vetor tem ID = i
    Só existem 10 hamburgueres na hamburgueria, logo o vetor tem 10 espaços
*/
typedef struct {
    Hamburguer* hamburgueres[10];
} Cardapio;

// Aloca cada hambúrguer com alocação dinâmica para evitar uma área enorme de memória colada com vários hambúrgueres, cada um com sua pilha
void inicializa_cardapio (Cardapio* c)
{
    for (int i = 0; i < 10; i++)
    {
        Hamburguer* ptr;
        switch (i)
        {
        case 0:
            ptr = criar_bit_and_bacon(i);
            break;
        case 1:
            ptr = criar_duck_cheese(i);
            break;
        case 2:
            ptr = criar_quackteirao(i);
            break;
        case 3:
            ptr = criar_big_pato(i);
            break;
        case 4:
            ptr = criar_duck_n_break(i);
            break;
        case 5:
            ptr = criar_barberqueue(i);
            break;
        case 6:
            ptr = criar_quack_overflow(i);
            break;
        case 7:
            ptr = criar_if_duck_else(i);
            break;
        case 8:
            ptr = criar_while_quack(i);
            break;
        case 9:
            ptr = criar_quack_end(i);
            break;
        }
        c->hamburgueres[i] = ptr;
        c->hamburgueres[i]->valor = calcula_valor(c->hamburgueres[i]->receita);
    }
}

void libera_cardapio (Cardapio* c)
{
    for (int i = 0; i < 10; i++)
    {
        free(c->hamburgueres[i]);
    }
}

#endif
