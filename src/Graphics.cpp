#include "Graphics.h"
#include "Pixel.h"

const PPM& Graphics::MakeGreyScale(PPM& image)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    const unsigned int width = image.getWidth();
    const unsigned int height = image.getHeight();

    // grey-ify all pixels
    std::transform(pixelVector.begin(), pixelVector.end(), pixelVector.begin(),
                   [](Pixel &p)
                   {
                       unsigned int gray = (p["red"] + p["green"] + p["blue"]) / 3;
                       return Pixel(gray, gray, gray);
                   });
    PPM modifiedImage = image;
    modifiedImage.setPixels(pixelVector);
    return modifiedImage;
}