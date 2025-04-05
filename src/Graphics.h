#include "PPM.h"

class Graphics {
public:
    static const PPM& ApplyFilter(PPM& image, const char* filter);
    static const PPM MakeGreyScale(const PPM& image);
    static const PPM& RotateImage(PPM& image, double rotationAngle);
    static const PPM& ScaleImage(PPM& image, double scale);
    static const PPM& TranslateImage(PPM& image, int xOffset, int yOffset);
};