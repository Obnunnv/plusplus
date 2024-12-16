//C++ version of cs50x Filter more problem set
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <getopt.h>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>

#pragma pack(push, 1)

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint16_t WORD;

typedef struct
{
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} RGBTRIPLE;

#pragma pack(pop)

void grayscale(int height, int width, std::vector<std::vector<RGBTRIPLE>>& image)
{
    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            int average = round((image[i][k].rgbtBlue + image[i][k].rgbtGreen + image[i][k].rgbtRed) / 3.0);
            image[i][k].rgbtBlue = average;
            image[i][k].rgbtGreen = average;
            image[i][k].rgbtRed = average;
        }
    }
}

void reflect(int height, int width, std::vector<std::vector<RGBTRIPLE>>& image)
{
    for (int bruh = 0; bruh < height; bruh++) {
        for (int ring = 0; ring < width / 2; ring++) {
            RGBTRIPLE temp = image[bruh][ring];
            image[bruh][ring] = image[bruh][width - 1 - ring];
            image[bruh][width - 1 - ring] = temp;
        }
    }
}

void blur(int height, int width, std::vector<std::vector<RGBTRIPLE>>& image)
{
    RGBTRIPLE temp[height][width];

    for (int tint = 0; tint < height; tint++) {
        for (int kick = 0; kick < width; kick++) {
            int redTotal = 0, greenTotal = 0, blueTotal = 0;
            int count = 0;

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int newTint = tint + i;
                    int newKick = kick + j;

                    if (newTint >= 0 && newTint < height && newKick >= 0 && newKick < width) {
                        redTotal += image[newTint][newKick].rgbtRed;
                        greenTotal += image[newTint][newKick].rgbtGreen;
                        blueTotal += image[newTint][newKick].rgbtBlue;
                        count++;
                    }
                }
            }

            temp[tint][kick].rgbtRed = round((float) redTotal / count);
            temp[tint][kick].rgbtGreen = round((float) greenTotal / count);
            temp[tint][kick].rgbtBlue = round((float) blueTotal / count);
        }
    }

    for (int tint = 0; tint < height; tint++) {
        for (int kick = 0; kick < width; kick++) {
            image[tint][kick] = temp[tint][kick];
        }
    }
}

void edges(int height, int width, std::vector<std::vector<RGBTRIPLE>>& image)
{
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int redX = 0, greenX = 0, blueX = 0;
            int redY = 0, greenY = 0, blueY = 0;

            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int newI = i + di;
                    int newJ = j + dj;

                    if (newI >= 0 && newI < height && newJ >= 0 && newJ < width) {
                        redX += image[newI][newJ].rgbtRed * Gx[di + 1][dj + 1];
                        greenX += image[newI][newJ].rgbtGreen * Gx[di + 1][dj + 1];
                        blueX += image[newI][newJ].rgbtBlue * Gx[di + 1][dj + 1];

                        redY += image[newI][newJ].rgbtRed * Gy[di + 1][dj + 1];
                        greenY += image[newI][newJ].rgbtGreen * Gy[di + 1][dj + 1];
                        blueY += image[newI][newJ].rgbtBlue * Gy[di + 1][dj + 1];
                    }
                }
            }

            int red = round(sqrt(redX * redX + redY * redY));
            int green = round(sqrt(greenX * greenX + greenY * greenY));
            int blue = round(sqrt(blueX * blueX + blueY * blueY));

            temp[i][j].rgbtRed = (red > 255) ? 255 : red;
            temp[i][j].rgbtGreen = (green > 255) ? 255 : green;
            temp[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = temp[i][j];
        }
    }
}

int main(int argc, char** argv) {
    const char *filters = "begr";

    char filter = getopt(argc, argv, filters);
    if (filter == '?') {
        std::cerr << "Invalid filter" << "\n";
        return 1;
    }

    if (getopt(argc, argv, filters) != -1) {
        std::cerr << "Only one filter allowed" << "\n";
        return 2;
    }

    if (argc != optind + 2) {
        std::cerr << "Usage: ./filter [flag] infile outfile" << "\n";
        return 3;
    }

    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    std::ifstream inptr(infile);
    if (!inptr) {
        std::cerr << "Could not open " << infile << "\n";
        return 4;
    }

    std::ofstream outptr(outfile);
    if (!outptr) {
        std::cerr << "Could not create " << outfile << "\n";
        return 5;
    }

    BITMAPFILEHEADER bf;
    inptr.read(reinterpret_cast<char*>(&bf), sizeof(bf));

    BITMAPINFOHEADER bi;
    inptr.read(reinterpret_cast<char*>(&bi), sizeof(bi));

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0) {
        inptr.close();
        outptr.close();
        std::cerr << "Unsupported file format" << "\n";
        return 6;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    std::vector<std::vector<RGBTRIPLE>> image(height, std::vector<RGBTRIPLE>(width));

    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0; i < height; i++) {
        inptr.read(reinterpret_cast<char*>(&image[i][0]), width * sizeof(RGBTRIPLE));
        inptr.seekg(padding, std::ios::cur);
    }

    switch (filter) {
        case 'b':
            blur(height, width, image);
            break;
        case 'e':
            edges(height, width, image);
            break;
        case 'g':
            grayscale(height, width, image);
            break;
        case 'r':
            reflect(height, width, image);
            break;
    }

    outptr.write(reinterpret_cast<char*>(&bf), sizeof(BITMAPFILEHEADER));
    outptr.write(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));

    for (int i = 0; i < height; i++) {
        outptr.write(reinterpret_cast<char*>(&image[i][0]), sizeof(RGBTRIPLE) * width);
        for (int k = 0; k < padding; k++) {
            outptr.put(static_cast<char>(0x00));
        }
    }

    inptr.close();
    outptr.close();

    return 0;
}
