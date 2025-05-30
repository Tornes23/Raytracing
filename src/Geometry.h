#pragma once
#include <iostream>
#include <memory>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "Color.h"

struct Mesh;
struct Object;

struct ContactInfo
{
    glm::vec3 mContact = glm::vec3(0.0F);
    glm::vec3 mNormal = glm::vec3(0.0F);
    float mTI = -1.0F;
    const Object* mCollidedWith = nullptr;
    Color mColor = Color::Black;
    bool IsValid();
};

struct Ray
{
    Ray(const glm::vec3& p0 = glm::vec3(0.0f), const glm::vec3& vec = glm::vec3(0.0f));
    glm::vec3 mP0;
    glm::vec3 mV;

    glm::vec3 mAttenuation = glm::vec3(1.0f);
    float mIndexOfRefraction = 1.0f;
};

struct Geometry
{
    virtual ~Geometry();
    Geometry(std::shared_ptr<Mesh> model = nullptr) : mModel(model) {}
    virtual bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const = 0;
    std::shared_ptr<Mesh> mModel;
};

struct Triangle : Geometry
{
    //constructor
    Triangle(const char** info = nullptr);
	Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const;
    float GetIntersectionTime(const Ray& ray) const;
    void GetEdgesFromVertices(glm::vec3& edgeA, glm::vec3& edgeB) const;
    Triangle ApplyMatrix(const glm::mat4x4& mat);

	const glm::vec3& operator[](int i) const;
	glm::vec3& operator[](int i);

    //necessary data
    union 
    {
        struct
        {
            glm::vec3 mV0;
            glm::vec3 mV1;
            glm::vec3 mV2;
        };
        std::array<glm::vec3, 3> mPoints;
    };

    glm::vec3 mNormal = glm::vec3(0.0F);
    glm::mat2 mInverseMatrix = glm::mat2(0.0f);

};

struct Object;

struct scene_triangle
{
	scene_triangle() {}
	Triangle geometry;
    Object* owner = nullptr;

	glm::vec3 const& operator[](int index) const { return geometry[index]; }
	glm::vec3& operator[](int index) { return geometry[index]; }
};

struct Mesh
{
    Mesh() {}
    Mesh(const std::string& obj);
    Mesh(const std::string& obj, const glm::mat4x4& m2w);
    //Mesh(const std::string& gltf);
    std::vector<Triangle> mTriangles;
};

struct Segment
{
    //constructor
    Segment(const glm::vec3& p0, const glm::vec3& p1) : mP0(p0), mP1(p1) {}

    glm::vec3 mP0;
    glm::vec3 mP1;
};

struct Polygon : Geometry
{
    //constructor
    Polygon(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const;
};


struct Model : Geometry
{
    //constructor
    Model(const char** info = nullptr, const glm::mat4x4& m2w = glm::mat4x4(1.0F));
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const;

};


struct Plane : Geometry
{
    //constructor
    Plane(const glm::vec3& norm = glm::vec3(0.0F), const glm::vec3& p = glm::vec3(0.0F)) : mNormal(norm), mP(p) {}
    bool CheckIntersection(const Ray& ray, const glm::vec3& point, glm::vec2& interval) const;
    bool CheckIntersection(const Ray& ray, glm::vec2& interval) const;
    bool CheckIntersection(const Ray& ray, const glm::vec3& point, ContactInfo& info) const;
    float GetIntersectionTime(const Ray& ray) const;
    glm::vec3 ProjectPointInPlane(const glm::vec3& point) const;

    //necessary data
    glm::vec3 mNormal;
    glm::vec3 mP;
};


struct AABB : Geometry
{
    //constructor

    AABB(const glm::vec3& min, const glm::vec3& max);
    AABB(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info) const;
    float GetIntersectionTime(const Ray& ray) const;
    bool CheckIntersectionWithPoint(const glm::vec3& point) const;

    //necessary data
    std::vector<glm::vec3> mVectors;
    glm::vec3 mMin;
    glm::vec3 mMax;
};

struct Box : Geometry
{
    //constructor

    Box(const glm::vec3& width, const glm::vec3& height, const glm::vec3& length);
    Box(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info) const;

    //necessary data
    std::vector<glm::vec3> mVectors;
};

struct Sphere : Geometry
{
    //constructor
    Sphere(float radius) : mRadius(radius) {}
    Sphere(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const;

    float mRadius;
};


