#ifndef IMAGE_H_
#define IMAGE_H_
 
#include <cmath>
 
/*!
 * \class [Image]
 * \brief image pgm class
 *
 * La classe regroupe un ensemble de méthodes utiles pour traiter une image format pgm sans utilisation de lib externe.
 *
 * \author Soufiane RITAL
 * \date 11 10 2012
 */
 
class Image {
 public:
 
    Image();
    Image(int numRows, int numCols, int grayLevels);
    ~Image();
    Image(const Image& oldImage);
    void setImageInfo(int numRows, int numCols, int maxVal);
    void getImageInfo(int &numRows, int &numCols, int &maxVal);
    void operator=(const Image&);
    void rotateImage(int theta, Image& oldImage);
    int getPixelVal(int row, int col);
    void setPixelVal(int row, int col, int value);
    bool inBounds(int row, int col);
    void convolveImage(Image& resImage);
 
 private :
    int N; // number of rows
    int M; // number of columns
    int Q; // number of gray levels
    int **pixelVal;
 
};
#endif /* IMAGE_H_ */
