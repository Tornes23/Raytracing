#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyOBJ/tiny_obj_loader.h>
#include <glm/gtx/transform.hpp>
#include <glm/mat2x2.hpp>
#include "Geometry.h"
#include "Utils.h"
#include "GraphicsManager.h"

Sphere::Sphere(const char** info)
{
	//parse and shi
	mRadius = Utils::GetFloat(info);
}

bool Sphere::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
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

bool Box::CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info)
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

bool Plane::CheckIntersection(const Ray& ray, const glm::vec3& point, glm::vec2& interval)
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

bool Plane::CheckIntersection(const Ray& ray, glm::vec2& interval)
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

bool Plane::CheckIntersection(const Ray& ray, const glm::vec3& point, ContactInfo& info)
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

Ray::Ray(const glm::vec3& p0, const glm::vec3& vec)
{
	mP0 = p0;
	mV = glm::normalize(vec);
}

Polygon::Polygon(const char** info)
{
	int vertices = Utils::GetInt(info);

	mVertices.resize(vertices);
	for (int i = 0; i < vertices; i++)
		mVertices[i] = Utils::GetVector(info);

	Triangulate();
}

bool Polygon::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
{
	const float maxFloat = std::numeric_limits<float>::max();
	float minTime = maxFloat;

	for (int i = 0; i < mTriangles.size(); i++)
	{
		ContactInfo temp;
		mTriangles[i].CheckIntersection(ray, glm::vec3(0.0f), temp);
		if (temp.mTI >= 0.0f && temp.mTI < minTime)
		{
			minTime = temp.mTI;
			info.mTI = temp.mTI;
			info.mNormal = mTriangles[i].mNormal;
		}
	}

	if (minTime == maxFloat || info.mTI < 0.0f)
	{
		return false;
	}

	info.mContact = ray.mP0 + ray.mV * info.mTI;

	return true;
}

void Polygon::Triangulate()
{
	for (size_t i = 0; i < mVertices.size() - 2; i++)
	{
		mTriangles.push_back(Triangle(mVertices[i], mVertices[i + 1], mVertices[i + 2]));
	}
}

Model::Model(const char** info, const glm::mat4x4& m2w)
{
	std::string file = *info;
	mModel = std::make_shared<Mesh>(Mesh(file, m2w));
}

bool Model::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
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

bool ContactInfo::IsValid() { return mTI != std::numeric_limits<float>::max() && mTI >= 0.0F; }

AABB::AABB(const glm::vec3& width, const glm::vec3& height, const glm::vec3& length)
{
	mVectors.resize(3);

	mVectors[0] = length;
	mVectors[1] = width;
	mVectors[2] = height;
}

AABB::AABB(const char** info)
{
	mVectors.resize(3);

	mVectors[0] = Utils::GetVector(info);//length
	mVectors[1] = Utils::GetVector(info);//width
	mVectors[2] = Utils::GetVector(info);//height
}

bool AABB::CheckIntersection(const Ray& ray, const glm::vec3& corner, ContactInfo& info)
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

Triangle::Triangle(const char** info)
{

}

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, bool definedInEdges)
{
	mDefinedInEdges = definedInEdges;

	mV0 = v0;
	mV1.mEdge = v1 - v0;
	mV2.mEdge = v2 - v0;

	mNormal = glm::normalize(glm::cross(mV1.mEdge, mV2.mEdge));
	mInverseMatrix = glm::inverse(glm::mat2(glm::dot(mV1.mEdge, mV1.mEdge), glm::dot(mV1.mEdge, mV2.mEdge), 
											glm::dot(mV1.mEdge, mV2.mEdge), glm::dot(mV2.mEdge, mV2.mEdge)));
	
	if (!mDefinedInEdges)
	{
		mV1.mVertex = v1;
		mV2.mVertex= v2;
	}
}

bool Triangle::CheckIntersection(const Ray& ray, const glm::vec3& center, ContactInfo& info)
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

void Triangle::GetEdgesFromVertices(glm::vec3& edgeA, glm::vec3& edgeB)
{
	edgeA = mV1.mVertex - mV0;
	edgeB = mV2.mVertex - mV0;
}

Triangle Triangle::ApplyMatrix(const glm::mat4x4& mat)
{
	glm::vec4 v0(mV0, 1.0f);
	glm::vec4 v1(mV1.mVertex, 1.0f);
	glm::vec4 v2(mV2.mVertex, 1.0f);

	return Triangle(mat * v0, mat * v1, mat * v2);
}
