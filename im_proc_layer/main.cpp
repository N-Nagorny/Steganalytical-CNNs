#include "Image.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
 
using namespace std;
 
int readPGM(char[], Image&);
int readPPM(char[], Image&, Image&, Image&);
int writePPM(char[], Image&);
int writePPM(char[], Image&, Image&, Image&);
int readImageHeader(char[], int&, int&, int&, bool&);
 
int main( int argc, char *argv[])
{
 
    if (argc < 4){
        std::cout << "usage main task in.pgm out.pgm" << std::endl;
        return 1;
    }
 
    int M, N, Q; // rows, cols, grayscale
    bool type;
 
    // read image header
    readImageHeader(argv[2], N, M, Q, type);
    if (type == false) {
        Image image(N, M, Q);
        readPGM(argv[2], image);
        if (std::string(argv[1]).compare("convert"))
            image.convolveImage(image); 
        writePPM(argv[3], image);
    }
    else {
        Image red(N, M, Q), green(N, M, Q), blue(N, M, Q);
        readPPM(argv[2], red, green, blue);
        if (std::string(argv[1]).compare("convert")) {
            red.convolveImage(red);
            green.convolveImage(green);
            blue.convolveImage(blue);
        }
        writePPM(argv[3], red, green, blue);
    }
    std::cout << argv[1] << ' ' << argv[2] << " > " << argv[3] << std::endl;
    return 0;
}
 
int readPGM(char fname[], Image& image)
{
    int i, j;
    int N = 0;
    int M = 0;
    int Q = 0;
    unsigned char *charImage;
    char header [100], *ptr;
    ifstream ifp;
 
    ifp.open(fname, ios::in | ios::binary);
 
    if (!ifp)
    {
        cout << "Can't read image: " << fname << endl;
        exit(1);
    }
 
    // read header
    ifp.getline(header,100,'\n');
    if ( (header[0]!=80) || (header[1]!=53) )
    {
        cout << "Image " << fname << " is not PGM" << endl;
        exit(1);
    }

    M=strtol(header+2,&ptr,0);
    N=strtol(ptr, &ptr, 0);
    Q=strtol(ptr,&ptr,0);

    if (M == 0 & N == 0 & Q == 0) { 
        ifp.getline(header,100,'\n');
        while(header[0]=='#')
            ifp.getline(header,100,'\n');
 
        M=strtol(header,&ptr,0);
        N=strtol(ptr, &ptr, 0);
 
        ifp.getline(header,100,'\n');
        Q=strtol(header,&ptr,0);
    }
    charImage = (unsigned char *) new unsigned char [M*N];
 
    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

    if (ifp.fail())
    {
        cout << "Image " << fname << " has wrong size" << endl;
        exit(1);
    }
 
    ifp.close();
 
    int val;
    for(i=0; i<N; i++)
        for(j=0; j<M; j++)
        {
            val = (int)charImage[i*M+j];
            image.setPixelVal(i, j, val);
        }
 
    delete [] charImage;
    return (1);
}

int readPPM(char fname[], Image& red, Image& green, Image& blue)
{
    int i, j;
    int N = 0;
    int M = 0;
    int Q = 0;
    unsigned char *charImage;
    char header [100], *ptr;
    ifstream ifp;
 
    ifp.open(fname, ios::in | ios::binary);
 
    if (!ifp)
    {
        cout << "Can't read image: " << fname << endl;
        exit(1);
    }
 
    // read header
    ifp.getline(header,100,'\n');

    if ( (header[0]!=80) || (header[1]!=54) )
    {
        cout << "Image " << fname << " is not PPM" << endl;
        exit(1);
    }

    M=strtol(header+2,&ptr,0);
    N=strtol(ptr, &ptr, 0);
    Q=strtol(ptr,&ptr,0);

    if (M == 0 & N == 0 & Q == 0) { 
        ifp.getline(header,100,'\n');
        while(header[0]=='#')
            ifp.getline(header,100,'\n');
 
        M=strtol(header,&ptr,0);
        N=strtol(ptr, &ptr, 0);
 
        ifp.getline(header,100,'\n');
        Q=strtol(header,&ptr,0);
    }
    charImage = (unsigned char *) new unsigned char [3*M*N];
 
    ifp.read( reinterpret_cast<char *>(charImage), 3*(M*N)*sizeof(unsigned char));

    if (ifp.fail())
    {
        cout << "Image " << fname << " has wrong size" << endl;
        exit(1);
    }
 
    ifp.close();
 
    int val;
    for(i=0; i<N; i++)
        for(j=0; j<M; j++)
        {
            val = (int)charImage[i*M+j];
            red.setPixelVal(i, j, (int)charImage[i*3*M+3*j]);
            green.setPixelVal(i, j, (int)charImage[i*3*M+3*j+1]);
            blue.setPixelVal(i, j, (int)charImage[i*3*M+3*j+2]);
        }
 
    delete [] charImage;
    return (1);
}

int readImageHeader(char fname[], int& N, int& M, int& Q, bool& type)
{
    int i, j;
    unsigned char *charImage;
    char header [100], *ptr;
    ifstream ifp;
 
    ifp.open(fname, ios::in | ios::binary);
 
    if (!ifp)
    {
        cout << "Can't read image: " << fname << endl;
        exit(1);
    }
 
 // read header
 
    type = false; // PGM
 
    ifp.getline(header,100,'\n');

    if ( (header[0] == 80) && (header[1]== 53) )
    {
      type = false;
    }
    else if ( (header[0] == 80) && (header[1] == 54) )
    {
      type = true;
    }
    else
    {
        cout << "Image " << fname << " is not PGM or PPM" << endl;
        exit(1);
    }

    M=strtol(header+2,&ptr,0);
    N=strtol(ptr, &ptr, 0);
    Q=strtol(ptr,&ptr,0);

    if (M == 0 & N == 0 & Q == 0) { 
        ifp.getline(header,100,'\n');
        while(header[0]=='#')
            ifp.getline(header,100,'\n');
 
        M=strtol(header,&ptr,0);
        N=strtol(ptr, &ptr, 0);
 
        ifp.getline(header,100,'\n');
        Q=strtol(header,&ptr,0);
    }

    ifp.close();
 
    return(1);
}

int writePPM(char fname[], Image& image)
{
    int i, j;
    int N, M, Q;
    ofstream ofp;
 
    image.getImageInfo(N, M, Q);
 
    ofp.open(fname, ios::out | ios::binary);
    if (!ofp)
    {
        cout << "Can't open file: " << fname << endl;
        exit(1);
    }
 
    ofp << "P6" << endl;
    ofp << M << " " << N << endl;
    ofp << Q << endl;
    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++)
        {
            ofp << (unsigned char)image.getPixelVal(i, j);
            ofp << (unsigned char)image.getPixelVal(i, j);
            ofp << (unsigned char)image.getPixelVal(i, j);
        }
    }
    if (ofp.fail())
    {
        cout << "Can't write image " << fname << endl;
        exit(0);
    }
    ofp.close();
    return(1);
}

int writePPM(char fname[], Image& red, Image& green, Image& blue)
{
    int i, j;
    int N, M, Q;
    ofstream ofp;
 
    red.getImageInfo(N, M, Q);
 
    ofp.open(fname, ios::out | ios::binary);
    if (!ofp)
    {
        cout << "Can't open file: " << fname << endl;
        exit(1);
    }
 
    ofp << "P6" << endl;
    ofp << M << " " << N << endl;
    ofp << Q << endl;
    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++)
        {
            ofp << (unsigned char)red.getPixelVal(i, j);
            ofp << (unsigned char)green.getPixelVal(i, j);
            ofp << (unsigned char)blue.getPixelVal(i, j);
        }
    }
    if (ofp.fail())
    {
        cout << "Can't write image " << fname << endl;
        exit(0);
    }
    ofp.close();
    return(1);
}
