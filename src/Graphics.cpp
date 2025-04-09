#include "Graphics.h"
#include "Pixel.h"
#include <stdexcept>
#include <string>
#include <algorithm>

std::vector<float> Graphics::getFilterMatrix(const std::string &filter)
{
    std::vector<float> filterArray;
    if (filter == "blur")
    {
        float v = 1.0f / 9;
        filterArray = {v, v, v,
                       v, v, v,
                       v, v, v};
    }
    else if (filter == "sharpen")
    {
        float v = 1.0f / 3;
        filterArray = {0, -v, 0,
                       -v, 7 * v, -v,
                       0, -v, 0};
    }
    else if (filter == "edgeDetect")
    {
        filterArray = {0, 1, 0,
                       1, -4, 1,
                       0, 1, 0};
    }
    else if (filter == "emboss")
    {
        filterArray = {-2, -1, 0,
                       -1, 1, 1,
                       0, 1, 2};
    }
    else
    {
        throw std::invalid_argument("Unknown filter type: " + filter + ". ");
    }
    return filterArray;
}

/* Basic Functionality */
PPM Graphics::ApplyFilter(const PPM &image, const std::string &filter)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    std::vector<Pixel> modifiedPixelVector;
    PPM modifiedImage = image;
    std::vector<float> filterArray;

    const int width = image.getWidth();
    const int height = image.getHeight();
    modifiedPixelVector.resize(width * height);

    // define surrounding pixels for the current pixel
    const std::vector<int> offsets = {
        -width - 1,
        -width,
        -width + 1,
        -1,
        0,
        1,
        width - 1,
        width,
        width + 1};

    // get an appropriate filter matrix
    try
    {
        filterArray = getFilterMatrix(filter);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << "Returning the original image...";
        return image;
    }

    // applying filter
    for (int y = 0; y < height; y++) // traverse rows of pixels
    {
        for (int x = 0; x < width; x++) // traverse columns of pixels
        {
            float newR = 0, newG = 0, newB = 0;
            unsigned int pixelLoc = width * y + x;
            for (int k = 0; k < 9; k++)
            {
                int neighbourLoc = pixelLoc + offsets[k];
                if (neighbourLoc >= 0 && neighbourLoc < (height * width))
                {
                    newR += filterArray[k] * pixelVector[neighbourLoc]["red"];
                    newG += filterArray[k] * pixelVector[neighbourLoc]["green"];
                    newB += filterArray[k] * pixelVector[neighbourLoc]["blue"];
                }
            }
            // clamp
            newR = std::clamp(newR, 0.0f, 255.0f);
            newG = std::clamp(newG, 0.0f, 255.0f);
            newB = std::clamp(newB, 0.0f, 255.0f);

            Pixel newPixel(newR, newG, newB);
            modifiedPixelVector[pixelLoc] = newPixel;
        }
    }
    modifiedImage.setPixels(modifiedPixelVector);
    return modifiedImage;
}

PPM Graphics::MakeGreyScale(const PPM &image)
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
    catch (const std::exception &e)
    {
        std::cerr << "Error applying grey scale filter: " << e.what() << std::endl;
    }
    return modifiedImage;
}

PPM Graphics::RotateImage(const PPM &image, double rotationAngle)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    std::vector<Pixel> modifiedPixelVector;
    PPM modifiedImage = image;

    const int width = image.getWidth();
    const int height = image.getHeight();
    int centerX = width / 2;
    int centerY = height / 2;
    double radians = rotationAngle * M_PI / 180.0;
    modifiedPixelVector.resize(width * height);

    // define inverse rotational matrix
    const double rotationalMatrixInv[4] = {cos(radians), sin(radians),
                                           -sin(radians), cos(radians)};

    for (int y = 0; y < height; y++) // traverse rows of pixels
    {
        for (int x = 0; x < width; x++) // traverse columns of pixels
        {
            // shifted the center of rotation
            int shiftedY = y - centerY;
            int shiftedX = x - centerX;

            // matrix-vector multiplication
            int rotatedX = round(shiftedX * rotationalMatrixInv[0] + shiftedY * rotationalMatrixInv[1]);
            int rotatedY = round(shiftedX * rotationalMatrixInv[2] + shiftedY * rotationalMatrixInv[3]);

            // shifted back
            int newY = rotatedY + centerY;
            int newX = rotatedX + centerX;

            unsigned int pixelLoc = width * newY + newX;

            if (newY >= 0 && newY < height && newX >= 0 && newX < width)
            {
                Pixel newPixel(pixelVector[pixelLoc]["red"], pixelVector[pixelLoc]["green"], pixelVector[pixelLoc]["blue"]);
                modifiedPixelVector[y * width + x] = newPixel;
            }
        }
    }
    modifiedImage.setPixels(modifiedPixelVector);
    return modifiedImage;
}

PPM Graphics::ScaleImage(const PPM &image, double scale)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    std::vector<Pixel> modifiedPixelVector;
    PPM modifiedImage = image;

    const int width = image.getWidth();
    const int height = image.getHeight();
    int centerX = width / 2;
    int centerY = height / 2;
    modifiedImage.setHeight(height * scale);
    modifiedImage.setWidth(width * scale);
    modifiedPixelVector.resize((width * scale) * (height * scale));

    // define inverse scaling matrix
    const double scalingMatrixInv[4] = {1 / scale, 0,
                                        0, 1 / scale};

    for (int y = 0; y < height * scale; y++) // traverse rows of pixels
    {
        for (int x = 0; x < width * scale; x++) // traverse columns of pixels
        {

            // matrix-vector multiplication
            int newX = round(x * scalingMatrixInv[0] + y * scalingMatrixInv[1]);
            int newY = round(x * scalingMatrixInv[2] + y * scalingMatrixInv[3]);

            unsigned int pixelLoc = width * newY + newX;

            if (newY >= 0 && newY < height && newX >= 0 && newX < width)
            {
                Pixel newPixel(pixelVector[pixelLoc]["red"], pixelVector[pixelLoc]["green"], pixelVector[pixelLoc]["blue"]);
                modifiedPixelVector[y * (width * scale) + x] = newPixel;
            }
        }
    }
    modifiedImage.setPixels(modifiedPixelVector);
    return modifiedImage;
}

PPM Graphics::TranslateImage(const PPM &image, int xOffset, int yOffset)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    std::vector<Pixel> modifiedPixelVector;
    PPM modifiedImage = image;

    const int width = image.getWidth();
    const int height = image.getHeight();
    modifiedPixelVector.resize(width * height);

    // define inverse translating matrix -> hommogenius coords
    const int translatingMatrixInv[9] = {1, 0, xOffset,
                                         0, 1, yOffset,
                                         0, 0, 1};

    for (int y = 0; y < height; y++) // traverse rows of pixels
    {
        for (int x = 0; x < width; x++) // traverse columns of pixels
        {

            // matrix-vector multiplication
            int newX = round(x * translatingMatrixInv[0] + y * translatingMatrixInv[1] + translatingMatrixInv[2]);
            int newY = round(x * translatingMatrixInv[3] + y * translatingMatrixInv[4] + translatingMatrixInv[5]);

            unsigned int pixelLoc = width * newY + newX;

            if (newY >= 0 && newY < height && newX >= 0 && newX < width)
            {
                Pixel newPixel(pixelVector[pixelLoc]["red"], pixelVector[pixelLoc]["green"], pixelVector[pixelLoc]["blue"]);
                modifiedPixelVector[y * width + x] = newPixel;
            }
        }
    }
    modifiedImage.setPixels(modifiedPixelVector);
    return modifiedImage;
}

/* Special Filters */
PPM Graphics::FloydSteinbergDither(const PPM &image)
{
    PPM greyifiedImg = Graphics::MakeGreyScale(image);
    PPM modifiedImage = image;
    std::vector<Pixel> greyifiedPixelVector = greyifiedImg.getPixels();
    std::vector<Pixel> quantizedPixelVector;
    std::vector<Pixel> modifiedPixelVector;
    quantizedPixelVector.reserve(greyifiedPixelVector.size());
    modifiedPixelVector.reserve(greyifiedPixelVector.size());

    // quantized
    for (auto pixel : greyifiedPixelVector)
    {
        if (pixel["red"] < 128)
        {
            quantizedPixelVector.push_back(Pixel(0, 0, 0));
        }
        else
        {
            quantizedPixelVector.push_back(Pixel(255, 255, 255));
        }
    }

    const int width = image.getWidth();
    const int height = image.getHeight();

    const std::vector<int> offsets = {
        -width - 1,
        -width,
        -width + 1,
        -1,
        0,
        1,
        width - 1,
        width,
        width + 1};

    // applying filter
    for (int y = 0; y < height; y++) // traverse rows of pixels
    {
        for (int x = 0; x < width; x++) // traverse columns of pixels
        {
            unsigned int pixelLoc = width * y + x;
            Pixel originalPixel = greyifiedPixelVector[pixelLoc];
            Pixel quantizedPixel = quantizedPixelVector[pixelLoc];

            int error[3] =
                {
                    originalPixel["red"] - quantizedPixel["red"],
                    originalPixel["green"] - quantizedPixel["green"],
                    originalPixel["blue"] - quantizedPixel["blue"]
                };

            modifiedPixelVector[pixelLoc] = quantizedPixel;

            // update surrounding pixels
            auto UpdatePixel = [&pixelLoc, &modifiedPixelVector, &error, &height, &width](const int offset, const float errorBias)
            {
                int neighbourLoc = pixelLoc + offset;
                if (neighbourLoc >= 0 && neighbourLoc < (height * width))
                {
                    Pixel p = modifiedPixelVector[neighbourLoc];
                    float newR = p["red"] + error[0] * errorBias;
                    float newG = p["green"] + error[1] * errorBias;
                    float newB = p["blue"] + error[2] * errorBias;

                    // clamp
                    newR = std::clamp(newR, 0.0f, 255.0f);
                    newG = std::clamp(newG, 0.0f, 255.0f);
                    newB = std::clamp(newB, 0.0f, 255.0f);

                    Pixel newPixel(newR, newG, newB);
                    modifiedPixelVector[neighbourLoc] = newPixel;
                }
            };
            UpdatePixel(offsets[5], 7.0f / 16.0f);
            UpdatePixel(offsets[6], 3.0f / 16.0f);
            UpdatePixel(offsets[7], 5.0f / 16.0f);
            UpdatePixel(offsets[8], 1.0f / 16.0f);
        }
    }

    modifiedImage.setPixels(modifiedPixelVector);
    return modifiedImage;
}

int main()
{
    std::ifstream infile("./assets/Shahriar.ppm");
    PPM inputImage(infile);

    try
    {
        // PPM result = Graphics::MakeGreyScale(inputImage);
        // PPM result = Graphics::ApplyFilter(inputImage, "sharpen");
        // PPM result = Graphics::RotateImage(inputImage, 45);
        // PPM result = Graphics::ScaleImage(inputImage, 3);
        // PPM result = Graphics::TranslateImage(inputImage, 50, 0);
        PPM result = Graphics::FloydSteinbergDither(inputImage);
        result.saveImageToFile("./assets/NewShahriar.ppm");
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    infile.close();

    return 0;
}