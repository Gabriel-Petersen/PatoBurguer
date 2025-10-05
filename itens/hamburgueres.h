#ifndef HAMBURGUERES_H
#define HAMBURGUERES_H
#include "../estruturas_de_dados/pilha.h"
#include<stdlib.h>
#include<string.h>
#define MARGEM_LUCRO 0.1 //10% de lucro

typedef struct{
    char nome[30];
    tp_pilha receita;
    int id;
    float valor;
}Hamburguer;

// Dada uma pilha de ingredientes, calcula o valor que hambúrguer deve ser vendido
float calcula_valor(tp_pilha ingredientes)
{
    float v = 0;
    Ingrediente i;
    while (pop(&ingredientes, &i))
    {
        v += i.valor;
    }

    return (1 + MARGEM_LUCRO) * v;
}

Hamburguer* criar_bit_and_bacon(int i)
{
	Hamburguer* bit_and_bacon = (Hamburguer*)malloc(sizeof(Hamburguer));
	
    strcpy(bit_and_bacon->nome,"Bit and bacon");
    bit_and_bacon->id=i;
    
    inicializa_pilha(&(bit_and_bacon->receita));
    push(&(bit_and_bacon->receita),PAO_BAIXO);
    push(&(bit_and_bacon->receita),CARNE);
    push(&(bit_and_bacon->receita),QUEIJO);
    push(&(bit_and_bacon->receita),BACON);
    push(&(bit_and_bacon->receita),PAO_CIMA);
    return bit_and_bacon;
}

Hamburguer* criar_duck_cheese(int i)
{
    Hamburguer* duck_cheese = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(duck_cheese->nome,"Duck Cheese");
    duck_cheese->id=i;
    
    inicializa_pilha(&(duck_cheese->receita));
    push(&(duck_cheese->receita),PAO_BAIXO);
    push(&(duck_cheese->receita),QUEIJO);
    push(&(duck_cheese->receita),TOMATE);
    push(&(duck_cheese->receita),ALFACE);
    push(&(duck_cheese->receita),PAO_CIMA);

    return duck_cheese;
}

Hamburguer* criar_quackteirao(int i)
{
    Hamburguer* quackteirao = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(quackteirao->nome,"Quackteirão");
    quackteirao->id=i;
    
    inicializa_pilha(&(quackteirao->receita));
    push(&(quackteirao->receita),PAO_BAIXO);
    push(&(quackteirao->receita),QUEIJO);
    push(&(quackteirao->receita),CARNE);
    push(&(quackteirao->receita),QUEIJO);
    push(&(quackteirao->receita),ALFACE);
    push(&(quackteirao->receita),PAO_CIMA);

    return quackteirao;
}

Hamburguer* criar_big_pato(int i)
{
    Hamburguer* big_pato = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(big_pato->nome,"Big Pato");
    big_pato->id=i;
    
    inicializa_pilha(&(big_pato->receita));
    push(&(big_pato->receita),PAO_BAIXO);
    push(&(big_pato->receita),ALFACE);
    push(&(big_pato->receita),QUEIJO);
    push(&(big_pato->receita),CARNE);
    push(&(big_pato->receita),PAO_BAIXO);
    push(&(big_pato->receita),ALFACE);
    push(&(big_pato->receita),CARNE);
    push(&(big_pato->receita),PAO_CIMA);

    return big_pato;
}

Hamburguer* criar_duck_n_break(int i)
{
    Hamburguer* duck_n_break = (Hamburguer*)malloc(sizeof(Hamburguer));
    
	strcpy(duck_n_break->nome,"Duck && Break");
    duck_n_break->id=i;
    
    inicializa_pilha(&(duck_n_break->receita));
    push(&(duck_n_break->receita),PAO_BAIXO);
    push(&(duck_n_break->receita),MAIONESE_TEMPERADA);
    push(&(duck_n_break->receita),CARNE);
    push(&(duck_n_break->receita),OVO_FRITO);
    push(&(duck_n_break->receita),QUEIJO);
    push(&(duck_n_break->receita),BACON);
    push(&(duck_n_break->receita),PAO_CIMA);
    
    return duck_n_break;
}

Hamburguer* criar_barberqueue(int i)
{
    Hamburguer* barberqueue = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(barberqueue->nome,"Barberqueue");
    barberqueue->id=i;
    
    inicializa_pilha(&(barberqueue->receita));
    push(&(barberqueue->receita), PAO_BAIXO);
    push(&(barberqueue->receita), CARNE);
    push(&(barberqueue->receita), ALFACE);
    push(&(barberqueue->receita), BARBERCUE);
    push(&(barberqueue->receita), ONION_RINGS);
    push(&(barberqueue->receita), QUEIJO);
    push(&(barberqueue->receita), PAO_CIMA);
    
    return barberqueue;
}

Hamburguer* criar_quack_overflow(int i)
{
    Hamburguer* quack_overflow = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(quack_overflow->nome,"Quack Overflow");
    quack_overflow->id=i;
    
    inicializa_pilha(&(quack_overflow->receita));
    push(&(quack_overflow->receita), PAO_BAIXO);
    push(&(quack_overflow->receita), QUEIJO);
    push(&(quack_overflow->receita), PICLES);
    push(&(quack_overflow->receita), CEBOLA_CARAMELIZADA);
    push(&(quack_overflow->receita), ONION_RINGS);
    push(&(quack_overflow->receita), CARNE);
    push(&(quack_overflow->receita), PAO_BAIXO);
    push(&(quack_overflow->receita), CARNE);
    push(&(quack_overflow->receita), ONION_RINGS);
    push(&(quack_overflow->receita), CEBOLA_CARAMELIZADA);
    push(&(quack_overflow->receita), PICLES);
    push(&(quack_overflow->receita), QUEIJO);
    push(&(quack_overflow->receita), PAO_CIMA);
    
    return quack_overflow;
}

Hamburguer* criar_if_duck_else(int i)
{
    Hamburguer* if_duck_else = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(if_duck_else->nome,"if (Duck) else");
    if_duck_else->id=i;
    
    inicializa_pilha(&(if_duck_else->receita));
    push(&(if_duck_else->receita), PAO_BAIXO);
    push(&(if_duck_else->receita), MAIONESE_TEMPERADA);
    push(&(if_duck_else->receita), QUEIJO);
    push(&(if_duck_else->receita), CARNE);
    push(&(if_duck_else->receita), OVO_FRITO);
    push(&(if_duck_else->receita), ALFACE);
    push(&(if_duck_else->receita), PAO_CIMA);

    return if_duck_else;
}

Hamburguer* criar_while_quack(int i)
{
    Hamburguer* while_quack = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(while_quack->nome,"while (Quack)");
    while_quack->id=i;

    inicializa_pilha(&(while_quack->receita));
    push(&(while_quack->receita), PAO_BAIXO);
    push(&(while_quack->receita), ALFACE);
    push(&(while_quack->receita), MAIONESE_TEMPERADA);
    push(&(while_quack->receita), CARNE);
    push(&(while_quack->receita), ONION_RINGS);
    push(&(while_quack->receita), QUEIJO);
    push(&(while_quack->receita), PAO_CIMA);

    return while_quack;
}

Hamburguer* criar_quack_end(int i)
{
    Hamburguer* quack_end = (Hamburguer*)malloc(sizeof(Hamburguer));
    
    strcpy(quack_end->nome,"Quack<<endl");
    quack_end->id=i;

	inicializa_pilha(&(quack_end->receita));
    push(&(quack_end->receita), PAO_CIMA);
    push(&(quack_end->receita), MAIONESE_TEMPERADA);
    push(&(quack_end->receita), CARNE);
    push(&(quack_end->receita), PICLES);
    push(&(quack_end->receita), TOMATE);
    push(&(quack_end->receita), ALFACE);
    push(&(quack_end->receita), QUEIJO);
    push(&(quack_end->receita), PAO_BAIXO);

    return quack_end;
}

#endif