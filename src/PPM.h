#ifndef PPM_H
#define PPM_H

#include "Pixel.h"
#include <vector>
#include <fstream>
#include <string.h>

class PPM {
public:
    // Destructor
    ~PPM();
    // Getters
    std::string getComment() const;
    unsigned int getHeight() const;
    std::string getMagic() const;
    unsigned int getMaxColor() const;
    unsigned int getSize() const;
    unsigned int getWidth() const;
    const Pixel & operator[](unsigned int) const;
    Pixel & operator[](unsigned int);
    // Constructors
    const PPM & operator=(const PPM &);
    const PPM & operator=(PPM &&);
    PPM();
    PPM(const PPM &);
    PPM(PPM &&);
    PPM(std::ifstream &);
    // Setters
    void setComment(std::string);
    void setMagic(std::string);
    void setMaxColor(unsigned int);
    void setWidth(unsigned int);
    void setHeight(unsigned int);
    void resize(unsigned int);
    // Output
    void saveImageToFile(std::string);
private:
    std::string magic;
    std::string comment;
    unsigned int maxColor;
    unsigned int height;
    unsigned int width;
    std::vector<Pixel> pixels;
};

#endif