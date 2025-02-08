#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <chrono>
#include <thread>
#include "image.h"

void printFirst6Pixels(const std::string& imagePath) {
    std::ifstream inFile(imagePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Skip BMP header (54 bytes)
    inFile.seekg(54);

    // Read first 6 pixels (each pixel is 3 bytes: B, G, R)
    std::vector<unsigned char> pixelData(6 * 3);  
    inFile.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

    // Print RGB values
    std::cout << "First 6 Pixels RGB values:" << std::endl;
    for (int i = 0; i < 6; i++) {
        unsigned char B = pixelData[i * 3];       // Blue
        unsigned char G = pixelData[i * 3 + 1];   // Green
        unsigned char R = pixelData[i * 3 + 2];   // Red
        std::cout << "Pixel " << i + 1 << " -> R: " << (int)R << " G: " << (int)G << " B: " << (int)B << std::endl;
    }
}

void hideMessage(const std::string& inputImage, const std::string& outputImage, const std::string& message) {
    std::ifstream inFile(inputImage, std::ios::binary);
    std::ofstream outFile(outputImage, std::ios::binary);

    if (!inFile || !outFile) {
        std::cerr << "\n\t\tImage File does not exist. Creating new Image" << std::endl;
        // Create an image with width 100 and height 100
        int width = 100;
        int height = 100;
        Image img(width, height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                img.SetColor(Color(0.5f + x*2 , 0.0f + y+9, 1.0f + 8), x, y);  
            }
        }

        img.Export("image.bmp");
        std::ifstream inFile(inputImage, std::ios::binary);
        std::ofstream outFile(outputImage, std::ios::binary);
        hideMessage(inputImage, outputImage, message);
        return;
    }

    // Read BMP header
    std::vector<unsigned char> header(54);
    inFile.read(reinterpret_cast<char*>(header.data()), 54);
    outFile.write(reinterpret_cast<char*>(header.data()), 54);

    // Read BMP pixel data
    std::vector<unsigned char> imageData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

    int msgIndex = 0;
    int msgSize = message.size();

    for (int i = 0; i < msgSize; i++) {
        char c = message[i];

        // Encode into three consecutive pixels
        int pixelIndex = msgIndex * 9;  // 3 pixels per char, each pixel has 3 channels

        if (pixelIndex + 8 >= imageData.size()) {
            std::cerr << "Image too small for message!" << std::endl;
            return;
        }
        // else{
        //     // std::cout<<"Message max size : "<< imageData.size()<<"\n";
        // }

        // First pixel stores bits 0-2
        imageData[pixelIndex]     = (imageData[pixelIndex]     & 0xFE) | ((c >> 7) & 1);  // R
        imageData[pixelIndex + 1] = (imageData[pixelIndex + 1] & 0xFE) | ((c >> 6) & 1);  // G
        imageData[pixelIndex + 2] = (imageData[pixelIndex + 2] & 0xFE) | ((c >> 5) & 1);  // B

        // Second pixel stores bits 3-5
        imageData[pixelIndex + 3] = (imageData[pixelIndex + 3] & 0xFE) | ((c >> 4) & 1);  // R
        imageData[pixelIndex + 4] = (imageData[pixelIndex + 4] & 0xFE) | ((c >> 3) & 1);  // G
        imageData[pixelIndex + 5] = (imageData[pixelIndex + 5] & 0xFE) | ((c >> 2) & 1);  // B

        // Third pixel stores bits 6-7 and delimiter
        imageData[pixelIndex + 6] = (imageData[pixelIndex + 6] & 0xFE) | ((c >> 1) & 1);  // R
        imageData[pixelIndex + 7] = (imageData[pixelIndex + 7] & 0xFE) | ((c >> 0) & 1);  // G
        imageData[pixelIndex + 8] = (imageData[pixelIndex + 8] & 0xFE) | (i < msgSize - 1 ? 1 : 0);  // B (delimiter)

        msgIndex++;
    }

    // Write modified data
    outFile.write(reinterpret_cast<char*>(imageData.data()), imageData.size());

    std::cout << "Message hidden successfully in: " << outputImage << std::endl;
}


void extractMessage(const std::string& imagePath) {
    std::ifstream inFile(imagePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // Skip BMP header
    inFile.seekg(54);

    std::vector<unsigned char> imageData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

    std::string message;
    char currentChar = 0;
    int bitIndex = 0;

    for (size_t i = 0; i < imageData.size(); i += 9) {  // Process in 3-pixel groups
        currentChar = 0;

        // Read first pixel
        currentChar |= ((imageData[i]     & 1) << 7);  // R
        currentChar |= ((imageData[i + 1] & 1) << 6);  // G
        currentChar |= ((imageData[i + 2] & 1) << 5);  // B

        // Read second pixel
        currentChar |= ((imageData[i + 3] & 1) << 4);  // R
        currentChar |= ((imageData[i + 4] & 1) << 3);  // G
        currentChar |= ((imageData[i + 5] & 1) << 2);  // B

        // Read third pixel
        currentChar |= ((imageData[i + 6] & 1) << 1);  // R
        currentChar |= ((imageData[i + 7] & 1) << 0);  // G

        message += currentChar;

        // Check delimiter (B channel of third pixel)
        if ((imageData[i + 8] & 1) == 0) {
            break;
        }
    }

    std::cout << "Extracted Message: \t\t" << message << std::endl;
}

int main() {
    // std::string message = "Hello";  // Secret message
    std::string message = "hello justa tirkey!! how are you? great to see you. I missed you so much. See you guys later. Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle. I'm sorry, I'm having trouble responding to requests right now. Let's try this again in a bit.Hope this helps. The largest message of all time I am storing how the hell It can handle.  ";

    // for (char c : message) {
    //     std::bitset<8> bits(c);
    //     std::cout << bits << " ";
    // }

    // printFirst6Pixels("image.bmp");
    hideMessage("image.bmp", "hidden_image.bmp", message);
    extractMessage("hidden_image.bmp");
    // printFirst6Pixels("hidden_image.bmp");
    return 0;
}
