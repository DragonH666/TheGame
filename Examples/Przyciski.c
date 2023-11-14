#include "Engine.h"

typedef struct{
    float x;
} data;

void Init(data* Data, float MouseX, float MouseY){
    Data->x = 0;
}

void Update(data* Data, float MouseX, float MouseY){
    float Size;
    if(IsHold(KEY_A)){
        Size = 100;
    }
    else{
        Size = 400;
    }
    DrawRect(Data->x, 0, Size, Size, White);
    
    if(IsPressed(MOUSE_BUTTON_LEFT)){
        Data->x += 10;
    }
    if(IsReleased(MOUSE_BUTTON_LEFT)){
        Data->x += 20;
    }
}

/*
IsHold(przycisk)
zwraca prawde gdy przycisk jest wciśniety, a fałsz gdy nie jest

IsPressed(przycisk)
zwraca prawde gdy przycisk został właśnie wciśniety, a fałsz gdy nie został

IsReleased(przycisk)
zwraca prawde gdy przycisk został właśnie puszczony, a fałsz gdy nie został

przyciski:
KEY_A, KEY_B, KEY_C, ... 
KEY_0, KEY_1, KEY_2, ...
KEY_F1, KEY_F2, KEY_F3, ...

MOUSE_BUTTON_LEFT, MOUSE_BUTTON_LEFT, MOUSE_BUTTON_MIDDLE

KEY_SPACE              
KEY_APOSTROPHE         ' 
KEY_COMMA              ,
KEY_MINUS              -
KEY_PERIOD             .
KEY_SLASH              /
 KEY_SEMICOLON          ;
 KEY_EQUAL              =
 KEY_LEFT_BRACKET       [
 KEY_BACKSLASH          \
 KEY_RIGHT_BRACKET      ]
KEY_GRAVE_ACCENT       `

 KEY_ESCAPE             
 KEY_ENTER              
 KEY_TAB                
 KEY_BACKSPACE          
 KEY_INSERT             
 KEY_DELETE             
 KEY_RIGHT              
 KEY_LEFT               
 KEY_DOWN               
 KEY_UP                 
 KEY_PAGE_UP            
 KEY_PAGE_DOWN          
 KEY_HOME               
 KEY_END                
 KEY_CAPS_LOCK          
 KEY_SCROLL_LOCK        
 KEY_NUM_LOCK           
 KEY_PRINT_SCREEN       
 KEY_PAUSE              

 KEY_LEFT_SHIFT         
 KEY_LEFT_CONTROL       
 KEY_LEFT_ALT           
 KEY_RIGHT_SHIFT        
 KEY_RIGHT_CONTROL      
 KEY_RIGHT_ALT          
 KEY_MENU               

*/