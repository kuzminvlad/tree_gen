#ifndef CAMERA_H
#define CAMERA_H

#include <includedlib.h>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW        = -90.0f;
const float PITCH      =  0.0f;
const float SPEED      =  10.0f;
const float ZOOM       =  45.0f;

class Camera
{
private:
    glm::mat4 camera;
    glm::mat4 view;
public:
    glm::vec3 Position, Front, Up, Right, WorldUp;
    float Yaw, Pitch;
    float MovementSpeed, Zoom;
    float width, height;
    glm::mat4 projection;

    Camera(glm::vec3 position = glm::vec3(0.0f, 150.0f, 300.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    glm::mat4 GetCamera();
    glm::mat4 GetView();

    void ProcessKeyboard(Camera_Movement direction);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void updateCameraVectors();
    void updateScreenSize(float w, float h);
    void updateCamera();
};

#endif // CAMERA_H
