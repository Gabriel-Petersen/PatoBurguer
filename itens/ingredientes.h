#ifndef INGREDIENTES_H
#define INGREDIENTES_H

#define QTD_INGREDIENTES 13

typedef struct{
    char nome[20];
    int id;
    float valor;
} Ingrediente;

#define PAO_CIMA (Ingrediente) {"Topo de Pão", 0, 2}
#define PAO_BAIXO (Ingrediente) {"Base de Pão", 1, 2}
#define CARNE (Ingrediente) {"Carne", 2, 10}
#define BACON (Ingrediente) {"Bacon", 3, 8.4}
#define QUEIJO (Ingrediente) {"Queijo", 4, 5.5}
#define ALFACE (Ingrediente) {"Alface", 5, 3.2}
#define TOMATE (Ingrediente) {"Tomate", 6, 2.8}
#define MAIONESE_TEMPERADA (Ingrediente) {"Maionese Temperada", 7, 4}
#define BARBERCUE (Ingrediente) {"Barbercue", 8, 4}
#define CEBOLA_CARAMELIZADA (Ingrediente) {"Cebola Caramelizada", 9, 5.2}
#define PICLES (Ingrediente) {"Picles",10, 1.9}
#define OVO_FRITO (Ingrediente) {"Ovo Frito", 11, 3}
#define ONION_RINGS (Ingrediente) {"Onion Rings", 12, 4.1}

#endif