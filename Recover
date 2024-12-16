//C++ version of cs50x recover problem set
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <iomanip>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Missing Command Line arguments" << "\n";
        return 1;
    }

    uint8_t buffer[512];
    std::ifstream input(argv[1], std::ios::binary);
    std::ofstream output;
    int file_count = 0;

    if (!input) {
        std::cerr << "Could not open file" << "\n";
        return 2;
    }

    while (input.read(reinterpret_cast<char*>(buffer), sizeof(buffer))) {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            if (output.is_open()) {
                output.close();
            }


            std::stringstream ss;
            ss << std::setw(3) << std::setfill('0') << file_count << ".jpg";
            std::string filename = ss.str();


            output.open(filename, std::ios::binary);
            if (!output) {
                std::cerr << "Could not open output file: " << filename << "\n";
                return 3;
            }

            file_count++;
        }


        if (output.is_open()) {
            output.write(reinterpret_cast<char*>(buffer), sizeof(buffer));
        }
    }


    if (output.is_open()) {
        output.close();
    }

    return 0;
}
