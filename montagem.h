#define USE_SHORTCUTS
#include "include/graphycs_all.h"
#include "itens/cardapio.h"
#include "estruturas_de_dados/fila.h"
#include "assets/piskel_ingredientes.h"

// SOMENTE VISUAL, NÃO HÁ IMPLEMENTAÇÃO LÓGICA DE MONTAGEM, APENAS ESTÉTICA (ainda)
// O visual do sprite de cada ingrediente é meramente mergido ao pressioar M. Porém, o estado lógico da montagem não é salvo na memória
// Não há como escolher ingredientes. Eles estão numa ordem e, ao finalizar, o programa entra em loop eterno até pressionar P

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

void etapa_de_montagem (Fila_E* fila_de_pedidos, Cardapio* c)
{
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
}