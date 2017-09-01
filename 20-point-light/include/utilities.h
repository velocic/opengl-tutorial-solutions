#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

namespace Utilities
{
    namespace Math
    {
        const double PI = 3.1415926535897;

        double degreesToRadians(double angle);
        double radiansToDegrees(double angle);
    }

    namespace File
    {
        bool getFileContents(std::vector<uint8_t> &fileBuffer, const std::string &filePath);
    }
}

#endif
