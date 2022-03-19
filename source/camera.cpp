#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

#include "Camera.hpp"
#include "helperOperators.hpp"

namespace sw
{

    Camera::Camera()
    {}

    Camera::Camera(glm::fvec3 pos, glm::fvec3 lookAt, glm::fvec3 yAxis):
    pos {pos}, lookAt {lookAt}, yAxis {yAxis}
    {}

    void Camera::updateModelViewMatrix()
    {
        glm::fvec3 z {glm::normalize(-lookAt)};
        xAxis = {glm::cross(yAxis, z)};
        modelViewMatrix = {{xAxis, 0.f}, {yAxis, 0.f}, {z, 0.f}, {pos, 1.f}};
        modelViewMatrix = glm::inverse(modelViewMatrix);
    }

    void Camera::updatePPMatrix() // update the perpective projection matrix
    {
        auto top = near_ * glm::tan(glm::radians(fovy/2));
        auto bottom = -top;
        auto right = top * aspect;
        auto left = -right;
        ppMatrix[0][0] = near_/right;
        ppMatrix[1][1] = near_/top;
        ppMatrix[2][2] = (-(near_ + far_)/(far_ - near_));
        ppMatrix[3][2] = (-2*far_ * near_)/(far_ - near_);
        ppMatrix[2][3] = -1.f;
    }

    void Camera::updateViewMatrix()
    {
        viewMat = ppMatrix * modelViewMatrix;
    }

    const GLfloat* Camera::getModelViewMatrixData()
    {
        return &modelViewMatrix[0][0];
    }

    const GLfloat* Camera::getPerspectiveMatrixData()
    {
        return &ppMatrix[0][0];
    }

    const GLfloat* Camera::getViewMatrixData()
    {
        return &viewMat[0][0];
    }

    glm::mat4x4& Camera::getModelViewMatrix()
    {
        return modelViewMatrix;
    }

    glm::mat4x4& Camera::getPerspectiveMatrix()
    {
        return ppMatrix;
    }

    glm::mat4x4& Camera::getViewMatrix()
    {
        return viewMat;
    }

    void Camera::setPosition(glm::vec3 position)
    {
        pos = position;

        updateModelViewMatrix();
        updateViewMatrix();
    }

    void Camera::update()
    {
        updateModelViewMatrix();
        updatePPMatrix();
        updateViewMatrix();
    }

    void Camera::move(glm::vec3 translation)
    {
        pos += translation;
    }
    
    void Camera::rotate(glm::vec3 axis, GLfloat angle)
    {
        lookAt = glm::rotate(angle, axis) * glm::vec4 {lookAt, 1.f};
    }

    SphereCamera::SphereCamera(glm::vec3 position, GLfloat phi, GLfloat theta):
    phi{phi}, theta{theta}
    {
        pos = position;
        lookAt = {cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta)};
        glm::vec3 v {glm::cross(lookAt, {0.f, 0.f, 1.f})};
        yAxis = glm::rotate(glm::half_pi<GLfloat>(), v) * glm::vec4{lookAt, 0.f};
    }

    void SphereCamera::rotate(GLfloat phi_rot, GLfloat theta_rot)
    {
        if ((theta - theta_rot) < .1)
        {
            theta = .1;
        }

        else if ((theta - theta_rot) > (glm::pi<GLfloat>() - .1))
        {
            theta = glm::pi<GLfloat>() - .1;
        }    
        else
            theta -= theta_rot;
        
        phi -= phi_rot;

        lookAt = {cos(phi)*sin(theta), sin(phi)*sin(theta), cos(theta)};
        glm::vec3 v {glm::cross(lookAt, {0.f, 0.f, 1.f})};
        yAxis = glm::rotate(glm::half_pi<GLfloat>(), v) * glm::vec4{lookAt, 0.f};
    }

    void SphereCamera::move(glm::vec3 translation)
    {
        pos += translation.x * xAxis;
        pos += translation.y * lookAt;
        pos += translation.z * yAxis;
    }

    void SphereCamera::setRotation(GLfloat newPhi, GLfloat newTheta)
    {
        phi = newPhi;
        theta = newTheta;
    }

    void SphereCamera::setPosition(GLfloat x, GLfloat y, GLfloat z)
    {
        pos = glm::vec3{x, y, z};
    }

    void SphereCamera::printPosition()
    {
        std::cout << pos;
    }

}