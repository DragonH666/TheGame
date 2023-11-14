#include "Engine.h"

typedef struct{
} data;

void Init(data* Data, float MouseX, float MouseY){
    
}

void Update(data* Data, float MouseX, float MouseY){
    DrawRect(0, 0, 100, 100, White);
    DrawRect(200, 250, 300, 200, Red);
    DrawRect(50, 20, 40, 200, Violet);
}

/*
DrawRect(PozycjaX, PozycjaY, Szerokość, Wysokość, Kolor)
Dostępne kolory:
- White
 - Black
- Red
 - Green
- Blue
 - Yellow
 - Violet
- Cyan

void Init(data* Data, float MouseX, float MouseY){
    ... <-tu kod będzie wykonany tylko na początku programu
}

void Update(data* Data, float MouseX, float MouseY){
    ... <-tu kod będzie wykonywany każdą klatke
}
*/