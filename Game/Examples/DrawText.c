#include "../Engine/game.c"

typedef struct{
    font* Font;
} data;

void Init(data* Data, float MouseX, float MouseY){
    Data->Font = LoadFont("Roboto-Regular.ttf", 90);
}

void Update(data* Data, float MouseX, float MouseY){
    DrawText(0, 90, "Text1\nText 2", White, Data->Font);
}

/*
LoadFont(NazwaCzcionki, RozmiarCzcionki)
 Czcionka musi znajdować się w folderze Resources!!!
Zwaraca typ font*

DrawText(PozycjaX, PozycjaY, Text, Color, Czcionka)
*/