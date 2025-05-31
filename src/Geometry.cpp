#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyOBJ/tiny_obj_loader.h>
#include <glm/gtx/norm.hpp>
#include <glm/mat2x2.hpp>
#include "Geometry.h"
#include "Utils.h"
#include "GraphicsManager.h"

Geometry::~Geometry()
{
	if (mModel)
	{
		mModel.reset();
	}
}

Sphere::Sphere(const char** info)
{
	//parse and shi
	mRadius = Utils::GetFloat(info);
}

bool Sphere::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const
{
	float a = glm::dot(ray.mV, ray.mV);
	glm::vec3 cp = ray.mP0 - center;
	float b = 2.0F * glm::dot(ray.mV, cp);
	float c = glm::dot(cp, cp) - (mRadius * mRadius);
	float discriminant = (b * b) - 4.0F * a * c;

	if (discriminant < 0.0F) return false;

	const float sqrDiscriminant = glm::sqrt(discriminant);

	float mT0 = (-b - sqrDiscriminant) / (2.0f * a);
	float mT1 = (-b + sqrDiscriminant) / (2.0f * a);

	if (mT0 < 0.0F && mT1 < 0.0F) return false;

	if (mT0 > 0.0F)
		info.mTI = mT0;
	else if (mT1 > 0.0F)
		info.mTI = mT1;

	info.mContact = ray.mP0 + ray.mV * info.mTI;
	info.mNormal = glm::normalize(info.mContact - center);

	if (glm::dot(info.mNormal, ray.mV) > 0)
		info.mNormal = -info.mNormal;

	return true;
}

Box::Box(const glm::vec3& length, const glm::vec3& width, const glm::vec3& height)
{
	mVectors.resize(3);

	mVectors[0] = length;
	mVectors[1] = width;
	mVectors[2] = height;

}

Box::Box(const char** info)
{
	mVectors.resize(3);

	mVectors[0] = Utils::GetVector(info);//length
	mVectors[1] = Utils::GetVector(info);//width
	mVectors[2] = Utils::GetVector(info);//height
}

bool Box::CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info) const
{
	glm::vec2 mainInterval(0.0F, std::numeric_limits<float>::max());

	std::vector<Plane> planes(6);
	planes[0] = Plane(glm::normalize(glm::cross(mVectors[0], mVectors[2])), corner);//actually front
	planes[1] = Plane(-planes[0].mNormal, corner + mVectors[1]);// actually back
	planes[2] = Plane(glm::normalize(glm::cross(mVectors[2], mVectors[1])), corner);//left plane
	planes[3] = Plane(-planes[2].mNormal, corner + mVectors[0]);//right plane
	planes[4] = Plane(glm::normalize(glm::cross(mVectors[1], mVectors[0])), corner);// actually bottom
	planes[5] = Plane(-planes[4].mNormal, corner + mVectors[2]);//actually top

	ContactInfo temp;
	int indexMax = -1;
	int closestPlaneIndx = -1;
	for (int i = 0; i < 6; i++)
	{
		glm::vec2 interval(0.0F, std::numeric_limits<float>::max());

		if (!planes[i].CheckIntersection(ray, interval)) 
			return false;

		if (mainInterval.x < interval.x)
			closestPlaneIndx = i;

		if (mainInterval.x == 0 && mainInterval.y > interval.y)
			indexMax = i;


		mainInterval.x = glm::max(mainInterval.x, interval.x);
		mainInterval.y = glm::min(mainInterval.y, interval.y);

		if (mainInterval.y < mainInterval.x) 
			return false;

	}

	info.mTI = mainInterval.x;

	if (mainInterval.x == 0.0F) {
		info.mTI = mainInterval.y;
		info.mNormal = planes[indexMax].mNormal;
	}
	else {
		info.mNormal = planes[closestPlaneIndx].mNormal;
	}

	if (glm::dot(info.mNormal, ray.mV) > 0)
		info.mNormal = -info.mNormal;
	
	info.mContact = ray.mP0 + info.mTI * ray.mV;

	//DEBUG
	//std::cout << "when valid intersection normal is = (" << info.mNormal.x << ", " << info.mNormal.y << ", " << info.mNormal.z << ")\n";
	return true;

}

bool Plane::CheckIntersection(const Ray& ray, const glm::vec3& point, glm::vec2& interval) const
{
	float raydot = glm::dot(ray.mV, mNormal);
	float epsilon = std::numeric_limits<float>::epsilon();
	if(raydot < epsilon && -epsilon < raydot)
		return false;

	//glm::vec3 cp = glm::normalize(ray.mP0 - point);
	glm::vec3 cp = ray.mP0 - point;
	float dot = glm::dot(cp, mNormal);
	float time = -dot / raydot;

	if (raydot <= 0.0F)
	{
		if (dot >= 0.0F)
			interval.x = time;
	}
	else
	{
		if (dot < 0.0F)
			interval.y = time;
		else
			return false;
	}

	return true;
}

bool Plane::CheckIntersection(const Ray& ray, glm::vec2& interval) const
{
	glm::vec3 cp = ray.mP0 - mP;

	float dot = glm::dot(cp, mNormal);
	float raydot = glm::dot(ray.mV, mNormal);
	float time = -dot / raydot;

	if (raydot <= 0.0F)
	{
		if (dot >= 0.0F)
			interval.x = time;
	}
	else
	{
		if (dot <= 0.0F)
			interval.y = time;
		else
			return false;
	}

	return true;
}

bool Plane::CheckIntersection(const Ray& ray, const glm::vec3& point, ContactInfo& info) const
{
	float dot = glm::dot(ray.mV, mNormal);
	if (dot == 0.0F)
		return false;

	glm::vec3 cp = ray.mP0 - point;
	float time = -glm::dot(cp, mNormal) / dot;

	if (time < 0.0F)
		return false;

	info.mTI = time;
	info.mContact = ray.mP0 + time * ray.mV;
	info.mNormal = mNormal;

	return true;
}

float Plane::GetIntersectionTime(const Ray& ray) const
{
	//computing the dot product
	float dot = glm::dot(mNormal, ray.mV);

	//if the dot product is 0
	if (dot <= cEpsilon && dot >= -cEpsilon)
		return -1.0F;

	//using the formula to compute the time
	float d = (mNormal.x * mP.x) + (mNormal.y * mP.y) + (mNormal.z * mP.z);

	float t = (d - glm::dot(mNormal, ray.mP0)) / dot;

	//if the t is negative return -1
	if (t < 0.0F)
		return -1.0F;

	//returning the time
	return t;
}

glm::vec3 Plane::ProjectPointInPlane(const glm::vec3& point) const
{
	//projecting the point onto the plane
	glm::vec3 closest;

	float lenghtSqr = glm::length2(mNormal);

	closest = point - (glm::dot((point - mP), mNormal) / lenghtSqr) * mNormal;

	//returning the projected point
	return closest;
}

Ray::Ray(const glm::vec3& p0, const glm::vec3& vec)
{
	mP0 = p0;
	mV = glm::normalize(vec);
}

Polygon::Polygon(const char** info)
{
	int vertexCount = Utils::GetInt(info);
	std::vector<glm::vec3> vertices;

	vertices.resize(vertexCount);
	for (int i = 0; i < vertexCount; i++)
		vertices[i] = Utils::GetVector(info);

	mModel = std::make_shared<Mesh>();
	for (size_t i = 0; i < vertices.size() - 2; i++)
	{
		mModel->mTriangles.push_back(Triangle(vertices[i], vertices[i + 1], vertices[i + 2]));
	}
}

bool Polygon::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const
{
	const float maxFloat = std::numeric_limits<float>::max();
	float minTime = maxFloat;

	for (int i = 0; i < mModel->mTriangles.size(); i++)
	{
		ContactInfo temp;
		mModel->mTriangles[i].CheckIntersection(ray, glm::vec3(0.0f), temp);
		if (temp.mTI >= 0.0f && temp.mTI < minTime)
		{
			minTime = temp.mTI;
			info.mTI = temp.mTI;
			info.mNormal = mModel->mTriangles[i].mNormal;
		}
	}

	if (minTime == maxFloat || info.mTI < 0.0f)
	{
		return false;
	}

	info.mContact = ray.mP0 + ray.mV * info.mTI;

	return true;
}

Model::Model(const char** info, const glm::mat4x4& m2w)
{
	std::string file = *info;
	mModel = std::make_shared<Mesh>(Mesh(file, m2w));
}

bool Model::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const
{
	const float maxFloat = std::numeric_limits<float>::max();
	float minTime = maxFloat;
	std::vector<Triangle>& triangles = mModel->mTriangles;

	ContactInfo result;
	for (int i = 0; i < triangles.size(); i++)
	{
		ContactInfo temp;
		if (!triangles[i].CheckIntersection(ray, glm::vec3(0.0f), temp))
		{
			continue;
		}

		if (temp.mTI >= 0.0f && temp.mTI < minTime)
		{
			minTime = temp.mTI;
			result.mTI = temp.mTI;
			result.mNormal = triangles[i].mNormal;
		}
	}

	if (minTime == maxFloat || result.mTI < 0.0f)
	{
		return false;
	}

	info.mTI = result.mTI;
	info.mNormal = result.mNormal;
	info.mContact = ray.mP0 + ray.mV * info.mTI;

	return true;
}

Mesh::Mesh(const std::string& obj)
{
	if (obj.empty())
	{
		return;
	}

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool valid = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, obj.c_str());
	if (valid)
	{
			mTriangles.resize(shapes[0].mesh.num_face_vertices.size());
			//triangulate
			auto mesh = shapes[0].mesh;
			int index = 0;
			for (size_t i = 0; i < mesh.indices.size(); i += 3) {
				
				int index0 = mesh.indices[i].vertex_index * 3;
				int index1 = mesh.indices[i + 1].vertex_index * 3;
				int index2 = mesh.indices[i + 2].vertex_index * 3;

				glm::vec3 v0 = glm::vec3(attrib.vertices[index0 + 0], 
									attrib.vertices[index0 + 1],
									attrib.vertices[index0 + 2]);

				glm::vec3 v1 = glm::vec3(attrib.vertices[index1 + 0],
									attrib.vertices[index1 + 1],
									attrib.vertices[index1 + 2]);

				glm::vec3 v2 = glm::vec3(attrib.vertices[index2 + 0],
									attrib.vertices[index2 + 1],
									attrib.vertices[index2 + 2]);

				mTriangles[index] = Triangle(v0, v1, v2);
				index++;
			}
	}
}

Mesh::Mesh(const std::string& obj, const glm::mat4x4& m2w)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<glm::vec3> vertices;

	std::string err;

	bool valid = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, obj.c_str());
	if (valid)
	{
		for (size_t i = 0; i < shapes.size(); i++)
		{
			auto mesh = shapes[i].mesh;
			size_t index_offset = 0;

			for (size_t j = 0; j < mesh.num_face_vertices.size(); j++)
			{
				// Loop over vertices in the face.
				size_t fv = size_t(mesh.num_face_vertices[j]);
				for (size_t k = 0; k < fv; k++)
				{
					// access to vertex
					tinyobj::index_t idx = mesh.indices[index_offset + k];

					tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
					tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
					tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
					vertices.push_back({ vx, vy, vz });
				}
				index_offset += fv;
			}
		}

		glm::mat4x4 transform = m2w;
		std::transform(vertices.begin(), vertices.end(), vertices.begin(), [&transform](glm::vec3& vertex)
			{
				return transform * glm::vec4(vertex, 1.0f);
			}
		);

		for (size_t i = 0; i < vertices.size(); i+=3)
		{
			mTriangles.push_back(Triangle(vertices[i], vertices[i + 1], vertices[i + 2]));
		}
	}
}

bool ContactInfo::IsValid() { return mCollidedWith != nullptr; }

ContactInfo& ContactInfo::operator=(const ContactInfo& rhs)
{
	mContact = rhs.mContact;
	mNormal = rhs.mNormal;
	mTI = rhs.mTI;
	mCollidedWith = rhs.mCollidedWith;
	mColor = rhs.mColor;

	return *this;
}

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
{
	mVectors.resize(3);

	mMin = min;//get min
	mMax = max;//get max
}

AABB::AABB(const char** info)
{
	mMin = Utils::GetVector(info);//get min
	mMax = Utils::GetVector(info);//get max
}

bool AABB::CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info) const
{
	bool all = true;
	glm::vec2 mainInterval(0.0F, std::numeric_limits<float>::max());

	glm::vec3 cp = glm::normalize(ray.mP0 - corner);
	std::vector<Plane> planes(6);
	std::vector<glm::vec3> displacements(6);

	planes[0] = Plane(glm::normalize(glm::cross(mVectors[0], mVectors[2])));//front plane
	planes[1] = Plane(-planes[0].mNormal);// back plane
	planes[2] = Plane(glm::normalize(glm::cross(mVectors[2], mVectors[1])));//left plane
	planes[3] = Plane(-planes[2].mNormal);//right plane
	planes[4] = Plane(glm::normalize(glm::cross(mVectors[1], mVectors[0])));// bot plane
	planes[5] = Plane(-planes[4].mNormal);//top plane

	displacements[0] = glm::vec3(0.0F);//front plane
	displacements[1] = mVectors[1];// back plane
	displacements[2] = glm::vec3(0.0F);//left plane
	displacements[3] = mVectors[0];// right plane
	displacements[4] = glm::vec3(0.0F);//bottom plane
	displacements[5] = mVectors[2];// top plane

	ContactInfo temp;
	int indexMin = 0;
	int indexMax = 0;
	for (int i = 0; i < 6; i++)
	{
		glm::vec2 interval(0.0F, std::numeric_limits<float>::max());
		float raydot = glm::dot(ray.mV, planes[i].mNormal);
		glm::vec3 point = corner + displacements[i];
		if (raydot == 0.0F || !planes[i].CheckIntersection(ray, point, interval))
		{
			all = false;
			break;
		}

		if (mainInterval.x < interval.x)
			indexMin = i;

		mainInterval.x = glm::max(mainInterval.x, interval.x);
		mainInterval.y = glm::min(mainInterval.y, interval.y);

		if (interval.x == 0.0f && mainInterval.y < interval.y)
			indexMax = i;

		if (mainInterval.y < mainInterval.x)
		{
			all = false;
			break;
		}

	}

	if (!all)
		return false;

	if (mainInterval.x == 0.0F)
	{
		info.mTI = mainInterval.y;
		info.mNormal = planes[indexMax].mNormal;
	}
	else
	{
		info.mTI = mainInterval.x;
		info.mNormal = planes[indexMin].mNormal;
	}

	info.mContact = ray.mP0 + info.mTI * ray.mV;


	if (glm::dot(info.mNormal, ray.mV) > 0)
		info.mNormal = -info.mNormal;


	return true;
}

float AABB::GetIntersectionTime(const Ray& ray) const
{
	if (CheckIntersectionWithPoint(ray.mP0))
		return 0.0F;

	float tMin = -FLT_MAX;
	float tMax = FLT_MAX;

	for (int i = 0; i < 3; i++)
	{
		if (ray.mV[i] <= cEpsilon && ray.mV[i] >= -cEpsilon)
		{
			if (ray.mP0[i] < mMin[i] || ray.mP0[i] > mMax[i])
				return -1.0F;
		}
		else
		{
			float divider = 1.0F / ray.mV[i];

			float t1 = (mMin[i] - ray.mP0[i]) * divider;
			float t2 = (mMax[i] - ray.mP0[i]) * divider;

			if (t1 > t2)
				std::swap(t1, t2);

			if (t1 > tMin)
				tMin = t1;

			if (t2 < tMax)
				tMax = t2;

			if (tMin > tMax)
				return -1.0F;
		}
	}

	if (tMin < 0.0F)
		return -1.0F;

	return tMin;
}

bool AABB::CheckIntersectionWithPoint(const glm::vec3& point) const
{
	//check if the point is inside the min and max values on the three axis
	const bool inX = point.x >= mMin.x && point.x <= mMax.x;

	const bool inY = point.y >= mMin.y && point.y <= mMax.y;

	const bool inZ = point.z >= mMin.z && point.z <= mMax.z;

	//if is inside all return true
	if (inX && inY && inZ)
		return true;

	return false;
}

Triangle::Triangle(const char** info)
{

}

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{

	mV0 = v0;
	mV1 = v1 - v0;
	mV2 = v2 - v0;

	mNormal = glm::normalize(glm::cross(mV1, mV2));
	mInverseMatrix = glm::inverse(glm::mat2(glm::dot(mV1, mV1), glm::dot(mV1, mV2), 
											glm::dot(mV1, mV2), glm::dot(mV2, mV2)));
	
	mV1 = v1;
	mV2 = v2;
}

bool Triangle::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info) const
{
	glm::vec3 normal = mNormal;
	if (glm::dot(normal, ray.mV) > 0)
		normal = -normal;

	Plane p(normal, mV0);

	glm::vec2 interval{0.0, std::numeric_limits<float>::max()};
	bool intersection = p.CheckIntersection(ray, interval);

	// No intersection with the triangle
	if (!intersection) return false;

	if (interval.x <= Raytracer.GetEpsilon())
	{
		info.mTI = interval.y;
	}
	else
	{
		info.mTI = interval.x;
	}

	glm::vec3 intersection_point = ray.mP0 + (ray.mV * info.mTI);
	glm::vec3 cPi = intersection_point - mV0;
	glm::vec3 edgeA;
	glm::vec3 edgeB;

	GetEdgesFromVertices(edgeA, edgeB);

	glm::vec2 alpha_beta = mInverseMatrix * glm::vec2(glm::dot(cPi, edgeA), glm::dot(cPi, edgeB));

	// No intersection with the triangle
	if (alpha_beta.x < 0.0 || alpha_beta.y < 0.0 || (alpha_beta.x + alpha_beta.y) > 1.0) return false;

	// default intersection data (ignored when no intersection)
	info.mContact = intersection_point;
	info.mNormal = normal;

	// intersection with the triangle
	return true;
}

float Triangle::GetIntersectionTime(const Ray& ray) const
{
	//checking if the ray intersects with the plane created by the tirangle
	glm::vec3 normal = glm::normalize(glm::cross(mV0 - mV1, mV0 - mV2));
	Plane p(normal, mV0);
	float time = p.GetIntersectionTime(ray);

	//if they intersect
	if (time != -1.0F)
	{
		//checking point containment using barycentric coordinates
		glm::vec3 point = ray.mP0 + (time * ray.mV);

		glm::vec3 edge0(mV1 - mV0);
		glm::vec3 edge1(mV2 - mV0);
		glm::vec3 edge2(point - mV0);

		float dot00 = glm::dot(edge0, edge0);
		float dot01 = glm::dot(edge0, edge1);
		float dot02 = glm::dot(edge0, edge2);
		float dot11 = glm::dot(edge1, edge1);
		float dot12 = glm::dot(edge1, edge2);

		float divider = (dot00 * dot11) - (dot01 * dot01);

		if (divider == 0.0F)
			return -1.0F;

		float u = ((dot11 * dot02) - (dot01 * dot12)) / divider;
		float v = ((dot00 * dot12) - (dot01 * dot02)) / divider;

		if (u >= 0.0F && v >= 0 && (u + v <= 1.0F))
			return time;

	}

	//if they dont intersect return -1
	return -1.0F;
}

void Triangle::GetEdgesFromVertices(glm::vec3& edgeA, glm::vec3& edgeB) const
{
	edgeA = mV1 - mV0;
	edgeB = mV2 - mV0;
}

Triangle Triangle::ApplyMatrix(const glm::mat4x4& mat)
{
	glm::vec4 v0(mV0, 1.0f);
	glm::vec4 v1(mV1, 1.0f);
	glm::vec4 v2(mV2, 1.0f);

	return Triangle(mat * v0, mat * v1, mat * v2);
}

const glm::vec3& Triangle::operator[](int i) const
{
	assert(i <= 2 && i >= 0);

	return mPoints[i];
}

glm::vec3& Triangle::operator[](int i)
{
	assert(i <= 2 && i >= 0);

	return mPoints[i];
}
