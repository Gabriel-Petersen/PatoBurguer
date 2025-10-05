#ifndef FILA_H
#define FILA_H

#include<stdio.h>
#include<stdbool.h>
#define MAX_FILA 100

#define proximo(x) ((x+1) % MAX_FILA)

typedef int tp_item_fila;

typedef struct {
	tp_item_fila item[MAX_FILA];
	int ini, fim;
} Fila_E;

void inicializa_fila (Fila_E* q)
{
	q->fim = q->ini = MAX_FILA-1;
}

bool fila_vazia (Fila_E* q)
{
	return q->ini == q->fim;
}

bool fila_cheia (Fila_E* q)
{
	return proximo(q->fim) == q->ini;
}

bool enqueue (Fila_E* q, tp_item_fila x)
{
	if (fila_cheia(q)) return false;
	q->fim = proximo(q->fim);
	q->item[q->fim] = x;
	return true;
}

bool dequeue (Fila_E* q, tp_item_fila* x)
{
	if (fila_vazia(q)) return false;
	q->ini = proximo(q->ini);
	if (x != NULL)
		*x = q->item[q->ini];
	return true;
}

bool front (Fila_E* q, tp_item_fila* x)
{
	if (fila_vazia(q)) return false;
	if (x != NULL)
		*x = q->item[proximo(q->ini)];
	return true;
}

int tamanho_fila (Fila_E* q)
{
	if (fila_vazia(q)) return 0;
	int qtd = q->fim - q->ini;
	return (MAX_FILA + qtd) % MAX_FILA;
}

void print_fila (Fila_E q)
{
	tp_item_fila e;
	while (!fila_vazia(&q))
	{
		dequeue(&q, &e);
		printf("%d ", e);
	}
	printf("\n");
}

#endif