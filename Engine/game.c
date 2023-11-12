typedef struct{
    float x, y, z, w;
} v4;
#include "Common.h"

v4 White = {1.f, 1.f, 1.f, 1.f};
v4 Black = {0.f, 0.f, 0.f, 1.f};
v4 Red = {1.f, 0.f, 0.f, 1.f};
v4 Green = {0.f, 1.f, 0.f, 1.f};
v4 Blue = {0.f, 0.f, 1.f, 1.f};
v4 Yellow = {1.f, 1.f, 0.f, 1.f};
v4 Violet = {1.f, 0.f, 1.f, 1.f};
v4 Cyan = {0.f, 1.f, 1.f, 1.f};

export_draw_rect DrawRect;
export_draw_image DrawImage;
export_load_image LoadImage;

export_is_pressed IsPressed;
export_is_hold IsHold;
export_is_released IsReleased;

void SetFunctions(export_draw_rect ExportDrawRect, export_draw_image ExportDrawImage, export_load_image ExportLoadImage, export_is_pressed ExportIsPressed, export_is_hold ExportIsHold, export_is_released ExportIsReleased){
    DrawRect = ExportDrawRect;
    DrawImage = ExportDrawImage;
    LoadImage = ExportLoadImage;
    
    IsPressed  = ExportIsPressed;
    IsHold  = ExportIsHold;
    IsReleased  = ExportIsReleased;
    
}