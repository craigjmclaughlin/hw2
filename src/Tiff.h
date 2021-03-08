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

    /*
     * Function name: tiffRead
     * Description: Reads in filename, displays and stores data
     * Arguments: std::string filename- name of file to process
     * Returns: void
     */

    static void tiffRead(std::string filename);

    /*
     * Function name: tiffStat
     * Description: Reads in filename, prints out IFD directory information
     * Arguments: std::string filename- name of file to process
     * Returns: void
     */
    static void tiffStat(std::string filename);

    /*
     * Function name: tiffWrite
     * Description: reads in filename,
     * Arguments: std::string filename- name of file to process
     * Returns: void
     */
    static void tiffWrite(std::string filename, size_t x0, size_t y0, size_t xc, size_t yc);

    /*
     * Function name: swapBytes
     * Description: converts contents of pointer from big to little endian or vice versa
     * Arguments: long* pointer- pointer of contents to swap
     *            short typeSize- number of bytes to swap
     * Returns: void
     */
    static void swapBytes(long* pointer, short typeSize);

    /*
     * Function name: swapTwo
     * Description: swaps two bytes
     * Arguments: short* two- pointer to two bytes to swap
     * Returns: void
     */
    static void swapTwo(short* two);

    /*
     * Function name: swapFour
     * Description: swaps four bytes
     * Arguments: long* four- pointer to four bytes to swap
     * Returns: void
     */
    static void swapFour(long* four);

    /*
     * Function name: checkLE
     * Description: determines if machine is little endian
     * Arguments: none
     * Returns: true if little endian, else false
     */
    static bool checkLE();

    /*
     * Function name: checkOffset
     * Description: checks if the offset field of an IFD entry contains an offset or value
     * Arguments: long count- value being checked
     *            short type- type of value
     * Returns: true if offset, false if value
     */
    static bool checkOffset(long count, short type);

    /*
     * Function name: getTypeSize
     * Description: returns the byte count of a type
     * Arguments: short type- type of IFD entry data
     * Returns: byte size of type
     */
    static size_t getTypeSize(short type);

    /*
     * Function name: getTypeName
     * Description: returns string name of type
     * Arguments:short type- number corresponding to type
     * Returns: string containing the type name for output
     */
    static std::string getTypeName(short type);

    /*
     * Function name: fileExists
     * Description: checks if a given filename exists
     * Arguments: std::string filename- name of file to process
     * Returns: true if file exists, else false
     */
    static bool fileExists(std::string fileName);

};

#endif //CS3258_CMAKE_TIFF_H
