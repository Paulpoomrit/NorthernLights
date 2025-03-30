#include <string>
#include <vector>
#include <Pixel.h>

class PPM {
private:
    std::string comment;
    unsigned int height;
    std::string magic;
    unsigned int maxColor;
    std::vector<Pixel> pixels;
    unsigned int width;
};