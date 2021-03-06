#ifndef _WP43S_PIC_COMPARE_H
#define _WP43S_PIC_COMPARE_H

#include <string>
#include <iostream>

struct DiffArea
{
    int X_MIN;
    int Y_MIN;
    int X_MAX;
    int Y_MAX;

    inline DiffArea():X_MIN(-1),X_MAX(-1),Y_MIN(-1),Y_MAX(-1){}
    inline bool Exists()
    {
        return (X_MIN != X_MAX) && (Y_MIN != Y_MAX);
    }
    inline void Print()
    {
        std::cout << "Debug: diff area xmin: "<< X_MIN << ", xmax: " << X_MAX <<
            ", ymin: "<< Y_MIN << ", ymax: " << Y_MAX << std::endl;
    }
    inline int Width()
    {
        return X_MAX - X_MIN;
    }
    inline int Height()
    {
        return Y_MAX - Y_MIN;
    }
};

// This class compares 2 bmp files with same size, but
// find out the diff area in order for partial re-fresh
class CBMPComparator
{
public:
    static DiffArea GetDiffArea(std::string base_img_path, std::string new_img_path);
    static void SaveDiffAreaToFile(std::string new_img_path, DiffArea diff_area, std::string dest_path);
};
#endif
