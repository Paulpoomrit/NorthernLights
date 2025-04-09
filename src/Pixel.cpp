#include "Pixel.h"
#include <cstring>

Pixel::InputOutOfBoundsException::InputOutOfBoundsException(const char *message, const char *index) : errorMessage(message),
                                                                                                      offendingIndex(index)
{
}

const char *Pixel::InputOutOfBoundsException::returnError() const
{
    return errorMessage;
}

const char *Pixel::InputOutOfBoundsException::returnOffendingIndex() const
{
    return offendingIndex;
}

Pixel::~Pixel() {}

const unsigned int &Pixel::operator[](const char *key) const
{
    if (strcmp(key, "red") == 0)
        return this->red;
    else if (strcmp(key, "green") == 0)
        return this->green;
    else if (strcmp(key, "blue") == 0)
        return this->blue;
    else
        throw InputOutOfBoundsException("Key is not a valid color. Try: \"red\", \"green\", or \"blue\".", key);
}

unsigned int &Pixel::operator[](const char *key)
{
    if (strcmp(key, "red") == 0)
        return this->red;
    else if (strcmp(key, "green") == 0)
        return this->green;
    else if (strcmp(key, "blue") == 0)
        return this->blue;
    else
        throw InputOutOfBoundsException("Key is not a valid color. Try: \"red\", \"green\", or \"blue\".", key);
}

Pixel::Pixel() : red(0),
                 green(0),
                 blue(0) {};

Pixel::Pixel(const Pixel &otherPixel) : red(otherPixel.red),
                                        green(otherPixel.green),
                                        blue(otherPixel.blue) {};

Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b) : red(r),
                                                               green(g),
                                                               blue(b) {};
