#include "pic_compare.h"
#include "EasyBMP.h"
#include <iostream>

bool ArePixelDiff(const RGBApixel& a, const RGBApixel& b)
{
    return (a.Blue != b.Blue) ||
        (a.Green != b.Green) ||
        (a.Red != b.Red);
}

DiffArea CBMPComparator::GetDiffArea(std::string base_img_path, std::string new_img_path)
{
    DiffArea area{-1,-1,-1,-1};
    BMP base_bmp, new_bmp;
    bool load_result = base_bmp.ReadFromFile(base_img_path.c_str());
    if(!load_result)
    {
        std::cout << "Unable to load base image file" << std::endl;
        exit(1);
    }
    load_result = new_bmp.ReadFromFile(new_img_path.c_str());
    if(!load_result)
    {
        std::cout << "Unable to load new image file" << std::endl;
        exit(1);
    }
    int width = base_bmp.TellWidth();
    int height = base_bmp.TellHeight();
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if (ArePixelDiff(base_bmp.GetPixel(i, j), new_bmp.GetPixel(i, j)))
            {
                if(area.X_MIN < 0)
                {
                    area.X_MIN = i;
                }
                else
                {
                    if(area.X_MIN > i)
                    {
                        area.X_MIN = i;
                    }
                }
                if(area.Y_MIN < 0)
                {
                    area.Y_MIN = j;
                }
                else
                {
                    if(area.Y_MIN > j)
                    {
                        area.Y_MIN = j;
                    }
                }
                if (area.X_MAX < i)
                {
                    area.X_MAX = i;
                }
                if (area.Y_MAX < j)
                {
                    area.Y_MAX = j;
                }
            }
        }
    }
    area.Print();
    if (area.X_MIN > 0)
    {
        area.X_MIN = area.X_MIN - area.X_MIN % 8;
    }
    if (area.X_MAX > 0)
    {
        area.X_MAX = area.X_MAX + 8 - area.X_MAX % 8;
    }
    if (area.Y_MIN > 0)
    {
        area.Y_MIN = area.Y_MIN - area.Y_MIN % 8;
    }
    if (area.Y_MAX > 0)
    {
        area.Y_MAX = area.Y_MAX + 8 - area.Y_MAX % 8;
    }
    return area;
}

void CBMPComparator::SaveDiffAreaToFile(std::string new_img_path, DiffArea diff_area, std::string dest_path)
{
    if(!diff_area.Exists()) return;
    BMP new_bmp;
    int load_result = new_bmp.ReadFromFile(new_img_path.c_str());
    if(!load_result)
    {
        std::cout << "Unable to load new image file" << std::endl;
        exit(1);
    }
    BMP small_bmp;
    int width = diff_area.X_MAX - diff_area.X_MIN;
    int height = diff_area.Y_MAX - diff_area.Y_MIN;
    small_bmp.SetSize(width, height);
    small_bmp.SetBitDepth(1);
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            RGBApixel pixel = new_bmp.GetPixel(i + diff_area.X_MIN, j + diff_area.Y_MIN);
            small_bmp.SetPixel(i, j, pixel);
        }
    }
    small_bmp.WriteToFile(dest_path.c_str());
}
