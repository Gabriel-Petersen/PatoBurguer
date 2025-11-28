#define USE_SHORTCUTS
#define INDEVMODE
//#define SEM_MONTAGEM
//#define SEM_ARQUIVO_EXTERNO
#define _VSCODE

#include "montagem.h"
#include "assets/logo.h"
#include "loja.h"
#include "jogador.h"
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
    print_rgb_txt(COLOR_VERMELHO, VETOR_BAIXO, "Aguarde os clientes decidirem e a chapa aquecer\n");
    Sleep(1500);
    system("cls");
    print_rgb_txt(COLOR_VERDE, VETOR_NULO, "Os pedidos já foram escolhidos!\nPrepare sua memória, pois só serão mostrados uma vez!\n");
    print_rgb_txt(COLOR_AMARELO, nv2(-1, -1), "Digite qualquer coisa quando estiver pronto!\n");
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
        print_rgb_txt(COLOR_VERMELHO, VETOR_NULO, "Você tem %.1f segundos!", tempo);
        Sleep(100);
        tempo -= 0.1;
    }
    system("cls");

    int qtd_hamb;
    etapa_de_montagem(q, c, jog, &qtd_hamb);
    destruir_fila(q);
    q = NULL;
    return qtd_hamb;
}

void tela_inicial ()
{
    printf("Recomendamos colocar o terminal em tela cheia\n");
    print_rgb_txt(COLOR_AMARELO, nv2(-1, -1), "Pressione qualquer coisa para começar a jogar!...\n");
    getchar();
    Screen* tela_inicio = criar_tela(nv2(120, 30), COLOR_CIANO, 10);
    Obj fundo = criar_piskel_obj(logo_ini_data[0], LOGO_INI_FRAME_WIDTH, LOGO_INI_FRAME_HEIGHT);
    centralizar_objeto(fundo);
    desenhar_objeto(tela_inicio, fundo);
    troca_tema_com_fade("tema1");
    render(tela_inicio, true);
    
    print_rgb_txt(COLOR_AMARELO, nv2(-1, -1), "Pressione ENTER para começar a jogar!...\n");
    getchar();

    esconder_objeto(tela_inicio, fundo);
    excluir_objeto(fundo);
    fundo = NULL;
    excluir_tela(tela_inicio);
    tela_inicio = NULL;
}

void print_atualizacoes (Jogador* jog, int qtdh)
{
    Vector2 v = nv2(-1, -1);
    printf("Dia: ");
    print_rgb_txt(COLOR_CIANO, v, "%d\n", jog->dia_atual);

    printf("Dinheiro atual: ");
    print_rgb_txt(COLOR_VERDE, v, "%.2f PatoCoin$\n", jog->dinheiro);

    printf("Hambúrgueres feitos: ");
    print_rgb_txt(COLOR_ROXO, v, "%d\n\n", qtdh);
}

void load_audio ()
{
    audio_init();

    #ifdef _VSCODE
        audio_load("woosh", "../assets/soundtrack/woosh.mp3");
        audio_load("tema1", "../assets/soundtrack/tema1.wav");
        audio_load("tema2", "../assets/soundtrack/tema2.wav");
        audio_load("cash", "../assets/soundtrack/register.mp3");
        audio_load("punch", "../assets/soundtrack/soco.wav");
    #else
        audio_load("woosh", "assets/soundtrack/woosh.mp3");
        audio_load("tema1", "assets/soundtrack/tema1.wav");
        audio_load("tema2", "assets/soundtrack/tema2.wav");
        audio_load("cash", "assets/soundtrack/register.mp3");
        audio_load("punch", "assets/soundtrack/soco.wav");
    #endif

    audio_set_volume("tema1", 20.0f);
    audio_set_volume("tema2", 20.0f);
    audio_set_volume("punch", 80.0f);
}

int main ()
{
    setlocale(LC_CTYPE, "pt_BR.UTF-8");
    load_audio();
    tela_inicial();

    Cardapio cardapio;
    // INICIALIZANDO SEM VERIFICAR ARQUIVO DE SAVE-GAME (Futuro)
    Jogador* jog = inicializa_jogador(NULL);
    inicializa_cardapio(&cardapio);

    #ifndef SEM_ARQUIVO_EXTERNO
        if(inicializa_player_save()==0) return 0;
    #else
        printf("Build sem arquivo externo. Verifique se sua maquina aceita e, se sim, recompile.\n");
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

        print_atualizacoes(jog, qtd_hamburgueres);
        jog->dia_atual++;

        iniciar_loja(jog);

        printf("Insira X para sair do jogo ou qualquer outra coisa para seguir para o próximo dia: ");
        char c;
        scanf(" %c", &c);
        if (c == 'X' || c == 'x')
        {
            jog = destruir_jogador(jog);
            audio_shutdown();
            return 0;
        }
    }
}