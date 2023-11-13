#include "../Engine/game.c"

typedef struct{
    image CardImage;
} data;

void Init(data* Data, float MouseX, float MouseY){
    Data->CardImage = LoadImage("Card.png");
}

void Update(data* Data, float MouseX, float MouseY){
    DrawImage(200, 200, Data->CardImage);
}

/*
LoadImage(NazwaObrazka)
Obrazek musi znajdować się w folderze Resources!!!
Zwraca typ image*

DrawImage(PozycjaX, PozycjaY, Obrazek)

typedef struct{
            ...  <-- to wstawiasz zmienne które mają przetrwać dłużej niż jedną klatke
} data;
*/