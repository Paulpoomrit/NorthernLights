#include "Graphics.h"
#include "Pixel.h"
#include <stdexcept>
#include <string>
#include <algorithm>
#define M_PI 3.14159265358979323846264338327950288

/* Utilities */
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

int Graphics::pixelDifference(unsigned int a, unsigned int b)
{
    return (a > b) ? static_cast<int>(a - b) : -static_cast<int>(b - a);
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
        -width - 1, -width, -width + 1,
        -1, 0, 1,
        width - 1, width, width + 1};

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
    std::vector<Pixel> modifiedPixelVector;
    modifiedPixelVector.reserve(greyifiedPixelVector.size());
    modifiedPixelVector = greyifiedPixelVector;

    // Quantized lamda func
    auto QuantiseGreyscaleNBit = [](const Pixel p)
    {
        int nBits = 2; // how many shades of grey? (,,>﹏<,,)
        float fLevels = (1 << nBits) - 1;
        unsigned int greyVal = std::clamp(std::round(float(p["red"]) / 255.0f * fLevels) / fLevels * 255.0f, 0.0f, 255.0f);
        return Pixel(greyVal, greyVal, greyVal);
    };

    const int width = image.getWidth();
    const int height = image.getHeight();

    const std::vector<int> offsets = {
        -width - 1, -width, -width + 1,
        -1, 0, 1,
        width - 1, width, width + 1};

    // applying filter
    for (int y = 0; y < height; y++) // traverse rows of pixels
    {
        for (int x = 0; x < width; x++) // traverse columns of pixels
        {
            unsigned int pixelLoc = width * y + x;
            Pixel originalPixel = modifiedPixelVector[pixelLoc];
            Pixel quantizedPixel = QuantiseGreyscaleNBit(originalPixel);

            int error[3] = {
                pixelDifference(originalPixel["red"], quantizedPixel["red"]),
                pixelDifference(originalPixel["green"], quantizedPixel["green"]),
                pixelDifference(originalPixel["blue"], quantizedPixel["blue"])};

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

PPM Graphics::InPlaceEmboss(const PPM &image)
{
    std::vector<Pixel> pixelVector = image.getPixels();
    PPM modifiedImage = image;
    std::vector<float> filterArray;

    const int width = image.getWidth();
    const int height = image.getHeight();

    // define surrounding pixels for the current pixel
    const std::vector<int> offsets = {
        -width - 1, -width, -width + 1,
        -1, 0, 1,
        width - 1, width, width + 1};

    // get an appropriate filter matrix
    try
    {
        filterArray = getFilterMatrix("emboss");
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

            Pixel newPixel(newR, newG, newB);
            pixelVector[pixelLoc] = newPixel;
        }
    }
    modifiedImage.setPixels(pixelVector);
    return modifiedImage;
}

int main()
{
    std::cout << "Northern Lights: PPM Image Processing ⸜(｡˃ ᵕ ˂ )⸝♡\n";
    std::cout << "----------------------------\n";

    // Input handling
    std::string inputPath;
    std::cout << "Enter path to input PPM file: ";
    std::cin >> inputPath;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::ifstream infile(inputPath);
    if (!infile.is_open())
    {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    PPM inputImage(infile);
    infile.close();

    std::cout << "Image loaded. Dimensions: "
              << inputImage.getWidth() << "x" << inputImage.getHeight() << "\n\n";
    std::cout << "----------------------------\n";
    std::cout << "( •̯́ ₃ •̯̀) Available operations:\n";
    std::cout << "1. Apply Filter (blur/sharpen/edgeDetect/emboss)\n";
    std::cout << "2. Convert to Grayscale\n";
    std::cout << "3. Rotate Image\n";
    std::cout << "4. Scale Image\n";
    std::cout << "5. Translate Image\n";
    std::cout << "----------------------------\n";
    std::cout << "( ⸝⸝´꒳`⸝⸝) Special Filters: \n";
    std::cout << "6. Floyd-Steinberg Dithering\n";
    std::cout << "7. In-Place Emboss\n";

    int choice;
    std::cout << "Enter operation number (1-7): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    PPM result;
    try
    {
        switch (choice)
        {
        case 1:
        { // Apply Filter
            std::string filter;
            std::cout << "Enter filter type (blur/sharpen/edgeDetect/emboss): ";
            std::cin >> filter;
            result = Graphics::ApplyFilter(inputImage, filter);
            break;
        }
        case 2:
        { // Grayscale
            result = Graphics::MakeGreyScale(inputImage);
            break;
        }
        case 3:
        { // Rotate
            double angle;
            std::cout << "Enter rotation angle in degrees: ";
            std::cin >> angle;
            result = Graphics::RotateImage(inputImage, angle);
            break;
        }
        case 4:
        { // Scale
            double scale;
            std::cout << "Enter scale factor: ";
            std::cin >> scale;
            result = Graphics::ScaleImage(inputImage, scale);
            break;
        }
        case 5:
        { // Translate
            int xOffset, yOffset;
            std::cout << "Enter X offset: ";
            std::cin >> xOffset;
            std::cout << "Enter Y offset: ";
            std::cin >> yOffset;
            result = Graphics::TranslateImage(inputImage, xOffset, yOffset);
            break;
        }
        case 6:
        { // Dithering
            result = Graphics::FloydSteinbergDither(inputImage);
            break;
        }
        case 7:
        { // In-place Emboss
            result = Graphics::InPlaceEmboss(inputImage);
            break;
        }
        default:
        {
            std::cout << "Invalid option selected.\n";
            return 1;
        }
        }

        // Get output path and save
        std::string outputPath;
        std::cout << "Enter path to save modified image: ";
        std::cin >> outputPath;

        result.saveImageToFile(outputPath);
        std::cout << "Image saved to " << outputPath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}