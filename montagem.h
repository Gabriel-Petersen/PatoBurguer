#define USE_SHORTCUTS
#include "include/graphycs_all.h"
#include "itens/cardapio.h"
#include "estruturas_de_dados/fila_dinamica.h"
#include "estruturas_de_dados/pilha.h"
#include "assets/piskel_ingredientes.h"
#include "assets/mcBackground.h"
#include "assets/piskel_molhos.h"
#include "jogador.h"

typedef struct {
    Obj o;
    int i;
} IngredienteObj;

typedef struct {
    Screen *main_tela;
    Screen *cd_tela;
    Obj fundo_cardapio;
    Obj fundo_montagem;
    Obj molho_visual[2];
    Obj gaveta_extra;
    Obj cursor;
} CenarioMontagem;

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

bool esta_na_area_de_drop(Vector2 prato_pos, Objeto* cursor)
{
    const Vector2 drop_area = v_prod(nv2(20, 60), 0.5);
    Vector2 dist = nv2(
        abs(cursor->position.x - prato_pos.x),
        abs(cursor->position.y - prato_pos.y)
    );

    return (dist.x <= drop_area.x && dist.y <= drop_area.y);
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

    print_rgb_txt(COLOR_VERDE, nv2(3, 3), "Topo de Pão");
    print_rgb_txt(COLOR_VERDE, nv2(6, 9), "Carne");
    print_rgb_txt(COLOR_VERDE, nv2(6, 15), "Bacon");
    print_rgb_txt(COLOR_VERDE, nv2(5, 21), "Queijo");
    print_rgb_txt(COLOR_VERDE, nv2(4, 27), "Ovo Frito");

    print_rgb_txt(COLOR_VERDE, nv2(60, 2), "Base de Pão");

    print_rgb_txt(COLOR_VERDE, nv2(119, 3), "Alface");
    print_rgb_txt(COLOR_VERDE, nv2(119, 9), "Tomate");
    print_rgb_txt(COLOR_VERDE, nv2(119, 15), "Cebola");
    print_rgb_txt(COLOR_VERDE, nv2(119, 21), "Picles");
    print_rgb_txt(COLOR_VERDE, nv2(117, 27), "Onion Rings");
}

uint8_t cursor_no_molho (Obj cursor)
{
    const Vector2 maionese = nv2(29, 15); // Canto superior esquerdo em coordenadas de matriz
    const Vector2 barbecue = nv2(99, 15); // Canto superior esquerdo em coordenadas de matriz
    Vector2 pos = vector_sum(nv2(65, 15), cursor->position); // posição em coordenadas de matriz do centro do cursor

    if (pos.x >= maionese.x && pos.x < maionese.x + MOLHOS_FRAME_WIDTH &&
        pos.y >= maionese.y && pos.y < maionese.y + MOLHOS_FRAME_HEIGHT)
        {
            return 1;
        }
    
    if (pos.x >= barbecue.x && pos.x < barbecue.x + MOLHOS_FRAME_WIDTH &&
        pos.y >= barbecue.y && pos.y < barbecue.y + MOLHOS_FRAME_HEIGHT)
        {
            return 2;
        }
    
    return 0;
}

IngredienteObj pegar_ingrediente (Obj cursor, IngredienteObj segurando)
{
    if (segurando.o != NULL) return segurando;
    const Vector2 ret_size = nv2(15, 5);
    const Vector2 ini1 = nv2(1, 1);
    const Vector2 ini2 = nv2(115, 1);
    const Vector2 gaveta_extra_pos = nv2(46, 0);
    const Vector2 tam_gaveta = nv2(40, 4);
    const int passo = ret_size.y + 1;

    int ret_id = -1;
    Vector2 pos = vector_sum(nv2(65, 15), cursor->position); // posição em coordenadas de matriz do centro do cursor

    if (pos.x >= gaveta_extra_pos.x && pos.x < gaveta_extra_pos.x + tam_gaveta.x &&
        pos.y >= gaveta_extra_pos.y && pos.y < gaveta_extra_pos.y + tam_gaveta.y)
        {
            ret_id = 0; // se quer pão de baixo
        }
    else
    {
        if (pos.x >= ini1.x && pos.x < ini1.x + ret_size.x)
        {
            for (int i = 0; i < 5; i++)
            {
                int y = ini1.y + i*passo;
                if (pos.y >= y && pos.y < y + ret_size.y)
                {
                    ret_id = i+1;
                    break;
                }
            }
        }
        else if (pos.x >= ini2.x && pos.x < ini2.x + ret_size.x)
        {
            for (int i = 0; i < 5; i++)
            {
                int y = ini2.y + i*passo;
                if (pos.y >= y && pos.y < y + ret_size.y)
                {
                    ret_id = i+6;
                    break;
                }
            }
        }
    }
    
    if (ret_id == -1) return segurando;

    switch (ret_id)
    {
    case 0:
    case 1:
        ret_id ^= 1; // 0 vira 1 e 1 vira 0
        break;
    case 5:
        ret_id = 11;
        break;
    case 6:
    case 7:
        ret_id--;
        break;
    case 8:
    case 9:
        ret_id++;
        break;
    case 10:
        ret_id = 12;
        break;
    }

    int ing_data = ret_id;
    if (ing_data > 6) ing_data -= 2;
    Obj ingrediente = criar_piskel_obj(ingredientes_data[ing_data], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
    centralizar_objeto(ingrediente);
    ingrediente->position = cursor->position;

    return (IngredienteObj){ingrediente, ret_id};
}

void aplicar_molho_visual(Screen* tela, Obj hamburguer, Color nova_cor)
{
    if (hamburguer == NULL || hamburguer->info == NULL) return;

    esconder_objeto(tela, hamburguer); 
    
    int min_y = 0; 
    for (int i = 0; i < hamburguer->qtd_pixel; i++) 
    {
        if (hamburguer->info[i].position.y < min_y)
            min_y = hamburguer->info[i].position.y;
    }
    
    int y_limite_aplicacao = min_y + 2;

    for (int i = 0; i < hamburguer->qtd_pixel; i++)
    {
        Vector2 pos = hamburguer->info[i].position;

        if (pos.y <= y_limite_aplicacao)
        {
            if (abs(pos.x) < (hamburguer->size.x / 2))
                hamburguer->info[i].cor = nova_cor;
        }
    }

    desenhar_objeto(tela, hamburguer);
}

Obj criar_gaveta_extra ()
{
    Obj o1 = criar_retangulo_monocromatico(converter_ABGR_para_Color(0xff00284b), nv2(38, 2));
    centralizar_objeto(o1);
    Obj o2 = criar_retangulo_monocromatico(converter_ABGR_para_Color(0xff001629), nv2(40, 4));
    centralizar_objeto(o2);
    Obj gaveta_extra = merge_objeto(o1, o2, VETOR_NULO);

    return gaveta_extra;
}

CenarioMontagem inicializa_cenario_montagem()
{
    CenarioMontagem cenario;
    cenario.main_tela = criar_tela(nv2(131, 31), COLOR_CIANO, 50);
    cenario.cd_tela = criar_tela(nv2(131, 31), criar_cor(47, 33, 26), 60);

    cenario.fundo_cardapio = criar_piskel_obj(cardapiobg_data[0], CARDAPIOBG_FRAME_WIDTH, CARDAPIOBG_FRAME_HEIGHT);
    centralizar_objeto(cenario.fundo_cardapio);
    desenhar_objeto(cenario.cd_tela, cenario.fundo_cardapio);

    cenario.fundo_montagem = criar_piskel_obj(montagembg_data[0], MONTAGEMBG_FRAME_WIDTH, MONTAGEMBG_FRAME_HEIGHT);
    centralizar_objeto(cenario.fundo_montagem);
    desenhar_objeto(cenario.main_tela, cenario.fundo_montagem);

    cenario.molho_visual[0] = criar_piskel_obj(molhos_data[0], MOLHOS_FRAME_WIDTH, MOLHOS_FRAME_HEIGHT);
    cenario.molho_visual[1] = criar_piskel_obj(molhos_data[1], MOLHOS_FRAME_WIDTH, MOLHOS_FRAME_HEIGHT);
    
    cenario.gaveta_extra = criar_gaveta_extra();
    centralizar_objeto(cenario.gaveta_extra);
    teleportar_objeto(cenario.main_tela, cenario.gaveta_extra, nv2(0, -(cenario.main_tela->screen_size.y/2 - 1)));

    cenario.cursor = criar_cursor();
    centralizar_objeto(cenario.cursor);
    desenhar_objeto(cenario.main_tela, cenario.cursor);

    for (int i = 0; i < 2; i++)
    {
        centralizar_objeto(cenario.molho_visual[i]);
        int fator = (i == 0) ? -1 : 1;
        teleportar_objeto(cenario.main_tela, cenario.molho_visual[i], nv2(fator*35, 4));
    }

    return cenario;
}

void limpa_cenario_montagem(CenarioMontagem cenario)
{
    for (int i = 0; i < 2; i++)
    {
        esconder_objeto(cenario.main_tela, cenario.molho_visual[i]);
        excluir_objeto(cenario.molho_visual[i]);
    }
    esconder_objeto(cenario.main_tela, cenario.fundo_montagem);
    esconder_objeto(cenario.main_tela, cenario.cursor);
    esconder_objeto(cenario.main_tela, cenario.gaveta_extra);
    esconder_objeto(cenario.cd_tela, cenario.fundo_cardapio);

    excluir_objeto(cenario.fundo_montagem);
    excluir_objeto(cenario.fundo_cardapio);
    excluir_objeto(cenario.cursor);
    excluir_objeto(cenario.gaveta_extra);

    excluir_tela(cenario.main_tela);
    excluir_tela(cenario.cd_tela);
}

Obj monta_ingrediente(Screen* atual, Obj hamburguer, IngredienteObj* ingrediente_atual, 
                        Cardapio* c, tp_pilha* montagem, Vector2 prato_pos)
{
    Ingrediente novo = c->ingredientes[ingrediente_atual->i];
    esconder_objeto(atual, ingrediente_atual->o);
    esconder_objeto(atual, hamburguer);
    ingrediente_atual->o->position.x = hamburguer->position.x;
    int bonus = 2*(novo.id == PAO_CIMA.id);
    ingrediente_atual->o->position.y = hamburguer->position.y - hamburguer->size.y + 1 - bonus;
    Vector2 offset = vector_subtr(hamburguer->position, ingrediente_atual->o->position);

    Obj novo_hamburguer = merge_objeto(ingrediente_atual->o, hamburguer, offset);
    centralizar_objeto(novo_hamburguer);
    alterar_pivot_obj(novo_hamburguer, nv2(0, novo_hamburguer->size.y/2));
    teleportar_objeto(atual, novo_hamburguer, prato_pos);
    push(montagem, novo);

    ingrediente_atual->o = NULL;
    ingrediente_atual->i = -1;
    
    return novo_hamburguer;
}

void limpar_area_receita(Screen* tela_cardapio)
{
    for (int y = 10; y <= tela_cardapio->screen_size.y; y+=2) 
    {
        moveCursor(nv2(0, y));
        for (int x = 10; x < tela_cardapio->screen_size.x; x++) 
        {
            putchar(' ');
            tela_cardapio->buffer[y][x] = COR_NULA;
        }
    }
    moveCursor(VETOR_NULO);
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

void mostrar_controles (Screen* atual)
{
    moveCursor(nv2(0, atual->screen_size.y + 1));
    printf("Controles:\n");
    printf("WASD: Mover | ");
    printf("P: Pegar Ingrediente/Molho | ");
    printf("O: Largar | ");
    printf("C: Colocar no Prato | ");
    printf("M: Entregar | ");
    printf("X: Cardápio");
    moveCursor(VETOR_NULO);
}

/*
    Organiza toda a etapa de montagem, lendo e esvaziando a fila de pedidos a medida que o player vai cozinhando
    Retorna true se o jogo correu bem
    Retorna false caso GameOver (WIP) -> isso depende do estoque. GameOver = não há mais estoque
    Já atualiza a grana do jogador e (futuramente) atualiza seu gasto de ingredientes no estoque
*/
bool etapa_de_montagem (Fila_D* fila_de_pedidos, Cardapio* c, Jogador* jog)
{
    moveCursor(VETOR_NULO);
    system("cls");
    const Vector2 prato_pos = nv2(-1, 10);

    CenarioMontagem cenario = inicializa_cenario_montagem();

    Screen* atual = cenario.main_tela;
    bool no_cardapio = false;
    int pagina = 0;
    int pagina_antiga = -1;
    Obj t1 = NULL, t2 = NULL; // Inicializados para evitar lixo de memória na primeira checagem

    int hamb_id;
    front(fila_de_pedidos, &hamb_id);
    Hamburguer* pedido_atual = c->hamburgueres[hamb_id];
    tp_pilha montagem;
    inicializa_pilha(&montagem);
    push(&montagem, PAO_BAIXO);
    Obj hamburguer = criar_piskel_obj(ingredientes_data[1], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
    IngredienteObj ingrediente_atual = {NULL, -1};
    centralizar_objeto(hamburguer);
    alterar_pivot_obj(hamburguer, nv2(0, hamburguer->size.y/2));
    teleportar_objeto(cenario.main_tela, hamburguer, prato_pos);

    while (fila_vazia(fila_de_pedidos) == false)
    {
        render_com_texto(atual, no_cardapio);
        mostrar_controles(atual);

        char input = ler_teclado();
        switch (toupper(input))
        {
        case 'X': // abre/fecha o cardápio
            system("cls");
            moveCursor(VETOR_NULO);
            no_cardapio = (no_cardapio == false);
            atual = (no_cardapio == true) ? cenario.cd_tela : cenario.main_tela;
            limpar_buffer(atual);
            if (!no_cardapio) pagina_antiga = -1;
            continue;

        case 'C': // Coloca o ingrediente no burgão
            if (no_cardapio || esta_na_area_de_drop(prato_pos, cenario.cursor) == false) break;
            if (ingrediente_atual.o == NULL || ingrediente_atual.i == -1) break;
            if (altura_pilha(&montagem) > 16) break;

            hamburguer = monta_ingrediente(atual, hamburguer, &ingrediente_atual, c, &montagem, prato_pos);
            break;

        case 'M': // Envia o ingrediente ao cliente
            int alt = altura_pilha(&(pedido_atual->receita));
            float fator = (alt - compara_pilhas(montagem, pedido_atual->receita))/alt;
            float ganho = pedido_atual->valor * fator;
            jog->dinheiro += ganho;
            inicializa_pilha(&montagem);
            push(&montagem, PAO_BAIXO);

            esconder_objeto(cenario.main_tela, hamburguer);
            excluir_objeto(hamburguer);
            hamburguer = criar_piskel_obj(ingredientes_data[1], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
            centralizar_objeto(hamburguer);
            alterar_pivot_obj(hamburguer, nv2(0, hamburguer->size.y/2));
            teleportar_objeto(cenario.main_tela, hamburguer, prato_pos);

            moveCursor(nv2(0, 3+atual->screen_size.y));
            dequeue(fila_de_pedidos, &hamb_id);
            if (front(fila_de_pedidos, &hamb_id))
                pedido_atual = c->hamburgueres[hamb_id];
            
            if (pedido_atual != NULL) // Se ainda tem pedidos a fazer
            {
                /*
                    if (jog->estoque == NULL) return false; // GameOver!!! Sem ingredientes para fazer pedidos!
                    int qtd = buscar(jog->estoque, PAO_BAIXO);
                    if (qtd <= 0) return false; // GameOver!!! Sem mais bases de pão
                */
            }
            printf("Você enviou um hamburguer e ganhou %.2f PatoCoin$!\n", ganho);
            break;

        case 'P': // Pega o ingrediente
            if (no_cardapio) break;
            uint8_t m = cursor_no_molho(cenario.cursor);
            if (m != 0)
            {
                uint32_t corHex = (m == 1) ? 0xffa5ffdd : 0xff051a45;
                push(&montagem, c->ingredientes[m+6]);
                aplicar_molho_visual(atual, hamburguer, converter_ABGR_para_Color(corHex));
            }
            else
            {
                ingrediente_atual = pegar_ingrediente(cenario.cursor, ingrediente_atual);
                if (ingrediente_atual.o != NULL && ingrediente_atual.i != -1)
                    desenhar_objeto(atual, ingrediente_atual.o);
            }
            break;
        
        case 'O': // Larga o ingrediente
            if (no_cardapio || ingrediente_atual.o == NULL || ingrediente_atual.i == -1) break;
            esconder_objeto(atual, ingrediente_atual.o);
            excluir_objeto(ingrediente_atual.o);
            ingrediente_atual.o = NULL;
            ingrediente_atual.i = -1;
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
                limpar_area_receita(cenario.cd_tela);

                if (pagina_antiga != -1)
                {
                    esconder_objeto(cenario.cd_tela, t1);
                    esconder_objeto(cenario.cd_tela, t2);
                    excluir_objeto(t1); excluir_objeto(t2);
                }
                t1 = t2 = NULL;
                t1 = criar_objeto_de_texto(1, 1, c->hamburgueres[pagina]->nome);
                somar_cor_obj(t1, COLOR_VERMELHO);
                t2 = criar_objeto_de_texto(1, 1, c->hamburgueres[pagina+1]->nome);
                somar_cor_obj(t2, COLOR_VERMELHO);
                teleportar_objeto(cenario.cd_tela, t1, nv2(-55, -11));
                teleportar_objeto(cenario.cd_tela, t2, nv2(11, -11));

                render_com_texto(atual, no_cardapio);
                print_receita(c->hamburgueres[pagina]->receita, 11);
                print_receita(c->hamburgueres[pagina+1]->receita, 75);
            }
            pagina_antiga = pagina;
        }
        else
        {
            if (compare_vector(v, VETOR_NULO) == false)
            {
                if (compare_vector(v, VETOR_DIREITA) || compare_vector(v, VETOR_ESQUERDA))
                    mov_obj(atual, cenario.cursor, v_prod(v, 2));
                else
                    mov_obj(atual, cenario.cursor, v);
                if (ingrediente_atual.o != NULL)
                    teleportar_objeto(atual, ingrediente_atual.o, cenario.cursor->position);
            }
        }
    }

    if (t1 != NULL) {esconder_objeto(cenario.cd_tela, t1); excluir_objeto(t1);}
    if (t2 != NULL) {esconder_objeto(cenario.cd_tela, t2); excluir_objeto(t2);}

    printf("Parabéns! Você entregou todos os pedidos!\n");
    printf("Pressione qualquer coisa para continuar...\n");
    getchar();
    
    if (hamburguer->ref_node != NULL)
    {
        remover_da_lista(cenario.main_tela->obj_list, hamburguer->ref_node);
        hamburguer->ref_node = NULL;
    }
    excluir_objeto(hamburguer);
    limpa_cenario_montagem(cenario);

    return fila_vazia(fila_de_pedidos);
}
