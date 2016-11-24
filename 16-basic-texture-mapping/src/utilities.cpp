#include <utilities.h>

#include <fstream>

namespace Utilities
{
    namespace Math
    {
        double degreesToRadians(double angle)
        {
            return (angle * PI) / 180;
        }

        double radiansToDegrees(double angle)
        {
            return angle * (180/PI);
        }
    }

    namespace File
    {
        bool getFileContents(std::vector<uint8_t> &fileBuffer, const std::string &filePath)
        {
            std::ifstream inFileStream(filePath, std::ios::binary);

            if (!inFileStream) {
                return false;
            }

            inFileStream.seekg(0, std::ios::end);
            auto fileLength = inFileStream.tellg();
            inFileStream.seekg(0, std::ios::beg);

            fileBuffer.resize(fileLength);
            inFileStream.read(reinterpret_cast<char *>(fileBuffer.data()), fileLength);
        }
    }
}
