
#include <iostream>
#include "bmp_converter.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void write_monochrome_bitmap_file(const char* szSrcPath, const char *szDesPath)
{
	_bmp  m = { 0 };        //Define a bitmap structure
	FILE *fp;

	cv::Mat temp = cv::imread(szSrcPath, 0);

	fp = fopen(szDesPath, "wb+");
	if (NULL == fp)
	{
		printf("can't open the bmp imgae.\n");
		exit(0);
	}

	int widthStep = 0;
	widthStep = ((temp.cols + 31) / 32) * 4;

	//Write the file header
	m.file.bfType = 0X4D42;
	m.file.bfSize = widthStep * temp.rows + 62;       //File size, data size + file header size
	m.file.bfReserverd1 = 0;
	m.file.bfReserverd2 = 0;
	m.file.bfbfOffBits = 62;

	//Write the message header
	m.info.biSize = 40;
	m.info.biWidth = temp.cols;
	m.info.biHeight = temp.rows;
	m.info.biPlanes = 1;
	m.info.biBitcount = 1;
	m.info.biCompression = 0;
	m.info.biSizeImage = widthStep * temp.cols;
	m.info.biXPelsPermeter = 0;
	m.info.biClrUsed = 0;
	m.info.biClrImportant = 0;

	fseek(fp, 0, SEEK_SET);
	fwrite(&(m.file.bfType), sizeof(m.file.bfType), 1, fp);
	fseek(fp, 2, SEEK_SET);
	fwrite(&(m.file.bfSize), sizeof(m) - 2, 1, fp);

	//54-62 a total of eight bytes represent palette information
	uchar Palette[8] = { 0,0,0,0,255,255,255,0 };//
	fseek(fp, 54, SEEK_SET);
	fwrite(Palette, sizeof(uchar), 8, fp);

	uchar* data = new uchar[widthStep*temp.rows];
	memset(data, 0, widthStep*temp.rows);

	//Monochrome bitmap, 1 means white, 0 means black, the background of the result image is white (1), and the line is black (0)
	for (int i = 0; i < temp.rows; i++)
	{
		for (int j = 0; j < widthStep; j++)
		{
			uchar temp_data = 0;
			for (int k = 0; k < 8; k++)
			{
				if (j * 8 + k < temp.cols)
				{
					int temp_value = 0;
					//The threshold is set to 150
					/*if ((uchar)temp->imageData[(temp.rows - 1 - i)*temp->widthStep + j * 8 + k] > 150)
					{
						temp_value = 1 << (7 - k);
						temp_data += temp_value;
					}*/
				}
			}
			data[i*widthStep + j] = temp_data;
		}
	}

	fseek(fp, 62, SEEK_SET);
	fwrite(data, sizeof(uchar), widthStep*temp.rows, fp);

	delete[] data;
	data = NULL;
	fclose(fp);
	fp = NULL;
}