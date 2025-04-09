#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <exception>

class Pixel
{
public:
    class InputOutOfBoundsException
    {
    public:
        InputOutOfBoundsException(const char *, const char *);
        const char *returnError() const;
        const char *returnOffendingIndex() const;

    private:
        const char *errorMessage;
        const char *offendingIndex;
    };

    ~Pixel();
    const unsigned int &operator[](const char *) const;
    unsigned int &operator[](const char *);
    Pixel();
    Pixel(const Pixel &);
    Pixel(unsigned int, unsigned int, unsigned int);

private:
    unsigned int blue;
    unsigned int green;
    unsigned int red;
};

#endif
