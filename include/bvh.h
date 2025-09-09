#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "hittable.h"
#include "rand_utils.h"

struct BVH : public Hittable
{
    struct Node
    {
        std::shared_ptr<Hittable> object;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        AABB box;

        Node() : object(nullptr), left(nullptr), right(nullptr), box(AABB::empty()) {}
    };

    std::shared_ptr<Node> root;

    BVH(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end, FloatType time1)
    {
        if (start >= end) {
            root = nullptr;
        } else {
            root = build(objects, start, end, time1);
        }
    }

    BVH(std::vector<std::shared_ptr<Hittable>> &objects, FloatType time1)
        : BVH(objects, 0, objects.size(), time1) {}

    bool hit(const Ray &r, Interval t_range, HitRecord &rec) const override
    {
        return hit_node(root.get(), r, t_range, rec);
    }

    AABB bounding_box() const override { return root ? root->box : AABB::empty(); }
    AABB bounding_box(FloatType) const override { return root ? root->box : AABB::empty(); }

private:
    static std::shared_ptr<Node> build(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end, FloatType time1)
    {
        size_t object_span = end - start;
        
        if (object_span == 0) {
            return nullptr;
        }
        
        Node node;
        
        // Choose the longest axis instead of random
        int axis = 0;
        if (object_span > 1) {
            AABB total_box = AABB::empty();
            for (size_t i = start; i < end; ++i) {
                total_box = AABB::surrounding_box(total_box, objects[i]->bounding_box(time1));
            }
            axis = total_box.longest_axis();
        }
        
        auto comparator = [axis, time1](const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
        {
            const auto box_a = a->bounding_box(time1);
            const auto box_b = b->bounding_box(time1);
            return box_a.axis(axis).min < box_b.axis(axis).min;
        };

        if (object_span == 1)
        {
            node.object = objects[start];
            node.box = node.object->bounding_box(time1);
        }
        else
        {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            size_t mid = start + object_span / 2;
            node.left = build(objects, start, mid, time1);
            node.right = build(objects, mid, end, time1);
            node.box = AABB::surrounding_box(node.left->box, node.right->box);
        }

        return std::make_shared<Node>(node);
    }

    static bool hit_node(const Node *node, const Ray &r, Interval t_range, HitRecord &rec)
    {
        if (!node || !node->box.hit(r, t_range))
            return false;

        if (node->object)
            return node->object->hit(r, t_range, rec);

        bool hit_left = hit_node(node->left.get(), r, t_range, rec);
        bool hit_right = hit_node(node->right.get(), r, Interval(t_range.min, hit_left ? rec.t : t_range.max), rec);
        return hit_left || hit_right;
    }
};

