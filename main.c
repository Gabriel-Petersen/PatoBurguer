#define USE_SHORTCUTS
#include "montagem.h"
#include "assets/logo.h"
#include "jogador.h"
#include <time.h>
#include <windows.h>
#include <locale.h>

// Sorteia qtd pedidos do cardápio e, após todos os sorteios, mostra na tela os pedidos para memorização do jogador

int qtd_clientes (int qtd_dias)
{
    // sla fórmula aleatória
    return (int)log2(qtd_dias)*(qtd_dias+2);
}

void inicio_de_dia (Jogador* jog, Cardapio* c)
{
    Fila_E q;
    inicializa_fila(&q);
    int qtd = qtd_clientes(jog->dia_atual);

    printf("Os clientes estão chegando e fazendo seus pedidos...\n");
    print_rgb_txt(COLOR_VERMELHO, VETOR_BAIXO, "Aguarde os clientes decidirem e a chapa aquecer\n");
    Sleep(5000);
    system("cls");
    print_rgb_txt(COLOR_VERDE, VETOR_NULO, "Os pedidos já foram escolhidos!\nPrepare sua memória, pois só serão mostrados uma vez!\n");
    printf("Digite qualquer coisa quando estiver pronto!");
    getchar();
    system("cls");
    printf("\n");
    srand(time(NULL));

    while (!fila_vazia(&q)) {
        dequeue(&q, NULL);
    }
    for (int i=0;i<qtd;i++) {
        enqueue(&q, rand() % 10);
    }
    int cont = 1;
    while (!fila_vazia(&q)){
        int b;
        dequeue(&q, &b);
        printf("%d - %s\n", cont++, c->hamburgueres[b]->nome);
    }

    float tempo = 1.5*qtd;
    while (tempo > 0)
    {
        print_rgb_txt(COLOR_VERMELHO, VETOR_NULO, "Você tem %.1f segundos!", tempo);
        Sleep(100);
        tempo -= 0.1;
    }
    system("cls");

    etapa_de_montagem(&q, c);
}

void tela_inicial ()
{
    setlocale(LC_CTYPE, "pt_BR.UTF-8");
    Screen* tela_inicio = criar_tela(nv2(120, 30), COLOR_CIANO, 10);
    Obj fundo = criar_piskel_obj(logo_ini_data[0], LOGO_INI_FRAME_WIDTH, LOGO_INI_FRAME_HEIGHT);
    centralizar_objeto(fundo);
    desenhar_objeto(tela_inicio, fundo);
    render(tela_inicio, true);
    
    printf("Pressione qualquer coisa para começar a jogar!...");
    getchar();

    esconder_objeto(tela_inicio, fundo);
    excluir_objeto(fundo);
    fundo = NULL;
    excluir_tela(tela_inicio);
    tela_inicio = NULL;
}

int main ()
{
    Cardapio cardapio;
    // INICIALIZANDO SEM VERIFICAR ARQUIVO
    Jogador* jog = inicializa_jogador(NULL);
    inicializa_cardapio(&cardapio);
    inicio_de_dia(5, &cardapio);

    jog = destruir_jogador(jog);
    return 0;
}