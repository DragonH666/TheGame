typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
typedef s32 b32;

typedef const char* cstr;
typedef char str;

#define Assert(x) if(!(x)){ *(s8*)0 = 12; }

#define KILOBYTES(x) x*1024
#define MEGABYTES(x) KILOBYTES(x)*1024
#define GIGABYTES(x) MEGABYTES(x)*1024


struct memory{
    u8* Base;
    s32 Used;
    s32 Size;
};

u8* Alloc(s32 Size, memory* Memory){
    u8* Result = Memory->Base + Memory->Used;
    Memory->Used += Size;
    Assert(Memory->Used <= Memory->Size);
    return Result;
}

void Free(memory* Memory){
    Memory->Used = 0;
}

#define AllocArray(Count, Type, Memory) (Type*)Alloc((Count)*sizeof(Type), Memory)
