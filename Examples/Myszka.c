#include "Engine.h"

typedef struct{
} data;

void Init(data* Data, float MouseX, float MouseY){
}

void Update(data* Data, float MouseX, float MouseY){
    DrawRect(MouseX, MouseY, 100, 100, White);
}

/*
Pozycje Myszki są dane w argumentach MouseX i MouseY
*/