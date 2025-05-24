/**
* @file		 kdtree.cpp
* @author	 Nestor Uriarte,  nestor.uriarte@digipen.edu
* @date		 Thu Nov 26 03:37:29 2020
* @brief	 This file contains the definition of the KDTree class
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/
#pragma once
#include <vector>
#include "Geometry.h"

constexpr float cEpsilon = 1e-13f;

/**
 * Basic KDTree
 */
class kdtree
{
public:

	enum class classification_t
	{
		inside,
		outside,
		overlapping

	};


    classification_t classify_plane_triangle(const Plane& p, const Triangle& t);
    classification_t classify_plane_point(const Plane& plane, const glm::vec3& point);

    /**
     * Construction configuration
     */
    struct config
    {
        float cost_traversal = 1.0f;
        float cost_intersection = 80.0f;
        int   max_depth = 5;
    };

    void SetConfig(const config& buildConfig) { m_cfg = buildConfig; }

    /**
     * Statistics
     */
    struct debug_stats
    {
        size_t intersection_queries;
        size_t intersection_positive_queries;
    };

    /**
     * Result of an intersection query
     */
    struct intersection
    {
        // Index of the intersected triangle
        size_t triangle_index;

        // Time of intersection (t<0 if no intersection)
        float t;

        // Checks if intersected, for easy checks
        explicit operator bool() const { return t >= 0.0f; }
    };

    /**
     * KDTree Node structure
     *  Internals: Split, Axis, Right subnode index. (left subnode is next in the array)
     *  Leafs: Index of first triangle, amount of triangles
     */
    struct node
    {
    private:
        union
        {
            // Split position (internal only)
            float m_split;
            // Index of first triangle (leafs only)
            unsigned m_start_primitive;
        };
        union
        {
            // Subnode index (6MSB) + Axis (2LSB) (internal only)
            unsigned m_subnode_index;
            // Amount of triangles (6MSB) + 0b11 (leafs only)
            unsigned m_count;
        };

    public:
        void set_leaf(int first_primitive_index, int primitive_count);
        void set_internal(int axis, float split_point, int subnode_index);

        // Accessors
        [[nodiscard]] bool  is_leaf() const noexcept;
        [[nodiscard]] bool  is_internal() const noexcept;
        [[nodiscard]] int   primitive_count() const noexcept;
        [[nodiscard]] int   primitive_start() const noexcept;
        [[nodiscard]] int   next_child() const noexcept;
        [[nodiscard]] float split() const noexcept;
        [[nodiscard]] int   axis() const noexcept;
    };

    /**
     * Custom structure, reference to the original triangle in case of need of split
     * NOTE: If the KdTree does not split triangles, then storing a copy is overkill.
     */
    struct triangle_wrapper
    {
        // Generated triangle (or copied)
        Triangle tri;

        // Index of source triangle (so the application knows which triangles was being referenced)
        size_t original_index;

        [[nodiscard]] auto const& operator[](int i) const { return tri.mPoints[i]; }
        [[nodiscard]] auto& operator[](int i) { return tri.mPoints[i]; }
    };

private:
    // All recorded triangles (may contain duplicates)
    std::vector<size_t> m_indices;
    // KDTree nodes
    std::vector<node> m_nodes;
    // AABBs of nodes (same order)
    std::vector<AABB> m_aabbs;
    // Converted triangles
    std::vector<triangle_wrapper> m_triangles;
    // Configuration
    config m_cfg;

public:
    typedef std::vector<scene_triangle> triangle_container;

    /**
     * Builds the kdtree
     * @param triangles
     * @param config
     */
    void build(triangle_container const& all_triangles, const config& cfg);
    void build(triangle_container const& all_triangles);

    void build_tree(std::vector<triangle_wrapper> const& triangles, int depth);
    float get_split(std::vector<triangle_wrapper> const& triangles, int axis, float* min_cost);
    void split(std::vector<triangle_wrapper> const& triangles, int* left, int* right, int axis, float splitPoint);

    int cost_intersect(float surfaceA, int countA, float surfaceB, int countB);
    int cost_leaf(std::vector<triangle_wrapper> const& triangles);

    void split(std::vector<triangle_wrapper> const& triangles, std::vector<triangle_wrapper>& left, std::vector<triangle_wrapper>& right, int axis, float splitPoint);
    AABB computeBV(triangle_wrapper const& triangle);
    AABB computeBV(std::vector<triangle_wrapper> const& triangles);
    float compute_surface(const AABB& bv);

    /**
     * Retrieves the closest intersection
     * @param r
     * @param stats
     * @return intersection
     */
    [[nodiscard]] intersection get_closest(Ray const r, debug_stats* stats) const;
    intersection get_min(Ray const& r, int currNode, intersection& minT) const;

    [[nodiscard]] int get_depth() const;

    [[nodiscard]] const decltype(m_nodes)& nodes() const noexcept { return m_nodes; }
    [[nodiscard]] const decltype(m_indices)& indices() const noexcept { return m_indices; }
    [[nodiscard]] const decltype(m_aabbs)& aabbs() const noexcept { return m_aabbs; }
    [[nodiscard]] const decltype(m_triangles)& triangles() const noexcept { return m_triangles; }
    [[nodiscard]] decltype(m_triangles)& triangles() noexcept { return m_triangles; } // Debug

private:
};
