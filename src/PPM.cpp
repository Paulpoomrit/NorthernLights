#include "PPM.h"
#include "Pixel.h"
#include <vector>
#include <fstream>
#include <string.h>
#include <utility>
#include <string>

// ================= Destructor =================
PPM::~PPM() {}

// ================= Getters =================
std::string PPM::getComment() const
{
    return comment;
}

unsigned int PPM::getHeight() const
{
    return height;
}

std::string PPM::getMagic() const
{
    return magic;
}

unsigned int PPM::getMaxColor() const
{
    return maxColor;
}

unsigned int PPM::getSize() const
{
    return height * width;
}

unsigned int PPM::getWidth() const
{
    return width;
}

const Pixel &PPM::operator[](unsigned int index) const
{
    return pixels[index];
}

Pixel &PPM::operator[](unsigned int index)
{
    return pixels[index];
}

const std::vector<Pixel> &PPM::getPixels() const
{
    return this->pixels;
}

// ================= Constructors =================
const PPM &PPM::operator=(const PPM &otherPPM)
{
    if (this != &otherPPM)
    {
        magic = otherPPM.magic;
        comment = otherPPM.comment;
        maxColor = otherPPM.maxColor;
        height = otherPPM.height;
        width = otherPPM.width;
        pixels = otherPPM.pixels;
    }

    return *this;
}

const PPM &PPM::operator=(PPM &&otherPPM)
{
    if (this != &otherPPM)
    {
        magic = std::move(otherPPM.magic);
        comment = std::move(otherPPM.comment);
        maxColor = otherPPM.maxColor;
        height = otherPPM.height;
        width = otherPPM.width;
        pixels = std::move(otherPPM.pixels);
    }

    return *this;
}

PPM::PPM() : magic(),
             comment(),
             maxColor(0),
             height(0),
             width(0),
             pixels()
{
}

PPM::PPM(const PPM &otherPPM) : magic(otherPPM.magic),
                                comment(otherPPM.comment),
                                maxColor(otherPPM.maxColor),
                                height(otherPPM.height),
                                width(otherPPM.width),
                                pixels(otherPPM.pixels)
{
}

PPM::PPM(PPM &&otherPPM) : magic(std::move(otherPPM.magic)),
                           comment(std::move(otherPPM.comment)),
                           maxColor(otherPPM.maxColor),
                           height(otherPPM.height),
                           width(otherPPM.width),
                           pixels(std::move(otherPPM.pixels))
{
}

PPM::PPM(std::ifstream &file)
{
    // Magic number
    std::getline(file, magic);
    // Comment
    if (file.peek() == '#')
    {
        std::getline(file, comment);
        comment.erase(0, 1); // Remove leading '#'
    }
    // Dimensions
    file >> width >> height;
    // Max color
    file >> maxColor;
    // Pixels
    unsigned int r, g, b;
    for (int i = 0; i < width * height; i++)
    {
        file >> r >> g >> b;
        pixels.push_back(Pixel(r, g, b));
    }
}

// ================= Setters =================
void PPM::setComment(std::string newComment)
{
    comment = newComment;
}

void PPM::setMagic(std::string newMagic)
{
    magic = newMagic;
}

void PPM::setMaxColor(unsigned int newMaxColor)
{
    maxColor = newMaxColor;
}

void PPM::setWidth(unsigned int newWidth)
{
    width = newWidth;
}

void PPM::setHeight(unsigned int newHeight)
{
    height = newHeight;
}

// Resize the pixels vector
void PPM::resize(unsigned int newLength)
{
    pixels.resize(newLength);
}

void PPM::setPixels(const std::vector<Pixel> &newPixels)
{
    this->pixels = newPixels;
}

// ================= Output =================
void PPM::saveImageToFile(std::string outfileName)
{
    std::ofstream outfile(outfileName);

    // Magic number
    outfile << magic << '\n';
    // Comment
    if (!comment.empty())
        outfile << "#" << comment << '\n';
    // Dimensions
    outfile << width << ' ' << height << '\n';
    // Max color
    outfile << maxColor << '\n';
    // Pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Pixel &p = pixels[i * width + j];
            outfile << p["red"] << ' ' << p["green"] << ' ' << p["blue"] << ' ';
        }
        outfile << '\n';
    }
    std::cout << "done save image to file";
    outfile.close();
}
