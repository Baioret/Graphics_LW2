#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

const float PI = 3.14159265358979323846264338327950288;

#define ToRadian(x) ((x) * PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / PI)

class Pipeline
{
public:
    Pipeline()
    {
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    const glm::mat4* GetTrans() {
        glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;
        InitScaleTransform(ScaleTrans);
        InitRotateTransform(RotateTrans);
        InitTranslationTransform(TranslationTrans);

        m_transformation = TranslationTrans * RotateTrans * ScaleTrans;
        return &m_transformation;
    };

private:
    void InitScaleTransform(glm::mat4& m) const {
        m[0][0] = m_scale.x;   m[0][1] = 0.0f;        m[0][2] = 0.0f;        m[0][3] = 0.0f;
        m[1][0] = 0.0f;        m[1][1] = m_scale.y;   m[1][2] = 0.0f;        m[1][3] = 0.0f;
        m[2][0] = 0.0f;        m[2][1] = 0.0f;        m[2][2] = m_scale.z;   m[2][3] = 0.0f;
        m[3][0] = 0.0f;        m[3][1] = 0.0f;        m[3][2] = 0.0f;        m[3][3] = 1.0f;
    };
    void InitRotateTransform(glm::mat4& m) const {
        const float x = ToRadian(m_rotateInfo.x);
        const float y = ToRadian(m_rotateInfo.y);
        const float z = ToRadian(m_rotateInfo.z);

        glm::mat4 rx, ry, rz;

        rx[0][0] = 1.0f;    rx[0][1] = 0.0f;       rx[0][2] = 0.0f;        rx[0][3] = 0.0f;
        rx[1][0] = 0.0f;    rx[1][1] = cosf(x);    rx[1][2] = -sinf(x);    rx[1][3] = 0.0f;
        rx[2][0] = 0.0f;    rx[2][1] = sinf(x);    rx[2][2] = cosf(x);     rx[2][3] = 0.0f;
        rx[3][0] = 0.0f;    rx[3][1] = 0.0f;       rx[3][2] = 0.0f;        rx[3][3] = 1.0f;

        ry[0][0] = cosf(y);     ry[0][1] = 0.0f;    ry[0][2] = -sinf(y);    ry[0][3] = 0.0f;
        ry[1][0] = 0.0f;        ry[1][1] = 1.0f;    ry[1][2] = 0.0f;        ry[1][3] = 0.0f;
        ry[2][0] = sinf(y);     ry[2][1] = 0.0f;    ry[2][2] = cosf(y);     ry[2][3] = 0.0f;
        ry[3][0] = 0.0f;        ry[3][1] = 0.0f;    ry[3][2] = 0.0f;        ry[3][3] = 1.0f;

        rz[0][0] = cosf(z);     rz[0][1] = -sinf(z);    rz[0][2] = 0.0f;    rz[0][3] = 0.0f;
        rz[1][0] = sinf(z);     rz[1][1] = cosf(z);     rz[1][2] = 0.0f;    rz[1][3] = 0.0f;
        rz[2][0] = 0.0f;        rz[2][1] = 0.0f;        rz[2][2] = 1.0f;    rz[2][3] = 0.0f;
        rz[3][0] = 0.0f;        rz[3][1] = 0.0f;        rz[3][2] = 0.0f;    rz[3][3] = 1.0f;

        m = rz * ry * rx;
    };
    void InitTranslationTransform(glm::mat4& m) const {
        m[0][0] = 1.0f;        m[0][1] = 0.0f;        m[0][2] = 0.0f;        m[0][3] = 0.0f;
        m[1][0] = 0.0f;        m[1][1] = 1.0f;        m[1][2] = 0.0f;        m[1][3] = 0.0f;
        m[2][0] = 0.0f;        m[2][1] = 0.0f;        m[2][2] = 1.0f;        m[2][3] = 0.0f;
        m[3][0] = 0.0f;        m[3][1] = 0.0f;        m[3][2] = 0.0f;        m[3][3] = 1.0f;
    };

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;

    glm::mat4 m_transformation;
};