#include <iostream>
#include "image.h"

int main() {
    // Create an image with width 100 and height 100
    int width = 100;
    int height = 100;
    Image img(width, height);

    // Set pixel colors (you can customize this part to set colors for the whole image)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Set the color (r, g, b) for each pixel
            img.SetColor(Color(0.5f + x*2 , 0.0f + y+9, 1.0f + 8), x, y);  // Example color (purple)
        }
    }

    // Export the image to a file
    img.Export("image.bmp");

    return 0;
}
