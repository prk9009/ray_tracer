#ifndef CG1RAYTRACER_GROUPS_BVHIO_HEADER
#define CG1RAYTRACER_GROUPS_BVHIO_HEADER

#include <istream>
#include <ostream>
#include <vector>
#include <unordered_map>

#include <rt/groups/bvh.h>
#include <core/assert.h>

namespace rt {

struct BVHStreamInput : public BVH::Input {
    BVHStreamInput(std::istream& is, const Group& group);

    size_t getRootId() override { return rootId; }
    const BVH::SerializedNode& readNode(size_t nodeId) override { return nodes[nodeId]; }
    size_t getNodeCount() override { return nodes.size(); }

private:
    std::vector<BVH::SerializedNode> nodes;
    size_t rootId;
};

struct BVHStreamOutput : public BVH::Output {
    BVHStreamOutput(std::ostream& os, const Group& group)
        : rootId(0), os(os)
    {
        for (size_t i = 0; i < group.primitives.size(); ++i)
            primitiveToId[group.primitives[i]] = i;
    }
    ~BVHStreamOutput();

    void setRootId(size_t rootId) override { this->rootId = rootId; }
    void writeNode(size_t nodeId, const BVH::SerializedNode& node) override {
        assert(nodeId < nodes.size()) << "Trying to write a node with out-of-bounds index";
        nodes[nodeId] = node;
    }
    void setNodeCount(size_t nodeCount) override {
        nodes.resize(nodeCount);
    }

private:
    size_t rootId;
    std::ostream& os;
    std::unordered_map<Primitive*, size_t> primitiveToId;
    std::vector<BVH::SerializedNode> nodes;
};

}

#endif