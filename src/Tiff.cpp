//
// Created by Craig on 3/3/2021.
//

#include "Tiff.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <GL/gl.h>

#define	checkImageWidth 1024
#define	checkImageHeight 1024
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

Tiff::fileInfo Tiff::info= {};

void Tiff::tiffRead(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "file could not be opened\n";
        return;
    }

    Tiff::info.filename= filename;
    short magicNo;
    char buffer[3];
    buffer[2]='\0';

    file.seekg(0, std::ios::beg);
    file.read(buffer, 2);
    file.read((char *) &magicNo, 2);
    if(buffer[0]=='I' && buffer[1]=='I'){
        info.isLE=true;
    }else if(buffer[0]=='M' && buffer[1]=='M'){
        info.isLE=false;
    }else{
        std::cout << "File endianness is not formatted correctly\n";
        return;
    }
    bool endMatch = (checkLE() == info.isLE);
    std::cout << "Endmatch= " << endMatch << "\n";

    if (!endMatch) {
        swapTwo(&magicNo);
    }

    if (magicNo != 42) {
        std::cout << "Not a TIFF file, bad magic number " << magicNo;
        std::cout << "(0x" << std::hex << magicNo << ")\n";
        return;
    }

    file.read((char *) &info.ifdAddress, 4);
    if (!endMatch) {
        swapFour(&info.ifdAddress);
    }

    file.seekg(info.ifdAddress, std::ios::beg);
    file.read((char *) &info.ifdCount, 2);

    if (!endMatch) {
        swapTwo(&info.ifdCount);
    }

    short tag;
    long temp;
    long temp2;

    for (int i = 0; i < info.ifdCount; i++) {
        file.seekg((i*12) + 2 +info.ifdAddress, std::ios::beg);

        file.read((char *) &tag, 2);
        if(!endMatch){
            swapTwo(&tag);
        }
        info.entries[tag];

        file.read((char *) &info.entries[tag].type, 2);
        file.read((char *) &info.entries[tag].count, 4);
        file.read((char *) &info.entries[tag].offset, 4);

        if (!endMatch) {
            swapTwo(&info.entries[tag].type);
            swapFour(&info.entries[tag].count);
            swapFour(&info.entries[tag].offset);
        }

        if (Tiff::tagMap.find(tag) != tagMap.end()) {

            if (checkOffset(info.entries[tag].count, info.entries[i].type)) {
                file.seekg(info.entries[tag].offset, std::ios::beg);
            } else {
                file.seekg(12 * i + 10 + info.ifdAddress, std::ios::beg);
            }

            if(info.entries[tag].type!=5){
                new char[getTypeSize(info.entries[tag].type) * info.entries[tag].count];
            }else{

            }

            for (long j = 0; j < info.entries[tag].count; j++) {
                if(info.entries[tag].type!=5){
                    file.read((char *)&temp, getTypeSize(info.entries[tag].type));
                    if(!endMatch){
                        swapBytes((long*)&temp, getTypeSize(info.entries[tag].type));
                    }
                    info.entries[tag].data.emplace_back(temp);
                    temp2=0;
                }else{
                    file.read((char *)&temp, getTypeSize(info.entries[tag].type));
                    file.read((char *)&temp2, getTypeSize(info.entries[tag].type));
                    if(!endMatch){
                        swapBytes((long*)&temp, getTypeSize(info.entries[tag].type));
                        swapBytes((long*)&temp2, getTypeSize(info.entries[tag].type));
                    }
                    info.entries[tag].data.emplace_back(temp);
                    info.entries[tag].data.emplace_back(temp2);
                }
                temp=0;
            }
        }
    }
    long imageWidth, imageHeight, samplesPerPixel, rowsPerStrip;

    imageWidth= info.entries[256].data[0];
    imageHeight= info.entries[257].data[0];
    samplesPerPixel= info.entries[277].data[0];
    rowsPerStrip= info.entries[278].data[0];
    bool blackIsZero= info.entries[262].data[0];
    int numStrips= info.entries[279].count;

    std::cout <<  "width " << std::dec << imageWidth << "\n";
    std::cout << "height " << imageHeight << "\n";
    std::cout << "spp " << samplesPerPixel << "\n";
    std::cout << "rowsPerStrip " << rowsPerStrip << "\n";
    std::cout << "NumStrips " << numStrips << "\n";

    if(info.entries.find(284)->second.count !=0){
        if(info.entries[284].data[0] != 1){
            std::cout << "Only supports planarConfig=1. planarConfig= " << info.entries[284].data[0]  << "\n";
            return;
        }
    }

    if(imageWidth>1024 || imageHeight>1024){
        std::cout << "Only supports images up to 1024x1024.\n";
        return;
    }

    if(samplesPerPixel==1){
        for(int strip=0; strip < numStrips; strip++){
            int stripOffset= info.entries[273].data[strip];
            file.seekg(stripOffset);

            for (int i = 0; i < rowsPerStrip; i++) {
                for (int j = 0; j < imageWidth; j++) {
                    int c;
                    file.read((char*)&c, 1);
                    c=c%256;
                    if(!blackIsZero) {
                        c = 256 - c;
                    }

                    checkImage[imageHeight-i][j][0] = (GLubyte) c;
                    checkImage[imageHeight-i][j][1] = (GLubyte) c;
                    checkImage[imageHeight-i][j][2] = (GLubyte) c;
                    c=0;
                }

            }
        }
    }else if(samplesPerPixel==3){
        for(int strip=0; strip < numStrips; strip++){
            long stripOffset= info.entries[273].data[strip];
            file.seekg(stripOffset);
            short r=0;
            short b=0;
            short g=0;

            for (int i = 0; i < rowsPerStrip; i++) {
                for (int j = 0; j < imageWidth; j++) {
                    file.read((char*)&r, 1);
                    file.read((char*)&b, 1);
                    file.read((char*)&g, 1);
                    r=r%256;
                    b=b%256;
                    g=g%256;
                    if(!blackIsZero){
                        r=256-r; b=256-b; g=256-g;
                    }
                    checkImage[imageHeight-i][j][0] = (GLubyte) r;
                    checkImage[imageHeight-i][j][1] = (GLubyte) b;
                    checkImage[imageHeight-i][j][2] = (GLubyte) g;
                    r=0, b=0, g=0;
                }
            }
        }
    }else{
        std::cout << "Can only process grayscale and RGB images. Samples per pixel= " << samplesPerPixel << "\n";
        file.close();
        return;
    }
    file.close();

}

void Tiff::tiffStat(std::string filename) {

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "file could not be opened\n";
        return;
    }

    std::cout << filename << ":\n";

    //readheader
    short magicNo;
    char buffer[3];
    buffer[2]='\0';
    short ifdCount;
    long ifdAddress;
    bool fileIsLE;

    file.seekg(0, std::ios::beg);
    file.read(buffer, 2);
    file.read((char *) &magicNo, 2);
    if(buffer[0]=='I' && buffer[1]=='I'){
        fileIsLE=true;
    }else if(buffer[0]=='M' && buffer[1]=='M'){
        fileIsLE=false;
    }else{
        std::cout << "File endianness is not formatted correctly\n";
        return;
    }
    bool endMatch = (checkLE() == fileIsLE);

    if (!endMatch) {
        swapTwo(&magicNo);
    }

    if (magicNo != 42) {
        std::cout << "Not a TIFF file, bad magic number " << magicNo;
        std::cout << "(0x" << std::hex << magicNo << ")\n";
        return;
    }

    if(buffer[0]== 'I'){
        std::cout << "Magic: 0x4949 <little-endian> Version: 0x2a\n";
    }else{
        std::cout << "Magic: 0x4d4d <big-endian> Version: 0x2a\n";
    }

    file.read((char *) &ifdAddress, 4);
    if (!endMatch) {
        swapFour(&ifdAddress);
    }

    std::cout << "Directory 0: offset " << ifdAddress << " (0x" << std::hex << ifdAddress << ") next 0 (0)\n";

    file.seekg(ifdAddress, std::ios::beg);
    file.read((char *) &ifdCount, 2);

    if (!endMatch) {
        swapTwo(&ifdCount);
    }

    short tag=0;
    short type=0;
    long count=0;
    long offset=0;
    long temp=0;
    long temp2=0;

    for (int i = 0; i < ifdCount; i++) {
        file.seekg((i*12) + 2 +ifdAddress, std::ios::beg);

        file.read((char *) &tag, 2);
        file.read((char *) &type, 2);
        file.read((char *) &count, 4);
        file.read((char *) &offset, 4);

        if (!endMatch) {
            swapTwo(&tag);
            swapTwo(&type);
            swapFour(&count);
            swapFour(&offset);
        }


        if (Tiff::tagMap.find(tag) != tagMap.end()) {
            std::cout << Tiff::tagMap.find(tag)->second << " ";
            std::cout << "(" << std::dec << tag << ") ";
        }else{
            if(tag<0){
                std::cout << std::dec << tag + 65536 << "(0x" << std::hex << tag << ") ";
            }else{
                std::cout << std::dec << tag << "(0x" << std::hex << tag << ") ";
            }
        }
        std::cout << getTypeName(type) << " (" << type << ") ";
        std::cout << count;

        std::cout << "<";
        if (checkOffset(count, type)) {
            file.seekg(offset, std::ios::beg);
        } else {
            file.seekg(12 * i + 10 + ifdAddress, std::ios::beg);
        }

        for (long j = 0; j < count; j++) {
            file.read((char *) &temp, getTypeSize(type));
            if (!endMatch) {
                swapBytes((long *) &temp, getTypeSize(type));
            }
            if (type == 1) {
                if(temp==0){
                    std::cout << "00";
                }else{
                    std::cout << "0x" << std::hex << temp;
                }
                if (j != (count - 1)) {
                    std::cout << " ";
                }
            } else if (type == 2) {
                std::cout << ((char) temp);
            } else if (type == 5) {
                file.read((char*) &temp2, 4);
                if (!endMatch) {
                    swapBytes((long *) &temp2, 4);
                }
                std::cout << std::dec << (temp/temp2);
                if (j != (count - 1)) {
                    std::cout << " ";
                }
                temp2=0;

            } else {
                std::cout << std::dec << temp;
                if (j != (count - 1)) {
                    std::cout << " ";
                }
            }
            if (j >= 20) {
                std::cout << "...";
                j = count;
            }
            temp=0;
        }
        std::cout << ">\n";
    }
}

void Tiff::tiffWrite(std::string filename, size_t x0, size_t y0, size_t xc, size_t yc){
    std::ifstream file(info.filename);
    if(!file){
        std::cout << "this file could not be opened\n";
        return;
    }
    if(fileExists(filename)){
        std::cout << "this file already exists\n";
        return;
    }
    if(info.ifdCount==0){
        std::cout << "nothing has been read, nothing to write\n";
        return;
    }
    std::ofstream outFile("C:\\Users\\Craig\\Desktop\\3258-Source\\tiffWriteFiles\\" + filename, std::ios::binary);
    if(checkLE()){
        outFile.write("II", 2);
        outFile << std::hex << "II";
    }else{
        outFile.write("MM", 2);
        outFile << std::hex << "MM";
    }
    outFile.write("42", 2);
    outFile.write("8", 4);

    std::map<short, ifdEntry>::iterator it;

    for (it = info.entries.begin(); it != info.entries.end(); it++) {
        for(size_t i= 0; i< it->second.data.size(); i++){
            outFile.write((char*) &it->second.data[i], getTypeSize(it->second.type));
            if(it->second.type == 5){
                outFile.write((char*) &it->second.data[i], getTypeSize(it->second.type));
            }
        }
    }
    outFile.close();
}

void Tiff::swapBytes(long *pointer, short typeSize){
    if(typeSize==2){
        swapTwo((short*)pointer);
    }else if(typeSize==4){
        swapFour(pointer);
    }
}

void Tiff::swapTwo(short *two) {
    auto * tmp = (unsigned char *)two;
    unsigned int i;
    i = tmp[1];
    tmp[1] = tmp[0];
    tmp[0] = i;
}

void Tiff::swapFour(long *four) {
    auto * tmp = (unsigned char *)four;
    unsigned int i;
    i = tmp[3];
    tmp[3] = tmp[0];
    tmp[0] = i;
    i = tmp[2];
    tmp[2] = tmp[1];
    tmp[1] = i;
}

bool Tiff::checkLE() {
    int num = 1;
    return (*(char *) &num == 1);
}

bool Tiff::checkOffset(long count, short type) {
    if(type==5){
        return true;
    }
    return getTypeSize(type)*count>4;
}

size_t Tiff::getTypeSize(short type){
    if (type == 1 || type == 2) {
        return 1;
    } else if (type == 3) {
        return 2;
    } else if (type == 4 || type ==5) {
        return 4;
    } else{
        return 0;
    }
}

std::string Tiff::getTypeName(short type){
    if(type==1){
        return "BYTE";
    }else if(type==2){
        return "ASCII";
    }else if(type==3){
        return "SHORT";
    }else if(type==4){
        return "LONG";
    }else if(type==5){
        return "RATIONAL";
    }else{
        return "TYPE UNKNOWN";
    }
}

bool Tiff::fileExists(std::string fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

Tiff::TagMap Tiff::tagMap = {{254, "NewSubfileType"},
                             {255, "SubfileType"},
                             {256, "ImageWidth"},
                             {257, "ImageLength"},
                             {258, "BitsPerSample"},
                             {259, "Compression"},
                             {262, "PhotometricInterpretation"},
                             {263, "Threshholding"},
                             {264, "CellWidth"},
                             {265, "CellLength"},
                             {266, "FillOrder"},
                             {269, "DocumentName"},
                             {270, "ImageDescription"},
                             {271, "Make"},
                             {272, "Model"},
                             {273, "StripOffsets"},
                             {274, "Orientation"},
                             {277, "SamplesPerPixel"},
                             {278, "RowsPerStrip"},
                             {279, "StripByteCounts"},
                             {280, "MinSampleValue"},
                             {281, "MaxSampleValue"},
                             {282, "XResolution"},
                             {283, "YResolution"},
                             {284, "PlanarConfiguration"},
                             {285, "PageName"},
                             {286, "XPosition"},
                             {287, "YPosition"},
                             {288, "FreeOffsets"},
                             {289, "FreeByteCounts"},
                             {290, "GrayResponseUnit"},
                             {291, "GrayResponseCurve"},
                             {292, "T4Options"},
                             {293, "T6Options"},
                             {296, "ResolutionUnit"},
                             {297, "PageNumber"},
                             {301, "TransferFunction"},
                             {305, "Software"},
                             {306, "DateTime"},
                             {315, "Artist"},
                             {316, "HostComputer"},
                             {317, "Predictor"},
                             {318, "WhitePoint"},
                             {319, "PrimaryChromaticities"},
                             {320, "ColorMap"},
                             {321, "HalftoneHints"},
                             {322, "TileWidth"},
                             {323, "TileLength"},
                             {324, "TileOffsets"},
                             {325, "TileByteCounts"},
                             {332, "InkSet"},
                             {333, "InkNames"},
                             {334, "NumberOfInks"},
                             {336, "DotRange"},
                             {337, "TargetPrinter"},
                             {338, "ExtraSamples"},
                             {339, "SampleFormat"},
                             {340, "SMinSampleValue"},
                             {341, "SMaxSampleValue"},
                             {342, "TransferRange"},
                             {512, "JPEGProc"},
                             {513, "JPEGInterchangeFormat"},
                             {514, "JPEGInterchangeFormatLngth"},
                             {515, "JPEGRestartInterval"},
                             {517, "PEGLosslessPredictors"},
                             {518, "JPEGPointTransforms"},
                             {519, "JPEGQTables"},
                             {520, "JPEGDCTables"},
                             {521, "JPEGACTables"},
                             {529, "YCbCrCoefficients"},
                             {530, "YCbCrSubSampling"},
                             {531, "YCbCrPositioning"},
                             {532, "ReferenceBlackWhite"},
                             {33432, "Copyright"}};
