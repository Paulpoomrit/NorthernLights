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

public:
    const std::string& getComment() const { return comment; }
    void setComment(const std::string& newComment) { comment = newComment; }

    unsigned int getHeight() const { return height; }
    void setHeight(unsigned int newHeight) { height = newHeight; }

    const std::string& getMagic() const { return magic; }
    void setMagic(const std::string& newMagic) { magic = newMagic; }

    unsigned int getMaxColor() const { return maxColor; }
    void setMaxColor(unsigned int newMaxColor) { maxColor = newMaxColor; }

    const std::vector<Pixel>& getPixels() const { return pixels; }
    void setPixels(const std::vector<Pixel>& newPixels) { pixels = newPixels; }

    unsigned int getWidth() const { return width; }
    void setWidth(unsigned int newWidth) { width = newWidth; }
};