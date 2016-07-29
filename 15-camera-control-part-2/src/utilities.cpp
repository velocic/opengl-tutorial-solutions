#include <utilities.h>

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
}
