#define USE_SHORTCUTS
#include "include/graphycs_all.h"
#include "itens/cardapio.h"
#include "estruturas_de_dados/fila_dinamica.h"
#include "estruturas_de_dados/pilha.h"
#include "assets/piskel_ingredientes.h"
#include "assets/mcBackground.h"
#include "assets/piskel_molhos.h"

Vector2 get_direcao (char c)
{
    if (c == 'W')
        return VETOR_CIMA;
    else if (c == 'S')
        return VETOR_BAIXO;
    else if (c == 'A')
        return VETOR_ESQUERDA;
    else if (c == 'D')
        return VETOR_DIREITA;
    else
        return VETOR_NULO;
}


bool etapa_de_posicionamento (Screen* tela, Objeto** montagem, Objeto* ingrediente_atual, char inpt)
{
    Vector2 dir = get_direcao(inpt);
    if (compare_vector(dir, VETOR_NULO) == false)
        mover_objeto(tela, ingrediente_atual, get_direcao(inpt));

    if (inpt == 'M')
    {
        esconder_objeto(tela, *montagem);
        esconder_objeto(tela, ingrediente_atual);
        Vector2 offset = vector_subtr((*montagem)->position, ingrediente_atual->position);
        *montagem = merge_objeto(ingrediente_atual, *montagem, offset);
        centralizar_objeto(*montagem);
        teleportar_objeto(tela, *montagem, v_prod(VETOR_BAIXO, 7));
        return true;
    }
    return false;
}

Obj criar_cursor ()
{
    Pixel* data = (Pixel*)malloc(5*sizeof(Pixel));
    data[0] = criar_pixel(COLOR_BRANCO, VETOR_NULO);
    data[1] = criar_pixel(COLOR_BRANCO, VETOR_CIMA);
    data[2] = criar_pixel(COLOR_BRANCO, VETOR_BAIXO);
    data[3] = criar_pixel(COLOR_BRANCO, VETOR_DIREITA);
    data[4] = criar_pixel(COLOR_BRANCO, VETOR_ESQUERDA);

    return criar_objeto_custom(data, 5, true);
}

void render_com_texto (Screen* tela, bool no_cardapio)
{
    render (tela, true);

    if (no_cardapio) return;

    print_rgb_txt(COLOR_VERDE, nv2(7, 3), "Pão");
    print_rgb_txt(COLOR_VERDE, nv2(6, 9), "Carne");
    print_rgb_txt(COLOR_VERDE, nv2(6, 15), "Bacon");
    print_rgb_txt(COLOR_VERDE, nv2(5, 21), "Queijo");
    print_rgb_txt(COLOR_VERDE, nv2(4, 27), "Ovo Frito");

    print_rgb_txt(COLOR_VERDE, nv2(119, 3), "Alface");
    print_rgb_txt(COLOR_VERDE, nv2(119, 9), "Tomate");
    print_rgb_txt(COLOR_VERDE, nv2(119, 15), "Cebola");
    print_rgb_txt(COLOR_VERDE, nv2(119, 21), "Picles");
    print_rgb_txt(COLOR_VERDE, nv2(117, 27), "Onion Rings");
}

int id_cursor (Obj cursor)
{
    Vector2 pos = vector_sum(nv2(65, 15), cursor->position); // posição em coordenadas de matriz do centro do cursor
    
}

void print_receita (tp_pilha receita, int x0)
{
    Ingrediente i;
    int y0 = 10;
    int tam = altura_pilha(&receita);
    for (int num = 0; pilha_vazia(&receita) == false; num++)
    {
        pop(&receita, &i);
        print_rgb_txt(COLOR_BRANCO, nv2(x0, y0), "%d - %s", tam-num, i.nome);
        y0 += 2;
        if (num == 8)
        {
            x0 += 25;
            y0 = 10;
        }
    }
}

void etapa_de_montagem (Fila_D* fila_de_pedidos, Cardapio* c)
{
    moveCursor(VETOR_NULO);
    system("cls");

    Screen* main_tela = criar_tela(nv2(131, 31), COLOR_CIANO, 50);
    Screen* cd_tela = criar_tela(nv2(131, 31), criar_cor(47, 33, 26), 60);

    Obj fundo_cardapio = criar_piskel_obj(cardapiobg_data[0], CARDAPIOBG_FRAME_WIDTH, CARDAPIOBG_FRAME_HEIGHT);
    centralizar_objeto(fundo_cardapio);
    desenhar_objeto(cd_tela, fundo_cardapio);

    Obj fundo = criar_piskel_obj(montagembg_data[0], MONTAGEMBG_FRAME_WIDTH, MONTAGEMBG_FRAME_HEIGHT);
    Obj molho_visual[] = {
        criar_piskel_obj(molhos_data[0], MOLHOS_FRAME_WIDTH, MOLHOS_FRAME_HEIGHT),
        criar_piskel_obj(molhos_data[1], MOLHOS_FRAME_WIDTH, MOLHOS_FRAME_HEIGHT)
    };

    centralizar_objeto(fundo);
    desenhar_objeto(main_tela, fundo);
    Obj cursor = criar_cursor();
    centralizar_objeto(cursor);
    desenhar_objeto(main_tela, cursor);
    for (int i = 0; i < 2; i++)
    {
        centralizar_objeto(molho_visual[i]);
        int fator = (i == 0) ? -1 : 1;
        teleportar_objeto(main_tela, molho_visual[i], nv2(fator*35, 4));
    }

    Screen* atual = main_tela;
    bool no_cardapio = false;
    int pagina = 0;
    int pagina_antiga = -1;
    Obj t1, t2;

    int hamb_id;
    dequeue(fila_de_pedidos, &hamb_id);
    Hamburguer* pedido_atual = c->hamburgueres[hamb_id];

    do
    {
        render_com_texto(atual, no_cardapio);

        char input = ler_teclado();
        switch (toupper(input))
        {
        case 'X':
            system("cls");
            moveCursor(VETOR_NULO);
            no_cardapio = (no_cardapio == false);
            atual = (no_cardapio == true) ? cd_tela : main_tela;
            limpar_buffer(atual);
            continue;
        case 'T':
            print_vector(cursor->position, "Cursor");
            break;
        }

        Vector2 v = get_direcao(input);
        if (no_cardapio)
        {
            if (compare_vector(v, VETOR_DIREITA) && pagina < 8)
            {
                pagina += 2;
            }
            else if (compare_vector(v, VETOR_ESQUERDA) && pagina > 0)
            {
                pagina -= 2;
            }

            if (pagina_antiga != pagina)
            {
                limpar_buffer(cd_tela);
                system("cls");
                if (pagina_antiga != -1)
                {
                    esconder_objeto(cd_tela, t1);
                    esconder_objeto(cd_tela, t2);
                    excluir_objeto(t1); excluir_objeto(t2);
                }
                t1  = t2 = NULL;
                t1 = criar_objeto_de_texto(1, 1, c->hamburgueres[pagina]->nome);
                somar_cor_obj(t1, COLOR_VERMELHO);
                t2 = criar_objeto_de_texto(1, 1, c->hamburgueres[pagina+1]->nome);
                somar_cor_obj(t2, COLOR_VERMELHO);
                teleportar_objeto(cd_tela, t1, nv2(-55, -11));
                teleportar_objeto(cd_tela, t2, nv2(11, -11));
                render_com_texto(atual, no_cardapio);
                print_receita(c->hamburgueres[pagina]->receita, 11);
                print_receita(c->hamburgueres[pagina+1]->receita, 75);
            }
            pagina_antiga = pagina;
        }
        else
        {
            if (compare_vector(v, VETOR_NULO) == false)
                mov_obj(atual, cursor, get_direcao(input));
        }
    } while (fila_vazia(fila_de_pedidos) == false);

    for (int i = 0; i < 2; i++)
    {
        esconder_objeto(main_tela, molho_visual[i]);
        excluir_objeto(molho_visual[i]);
        molho_visual[i] = NULL;
    }
    
    esconder_objeto(main_tela, fundo);
    excluir_objeto(fundo);
    excluir_objeto(cursor);
    fundo = NULL;
    excluir_tela(main_tela);
    excluir_tela(cd_tela);
    cd_tela = NULL;
    main_tela = NULL;
}

/*
Screen* tela = criar_tela(nv2(120, 30), COLOR_CIANO, 0);
    Objeto* montagem = criar_piskel_obj(ingredientes_data[1], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
    Objeto* ingrediente_atual = criar_piskel_obj(ingredientes_data[2], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
    centralizar_objeto(montagem);
    centralizar_objeto(ingrediente_atual);
    desenhar_objeto(tela, ingrediente_atual);

    mover_objeto(tela, montagem, v_prod(VETOR_BAIXO, 8));
    int index = 2;
    bool criando = true;
    while (true)
    {
        char inpt = ler_teclado();
        if (inpt == 'P')
            break;

        if (criando && etapa_de_posicionamento(tela, &montagem, ingrediente_atual, inpt))
        {
            if (index < 6) 
            {
                ingrediente_atual = criar_piskel_obj(ingredientes_data[++index], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
                centralizar_objeto(ingrediente_atual);
                desenhar_objeto(tela, ingrediente_atual);
            } 
            else if (index == 6)
            {
                ingrediente_atual = criar_piskel_obj(ingredientes_data[0], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
                centralizar_objeto(ingrediente_atual);
                desenhar_objeto(tela, ingrediente_atual);
                index++;
            }
            else
            {
                ingrediente_atual = NULL;
                criando = false;
            }
        }

        render(tela, true);
        printf("Pressione P para sair\n");
    }

    excluir_objeto(montagem);
    montagem = NULL;
    excluir_tela(tela);
    tela = NULL;
*/