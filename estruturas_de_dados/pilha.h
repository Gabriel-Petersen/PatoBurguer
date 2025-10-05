#ifndef PILHA_H
#define PILHA_H
#include "../itens/ingredientes.h"
#include <stdbool.h>

#define MAX 16

typedef Ingrediente tp_item;

typedef struct {
	int topo;
	tp_item item[MAX];
} tp_pilha;

void inicializa_pilha(tp_pilha *p) {
	p->topo=-1;
}
bool pilha_cheia(tp_pilha *p) {
	if(p->topo==MAX-1)return 1;
	return 0;
}
bool pilha_vazia(tp_pilha *p) {
	if(p->topo==-1)return 1;
	return 0;
}
bool push(tp_pilha *p,tp_item a) {
	if(pilha_cheia(p))return 0;
	p->topo++;
	p->item[p->topo]=a;
	return 1;
}
bool pop(tp_pilha *p,tp_item *a) {
	if(pilha_vazia(p))return 0;
    tp_item retorno = p->item[p->topo];
    if (a != NULL)
	    *a=retorno;
	p->topo--;
	return 1;
}
bool top(tp_pilha *p,tp_item *a){
	if(pilha_vazia(p))return 0;
    tp_item retorno = p->item[p->topo];
    if (a != NULL){
	    *a=retorno;
	}
	
	return 1;
}
int altura_pilha(tp_pilha *p){
	return p->topo+1;
}

int compara_pilhas(tp_pilha pilha1, tp_pilha pilha2)
{
    tp_item a,b;
    int qtd_erros = 0;
        
    while(!pilha_vazia(&pilha1) && !pilha_vazia(&pilha2))
    {
        pop(&pilha1,&a);
        pop(&pilha2,&b);
        if(a.id!=b.id)
            qtd_erros++;
    }
    
    if (!pilha_vazia(&pilha1))
        qtd_erros += altura_pilha(&pilha1);
    else if (!pilha_vazia(&pilha2))
        qtd_erros += altura_pilha(&pilha2);
        
    return qtd_erros;
}

#endif