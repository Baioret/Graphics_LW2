#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

const float PI = 3.14159265358979323846264338327950288;

#define ToRadian(x) ((x) * PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / PI)

class Vector3
{
public:

    glm::vec3 v;

    Vector3() {
        v = glm::vec3();
    };

    Vector3(float _x, float _y, float _z)
    {
        v.x = _x;
        v.y = _y;
        v.z = _z;
    }

    ~Vector3() { };


    Vector3 Normalize()
    {
        const float Length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

        v.x /= Length;
        v.y /= Length;
        v.z /= Length;

        return *this;
    }

    Vector3 Cross(const Vector3& vec) const
    {
        const float _x = v.y * vec.v.z - v.z * vec.v.y;
        const float _y = v.z * vec.v.x - v.x * vec.v.z;
        const float _z = v.x * vec.v.y - v.y * vec.v.x;

        return Vector3(_x, _y, _z);
    }
};

class Matrix4
{
public:

    glm::mat4 m;

    Matrix4() {
        m = glm::mat4();
    };
    ~Matrix4() {};

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
        m[0][0] = ScaleX;   m[0][1] = 0.0f;        m[0][2] = 0.0f;        m[0][3] = 0.0f;
        m[1][0] = 0.0f;        m[1][1] = ScaleY;   m[1][2] = 0.0f;        m[1][3] = 0.0f;
        m[2][0] = 0.0f;        m[2][1] = 0.0f;        m[2][2] = ScaleZ;   m[2][3] = 0.0f;
        m[3][0] = 0.0f;        m[3][1] = 0.0f;        m[3][2] = 0.0f;        m[3][3] = 1.0f;
    };

    void InitRotateTransform(float RotateX, float RotateY, float RotateZ)
    {

        const float x = ToRadian(RotateX);
        const float y = ToRadian(RotateY);
        const float z = ToRadian(RotateZ);

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

    void InitTranslationTransform(float x, float y, float z)
    {
        m[0][0] = 1.0f;        m[0][1] = 0.0f;        m[0][2] = 0.0f;        m[0][3] = x;
        m[1][0] = 0.0f;        m[1][1] = 1.0f;        m[1][2] = 0.0f;        m[1][3] = y;
        m[2][0] = 0.0f;        m[2][1] = 0.0f;        m[2][2] = 1.0f;        m[2][3] = z;
        m[3][0] = 0.0f;        m[3][1] = 0.0f;        m[3][2] = 0.0f;        m[3][3] = 1.0f;
    };

    void InitPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar) {

        const float ar = Width / Height;
        const float zRange = zNear - zFar;
        const float tanHalfFOV = tanf(ToRadian(FOV / 2.0));

        m[0][0] = 1.0f / (tanHalfFOV * ar);
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = 0.0f;

        m[1][0] = 0.0f;
        m[1][1] = 1.0f / tanHalfFOV;
        m[1][2] = 0.0f;
        m[1][3] = 0.0f;

        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = (-zNear - zFar) / zRange;
        m[2][3] = 2.0f * zFar * zNear / zRange;

        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 1.0f;
        m[3][3] = 0.0f;
    }

    void InitCameraTransform(const Vector3& Target, const Vector3& Up)
    {
        Vector3 N = Target;
        N.Normalize();

        Vector3 U = Up;
        U.Normalize();

        U = U.Cross(Target);
        Vector3 V = N.Cross(U);

        m[0][0] = U.v.x;  m[0][1] = U.v.y;  m[0][2] = U.v.z;  m[0][3] = 0.0f;
        m[1][0] = V.v.x;  m[1][1] = V.v.y;  m[1][2] = V.v.z;  m[1][3] = 0.0f;
        m[2][0] = N.v.x;  m[2][1] = N.v.y;  m[2][2] = N.v.z;  m[2][3] = 0.0f;
        m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
    }
};

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

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar) {
        m_persProj.FOV = FOV;
        m_persProj.Width = Width;
        m_persProj.Height = Height;
        m_persProj.zNear = zNear;
        m_persProj.zFar = zFar;
    }

    void SetCamera(Vector3 Pos, Vector3 Target, Vector3 Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    const glm::mat4* GetTrans() {
        Matrix4 ScaleTrans, RotateTrans, TranslationTrans,
            CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

        ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
        RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
        TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);
        PersProjTrans.InitPerspectiveProj(m_persProj.FOV, m_persProj.Width, m_persProj.Height,
            m_persProj.zNear, m_persProj.zFar);

        CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.v.x, -m_camera.Pos.v.y, -m_camera.Pos.v.z);
        CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);

        m_transformation = PersProjTrans.m * CameraRotateTrans.m * CameraTranslationTrans.m *
            TranslationTrans.m * RotateTrans.m * ScaleTrans.m;

        return &m_transformation;
    };

private:

    struct {
        float Width;
        float Height;
        float zNear;
        float zFar;
        float FOV;
    } m_persProj;

    struct {
        Vector3 Pos;
        Vector3 Target;
        Vector3 Up;
    } m_camera;

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;

    glm::mat4 m_transformation;
};