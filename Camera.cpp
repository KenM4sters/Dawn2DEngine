#include "Camera.hpp"

OrthographicCamera::OrthographicCamera(const glm::vec3 position, uint32_t width, uint32_t height)
    : mPosition{position}
{
    RecalculateTransforms(width, height);
}

OrthographicCamera::~OrthographicCamera() 
{

}

void OrthographicCamera::RecalculateTransforms(uint32_t width, uint32_t height) 
{
    mView = glm::lookAt(mPosition, glm::vec3(0.0f), mUp);
    mProjection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);
}