#include "Pixel.h"

Pixel::~Pixel()
{
    // std::cout << "The pixel object is being destroyed";
}

const unsigned int &Pixel::operator[](const char *key)
{
    if (strcmp(key, "red") == 0)
    {
        return this->red;
    }
    else if (strcmp(key, "green") == 0)
    {
        return this->green;
    }
    else if (strcmp(key, "blue") == 0)
    {
        return this->blue;
    }
    else
    {
        throw std::runtime_error("Invalid Argument. Try: “red” “green” or “blue”.");
    }
}

Pixel::Pixel() : red(0), green(0), blue(0) {};

Pixel::Pixel(const Pixel &arg)
    : red(arg.red),
      green(arg.green),
      blue(arg.blue) {};

Pixel::Pixel(unsigned int redVal, unsigned int greenVal, unsigned int blueVal)
    : red(redVal),
      green(greenVal),
      blue(blueVal) {};

std::ostream &operator<<(std::ostream &os, const Pixel &px)
{
    os << px.red << " " << px.green << " " << px.blue;
    return os;
}