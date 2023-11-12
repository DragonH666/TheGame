#include <cmath>
#define Pi32 3.14159265359f


#define MIN(x, y) (x<y ? x : y)
#define MAX(x, y) (x>y ? x : y)


struct v2{
    f32 x;
    f32 y;
};

struct v3{
    f32 r;
    f32 g;
    f32 b;
};

union v4{
    struct{
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    struct{
        v3 rgb;
    };
};

v4 operator+(v4 Value1, v4 Value2){
    v4 Result = Value1;
    Result.r += Value2.r;
    Result.g += Value2.g;
    Result.b += Value2.b;
    Result.a += Value2.a;
    
    return Result;
}

v4 operator*(v4 Value1, f32 Value2){
    v4 Result = Value1;
    Result.r *= Value2;
    Result.g *= Value2;
    Result.b *= Value2;
    Result.a *= Value2;
    
    return Result;
}

v4 operator*(f32 Value2, v4 Value1){
    v4 Result = Value1;
    Result.r *= Value2;
    Result.g *= Value2;
    Result.b *= Value2;
    Result.a *= Value2;
    
    return Result;
}

v4 operator/(v4 Value1, f32 Value2){
    v4 Result = Value1;
    Result.r /= Value2;
    Result.g /= Value2;
    Result.b /= Value2;
    Result.a /= Value2;
    
    return Result;
}

v4 operator+=(v4& Value1, v4 Value2){
    Value1.r += Value2.r;
    Value1.g += Value2.g;
    Value1.b += Value2.b;
    Value1.a += Value2.a;
    
    return Value1;
}

v4 operator*=(v4& Value1, v4 Value2){
    Value1.r *= Value2.r;
    Value1.g *= Value2.g;
    Value1.b *= Value2.b;
    Value1.a *= Value2.a;
    
    return Value1;
}

v4 operator*=(v4& Value1, f32 Value2){
    Value1.r *= Value2;
    Value1.g *= Value2;
    Value1.b *= Value2;
    Value1.a *= Value2;
    
    return Value1;
}

v4 operator/=(v4& Value1, f32 Value2){
    Value1.r /= Value2;
    Value1.g /= Value2;
    Value1.b /= Value2;
    Value1.a /= Value2;
    
    return Value1;
}


v3 operator*=(v3& Value1, f32 Value2){
    Value1.r *= Value2;
    Value1.g *= Value2;
    Value1.b *= Value2;
    
    return Value1;
}


v3 operator*=(v3& Value1, v3 Value2){
    Value1.r *= Value2.r;
    Value1.g *= Value2.g;
    Value1.b *= Value2.b;
    
    return Value1;
}


v2 operator-(v2 Value){
    v2 Result;
    Result.x = -Value.x; 
    Result.y = -Value.y;
    
    return Result;
}

v2 operator+(v2 Value1, v2 Value2){
    v2 Result;
    Result.x = Value1.x + Value2.x; 
    Result.y = Value1.y + Value2.y;
    
    return Result;
}

v2 operator-(v2 Value1, v2 Value2){
    v2 Result;
    Result.x = Value1.x - Value2.x; 
    Result.y = Value1.y - Value2.y;
    
    return Result;
}

v2 operator-=(v2& Value1, v2 Value2){
    Value1.x -= Value2.x; 
    Value1.y -= Value2.y;
    
    return Value1;
}

v2 operator*(f32 Value2, v2 Value1){
    v2 Result = Value1;
    Result.x *= Value2;
    Result.y *= Value2;
    
    return Result;
}

v2 operator*=(v2& Value1, v2 Value2){
    Value1.x *= Value2.x;
    Value1.y *= Value2.y;
    
    return Value1;
}

v2 operator*(v2 Value1, f32 Value2){
    v2 Result = Value1;
    Result.x *= Value2;
    Result.y *= Value2;
    
    return Result;
}

v2 operator/(v2 Value1, f32 Value2){
    v2 Result = Value1;
    Result.x /= Value2;
    Result.y /= Value2;
    
    return Result;
}

f32 Lerp(f32 Value1, f32 Value2, f32 T){
    return Value1*(1-T) + Value2*T;
}

v2 Lerp(v2 Value1, v2 Value2, f32 T){
    v2 Result;
    Result.x = Lerp(Value1.x, Value2.x, T);
    Result.y = Lerp(Value1.y, Value2.y, T);
    
    return Result;
}

v3 Lerp(v3 Value1, v3 Value2, f32 T){
    v3 Result;
    Result.r = Lerp(Value1.r, Value2.r, T);
    Result.g = Lerp(Value1.g, Value2.g, T);
    Result.b = Lerp(Value1.b, Value2.b, T);
    
    return Result;
}

v4 Lerp(v4 Value1, v4 Value2, f32 T){
    v4 Result;
    Result.r = Lerp(Value1.r, Value2.r, T);
    Result.g = Lerp(Value1.g, Value2.g, T);
    Result.b = Lerp(Value1.b, Value2.b, T);
    Result.a = Lerp(Value1.a, Value2.a, T);
    
    return Result;
}

f32 Sqrt(f32 Num){
    return sqrt(Num);
}

s32 Sign(f32 Num){
    s32 Result = Num > 0 ? 1 : (Num < 0 ? -1 : 0);
    return Result;
}

f32 Dot(v2 Value1, v2 Value2){
    f32 Result = Value1.x*Value2.x + Value1.y*Value2.y;
    return Result;
}

v2 Perp(v2 Value){
    v2 Result = {-Value.y, Value.x};
    return Result;
}

f32 Clamp01(f32 Value){
    f32 Result = MAX(MIN(Value, 1), 0);
    return Result;
}

f32 Sin(f32 Angle){
    f32 Result = sin(Angle);
    return Result;
}

f32 Cos(f32 Angle){
    f32 Result = cos(Angle);
    return Result;
}

f32 Len(v2 Value){
    f32 Result = Sqrt(Value.x*Value.x + Value.y*Value.y);
    return Result;
}