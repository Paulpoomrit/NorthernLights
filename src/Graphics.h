#include "PPM.h"

class Graphics {
public:
    static const PPM ApplyFilter(const PPM& image, const std::string& filter);
    static const PPM MakeGreyScale(const PPM& image);
    static const PPM RotateImage(const PPM& image, double rotationAngle);
    static const PPM ScaleImage(const PPM& image, double scale);
    static const PPM TranslateImage(const PPM& image, int xOffset, int yOffset);

private:
    static std::vector<float> getFilterMatrix(const std::string& filter);
};