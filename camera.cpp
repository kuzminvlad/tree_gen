#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}


void Camera::updateScreenSize(float w, float h)
{
    width = w;
    height = h;
    projection = glm::perspective(Zoom, width/height, 0.1f, 50000.f);
    updateCamera();
}

glm::mat4 Camera::GetCamera()
{
    return camera;
}

glm::mat4 Camera::GetView()
{
    return view;
}

void Camera::updateCamera()
{
    view = glm::lookAt(Position, Position + Front, Up);
    camera = projection * view;
}

void Camera::ProcessKeyboard(Camera_Movement direction)
{
    if (direction == FORWARD)
        Position += Front * MovementSpeed ;
    if (direction == BACKWARD)
        Position -= Front * MovementSpeed ;
    if (direction == LEFT)
        Position -= Right * MovementSpeed ;
    if (direction == RIGHT)
        Position += Right * MovementSpeed ;
    updateCamera();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    else if (Zoom <= 1.0f)
        Zoom = 1.0f;
    else if (Zoom >= 45.0f)
        Zoom = 45.0f;
    projection = glm::perspective(Zoom, width/height, 0.1f, 10000.f);
    updateCamera();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
    updateCamera();
}
