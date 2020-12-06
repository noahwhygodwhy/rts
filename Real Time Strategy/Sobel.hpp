#ifndef SOBEL_H
#define SOBEL_H

#include <cmath>
#include <algorithm>

using namespace std;

float sobelX[3][3] = { {1.0, 0.0, -1.0},{2.0, 0.0, -2.0}, {1.0, 0.0, -1.0} };
float sobelY[3][3] = { {1.0, 2.0, 1.0},{0.0, 0.0, 0.0}, {-1.0, -2.0, -1.0} };

double sobel(float sobelMat[3][3], unsigned char* imageData, int width, int height, int nrChannels, int pixelOffset, int colorOffset)
{
    int sum = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int yAdjust = (i - 1) * (width * nrChannels);
            int xAdjust = (j - 1) * (nrChannels);
            sum += sobelMat[i][j] * *(imageData + pixelOffset + yAdjust + xAdjust + colorOffset);
        }
    }
    return (double)sum;
}
unsigned char* makeSobalImage(unsigned char* originalImage, int width, int height, int nrChannels)
{
    unsigned char* sobelImage = new unsigned char[(width * height * nrChannels) + 1];

    int sumX = 0;
    int sumY = 0;
    int sum;
    int maxSobalVal = 0;


    for (int i = 0; i < width * height; i++)
    {
        int pixelOffset = i * nrChannels;
        int y = i / width;
        int x = i % height;


        if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
        {
            *(sobelImage + pixelOffset + 0) = 0;
            *(sobelImage + pixelOffset + 1) = 0;
            *(sobelImage + pixelOffset + 2) = 0;
            *(sobelImage + pixelOffset + 3) = 255;
        }
        else
        {
            double rsx = sobel(sobelX, originalImage, width, height, nrChannels, pixelOffset, 0);
            double rsy = sobel(sobelY, originalImage, width, height, nrChannels, pixelOffset, 0);
            double rrealSum = sqrt((rsx * rsx) + (rsy * rsy));

            double gsx = sobel(sobelX, originalImage, width, height, nrChannels, pixelOffset, 1);
            double gsy = sobel(sobelY, originalImage, width, height, nrChannels, pixelOffset, 1);
            double grealSum = sqrt((gsx * gsx) + (gsy * gsy));

            double bsx = sobel(sobelX, originalImage, width, height, nrChannels, pixelOffset, 2);
            double bsy = sobel(sobelY, originalImage, width, height, nrChannels, pixelOffset, 2);
            double brealSum = sqrt((bsx * bsx) + (bsy * bsy));

            int avgSum = (int)((rrealSum + grealSum + brealSum) / 765.0f);

            maxSobalVal = std::max(maxSobalVal, avgSum);

            for (int colorOffset = 0; colorOffset < 3; colorOffset++)
            {
                *(sobelImage + pixelOffset + colorOffset) = avgSum;
            }
            *(sobelImage + pixelOffset + 3) = 255;

        }
    }

    for (int i = 0; i < width * height; i++)
    {
        int pixelOffset = i * nrChannels;
        int y = i / width;
        int x = i % height;
        for (int j = 0; j < 3; j++)
        {
            *(sobelImage + pixelOffset + j) = *(sobelImage + pixelOffset + j) / maxSobalVal * 255;
        }

    }

    return sobelImage;
}
#endif