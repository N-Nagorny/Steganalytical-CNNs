#include "Image.h"
#include <stdio.h> 
/**
 * Creates an Image 0x0
 */
Image::Image(){
    N = 0;
    M = 0;
    Q = 0;
    pixelVal = 0;
}
 
/**
 * Creates an Image of numRows x numCols and creates the arrays for it
 *
 *
 * \param numRows number of rows
 * \param numCols number of cols
 * \param grayLevels grey level.
 * \author Florence Tupin
 * \date 10 oct. 2012
 * \sa Image()
 */
Image::Image(int numRows, int numCols, int grayLevels)
{
    N = numRows;
    M = numCols;
    Q = grayLevels;
    pixelVal = new int *[N];
    for(int i = 0; i < N; i++)
    {
        pixelVal[i] = new int [M];
        for(int j = 0; j < M; j++)
            pixelVal[i][j] = 0;
    }
}
 
/**
 * destroy image
 */
Image::~Image()
{
    N = 0;
    M = 0;
    Q = 0;
    for(int i = 0; i < N; i++)
        delete pixelVal [N];
    delete pixelVal;
    pixelVal = 0;
}
 
/**
 * copies oldImage into new Image object
 *
 * @param oldImage object Image to copie.
 *
 */
Image::Image(const Image& oldImage)
{
    N = oldImage.N;
    M = oldImage.M;
    Q = oldImage.Q;
 
    pixelVal = new int* [N];
    for(int i = 0; i < N; i++)
    {
        pixelVal[i] = new int [M];
        for(int j = 0; j < M; j++)
            pixelVal[i][j] = oldImage.pixelVal[i][j];
    }
}
 
/**
 * copies oldImage into whatever you = it to
 *
 * @param oldImage object Image
 */
void Image::operator=(const Image& oldImage)
{
    N = oldImage.N;
    M = oldImage.M;
    Q = oldImage.Q;
 
    pixelVal = new int* [N];
    for(int i = 0; i < N; i++)
    {
        pixelVal[i] = new int [M];
        for(int j = 0; j < M; j++)
            pixelVal[i][j] = oldImage.pixelVal[i][j];
    }
}
 
/**
 * sets the number of rows, columns and graylevels
 *
 */
void Image::setImageInfo(int numRows, int numCols, int maxVal)
{
    N = numRows;
    M = numCols;
    Q = maxVal;
}
 
/**
 * returns the number of rows, columns and gray levels
 */
void Image::getImageInfo(int &numRows, int &numCols, int &maxVal)
{
    numRows = N;
    numCols = M;
    maxVal = Q;
}
 
/**
 * returns the gray value of a specific pixel
 */
int Image::getPixelVal(int row, int col)
{
    return pixelVal[row][col];
}
 
/**
 * sets the gray value of a specific pixel
 */
void Image::setPixelVal(int row, int col, int value)
{
    pixelVal[row][col] = value;
}
 
/**
 * checks to see if a pixel is within the image, returns true or false
 */
bool Image::inBounds(int row, int col)
{
    if(row >= N || row < 0 || col >=M || col < 0)
        return false;
    //else
    return true;
}
 
/**
 * convolves image
 */
void Image::convolveImage(Image& resImage) {
    int rows, cols, gray;
    rows = N;
    cols = M;
    gray = Q;
    float kernel[5*5] = {(float)-1/(float)12,  (float)2/(float)12,  (float)-2/(float)12,  (float)2/(float)12, (float)-1/(float)12,
                          (float)2/(float)12, (float)-6/(float)12,   (float)8/(float)12, (float)-6/(float)12,  (float)2/(float)12,
                         (float)-2/(float)12,  (float)8/(float)12, (float)-12/(float)12,  (float)8/(float)12, (float)-2/(float)12,
                          (float)2/(float)12, (float)-6/(float)12,   (float)8/(float)12, (float)-6/(float)12, (float) 2/(float)12,
                         (float)-1/(float)12,  (float)2/(float)12,  (float)-2/(float)12, (float) 2/(float)12, (float)-1/(float)12};

    int pad = 2;
    float *image = (float *) new float [(rows- 2*pad)* (cols- 2*pad)];
    int val;
    for(int i=pad; i<rows - pad; i++)
    {
        for(int j=pad; j<cols - pad; j++)
        {
            float weighted_pixel_sum = 0;
            for(int ky=0; ky<5; ky++){
                for(int kx =0; kx<5; kx++){

                    weighted_pixel_sum += (float)getPixelVal(i - pad + ky, j - pad + kx) * kernel[ky * 5 + kx];}}
            image[(i - pad) * (cols- 2*pad) + j - pad] = weighted_pixel_sum;
//            printf("%i,%i\n",i - pad,j - pad);	
        }
    }

    for (int i = 0; i<(rows- 2*pad)* (cols- 2*pad); i++) {
        if (image[i] < 0)
            image[i] = 0;
        else if (image[i] > 255)
            image[i] = 255;
    }
    Image tempImage(N- 2*pad,M- 2*pad,Q);
 
    for(int i = 0; i < rows- 2*pad; i++)
    {
        for(int j = 0; j < cols- 2*pad; j++)
            tempImage.pixelVal[i][j] = (int)image[i*(M- 2*pad) + j];
    }
 
    resImage = tempImage;
}
 
/**
 * based on users input and rotates it around the center of the image
 */
void Image::rotateImage(int theta, Image& oldImage)
    {
        int r0, c0;
        int r1, c1;
        int rows, cols;
        rows = oldImage.N;
        cols = oldImage.M;
        Image tempImage(rows, cols, oldImage.Q);
 
        float rads = (theta * 3.14159265)/180.0;
 
        r0 = rows / 2;
        c0 = cols / 2;
 
        for(int r = 0; r < rows; r++)
        {
        for(int c = 0; c < cols; c++)
        {
            r1 = (int) (r0 + ((r - r0) * cos(rads)) - ((c - c0) * sin(rads)));
            c1 = (int) (c0 + ((r - r0) * sin(rads)) + ((c - c0) * cos(rads)));
 
            if(inBounds(r1,c1))
            {
                tempImage.pixelVal[r1][c1] = oldImage.pixelVal[r][c];
            }
        }
        }
 
        for(int i = 0; i < rows; i++)
        {
        for(int j = 0; j < cols; j++)
        {
            if(tempImage.pixelVal[i][j] == 0)
                tempImage.pixelVal[i][j] = tempImage.pixelVal[i][j+1];
        }
        }
        oldImage = tempImage;
    }
