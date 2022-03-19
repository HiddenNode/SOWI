#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>


namespace sw
{
    class Camera
    {
    public:
        Camera(glm::fvec3 pos, glm::fvec3 looatAt, glm::fvec3 yAxis);

        const GLfloat* getModelViewMatrixData();
        const GLfloat* getPerspectiveMatrixData();
        const GLfloat* getViewMatrixData();
        glm::mat4x4& getModelViewMatrix();
        glm::mat4x4& getPerspectiveMatrix();
        glm::mat4x4& getViewMatrix();

        void setPosition(glm::vec3 pos);

        void update();
        void updateViewMatrix();
        void updatePPMatrix();
        void updateModelViewMatrix();

    private:
        void rotate(glm::vec3 axis, GLfloat angle);
        void move(glm::vec3 translation);

    protected:
        Camera();

    protected:
        // model-view matrix cordinates
        glm::fvec3 pos;
        glm::fvec3 lookAt;
        glm::fvec3 yAxis;
        glm::fvec3 xAxis;

    private:
        // perspective matrix inputs
        GLfloat near_ {0.1};
        GLfloat far_ {100};
        GLfloat fovy {90};
        GLfloat aspect {1};

        glm::mat4x4 modelViewMatrix {0.f};
        glm::mat4x4 ppMatrix {0.f};
        glm::mat4x4 viewMat {}; // ppMatrix * modelViewMatrix
    };

    class SphereCamera : public Camera
    {
    public:
        SphereCamera(glm::vec3 position, GLfloat phi, GLfloat theta);
        void rotate(GLfloat phi, GLfloat theta);
        void move(glm::vec3 translation);
        void setPosition(GLfloat x, GLfloat y, GLfloat z);
        void setRotation(GLfloat, GLfloat);
        void printPosition();

    private:
        GLfloat phi;
        GLfloat theta;
    };
}

#endif