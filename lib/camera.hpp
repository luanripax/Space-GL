#if ! defined CAMERA_HPP
#define CAMERA_HPP

#include <stdio.h>
#include <cmath>

class Camera {
private:
    
public:
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;
    Camera();
};

#endif