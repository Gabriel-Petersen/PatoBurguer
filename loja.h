#ifndef LOJA_H
#define LOJA_H

#define USE_SHORTCUTS
#include"include/graphycs_all.h"
#include"estruturas_de_dados/listade.h"
#include"assets/piskel_ingredientes.h"
#include"assets/piskel_molhos.h"
#include"assets/fundo_loja.h"
#include"jogador.h"
#include"audio.h"

#define POS_TITULO nv2(38, -12)
#define POS_PRECO nv2(40, 13)

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
	printf("\nQuantidade de moedas: %.2f",j->dinheiro);
	return;
}
void deixa_circular(Lista_DE *l){
	l->fim->prox=l->ini;
	l->ini->ant=l->fim;
	return;
}
void remove_circularidade(Lista_DE* l){
	l->ini->ant = l->fim->prox = NULL;
}
void preenche_ingredientes(Ingrediente ingredientes[]){
	ingredientes[0]=PAO_CIMA;
    ingredientes[1]=PAO_BAIXO;
    ingredientes[2]=CARNE;
    ingredientes[3]=BACON;
    ingredientes[4]=QUEIJO;
    ingredientes[5]=ALFACE;
    ingredientes[6]=TOMATE;
    ingredientes[7]=CEBOLA;
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
		centralizar_objeto(Item.obj);

		Item.titulo = criar_objeto_de_texto(1, 1, Item.ing.nome);
		if (Item.titulo == NULL) printf("Titulo do item %s ficou nulo\n", Item.ing.nome); 
		centralizar_objeto(Item.titulo);
		somar_cor_obj(Item.titulo, COLOR_ROXO);
		Obj ret;
		ret = criar_retangulo_monocromatico(criar_cor(224, 255, 255), vector_sum(Item.titulo->size, nv2(5, 1)));
		centralizar_objeto(ret);
		Item.titulo = merge_objeto(Item.titulo, ret, VETOR_NULO);

		Item.preco = criar_objeto_de_texto(1, 1, "Custo! %.2f", Item.ing.valor);
		if (Item.preco == NULL) printf("Titulo do item %s ficou nulo\n", Item.ing.nome); 
		centralizar_objeto(Item.preco);
		somar_cor_obj(Item.preco, COLOR_ROXO);
		ret = criar_retangulo_monocromatico(criar_cor(224, 255, 255), vector_sum(Item.preco->size, nv2(5, 1)));
		centralizar_objeto(ret);
		Item.preco = merge_objeto(Item.preco, ret, VETOR_NULO);

		insere_fim(I,Item);
	}
	deixa_circular(I);
	return I;
}
typedef struct{
	Screen *main_tela;
    Obj fundo_loja;
}CenarioLoja;


/*
	Gerencia todo o fluxo da loja -> mostragem de itens, compra e venda, em cima de uma lista duplamente encadeada circular
*/
void iniciar_loja(Jogador *J){
	troca_tema_com_fade("tema2");

	CenarioLoja cenario;
	cenario.main_tela=criar_tela(nv2(131,31),criar_cor(93,0,0),50);
	cenario.fundo_loja=criar_piskel_obj(fundo_loja_data[0],FUNDO_LOJA_FRAME_WIDTH,FUNDO_LOJA_FRAME_HEIGHT);
	centralizar_objeto(cenario.fundo_loja);
	desenhar_objeto(cenario.main_tela, cenario.fundo_loja);
	print_rgb_txt(COLOR_CIANO, nv2(-1, -1), "Bem vindo à loja!\n");
	printf("Aqui você pode comprar e vender ingredientes. Fique atento à sua quantidade de moedas, pois se elas acabarem você perde o jogo.\n");
	printf("Boas compras!\n");
	print_rgb_txt(COLOR_AMARELO, nv2(-1, -1), "Pressione ENTER para continuar\n");
	getchar();
	Lista_DE *I=criar_lista_de_ingredientes();
	Nodo *p=I->ini;
	teleportar_objeto(cenario.main_tela,p->ant->info.obj,nv2(-27,0));
	teleportar_objeto(cenario.main_tela,p->info.obj,nv2(0,-5));
	teleportar_objeto(cenario.main_tela,p->prox->info.obj,nv2(27,0));

    teleportar_objeto(cenario.main_tela,p->info.titulo,POS_TITULO);
	teleportar_objeto(cenario.main_tela,p->info.preco, POS_PRECO);

	while(true){
		render(cenario.main_tela,true);
		mostrar_controles_loja();
		mostrar_grana(J);
		char input=ler_teclado();
		switch(input){
			case 'X':
				esconder_objeto(cenario.main_tela,p->prox->info.obj);
				esconder_objeto(cenario.main_tela,p->ant->info.obj);
				esconder_objeto(cenario.main_tela,p->info.obj);
				esconder_objeto(cenario.main_tela, p->info.titulo);
				esconder_objeto(cenario.main_tela, p->info.preco);
				Nodo* atu = I->ini;
				for(int i=0;i<QTD_INGREDIENTES;i++, atu=atu->prox){
					excluir_objeto(atu->info.obj);
					excluir_objeto(atu->info.titulo);
					excluir_objeto(atu->info.preco);
				}
				remove_circularidade(I);
				destruir_lista(I);
				esconder_objeto(cenario.main_tela,cenario.fundo_loja);
				excluir_objeto(cenario.fundo_loja);
				excluir_tela(cenario.main_tela);
				return;
			case 'D':
			case 'A':
				audio_play("woosh", false);
				esconder_objeto(cenario.main_tela,p->ant->info.obj);
				esconder_objeto(cenario.main_tela,p->info.obj);
				esconder_objeto(cenario.main_tela,p->prox->info.obj);

				esconder_objeto(cenario.main_tela, p->info.titulo);
				esconder_objeto(cenario.main_tela, p->info.preco);

				if (input == 'A')
					p=p->ant;
				else
					p=p->prox;
				
				teleportar_objeto(cenario.main_tela,p->ant->info.obj,nv2(-27,0));
				teleportar_objeto(cenario.main_tela,p->info.obj,nv2(0,-5));
				teleportar_objeto(cenario.main_tela,p->prox->info.obj,nv2(27,0));

				teleportar_objeto(cenario.main_tela, p->info.titulo, POS_TITULO);
				teleportar_objeto(cenario.main_tela, p->info.preco, POS_PRECO);
				break;
			case 'C':
				audio_play("cash", false);
				if(J->dinheiro>=p->info.ing.valor){
					J->dinheiro-=p->info.ing.valor;
					//adiciona o ingrediente ao estoque
				}
				else printf("\nDinheiro insuficiente");
				break;
			case 'V':
				audio_play("cash", false);
				//if(buscar_no_estoque(jogador->estoque,p->info.ing)==NULL)break;
				J->dinheiro+=p->info.ing.valor;
				//remove o ingrediente do estoque
				break;
		}
	}
}

#endif