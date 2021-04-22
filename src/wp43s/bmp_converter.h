#ifndef _BMP_CONVERTER_H_
#define _BMP_CONVERTER_H_


//14byte file header
typedef  struct  _tagBITMAPFILEHEADER
{
	unsigned short    	bfType;       		//The type of bitmap file, must be BM 
	unsigned int       bfSize;       		//File size, in bytes
	unsigned short		bfReserverd1; 		//Bitmap file reserved word, must be 0 
	unsigned short		bfReserverd2; 		//Bitmap file reserved word, must be 0 
	unsigned int       bfbfOffBits;  		//The offset of the bitmap file header to the data, in bytes
} __attribute__((packed)) _BITMAPFILEHEADER;//Make the compiler not optimize, its size is 14 bytes

//40byte information header
typedef  struct  _tagBITMAPINFOHEADER
{
	unsigned int biSize;                        //The size of the structure, in bytes
	int  biWidth;                     			//The width of the graphics is in pixels
	int  biHeight;                     			//The height of the graphics is in pixels
	unsigned short biPlanes;               		//The level of the target device, must be 1 
	unsigned short biBitcount;             		//Color depth, the number of bits required for each pixel
	unsigned int biCompression;        			//Compression type of bitmap
	unsigned int biSizeImage;              		//The size of the bitmap, in bytes
	int  biXPelsPermeter;       				//The horizontal resolution of the bitmap, the number of pixels per meter
	int  biYPelsPermeter;       				//Bitmap vertical resolution, the number of pixels per meter
	unsigned int biClrUsed;            			//The number of colors in the color table actually used by the bitmap
	unsigned int biClrImportant;       			//The number of important colors in the bitmap display process
} __attribute__((packed))  _BITMAPINFOHEADER;	//Make the compiler not optimize, its size is 40 bytes

typedef  struct
{
	_BITMAPFILEHEADER  file; //File information area
	_BITMAPINFOHEADER  info; //Image information area
} __attribute__((packed)) _bmp;

void write_monochrome_bitmap_file(const char* szSrcPath, const char *szDesPath);

#endif