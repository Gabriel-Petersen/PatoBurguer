#define USE_SHORTCUTS

#include "include/graphycs_all.h"
#include "audio.h"
#include "itens/cardapio.h"
#include "estruturas_de_dados/fila_dinamica.h"
#include "estruturas_de_dados/pilha.h"
#include "estruturas_de_dados/grid_encadeado.h"
#include "assets/piskel_ingredientes.h"
#include "assets/mcBackground.h"
#include "assets/piskel_molhos.h"
#include "jogador.h"

void troca_tema_com_fade(char *novo_tema) {
    char *tema_antigo = NULL;

    if (strcmp(novo_tema, "tema1") == 0)
        tema_antigo = "tema2";
    else if (strcmp(novo_tema, "tema2") == 0) 
        tema_antigo = "tema1";
    
    if (tema_antigo != NULL && audio_is_playing(tema_antigo)) 
        audio_stop(tema_antigo);

    audio_play(novo_tema, true); 
    audio_set_volume(novo_tema, 0.0f); 

    audio_fade_in(novo_tema, 1600, 0.0f, 20.0f);
}

Obj criar_frame_retangular (Vector2 size, Color cor)
{
    int qtd_pixel = 2 * (size.x + size.y - 2); 

    Pixel* info = (Pixel*)malloc(qtd_pixel * sizeof(Pixel)); 
    if (info == NULL) return NULL;

    int idx = 0;

    for (int x = 0; x < size.x; x++) 
        info[idx++] = criar_pixel(cor, nv2(x, 0));
    
    for (int x = 0; x < size.x; x++) 
        info[idx++] = criar_pixel(cor, nv2(x, size.y - 1));
    
    for (int y = 1; y < size.y - 1; y++) 
    {
        info[idx++] = criar_pixel(cor, nv2(0, y));
        info[idx++] = criar_pixel(cor, nv2(size.x - 1, y));
    }

    return criar_objeto_custom(info, qtd_pixel, true);
}

typedef struct {
    Screen *main_tela;
    Screen *cd_tela;
    Obj fundo_cardapio;
    Obj fundo_montagem;
    Obj molho_visual[2];
    Obj gaveta_extra;

    Obj highlight;
    GridEncadeado* grid;
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

Vector2 converter_para_cartesiano (Screen* tela, Vector2 pos_matriz)
{
    Vector2 centro = centro_da_tela(tela);
    return vector_subtr(pos_matriz, centro);
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

bool navega_grid (Screen* atual, GridEncadeado* g, Obj* highlight, Vector2 direcao)
{
    NodoGrid* proximo = NULL;
    
    if (compare_vector(direcao, VETOR_CIMA))
        proximo = g->atu->cima;
    else if (compare_vector(direcao, VETOR_BAIXO))
        proximo = g->atu->baixo;
    else if (compare_vector(direcao, VETOR_ESQUERDA))
        proximo = g->atu->esq;
    else if (compare_vector(direcao, VETOR_DIREITA))
        proximo = g->atu->dir;
    
    if (proximo == NULL) return false;

    esconder_objeto(atual, *highlight);
    excluir_objeto(*highlight);

    g->atu = proximo;

    *highlight = criar_frame_retangular(g->atu->tam, COLOR_AMARELO);
    //centralizar_objeto(*highlight);
    teleportar_objeto(atual, *highlight, converter_para_cartesiano(atual, g->atu->pos_ini));

    return true;
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

    cenario.grid = criar_grid_encadeado();

    cenario.highlight = criar_frame_retangular(cenario.grid->ini->tam, COLOR_AMARELO);
    //centralizar_objeto(cenario.highlight);
    
    teleportar_objeto(cenario.main_tela,
                cenario.highlight,
                converter_para_cartesiano(cenario.main_tela, cenario.grid->ini->pos_ini)
            );
    desenhar_objeto(cenario.main_tela, cenario.highlight);

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
    esconder_objeto(cenario.main_tela, cenario.highlight);
    esconder_objeto(cenario.main_tela, cenario.gaveta_extra);
    esconder_objeto(cenario.cd_tela, cenario.fundo_cardapio);

    excluir_objeto(cenario.fundo_montagem);
    excluir_objeto(cenario.fundo_cardapio);
    excluir_objeto(cenario.highlight);
    excluir_objeto(cenario.gaveta_extra);

    destruir_grid_encadeado(cenario.grid);

    excluir_tela(cenario.main_tela);
    excluir_tela(cenario.cd_tela);
}

Obj monta_ingrediente (Screen* atual, Obj hamburguer, Ingrediente novo_ing_data, tp_pilha* montagem, Vector2 prato_pos)
{
    int piskel_data_id = novo_ing_data.id;
    if (piskel_data_id > 8) piskel_data_id -= 2;
    else if (piskel_data_id > 6) piskel_data_id--;
    
    Obj ingrediente = criar_piskel_obj(ingredientes_data[piskel_data_id], INGREDIENTES_FRAME_WIDTH, INGREDIENTES_FRAME_HEIGHT);
    centralizar_objeto(ingrediente);

    esconder_objeto(atual, hamburguer);
    esconder_objeto(atual, ingrediente);
    
    ingrediente->position.x = hamburguer->position.x;
    int bonus = 2*(novo_ing_data.id == PAO_CIMA.id);
    ingrediente->position.y = hamburguer->position.y - hamburguer->size.y + 1 - bonus;
    Vector2 offset = vector_subtr(hamburguer->position, ingrediente->position);

    Obj novo_hamburguer = merge_objeto(ingrediente, hamburguer, offset);
    centralizar_objeto(novo_hamburguer);
    alterar_pivot_obj(novo_hamburguer, nv2(0, novo_hamburguer->size.y/2));
    teleportar_objeto(atual, novo_hamburguer, prato_pos);
    
    push(montagem, novo_ing_data);
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
    printf("WASD: Navegar/Mover | ");
    printf("P: Colocar Ingrediente/Molho | ");
    printf("M: Entregar Pedido | ");
    printf("X: Abrir/Fechar Cardápio");
    moveCursor(VETOR_NULO);
}

/*
    Organiza toda a etapa de montagem, lendo e esvaziando a fila de pedidos a medida que o player vai cozinhando
    Retorna true se o jogo correu bem
    Retorna false caso GameOver (WIP) -> isso depende do estoque. GameOver == não há mais estoque
    Já atualiza a grana do jogador e (futuramente) atualiza seu gasto de ingredientes no estoque
*/
bool etapa_de_montagem (Fila_D* fila_de_pedidos, Cardapio* c, Jogador* jog, int* qtd_hamburgueres)
{
    troca_tema_com_fade("tema1");
    *qtd_hamburgueres = 0;
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
    centralizar_objeto(hamburguer);
    alterar_pivot_obj(hamburguer, nv2(0, hamburguer->size.y/2));
    teleportar_objeto(cenario.main_tela, hamburguer, prato_pos);

    while (fila_vazia(fila_de_pedidos) == false)
    {
        render_com_texto(atual, no_cardapio);
        mostrar_controles(atual);

        char input = ler_teclado();
        #ifdef INDEVMODE
            if (input == 'T') break;
        #endif

        switch (toupper(input))
        {
        case 'H':
            printf("Dinheiro atual: %lf\n", jog->dinheiro);
            break;
        case 'X': // abre/fecha o cardápio
            system("cls");
            moveCursor(VETOR_NULO);
            no_cardapio = (no_cardapio == false);
            atual = (no_cardapio == true) ? cenario.cd_tela : cenario.main_tela;
            limpar_buffer(atual);
            if (!no_cardapio) pagina_antiga = -1;
            continue;

        case 'M': // Envia o ingrediente ao cliente
            *qtd_hamburgueres += 1;
            int alt = altura_pilha(&(pedido_atual->receita));
            float fator = (float)((float)alt - compara_pilhas(montagem, pedido_atual->receita))/alt;
            float ganho = pedido_atual->valor * fator;
            jog->dinheiro += max(0, ganho);
            moveCursor(nv2(0, 34));
            audio_play("cash", false);
            printf("Você enviou um hamburguer e ganhou %.2f/%.2f PatoCoin$!\n", ganho, pedido_atual->valor);

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
            break;

        case 'P':
            if (no_cardapio) break;
            
            int id_ingrediente = cenario.grid->atu->ing_id;

            // de toda forma, id_ingrediente já é definido aqui. Pode fazer um if na quantidade e, se for false, chama "break"
            // se for true, pode debitar do estoque aqui mesmo

            if (id_ingrediente == 7 || id_ingrediente == 8) // se é um molho
            {
                uint32_t corHex = (id_ingrediente == 7) ? 0xffa5ffdd : 0xff051a45;
                push(&montagem, c->ingredientes[id_ingrediente]);
                aplicar_molho_visual(atual, hamburguer, converter_ABGR_para_Color(corHex));
                audio_play("punch", false);
            }
            else // se é ingrediente normal
            {
                if (altura_pilha(&montagem) > 16) break;
                audio_play("woosh", false);
                Ingrediente novo_ing_data = c->ingredientes[id_ingrediente];
                hamburguer = monta_ingrediente(atual, hamburguer, novo_ing_data, &montagem, prato_pos);
            }
            break;
        }

        Vector2 v = get_direcao(input);
        if (!no_cardapio)
        {
            navega_grid(cenario.main_tela, cenario.grid, &(cenario.highlight), v);
        }
        else
        {
            if (compare_vector(v, VETOR_DIREITA) && pagina < 8)
            {
                audio_play("woosh", false);
                pagina += 2;
            }
            else if (compare_vector(v, VETOR_ESQUERDA) && pagina > 0)
            {
                audio_play("woosh", false);
                pagina -= 2;
            }

            if (pagina_antiga != pagina)
            {
                limpar_area_receita(cenario.cd_tela);

                if (t1 != NULL)
                {
                    esconder_objeto(cenario.cd_tela, t1);
                    excluir_objeto(t1);
                }
                if (t2 != NULL)
                {
                    esconder_objeto(cenario.cd_tela, t2);
                    excluir_objeto(t2);
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
    }

    if (t1 != NULL) {esconder_objeto(cenario.cd_tela, t1); excluir_objeto(t1);}
    if (t2 != NULL) {esconder_objeto(cenario.cd_tela, t2); excluir_objeto(t2);}

    printf("\nParabéns! Você entregou todos os pedidos!\n");
    print_rgb_txt(COLOR_AMARELO, nv2(-1, -1), "Pressione ENTER para continuar!\n");
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