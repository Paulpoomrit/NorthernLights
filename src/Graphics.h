#include "PPM.h"

class Graphics {
public:
    static PPM ApplyFilter(const PPM& image, const std::string& filter);
    static PPM MakeGreyScale(const PPM& image);
    static PPM RotateImage(const PPM& image, double rotationAngle);
    static PPM ScaleImage(const PPM& image, double scale);
    static PPM TranslateImage(const PPM& image, int xOffset, int yOffset);

private:
    static std::vector<float> getFilterMatrix(const std::string& filter);
};