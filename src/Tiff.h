//
// Created by Craig on 3/3/2021.
//

#ifndef CS3258_CMAKE_TIFF_H
#define CS3258_CMAKE_TIFF_H

#include <string>
#include <map>
#include <vector>

class Tiff{
    struct ifdEntry{
        short type;
        long count;
        long offset;
        std::vector<long> data;

    };

    struct fileInfo{
        std::string filename;
        bool isLE;
        long ifdAddress;
        short ifdCount;
        std::map<short, ifdEntry> entries;
    };
    typedef std::map<short, std::string> TagMap;
private:

public:
    static TagMap tagMap;
    static fileInfo info;

    static void tiffRead(std::string filename);

    static void tiffStat(std::string filename);

    static void tiffWrite(std::string filename, size_t x0, size_t y0, size_t xc, size_t yc);

    static void swapBytes(long* pointer, short typeSize);

    static void swapTwo(short* two);

    static void swapFour(long* four);

    static bool checkLE();

    static bool checkOffset(long count, short type);

    static size_t getTypeSize(short type);

    static std::string getTypeName(short type);

    static bool fileExists(std::string fileName);








};

#endif //CS3258_CMAKE_TIFF_H
