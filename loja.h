#include"include/graphycs_all.h"
#include"itens/cardapio.h"
#include"estruturas_de_dados/fila_dinamica.h"
#include"estruturas_de_dados/listade.h"
#include"estruturas_de_dados/pilha.h"
#include"assets/piskel_ingredientes.h"
#include"assets/mcBackground.h"
#include"assets/piskel_molhos.h"
#include"jogador.h"
void mostrar_controles(Screen* atual){
    printf("Controles:\n");
    printf("X: Sair da Loja | ");
    printf("D: Próximo Ingrediente | ");
    printf("A: Ingrediente Anterior | ");
    printf("C: Comprar Ingrediente | ");
    printf("V: Vender Ingrediente | ");
    return;
}
void mostrar_grana(Jogador *J){
	printf("Quantidade de moedas: %Lf",J.dinheiro);
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
    ingredientes[7]=MAIONESE_TEMPERADA;
    ingredientes[8]=BARBERCUE;
    ingredientes[9]=CEBOLA_CARAMELIZADA;
    ingredientes[10]=PICLES;
    ingredientes[11]=OVO_FRITO;
    ingredientes[12]=ONION_RINGS;
	return;
}
typedef struct{
	Screen *main_tela;
    Screen *cd_tela;
    Obj fundo_loja;
}CenarioLoja
void iniciar_loja(Jogador *J){
	CenarioLoja Cenario;
	Cenario.main_tela=criar_tela(nv2(131,31),COLOR_CIANO,50);
	tela=Cenario.main_tela;
	printf("Bem vindo à loja!\n");
	printf("Aqui você pode comprar e vender ingredientes. Fique atento à sua quantidade de moedas, pois se elas acabarem você perde o jogo.\n");
	printf("Boas compras!\n");
	Lista_DE *I=inicializa_lista();
	Ingrediente ingredientes[QTD_INGREDIENTES];
	preenche_ingredientes(ingredientes);
	for(int i=0;i<QTD_INGREDIENTES;i++){
		ItemDaLista Item;
		Item.ing=ingredientes[i];
		Item.obg=criar_piskel_obj(ingredientes_data[i],INGREDIENTES_FRAME_WIDTH,INGREDIENTES_FRAME_HEIGHT);
		insere_fim(I,Item);
		centralizar_objeto(Item.obg);
	}
	deixa_circular(I);
	Nodo *p=I->ini;
	while(true){
		render(tela,true);
		mostrar_controles();
		char input=ler_teclado();
		switch(input){
			case 'X':
				return;
			case 'D':
				esconder_objeto(tela,p->obj);
				p=p->prox;
				desenhar_objeto(tela,p->obj);
				break;
			case 'A':
				esconder_objeto(tela,p->obj);
				p=p->ant;
				desenhar_objeto(tela,p->obj);
				break;
			case 'C':
				if(J->dinheiro>=p->ing->valor){
					J->dinheiro-=p->ing->valor;
					//adiciona o ingrediente ao estoque
				}
				else printf("Dinheiro insuficiente\n");
				break;
			case 'V':
				J->dinheiro+=p->ing->valor;
				//remove o ingrediente do estoque
				break;
		}
		mostrar_grana(J);
	}
}