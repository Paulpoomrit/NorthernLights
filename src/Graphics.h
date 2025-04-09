#include "PPM.h"

class Graphics
{
public:
    /* Basic Functionality */
    static PPM ApplyFilter(const PPM &image, const std::string &filter);
    static PPM MakeGreyScale(const PPM &image);
    static PPM RotateImage(const PPM &image, double rotationAngle);
    static PPM ScaleImage(const PPM &image, double scale);
    static PPM TranslateImage(const PPM &image, int xOffset, int yOffset);

    /* Special Filters */
    static PPM FloydSteinbergDither(const PPM &image);
    static PPM InPlaceEmboss(const PPM &image);

private:
    /* Utilities */
    static std::vector<float> getFilterMatrix(const std::string &filter);
    static int pixelDifference(unsigned int a, unsigned int b);
};