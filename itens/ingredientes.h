#ifndef INGREDIENTES_H
#define INGREDIENTES_H

typedef struct{
    char nome[20];
    int id;
    float valor;
} Ingrediente;

#define PAO_CIMA (Ingrediente) {"pao_cima", 0, 0}
#define PAO_BAIXO (Ingrediente) {"pao_baixo", 1, 0}
#define CARNE (Ingrediente) {"carne", 2, 0}
#define BACON (Ingrediente) {"bacon", 3, 0}
#define QUEIJO (Ingrediente) {"queijo", 4, 0}
#define ALFACE (Ingrediente) {"alface", 5, 0}
#define TOMATE (Ingrediente) {"tomate", 6, 0}
#define MAIONESE_TEMPERADA (Ingrediente) {"maionese_temperada", 7, 0}
#define BARBERCUE (Ingrediente) {"barbercue", 8, 0}
#define CEBOLA_CARAMELIZADA (Ingrediente) {"cebola_caramelizada", 9, 0}
#define PICLES (Ingrediente) {"picles",10, 0}
#define OVO_FRITO (Ingrediente) {"ovo_frito", 11, 0}
#define ONION_RINGS (Ingrediente) {"onion_rings", 12, 0}

#endif