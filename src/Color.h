#ifndef COLOR_H
#define COLOR_H

#include "math.h"

struct Color {
    float red;
    float green;
    float blue;
    float alpha;
};

// Simple overator for comparing two colors
inline bool operator==(const Color& A, const Color& B) {
    bool redEqual   = round(A.red*100)   == round(B.red*100);
    bool greenEqual = round(A.green*100) == round(B.green*100);
    bool blueEqual  = round(A.blue*100)  == round(B.blue*100);
    bool alphaEqual = round(A.alpha*100) == round(B.alpha*100);
    if (redEqual && greenEqual && blueEqual && alphaEqual) {
        return true;
    }
    return false;
}

#endif