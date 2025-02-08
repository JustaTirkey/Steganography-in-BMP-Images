#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

class Color {
public:
    int r, g, b;

    Color();  // Default constructor
    Color(int r, int g, int b);  // Constructor with RGB values
    ~Color();  // Destructor
};

class Image {
private:
    int m_width, m_height;
    std::vector<Color> m_color;  // A 2D array of Color objects

public:
    Image(int width, int height);
    ~Image();

    Color GetColor(int x, int y) const;  // Get the color of a pixel at (x, y)
    int GetHeight() const;
    int GetWidth() const;

    void SetColor(const Color& color, int x, int y);  // Set the color of a pixel at (x, y)
    void Export(const char* path) const;  // Export the image to a BMP file
};

#endif // IMAGE_H
