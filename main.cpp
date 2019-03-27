#include "Image.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
 
using namespace std;
 
int readPGM(char[], Image&);
int writePPM(char[], Image&);
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
    Image image(N, M, Q);
    readPGM(argv[2], image);
    if (std::string(argv[1]).compare("convert"))
        image.convolveImage(image); 
    writePPM(argv[3], image);
    std::cout << argv[1] << ' ' << argv[2] << " > " << argv[3] << std::endl;
    return 0;
}
 
int readPGM(char fname[], Image& image)
{
    int i, j;
    int N, M, Q;
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
 
    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');
 
    M=strtol(header,&ptr,0);
    N=atoi(ptr);
 
    ifp.getline(header,100,'\n');
    Q=strtol(header,&ptr,0);
 
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
 
    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');
 
    M=strtol(header,&ptr,0);
    N=atoi(ptr);
 
    ifp.getline(header,100,'\n');
 
    Q=strtol(header,&ptr,0);
 
    ifp.close();
 
    return(1);
}
int writePPM(char fname[], Image& image)
{
    int i, j;
    int N, M, Q;
    unsigned char *charImage;
    ofstream ofp;
 
    image.getImageInfo(N, M, Q);
 
    //charImage = (unsigned char *) new unsigned char [M*N];
    //int val;
    //for(i=0; i<N; i++)
    //{
    //    for(j=0; j<M; j++)
    //    {
    //        val = image.getPixelVal(i, j);
    //        charImage[i*M+j]=(unsigned char)val;
    //    }
    //}
 
    ofp.open(fname, ios::out | ios::binary);
    if (!ofp)
    {
        cout << "Can't open file: " << fname << endl;
        exit(1);
    }
 
    ofp << "P6" << endl;
    ofp << M << " " << N << endl;
    ofp << Q << endl;
    //for (i = 0; i < 3; i++)
    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++)
        {
            ofp << (unsigned char)image.getPixelVal(i, j);
            ofp << (unsigned char)image.getPixelVal(i, j);
            ofp << (unsigned char)image.getPixelVal(i, j);
        }
    }
    //ofp.write( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    if (ofp.fail())
    {
        cout << "Can't write image " << fname << endl;
        exit(0);
    }
    ofp.close();
    delete [] charImage;
    return(1);
}