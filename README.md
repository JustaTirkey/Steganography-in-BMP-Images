# Steganography in BMP Images

## Description
This project implements a simple steganography technique to hide messages within BMP images using the least significant bit (LSB) method. It allows users to encode and decode text messages within an image without significantly altering its visual appearance.

## Features
- **BMP Image Handling**: The project includes an image processing module for reading and writing BMP images.
- **Steganography Encoding**: Hides messages within the image pixels using LSB of RGB channels.
- **Steganography Decoding**: Extracts hidden messages from an image.
- **Automatic Image Creation**: If no image is provided, a new BMP image is generated to store the message.
- **Debugging Tools**: Option to print the RGB values of the first few pixels for verification.

## File Structure
```
📂 SteganographyProject
├── 📜 image.h            # Header file defining the Color and Image classes
├── 📜 image.cpp          # Implementation of the Image class, handling BMP image creation/export
├── 📜 steno.cpp          # Main file containing message hiding and extraction logic
├── 📜 README.md          # Documentation of the project
```

## Installation & Compilation
### Prerequisites
Ensure you have a C++ compiler that supports C++11 or later (e.g., g++ or MSVC).

### Compilation
Use the following command to compile the project using g++:
```sh
 g++ image.cpp steno.cpp -o steganography
```

### Running the Program
After compiling, run the executable:
```sh
 ./steganography
```

## How It Works
### Encoding (Hiding a Message)
1. Loads an existing BMP image or generates a new one.
2. Converts each character into an 8-bit binary representation.
3. Distributes the bits across three consecutive pixels (using RGB channels).
4. Uses the LSB of the blue channel in the third pixel as a delimiter (1 for continuation, 0 for end of message).
5. Saves the modified image.

### Decoding (Extracting a Message)
1. Reads the modified BMP image.
2. Extracts bits from the LSBs of RGB channels.
3. Reconstructs characters from the extracted bits.
4. Stops when the delimiter bit (0 in blue channel) is encountered.

## Example Output
```
Message hidden successfully in: hidden_image.bmp
Extracted Message: Hello, this is a secret!
```

## Future Improvements
- Support for more image formats (PNG, JPEG, etc.)
- Implement encryption for added security
- Improve efficiency and optimize storage capacity

## License
This project is open-source and available under the MIT License.

