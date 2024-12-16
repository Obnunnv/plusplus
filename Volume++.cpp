//C++ rendition of cs50x volume problem set
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <fstream>

int main(int argc, char** argv) {
    const int HEADER_SIZE = 44;
    if (argc != 4) {
        std::cerr << "Missing Command Line Arguments" << "\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if(!input) {
        std::cerr << "Could not open input file" << "\n";
        return 2;
    }

    std::ofstream output(argv[2], std::ios::binary);
    if (!output) {
        std::cerr << "Could not open output file" << "\n";
        input.close();
        return 3;
    }

    float factor = atof(argv[3]);
    char header[HEADER_SIZE];
    input.read(reinterpret_cast<char*>(header), sizeof(header));
    output.write(reinterpret_cast<char*>(header), sizeof(header));

    int16_t sample;
    while (input.read(reinterpret_cast<char*>(sample), sizeof(sample))) {
        sample = (int16_t)(sample * factor);
        if (sample > INT16_MAX) {
            sample = INT16_MAX * factor;
        }
        else if (sample < INT16_MIN) {
            sample = INT16_MIN * factor;
        }
        output.write(reinterpret_cast<char*>(sample), sizeof(sample));
    }

    if (input.is_open()) {
        input.close();
    }
    if (output.is_open()) {
        output.close();
    }
}
