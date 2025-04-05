#include "Graphics.h"
#include "Pixel.h"

const PPM Graphics::MakeGreyScale(const PPM &image)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    std::vector<Pixel> modifiedPixelVector;
    PPM modifiedImage = image;

    const unsigned int width = image.getWidth();
    const unsigned int height = image.getHeight();
    modifiedPixelVector.resize(width * height);

    try
    {
        // grey-ify all pixels
        std::transform(pixelVector.begin(), pixelVector.end(), modifiedPixelVector.begin(),
                       [](const Pixel &p)
                       {
                           unsigned int gray = (p["red"] + p["green"] + p["blue"]) / 3;
                           return Pixel(gray, gray, gray);
                       });
        modifiedImage.setPixels(modifiedPixelVector);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error applying grey scale filter: " << e.what() << std::endl;
    }
    return modifiedImage;
}

int main()
{
    std::ifstream infile("./assets/Shahriar.ppm");
    PPM inputImage(infile);

    try
    {
        PPM result = Graphics::MakeGreyScale(inputImage);
        result.saveImageToFile("./assets/GreyShahriar.ppm");
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    infile.close();

    return 0;
}