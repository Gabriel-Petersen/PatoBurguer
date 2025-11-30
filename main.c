#define USE_SHORTCUTS
#define INDEVMODE
//#define SEM_MONTAGEM
//#define SEM_ARQUIVO_EXTERNO
#define _VSCODE

#include "montagem.h"
#include "assets/logo.h"
#include "assets/game_over.h"
#include "loja.h"
#include "jogador.h"
#include "mergeSort.h"
#include <time.h>
#include <windows.h>
#include <locale.h>

// Sorteia qtd pedidos do cardápio e, após todos os sorteios, mostra na tela os pedidos para memorização do jogador
int qtd_clientes (int qtd_dias)
{
    
    #ifdef INDEVMODE
        return qtd_dias;
    #else
        return qtd_dias + 2;
    #endif
}

// Gera a fila de pedidos e chama a etapa de montagem
int inicio_de_dia (Jogador* jog, Cardapio* c)
{
    system("cls");
    Fila_D* q = inicializa_fila();
    int qtd = qtd_clientes(jog->dia_atual);

    printf("Os clientes estão chegando e fazendo seus pedidos...\n");
    print_rgb_txt(NULL, COLOR_VERMELHO, VETOR_BAIXO, "Aguarde os clientes decidirem e a chapa aquecer\n");
    Sleep(1500);
    system("cls");
    print_rgb_txt(NULL, COLOR_VERDE, VETOR_NULO, "Os pedidos já foram escolhidos!\nPrepare sua memória, pois só serão mostrados uma vez!\n");
    ler_teclado();
    print_rgb_txt(NULL, COLOR_AMARELO, nv2(-1, -1), "Digite qualquer coisa quando estiver pronto!\n");
    getchar();
    system("cls");
    printf("\n");
    srand(time(NULL));

    while (!fila_vazia(q)) {
        dequeue(q, NULL);
    }
    for (int i=0;i<qtd;i++) {
        int b = rand() % 10;
        enqueue(q, b);
        printf("%d - %s\n", i+1, c->hamburgueres[b]->nome);
    }

    float tempo = 2*qtd;
    while (tempo > 0)
    {
        print_rgb_txt(NULL, COLOR_VERMELHO, VETOR_NULO, "Você tem %.1f segundos!", tempo);
        Sleep(100);
        tempo -= 0.1;
    }
    system("cls");

    int qtd_hamb;
    bool res = etapa_de_montagem(q, c, jog, &qtd_hamb);
    audio_play("ding", false);
    destruir_fila(q);
    q = NULL;
    if (res)
        return qtd_hamb;
    else
        return -1;
}

// cria a tela inicial de logo do jogo
void tela_inicial ()
{
    printf("Recomendamos colocar o terminal em tela cheia\n");
    print_rgb_txt(NULL, COLOR_AMARELO, nv2(-1, -1), "Pressione ENTER para abrir o jogo!...\n");
    getchar();
    Screen* tela_inicio = criar_tela(nv2(120, 30), COLOR_CIANO, 10);
    Obj fundo = criar_piskel_obj(logo_ini_data[0], LOGO_INI_FRAME_WIDTH, LOGO_INI_FRAME_HEIGHT);
    centralizar_objeto(fundo);
    desenhar_objeto(tela_inicio, fundo);
    troca_tema_com_fade("tema2");
    render(tela_inicio, true);
    
    print_rgb_txt(NULL, COLOR_AMARELO, nv2(-1, -1), "Pressione ENTER para começar a jogar!...\n");
    getchar();

    esconder_objeto(tela_inicio, fundo);
    excluir_objeto(fundo);
    fundo = NULL;
    excluir_tela(tela_inicio);
    tela_inicio = NULL;
}

// Organiza o fim do jogo e mostra as estatísticas finais
void fim_de_jogo(Cardapio* cardapio, Jogador* jog, Screen* tela_go, Obj objetos[], int qtd_objetos)
{
    audio_stop("end");
    audio_play("ding", false);
    mergeSort(jog->itens_vendidos,0,9);
    printf("Estatísticas:\n");
    for(int i=0;i<10;i++){
        printf(
               "Hamburguer: %s\n  -> Quantidade vendida: %d\n\n", 
                cardapio->hamburgueres[jog->itens_vendidos[i].id]->nome, 
                jog->itens_vendidos[i].quantidade
        );
    }

    printf("Obrigado por jogar!\n");
    print_rgb_txt(NULL, COLOR_VERMELHO, nv2(-1, -1), "Pressione ENTER para encerrar...\n");
    getchar();
    if (qtd_objetos > 0)
    {
        for (int i = 0; i < qtd_objetos; i++)
        {
            esconder_objeto(tela_go, objetos[i]);
            excluir_objeto(objetos[i]);
        }
        excluir_tela(tela_go);
    }
    
    jog = destruir_jogador(jog);
    audio_shutdown();
}

// Mostra a tela de GameOver
void tela_gameOver (Cardapio* cd, Jogador* jog)
{
    audio_stop("tema1");
    audio_stop("tema2");
    audio_play("tururu", false);
    audio_play("end", false);
    Screen* s = criar_tela(nv2(131, 30), COLOR_PRETO, 0);
    Obj fundo = criar_piskel_obj(game_over_data[0], GAME_OVER_FRAME_WIDTH, GAME_OVER_FRAME_HEIGHT);
    centralizar_objeto(fundo);
    desenhar_objeto(s, fundo);

    Obj game = criar_objeto_de_texto(1, 3, "GAME");
    somar_cor_obj(game, COLOR_VERMELHO);
    Obj over = criar_objeto_de_texto(1, 3, "OVER!");
    somar_cor_obj(over, COLOR_VERMELHO);

    teleportar_objeto(s, game, nv2(-30, -7));
    teleportar_objeto(s, over, nv2(-18, 4));

    render(s, true);
    print_rgb_txt(NULL, COLOR_AMARELO, nv2(-1, -1), "Pressione ENTER para ver as estatísticas finais.\n");
    getchar();
    Obj objetos[] = {fundo, game, over};
    fim_de_jogo(cd, jog, s, objetos, 3);
}

// Mostra as atualizações diárias
void print_atualizacoes (Jogador* jog, int qtdh)
{
    Vector2 v = nv2(-1, -1);
    printf("Dia: ");
    print_rgb_txt(NULL, COLOR_CIANO, v, "%d\n", jog->dia_atual);

    printf("Dinheiro atual: ");
    print_rgb_txt(NULL, COLOR_VERDE, v, "%.2f PatoCoin$\n", jog->dinheiro);

    printf("Hambúrgueres feitos: ");
    print_rgb_txt(NULL, COLOR_ROXO, v, "%d\n\n", qtdh);
}

// Carrega os áudios
bool load_audio ()
{
    audio_init();

    #ifdef _VSCODE
        if (audio_load("woosh", "../assets/soundtrack/woosh.mp3") == 0) return false;
        if (audio_load("tema1", "../assets/soundtrack/tema1.wav") == 0) return false;
        if (audio_load("tema2", "../assets/soundtrack/tema2.wav") == 0) return false;
        if (audio_load("cash", "../assets/soundtrack/register.mp3") == 0) return false;
        if (audio_load("punch", "../assets/soundtrack/soco.wav") == 0) return false;
        if (audio_load("tururu", "../assets/soundtrack/tururu.wav") == 0) return false;
        if (audio_load("end", "../assets/soundtrack/end.mp3") == 0) return false;
        if (audio_load("ding", "../assets/soundtrack/dingDong.wav") == 0) return false;
    #else
        if (audio_load("woosh", "assets/soundtrack/woosh.mp3") == 0) return false;
        if (audio_load("tema1", "assets/soundtrack/tema1.wav") == 0) return false;
        if (audio_load("tema2", "assets/soundtrack/tema2.wav") == 0) return false;
        if (audio_load("cash", "assets/soundtrack/register.mp3") == 0) return false;
        if (audio_load("punch", "assets/soundtrack/soco.wav") == 0) return false;
        if (audio_load("tururu", "assets/soundtrack/tururu.wav") == 0) return false;
        if (audio_load("end", "assets/soundtrack/end.mp3") == 0) return false;
        if (audio_load("ding", "assets/soundtrack/dingDong.wav") == 0) return false;
    #endif

    audio_set_volume("tema1", 20.0f);
    audio_set_volume("tema2", 20.0f);
    audio_set_volume("punch", 80.0f);
    audio_set_volume("tururu", 80.0f);
    return true;
}

int main ()
{
    setlocale(LC_CTYPE, "pt_BR.UTF-8");
    if (load_audio() == false) return 0;
    tela_inicial();

    Cardapio cardapio;
    int din_inicial;
    ler_teclado();
    printf("Sanval está organizando a verba para o investimento nessa hamburgueria. Escolha a dificuldade:\n");
    printf("\tDigite 'F' para o modo fácil: Sanval liberou a carteira!\n");
    printf("\tDigite 'M' para o modo médio: Dá pra começar, mas o aumento de Soussa reduziu o investimento.\n");
    printf("\tDigite 'D' para o modo difícil: É a crise do pato! Desviaram o orçamento para gastar em hambúrguer de pato. Que horror!\n");
    
    while (true)
    {
        ler_teclado();
        printf("Digite a letra para a dificuldade desejada: ");
        char l;
        scanf("%c", &l);
        l = toupper(l);
        if (l == 'F')
        {
            din_inicial = 60;
            break;
        }
        else if (l == 'M')
        {
            din_inicial = 30;
            break;
        }
        else if (l == 'D')
        {
            din_inicial = 10;
            break;
        }
        else
            printf("Caractere %c não mapeado. Tente novamente...\n", l);
    }

    system("cls");
    Jogador* jog = inicializa_jogador(din_inicial);
    inicializa_cardapio(&cardapio);

    #ifndef SEM_ARQUIVO_EXTERNO
        if(inicializa_player_save(din_inicial)==0) return 0;
    #else
        printf("Build sem arquivo externo. Verifique se sua maquina aceita e, se sim, execute a versão alternativa.\n");
    #endif

    while (true)
    {
        int qtd_hamburgueres;

        #ifdef SEM_MONTAGEM
            qtd_hamburgueres=0;
        #else
            qtd_hamburgueres = inicio_de_dia(jog, &cardapio);
        #endif

        #ifndef SEM_ARQUIVO_EXTERNO
            atualiza_player_save(jog, qtd_hamburgueres);
        #endif

        if (qtd_hamburgueres == -1)
        {
            tela_gameOver(&cardapio, jog);
            break;
        }

        print_atualizacoes(jog, qtd_hamburgueres);
        jog->dia_atual++;

        iniciar_loja(jog);

        printf("Insira X para sair do jogo ou qualquer outra coisa para seguir para o próximo dia: ");
        char c;
        scanf(" %c", &c);
        if (c == 'X' || c == 'x')
        {
            fim_de_jogo(&cardapio, jog, NULL, NULL, 0);
            return 0;
        }
    }
}