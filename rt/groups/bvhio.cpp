#include <rt/groups/bvhio.h>
#include <unordered_map>

namespace rt {

BVHStreamInput::BVHStreamInput(std::istream& is, const Group& group) {
    std::string header;
    is >> header;
    release_assert(header == "BVH1") << "Invalid BVH header file";
    size_t nodeCount = 0;
    is >> nodeCount >> rootId;
    nodes.resize(nodeCount);
    for (size_t i = 0; i < nodeCount; ++i) {
        BVH::SerializedNode& node = nodes[i];
        std::string type;
        is >> type;
        is >> node.bbox.min.x >> node.bbox.min.y >> node.bbox.min.z;
        is >> node.bbox.max.x >> node.bbox.max.y >> node.bbox.max.z;
        if (type == "NODE") {
            is >> node.leftChildId >> node.rightChildId;
            release_assert(node.leftChildId < nodeCount && node.rightChildId < nodeCount) << "Invalid child node index";
            node.isLeaf = false;
        } else {
            release_assert(type == "LEAF") << "Invalid node type";
            size_t primCount = 0;
            is >> primCount;
            node.primitives.resize(primCount);
            for (size_t j = 0; j < primCount; ++j) {
                size_t id = 0;
                is >> id;
                release_assert(id < group.primitives.size()) << "Invalid primitive index";
                node.primitives[j] = group.primitives[id];
            }
            node.isLeaf = true;
        }
    }
}

BVHStreamOutput::~BVHStreamOutput() {
    os << "BVH1 " << nodes.size() << " " << rootId << "\n";
    for (const BVH::SerializedNode& node : nodes) {
        os << (node.isLeaf ? "LEAF" : "NODE") << "\n";
        os << node.bbox.min.x << " " << node.bbox.min.y << " " << node.bbox.min.z << "\n";
        os << node.bbox.max.x << " " << node.bbox.max.y << " " << node.bbox.max.z << "\n";
        if (!node.isLeaf) {
            os << node.leftChildId << " " << node.rightChildId << "\n";
        } else {
            os << node.primitives.size() << "\n";
            for (Primitive* primitive : node.primitives)
                os << primitiveToId[primitive] << " ";
            os << "\n";
        }
    }
}

}