#pragma once
#include <iostream>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Color.h"
struct Mesh;

struct ContactInfo
{
    glm::vec3 mContact = glm::vec3(0.0F);
    glm::vec3 mNormal = glm::vec3(0.0F);
    float mT0 = -1.0F;
    float mT1 = -1.0F;
    float mTI = -1.0F;
    Color mColor = Color::White;
};

struct Ray
{
    Ray(const glm::vec3& p0, const glm::vec3& vec);
    glm::vec3 mP0;
    glm::vec3 mV;
};

struct Geometry
{
    virtual ~Geometry() { mModel.reset(); }
    Geometry(std::shared_ptr<Mesh> model = nullptr) : mModel(model) {}
    virtual bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) = 0;
	std::shared_ptr<Mesh> mModel;
};

struct Segment
{
    //constructor
    Segment(const glm::vec3& p0, const glm::vec3& p1) : mP0(p0), mP1(p1) {}

    glm::vec3 mP0;
    glm::vec3 mP1;
};

struct Plane : Geometry
{
    //constructor
    Plane(const glm::vec3& norm = glm::vec3(0.0F)) : mNormal(norm) {}
    bool CheckIntersection(const Ray& ray, const glm::vec3& point, glm::vec2& interval);
    bool CheckIntersection(const Ray& ray, const glm::vec3& point, ContactInfo& info);

    //necessary data
    glm::vec3 mNormal;

};

struct Triangle : Geometry
{
    //constructor
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) : mV0(v0), mV1(v1), mV2(v2) {}
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info);

    //necessary data
    glm::vec3 mV0;
    glm::vec3 mV1;
    glm::vec3 mV2;
};

struct AABB : Geometry
{
    //constructor

    AABB(const glm::vec3& width, const glm::vec3& height, const glm::vec3& length);
    AABB(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info);

    //necessary data
    std::vector<glm::vec3> mVectors;
    // 0 is mWidth;
    // 1 is mHeight;
    // 2 is mLength;
};

struct Sphere : Geometry
{
    //constructor
    Sphere(float radius) : mRadius(radius) {}
    Sphere(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info);

    float mRadius;
};


