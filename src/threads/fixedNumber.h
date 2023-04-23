#include <stdint.h>

#define FBits 14
#define F (1<<FBits)
typedef int32_t fixedPoint;

fixedPoint intToFixed(int n);
int fixedToInt(fixedPoint x);
int fixedToNearestInt(fixedPoint x);
fixedPoint add(fixedPoint x, fixedPoint y);
fixedPoint subtract(fixedPoint x, fixedPoint y);
fixedPoint addInt(fixedPoint x, int n);
fixedPoint subtractInt(fixedPoint x, int n);
fixedPoint multiply(fixedPoint x, fixedPoint y);
fixedPoint divide(fixedPoint x, fixedPoint y);
fixedPoint multiplyInt(fixedPoint x, int n);
fixedPoint divideInt(fixedPoint x, int n);


