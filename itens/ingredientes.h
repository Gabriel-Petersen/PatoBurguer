#ifndef INGREDIENTES_H
#define INGREDIENTES_H

#define QTD_INGREDIENTES 13

typedef struct{
    char nome[20];
    int id;
    float valor;
} Ingrediente;

#define PAO_CIMA (Ingrediente) {"pao_cima", 0, 2}
#define PAO_BAIXO (Ingrediente) {"pao_baixo", 1, 2}
#define CARNE (Ingrediente) {"carne", 2, 10}
#define BACON (Ingrediente) {"bacon", 3, 8.4}
#define QUEIJO (Ingrediente) {"queijo", 4, 5.5}
#define ALFACE (Ingrediente) {"alface", 5, 3.2}
#define TOMATE (Ingrediente) {"tomate", 6, 2.8}
#define MAIONESE_TEMPERADA (Ingrediente) {"maionese_temperada", 7, 4}
#define BARBERCUE (Ingrediente) {"barbercue", 8, 4}
#define CEBOLA_CARAMELIZADA (Ingrediente) {"cebola_caramelizada", 9, 5.2}
#define PICLES (Ingrediente) {"picles",10, 1.9}
#define OVO_FRITO (Ingrediente) {"ovo_frito", 11, 3}
#define ONION_RINGS (Ingrediente) {"onion_rings", 12, 4.1}

#endif