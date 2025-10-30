#ifndef LOJA_H
#define LOJA_H

#define USE_SHORTCUTS
#include"include/graphycs_all.h"
#include"estruturas_de_dados/listade.h"
#include"assets/piskel_ingredientes.h"
#include"assets/piskel_molhos.h"
#include"jogador.h"

void mostrar_controles(){
    printf("Controles:\n");
    printf("X: Sair da Loja | ");
    printf("D: Próximo Ingrediente | ");
    printf("A: Ingrediente Anterior | ");
    printf("C: Comprar Ingrediente | ");
    printf("V: Vender Ingrediente | ");
    return;
}
inline void mostrar_grana(Jogador *j){
	printf("\nQuantidade de moedas: %.2Lf",j->dinheiro);
	return;
}
void deixa_circular(Lista_DE *l){
	l->fim->prox=l->ini;
	l->ini->ant=l->fim;
	return;
}
void preenche_ingredientes(Ingrediente ingredientes[]){
	ingredientes[0]=PAO_CIMA;
    ingredientes[1]=PAO_BAIXO;
    ingredientes[2]=CARNE;
    ingredientes[3]=BACON;
    ingredientes[4]=QUEIJO;
    ingredientes[5]=ALFACE;
    ingredientes[6]=TOMATE;
    ingredientes[7]=CEBOLA_CARAMELIZADA;
    ingredientes[8]=PICLES;
    ingredientes[9]=OVO_FRITO;
    ingredientes[10]=ONION_RINGS;
	ingredientes[11]=MAIONESE_TEMPERADA;
    ingredientes[12]=BARBERCUE;
	return;
}
typedef struct{
	Screen *main_tela;
    Obj fundo_loja; // Se sobrar tempo, fazer um desenho no piskel e colocar o arquivo na pasta assets para fazer um fundinho p/ loja
}CenarioLoja;

void iniciar_loja(Jogador *J){
	CenarioLoja cenario;
	cenario.main_tela=criar_tela(nv2(131,31),COLOR_CIANO,50);
	// Se você fizer um desenho, faça cenario.fundo_loja = criar_piskel bla bla bla
	// Se você fizer um desenho, centralize cenario.fundo_loja e desenhe. Note que deve esconder e excluir ao limpar memória
	printf("Bem vindo à loja!\n");
	printf("Aqui você pode comprar e vender ingredientes. Fique atento à sua quantidade de moedas, pois se elas acabarem você perde o jogo.\n");
	printf("Boas compras!\n");

	// Coloca o printf (digite algo para seguir)
	// coloca getchar() -> olhe a main para ter uma referência

	Lista_DE *I=inicializa_lista();
	Ingrediente ingredientes[QTD_INGREDIENTES];
	preenche_ingredientes(ingredientes);
	for(int i=0;i<QTD_INGREDIENTES;i++){
		ItemDaLista Item;
		Item.ing=ingredientes[i];
		if (i < 11)
			Item.obj=criar_piskel_obj(ingredientes_data[i],INGREDIENTES_FRAME_WIDTH,INGREDIENTES_FRAME_HEIGHT);
		else
			Item.obj=criar_piskel_obj(molhos_data[i-11],MOLHOS_FRAME_WIDTH,MOLHOS_FRAME_HEIGHT);
		insere_fim(I,Item);
		centralizar_objeto(Item.obj);
	}
	deixa_circular(I);
	Nodo *p=I->ini;

	// desenhe p->info.obj

	while(true){
		render(cenario.main_tela,true);
		mostrar_controles();
		char input=ler_teclado();
		switch(input){
			case 'X':
				// LIMPE MEMÓRIA AQUI
				// Limpe memória com uma função a parte ou aqui dentro mesmo
				// Para percorrer a lista:
				// for(int i = 0, Nodo *atu = I->ini; i < QTD_INGREDIENTES; i++, atu = atu->prox)
				//{
					// esconda atu->info.obj
					// exclua atu->info.obj
				//}
				return;
			case 'D':
				esconder_objeto(cenario.main_tela,p->info.obj);
				p=p->prox;
				desenhar_objeto(cenario.main_tela,p->info.obj);
				break;
			case 'A':
				esconder_objeto(cenario.main_tela,p->info.obj);
				p=p->ant;
				desenhar_objeto(cenario.main_tela,p->info.obj);
				break;
			case 'C':
				if(J->dinheiro>=p->info.ing.valor){
					J->dinheiro-=p->info.ing.valor;
					//adiciona o ingrediente ao estoque
				}
				else printf("Dinheiro insuficiente\n");
				break;
			case 'V':
				// Coloque um if no comentário: 
				// Se buscar(jogador->estoque, p->info.ing) for nulo é porque ele não tem esse ingrediente no estoque -> break;
				J->dinheiro+=p->info.ing.valor;
				//remove o ingrediente do estoque
				break;
		}
		mostrar_grana(J);
	}
}

#endif