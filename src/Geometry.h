#pragma once
#include <memory>
#include <SFML/System/Vector3.hpp>
struct Mesh;

struct Geometry
{
    Geometry(std::shared_ptr<Mesh> model = nullptr) : mModel(model) {}
	std::shared_ptr<Mesh> mModel;
};

struct Segment
{
    //constructor
    Segment(const sf::Vector3<float>& p0, const sf::Vector3<float>& p1) : mP0(p0), mP1(p1) {}

    sf::Vector3<float> mP0;
    sf::Vector3<float> mP1;
};

struct Plane : Geometry
{
    //constructor
    Plane(const sf::Vector3<float>& pos, const sf::Vector3<float>& norm) : mNormal(norm) {}

    //necessary data
    sf::Vector3<float> mNormal;

};

struct Triangle : Geometry
{
    //constructor
    Triangle(const sf::Vector3<float>& v0, const sf::Vector3<float>& v1, const sf::Vector3<float>& v2) : mV0(v0), mV1(v1), mV2(v2) {}

    //necessary data
    sf::Vector3<float> mV0;
    sf::Vector3<float> mV1;
    sf::Vector3<float> mV2;
};

struct AABB : Geometry
{
    //constructor
    AABB(const sf::Vector3<float>& min, const sf::Vector3<float>& max) : mMin(min), mMax(max) {}

    //necessary data
    sf::Vector3<float> mMin;
    sf::Vector3<float> mMax;
};

struct Sphere : Geometry
{
    //constructor
    Sphere(float radius) : mRadius(radius) {}
    Sphere(const char* info = nullptr);

    float mRadius;
};

struct Ray
{
    Ray(const sf::Vector3<float>& p0, const sf::Vector3<float>& vec) : mP0(p0), mV(vec) {}
	sf::Vector3<float> mP0;
	sf::Vector3<float> mV;
};
