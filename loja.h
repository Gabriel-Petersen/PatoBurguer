#ifndef LOJA_H
#define LOJA_H

#define USE_SHORTCUTS
#include"include/graphycs_all.h"
#include"estruturas_de_dados/listade.h"
#include"assets/piskel_ingredientes.h"
#include"assets/piskel_molhos.h"
#include"assets/fundo_loja.h"
#include"jogador.h"

void mostrar_controles_loja(){
    printf("Controles:\n");
    printf("X: Sair da Loja | ");
    printf("D: Próximo Ingrediente | ");
    printf("A: Ingrediente Anterior | ");
    printf("C: Comprar Ingrediente | ");
    printf("V: Vender Ingrediente | ");
    return;
}
void mostrar_grana(Jogador *j){
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

Lista_DE* criar_lista_de_ingredientes ()
{
	Lista_DE *I=inicializa_lista();
	Ingrediente ingredientes[QTD_INGREDIENTES];
	preenche_ingredientes(ingredientes);
	for(int i=0;i<QTD_INGREDIENTES;i++){
		ItemDaLista Item;
		Item.ing=ingredientes[i];
		if(i<11)Item.obj=criar_piskel_obj(ingredientes_data[i],INGREDIENTES_FRAME_WIDTH,INGREDIENTES_FRAME_HEIGHT);
		else Item.obj=criar_piskel_obj(molhos_data[i-11],MOLHOS_FRAME_WIDTH,MOLHOS_FRAME_HEIGHT);
		insere_fim(I,Item);
		centralizar_objeto(Item.obj);
	}
	deixa_circular(I);
	return I;
}

typedef struct{
	Screen *main_tela;
    Obj fundo_loja; // Se sobrar tempo, fazer um desenho no piskel e colocar o arquivo na pasta assets para fazer um fundinho p/ loja
}CenarioLoja;
void iniciar_loja(Jogador *J){
	CenarioLoja cenario;
	cenario.main_tela=criar_tela(nv2(131,31),criar_cor(168,0,0),50);
	cenario.fundo_loja=criar_piskel_obj(fundo_loja_data[0],FUNDO_LOJA_FRAME_WIDTH,FUNDO_LOJA_FRAME_HEIGHT);
	centralizar_objeto(cenario.fundo_loja);
	desenhar_objeto(cenario.main_tela, cenario.fundo_loja);
	printf("Bem vindo à loja!\n");
	printf("Aqui você pode comprar e vender ingredientes. Fique atento à sua quantidade de moedas, pois se elas acabarem você perde o jogo.\n");
	printf("Boas compras!\n");
	printf("Aperte qualquer tecla para continuar\n");
	getchar();
	Lista_DE *I=criar_lista_de_ingredientes();
	Nodo *p=I->ini;
	desenhar_objeto(cenario.main_tela,p->info.obj);
	while(true){
		render(cenario.main_tela,true);
		mostrar_controles_loja();
		mostrar_grana(J);
		char input=ler_teclado();
		switch(input){
			case 'X':
				esconder_objeto(cenario.main_tela,p->info.obj);
				Nodo* atu = I->ini;
				for(int i=0;i<QTD_INGREDIENTES;i++, atu=atu->prox)excluir_objeto(atu->info.obj);
				destruir_lista(I);
				esconder_objeto(cenario.main_tela,cenario.fundo_loja);
				excluir_objeto(cenario.fundo_loja);
				excluir_tela(cenario.main_tela);
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
				//if(buscar_no_estoque(jogador->estoque,p->info.ing)==NULL)break;
				J->dinheiro+=p->info.ing.valor;
				//remove o ingrediente do estoque
				break;
		}
	
	}
}

#endif