#pragma once
#include <iostream>
#include <memory>
#include <glm/vec3.hpp>
struct Mesh;

struct ContactInfo
{
    glm::vec3 mContact;
    glm::vec3 mNomal;
    float mT0;
    float mT1;
    float mTI;
};

struct Ray
{
    Ray(const glm::vec3& p0, const glm::vec3& vec) : mP0(p0), mV(vec) {}
    glm::vec3 mP0;
    glm::vec3 mV;
};

struct Geometry
{
    virtual ~Geometry() { std::cout << "Calling base destructor\n";  mModel.reset(); }
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
    Plane(const glm::vec3& pos, const glm::vec3& norm) : mNormal(norm) {}
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info);

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

    AABB(const glm::vec3& width, const glm::vec3& height, const glm::vec3& length) : mWidth(width), mHeight(height), mLength(length) {}
    AABB(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info);

    //necessary data
    glm::vec3 mWidth;
    glm::vec3 mHeight;
    glm::vec3 mLength;
};

struct Sphere : Geometry
{
    //constructor
    Sphere(float radius) : mRadius(radius) {}
    Sphere(const char** info = nullptr);
    bool CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info);

    float mRadius;
};


