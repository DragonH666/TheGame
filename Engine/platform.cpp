#include "windows.h"
#include "glad.h"
#include "glfw/glfw3.h"
#include <stdio.h>
#include <sys/stat.h>
#include <ctime>
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "game.h"
#include "math.h"
#include "Common.h"
#define MemFence _WriteBarrier(); _ReadBarrier();

/*TODO
 - change Sleep to waking up
*/

#define INPUT_PRESSED (1 << 0)
#define INPUT_HOLD (1 << 1)
#define INPUT_RELEASED (1 << 2)

typedef void (*game_set_functions)(export_draw_rect, export_draw_image, export_load_image, export_is_pressed, export_is_hold, export_is_released);
typedef void (*game_init)(void*, f32, f32);
typedef void (*game_update)(void*, f32, f32);

u8* ReadFile(cstr Name, memory* Memory){
    FILE* File = fopen(Name, "rb");
    Assert(File);
    
    fseek(File, 0, SEEK_END);
    s32 Size = ftell(File);
    fseek(File, 0, SEEK_SET);
    Assert(Size);
    
    u8* Buffer = Alloc(Size, Memory);
    fread(Buffer, Size, 1, File);
    fclose(File);
    return Buffer;
}


void WriteFile(cstr Name, s8* Buffer, s32 Size){
    FILE* File = fopen(Name, "wb");
    Assert(File);
    
    fwrite(Buffer, Size, 1, File);
    fclose(File);
}


struct mouse{
    b32 Pressed;
    b32 Hold;
    b32 Released;
    b32 RPressed;
    b32 RHold;
    b32 RReleased;
    b32 DoubleClick;
    v2 Pos;
    
    mouse(mouse volatile& Mouse){
        Pressed = Mouse.Pressed;
        Hold = Mouse.Hold;
        Released = Mouse.Released;
        RPressed = Mouse.RPressed;
        RHold = Mouse.RHold;
        RReleased = Mouse.RReleased;
        DoubleClick = Mouse.DoubleClick;
        Pos.x = Mouse.Pos.x;
        Pos.y = Mouse.Pos.y;
        
    }
    
    
    void operator=(mouse volatile& Mouse){
        Pressed = Mouse.Pressed;
        Hold = Mouse.Hold;
        Released = Mouse.Released;
        RPressed = Mouse.RPressed;
        RHold = Mouse.RHold;
        RReleased = Mouse.RReleased;
        DoubleClick = Mouse.DoubleClick;
        Pos.x = Mouse.Pos.x;
        Pos.y = Mouse.Pos.y;
        
    }
    
    
    mouse(){
        
    }
};

void RenderTexture(v2 Pos, texture Texture, v4 Color = {1, 1, 1, 1}, f32 RotationY3D=0, f32 Scale=1.f);
void RenderTexture(v2 Pos, s32 ID, s32 Width, s32 Height, v4 Color = {1, 1, 1, 1});

struct render_texture_queue_node{
    v2 Pos;
    s32 TextureID;
    s32 Width;
    s32 Height;
    v4 Color;
};

struct render_info{
    u32 VAO;
    u32 VBO;
    u32 DefaultShader;
    u32 DefaultTexturesUniform;
    u32 DefaultTransformUniform;
    u32 Shader;
    u32 TexturesUniform;
    u32 TransformUniform;
} RenderInfo;


struct vertex{
    v2 Pos;
    v2 TextCoord;
    f32 Index;
    v4 Color;
};
#define MAX_DRAW_TEXTURES 1500

void RenderQueue(render_texture_queue_node* Queue, s32 QueueSize, s32 Width, s32 Height){
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    s32 Indexs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    f32 Transform[] = {
        2.f/Width, 0,
        0, 2.f/Height,
    };
    
    glUseProgram(RenderInfo.Shader);
    glUniformMatrix2fv(RenderInfo.TransformUniform, 1, 0, Transform);
    glUniform1iv(RenderInfo.TexturesUniform, 16, Indexs);
    
    glBindVertexArray(RenderInfo.VAO);
    
    s32 TexturesIDs[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    u32 TexturesIDsSize = 0;
    f32 NewData[sizeof(vertex)/sizeof(f32)*4* MAX_DRAW_TEXTURES] = {};
    vertex* Data = (vertex*)NewData;
    s32 ToDraw = 0;
    for(s32 i=0; i<QueueSize; i++){
        render_texture_queue_node Node = Queue[i];
        
        s32 TextureIDIndex = -1;
        
        for(s32 j=0; j<TexturesIDsSize; j++){
            if(Node.TextureID == TexturesIDs[j]){
                TextureIDIndex = j;
            }
        }
        
        if(TextureIDIndex == -1){
            if(TexturesIDsSize < 16){
                glActiveTexture(GL_TEXTURE0 + TexturesIDsSize);
                glBindTexture(GL_TEXTURE_2D, Node.TextureID);
                
                TextureIDIndex = TexturesIDsSize;
                TexturesIDs[TexturesIDsSize++] = Node.TextureID;
            }
            else{
                glBindVertexArray(RenderInfo.VAO);
                glBindBuffer(GL_ARRAY_BUFFER, RenderInfo.VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex)*4 * MAX_DRAW_TEXTURES, NewData);
                glDrawElements(GL_TRIANGLES, 6*ToDraw, GL_UNSIGNED_INT, 0);
                
                Data = (vertex*)NewData;
                TexturesIDsSize = 0;
                ToDraw = 0;
                
                glActiveTexture(GL_TEXTURE0 + TexturesIDsSize);
                glBindTexture(GL_TEXTURE_2D, Node.TextureID);
                
                TextureIDIndex = TexturesIDsSize;
                TexturesIDs[TexturesIDsSize++] = Node.TextureID;
            }
        }
        
        Data[0].Pos = Node.Pos;
        Data[0].TextCoord = v2{0.f, 1.f};
        Data[0].Index = TextureIDIndex;
        Data[0].Color = Node.Color;
        
        Data[1].Pos = Node.Pos + v2{(f32)Node.Width, 0};
        Data[1].TextCoord = v2{1.f, 1.f};
        Data[1].Index = TextureIDIndex;
        Data[1].Color = Node.Color;
        
        Data[2].Pos = Node.Pos + v2{(f32)Node.Width, (f32)Node.Height};
        Data[2].TextCoord = v2{1.f, 0.f};
        Data[2].Index = TextureIDIndex;
        Data[2].Color = Node.Color;
        
        Data[3].Pos = Node.Pos + v2{0, (f32)Node.Height};
        Data[3].TextCoord = v2{0.f, 0.f};
        Data[3].Index = TextureIDIndex;
        Data[3].Color = Node.Color;
        
        ToDraw++;
        Data = Data + 4;
    }
    glBindVertexArray(RenderInfo.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, RenderInfo.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex)*4 * MAX_DRAW_TEXTURES, NewData);
    glDrawElements(GL_TRIANGLES, 6*ToDraw, GL_UNSIGNED_INT, 0);
    
    s32 Error = glGetError();
    if(Error){
        //printf("GLError: %d\n", Error);
    }
    glGetError();
    glUseProgram(0);
    glBindVertexArray(0);
}
render_texture_queue_node RenderTextureQueue[1000];
s32 RenderTextureQueueSize;
s32 EmptyTextureID;

void RenderTexture(v2 Pos, texture Texture, v4 Color, f32 RotationY3D, f32 Scale){
    RenderTextureQueue[RenderTextureQueueSize++] = {Pos, Texture.ID, (s32)(Texture.Width*Scale), (s32)(Texture.Height*Scale), Color};
}

void RenderTexture(v2 Pos, s32 ID, s32 Width, s32 Height, v4 Color){
    RenderTextureQueue[RenderTextureQueueSize++] = {Pos, ID, Width, Height, Color};
}

s32 AddTexture(u32* Pixels, s32 Width, s32 Height){
    u32 Texture; 
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
    
    return Texture;
}

volatile b32 CanAccessInputs = 1;
volatile b32 Inputs[349] = {};
b32 CopyInputs[349];
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    while(1){
        b32 AllowedToAccess = InterlockedExchange((LONG* volatile)&CanAccessInputs, 0);
        if(AllowedToAccess){
            if (action == GLFW_PRESS){
                Inputs[button] |= INPUT_PRESSED | INPUT_HOLD;
            }
            else if (action == GLFW_RELEASE){
                Inputs[button] = INPUT_RELEASED;
            }
            InterlockedExchange((LONG* volatile)&CanAccessInputs, 1);
            break;
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    while(1){
        b32 AllowedToAccess = InterlockedExchange((LONG* volatile)&CanAccessInputs, 0);
        if(AllowedToAccess){
            if (action == GLFW_PRESS){
                Inputs[key] |= INPUT_PRESSED | INPUT_HOLD;
            }
            else if (action == GLFW_RELEASE){
                Inputs[key] = INPUT_RELEASED;
            }
            InterlockedExchange((LONG* volatile)&CanAccessInputs, 1);
            break;
        }
    }
}

b32 Run = 1;

void WindowCloseCallback(GLFWwindow* window)
{
    Run = 0;
}

const v4 White = {1.f, 1.f, 1.f, 1.f};
void ExportDrawRect(f32 x, f32 y, s32 Width, s32 Height, v4 Color){
    RenderTextureQueue[RenderTextureQueueSize++] = {v2{x, y}, EmptyTextureID, Width, Height, Color};
}

void ExportDrawImage(f32 x, f32 y, image Image){
    RenderTextureQueue[RenderTextureQueueSize++] = {v2{x, y}, Image.ID, Image.Width, Image.Height, White};
}

char* ResourcesPath;
image ExportLoadImage(const char* Path){
    s32 Width, Height, n;
    char FinalPath[256+100] = {};
    strcpy(FinalPath, ResourcesPath);
    strcat(FinalPath, Path);
    u32* Pixels = (u32*)stbi_load(FinalPath, &Width, &Height, &n, 4);
    
    s32 ID = AddTexture(Pixels, Width, Height);
    image Image;
    Image.Width = Width;
    Image.Height = Height;
    Image.ID = ID;
    return Image;
}

b32 ExportIsPressed(s32 Code){
    b32 Result = 0;
    if(CopyInputs[Code] & INPUT_PRESSED){
        Result = 1;
    }
    return Result;
}

b32 ExportIsHold(s32 Code){
    b32 Result = 0;
    if(CopyInputs[Code] & INPUT_HOLD){
        Result = 1;
    }
    return Result;
}

b32 ExportIsReleased(s32 Code){
    b32 Result = 0;
    if(CopyInputs[Code] & INPUT_RELEASED){
        Result = 1;
    }
    return Result;
}

DWORD WINAPI ThreadProc(LPVOID Parameter){
    GLFWwindow* window = (GLFWwindow*)Parameter;
    
    glfwMakeContextCurrent(window);
    
    s32 WindowWidth = 1280;
    s32 WindowHeight = 720;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        return -2;
    }
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    glfwSwapInterval(1);
    
    const char *VertexShaderSource = R"(
#version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;;
        layout (location = 2) in float aIndex;
        layout (location = 3) in vec4 aColor;

 out vec2 TexCoord;
out float Index;
out vec4 Color;
        uniform mat2 Transform;
void main()
        {
vec2 Pos = Transform * aPos;
           gl_Position = vec4(Pos, 0, 1.f) - vec4(1, 1, 0, 0);
TexCoord = aTexCoord;
Index = aIndex;
Color = aColor;
}
)";
    
    const char *FragmentShaderSource = R"(
#version 330 core
            out vec4 FragColor;

in vec2 TexCoord;
in float Index;
in vec4 Color; 
uniform sampler2D Textures[16];
            void main()
            {
vec4 C = texture(Textures[int(Index)], TexCoord) * Color;
FragColor = C;
            }
)";
    
    unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);
    
    int Success;
    char InfoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", InfoLog);
    }
    
    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", InfoLog);
    }
    
    unsigned int ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", InfoLog);
    }
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    
    unsigned int indices[sizeof(u32)*6 * MAX_DRAW_TEXTURES];
    for(s32 i=0; i<MAX_DRAW_TEXTURES; i++){
        u32* Ptr = indices + i*6;
        Ptr[0] = i*4;
        Ptr[1] = i*4 + 1;
        Ptr[2] = i*4 + 2;
        
        Ptr[3] = i*4 + 0;
        Ptr[4] = i*4 + 2;
        Ptr[5] = i*4 + 3;
    }
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*4 * MAX_DRAW_TEXTURES, 0, GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(2*sizeof(f32)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(4*sizeof(f32)));
    glEnableVertexAttribArray(2);
    
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(5*sizeof(f32)));
    glEnableVertexAttribArray(3);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    s32 TexturesUniform = glGetUniformLocation(ShaderProgram, "Textures");
    s32 TransformUniform = glGetUniformLocation(ShaderProgram, "Transform");
    
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    RenderInfo.VAO = VAO;
    RenderInfo.VBO = VBO;
    RenderInfo.Shader = ShaderProgram;
    RenderInfo.TexturesUniform = TexturesUniform;
    RenderInfo.TransformUniform = TransformUniform;
    
    void* Memory = malloc(MEGABYTES(4));
    memset(Memory, 0, MEGABYTES(4));
    
    HMODULE DllHandle = LoadLibraryA("game.dll");
    if(!DllHandle){
        printf("dll did not load\n");
    }
    
    game_set_functions GameSetFunctions = (game_set_functions)GetProcAddress(DllHandle, "SetFunctions");
    game_init GameInit = (game_init)GetProcAddress(DllHandle, "Init");
    game_update GameUpdate = (game_update)GetProcAddress(DllHandle, "Update");
    if(!GameSetFunctions || !GameUpdate){
        printf("Load Function Error: %dl, %dl\n", GameSetFunctions, GameUpdate);
    }
    GameSetFunctions(ExportDrawRect, ExportDrawImage, ExportLoadImage, ExportIsPressed, ExportIsHold, ExportIsReleased);
    f64 MouseX, MouseY;
    glfwGetCursorPos(window, &MouseX, &MouseY);
    GameInit(Memory, (f32)MouseX, (f32)MouseY);
    while (Run)
    {
        f64 MouseX, MouseY;
        glfwGetCursorPos(window, &MouseX, &MouseY);
        MouseY = WindowHeight - MouseY;
        
        while(1){
            b32 Allowed = InterlockedExchange((LONG* volatile)&CanAccessInputs, 0);
            if(Allowed){
                memcpy(CopyInputs, (const void*)Inputs, sizeof(b32)*349);
                for(s32 i=0; i<349; i++){
                    Inputs[i] &= ~INPUT_PRESSED;
                    Inputs[i] &= ~INPUT_RELEASED;
                }
                InterlockedExchange((LONG* volatile)&CanAccessInputs, 1);
                break;
            }
        }
        
        GameUpdate(Memory, MouseX, MouseY);
        u32 Pixels[] = {0xffffffff};
        EmptyTextureID = AddTexture(Pixels, 1, 1);
        
        
        glfwGetWindowSize(window, &WindowWidth, &WindowHeight);
        glViewport(0, 0, WindowWidth, WindowHeight);
        RenderQueue(RenderTextureQueue, RenderTextureQueueSize, WindowWidth, WindowHeight);
        RenderTextureQueueSize = 0;
        
        glfwSwapBuffers(window);
    }
    return 0;
}

int main(void)
{
    GLFWwindow* window;
    
    int Width = 1280;
    int Height = 720;
    
    if (!glfwInit()){
        printf("Init error");
        return -1;
    }
    
    window = glfwCreateWindow(Width, Height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwSetWindowCloseCallback(window, WindowCloseCallback);
    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    
    ResourcesPath = (char*)malloc(256 + 100);
    s32 PathSize = GetModuleFileNameA(NULL, ResourcesPath, 256 + 100);
    
    s32 SlashCount = 0;
    for(s32 i=0; i<PathSize; i++){
        if(ResourcesPath[PathSize-i-1] == '\\'){
            SlashCount++;
        }
        if(SlashCount == 2){
            ResourcesPath[PathSize-i] = '\0';
            strcat(ResourcesPath, "Resources\\");
            break;
        }
    }
    if(SlashCount != 2){
        printf("ERROR: Resources folder not found where it should be");
    }
    
    CreateThread(0, 0, ThreadProc, window, 0, 0);
    
    while (Run)
    {
        glfwPollEvents();
        Sleep(1);
    }
    
    glfwTerminate();
    return 0;
}