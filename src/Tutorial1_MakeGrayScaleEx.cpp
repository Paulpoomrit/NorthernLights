#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Pixel.h"

void extractHeaderInformation(std::ifstream &infile, std::ofstream &outfile, std::string &line)
{
    for (int i = 0; i < 3; i++)
    {
        std::getline(infile, line);
        outfile << line << "\n";
    }
}

void getHeightAndWidth(std::ifstream &infile, std::ofstream &outfile, std::string &line, int &height, int &width)
{
    std::istringstream iss(line);
    iss >> width >> height;
    std::cout << width << "*" << height;

    std::getline(infile, line);
    outfile << line << "\n";
}

void populatePixelVector(std::vector<Pixel> &pixelVector, std::ifstream &infile, std::string &line, const int &height, const int &width)
{
    // emplace all Pixels
    for (int i = 0; i < height; i++)
    {
        std::getline(infile, line);
        std::istringstream iss(line);
        for (int j = 0; j < width; j++)
        {
            unsigned int red, green, blue;
            iss >> red >> green >> blue;
            pixelVector.emplace_back(red, green, blue);
        }
    }
}

void makeGrayScale(std::ifstream &infile, std::string newName)
{
    int height, width;
    std::vector<Pixel> pixelVector;
    std::string line;
    std::ofstream outfile("../assets/" + newName);

    extractHeaderInformation(infile, outfile, line);
    getHeightAndWidth(infile, outfile, line, height, width);

    pixelVector.reserve(height * width);
    populatePixelVector(pixelVector, infile, line, height, width);

    // grey-ify all pixels
    std::transform(pixelVector.begin(), pixelVector.end(), pixelVector.begin(),
                   [](Pixel &p)
                   {
                       unsigned int gray = (p["red"] + p["green"] + p["blue"]) / 3;
                       return Pixel(gray, gray, gray);
                   });

    // writes to outfile
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            const Pixel &p = pixelVector[(i * width) + j];
            outfile << p;
            if (j < width - 1)
            {
                outfile << " ";
            }
        }
        outfile << "\n";
    }
}

int main()
{
    std::ifstream infile("../assets/Shahriar.ppm");

    try
    {
        makeGrayScale(infile, "Shahriar_Inverted.ppm");
    }
    catch (const char *s)
    {
        std::cout << s;
    }
    infile.close();

    return 0;
}