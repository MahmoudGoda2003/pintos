#include <stdint.h>

#define FBits 14
#define F (1<<FBits)
typedef int32_t fixedPoint;

fixedPoint intToFixed(int n){
    return n * F;
}

int fixedToInt(fixedPoint x){
    return x / F;
}

int fixedToNearestInt(fixedPoint x){
    if(x <= 0){
        return ((x - F)/2)/F;
    }else{
        return ((x + F)/2)/F;
    }
}

fixedPoint add(fixedPoint x, fixedPoint y){
    return x+y;
}

fixedPoint subtract(fixedPoint x, fixedPoint y){
    return x-y;
}

fixedPoint addInt(fixedPoint x, int n){
    return x + intToFixed(n);
}

fixedPoint subtractInt(fixedPoint x, int n){
    return x - intToFixed(n);
}

fixedPoint multiply(fixedPoint x, fixedPoint y){
    return ((int64_t)x)*y/F;
}

fixedPoint divide(fixedPoint x, fixedPoint y){
    return ((int64_t)x)*F/y;
}

fixedPoint multiplyInt(fixedPoint x, int n){
    return x*n;
}

fixedPoint divideInt(fixedPoint x, int n){
    return x/n;
}
