#include <rt/groups/bvh.h>
#include <queue>
#include <vector>

namespace rt {

BVH::BVH()
{
    /* TODO */
}

struct BVHPrimInfo {
	BVHPrimInfo() {}
	BVHPrimInfo(size_t primitiveNumber, const BBox& bounds)
		: primitiveNumber(primitiveNumber),
		bounds(bounds),
		centroid(.5f * bounds.min + .5f * bounds.max) {}
	size_t primitiveNumber;
	BBox bounds;
	Point centroid;
};

struct BVHBuildNode {
	bool isLeaf;
	BBox bbox;
	BVHBuildNode* children[2];
	std::vector<Primitive*> primitives;//for leaves to store the primitives data
};

void BVH::rebuildIndex() {

    if (primitives.empty()) return;
	// std::cout << primitives.size()<<std::endl;
    std::vector<BVHPrimInfo> primitiveInfo(primitives.size());
	for (size_t i = 0; i < primitives.size(); i++)
		primitiveInfo[i] = { i , primitives[i]->getBounds() };
	
    int totalNodes = 0;

    root = recBuild(primitiveInfo, 0, primitives.size(), &totalNodes);
	// std::cout << totalNodes;
}
/*
struct SerializedNode {
	bool isLeaf;
	BBox bbox;

	// For leaves only
	std::vector<Primitive*> primitives;

	// For internal nodes only
	size_t leftChildId;
	size_t rightChildId;
};
*/
// std::vector<Primitive*> primitives;

int choose_axis(Vector diag) {
    if (diag.x >= diag.y && diag.x >= diag.z) return 0;
    else if (diag.y >= diag.x && diag.y >= diag.z) return 1;
    else return 2;
}

BVHBuildNode *BVH::recBuild(std::vector<BVHPrimInfo>& primitiveInfo, int start, int end, int* totalNodes) {
	BVHBuildNode* node = new BVHBuildNode();
	node->children[0] = NULL;
	node->children[1] = NULL;
	(*totalNodes)++;
	float pmid;
	BBox bounds = BBox::empty();
	for (int i = start; i < end; i++)
		bounds.extend(primitiveInfo[i].bounds);

	node->bbox = bounds;
	int nPrims = end - start;
	if (nPrims<3) {
		node->isLeaf = true;
        std::vector<Primitive*> orderedPrims;
		for (int i = start; i < end; ++i) {
			int primNum = primitiveInfo[i].primitiveNumber;
			orderedPrims.push_back(primitives[primNum]);
		}		
        node->primitives = orderedPrims;

        return node;
    }

	else {
		BBox centroidBounds = BBox::empty();
		for (int i = start; i < end; i++) centroidBounds.extend(primitiveInfo[i].centroid);


		Vector diag = centroidBounds.max - centroidBounds.min;
		int mid = (start + end) / 2;
		if (diag.x>diag.y && diag.x>diag.z) {
			if (centroidBounds.max.x == centroidBounds.min.x) {
				node->isLeaf = true;
				std::vector<Primitive*> orderedPrims;
				for (int i = start; i < end; ++i) {
					int primNum = primitiveInfo[i].primitiveNumber;
					orderedPrims.push_back(primitives[primNum]);
				}
				node->primitives = orderedPrims;

				return node;
			}
			else {
				float pmid = (centroidBounds.max.x + centroidBounds.min.x) * 0.5;
				BVHPrimInfo* midPtr = std::partition(
					&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
					[pmid](const BVHPrimInfo& pi) {
						return pi.centroid.x < pmid;
					});
				mid = midPtr - &primitiveInfo[0];
				//if (mid != start && mid != end) break;
			}
		}
		else if (diag.y>diag.z) {
			if (centroidBounds.max.y == centroidBounds.min.y) {
				node->isLeaf = true;
				std::vector<Primitive*> orderedPrims;
				for (int i = start; i < end; ++i) {
					int primNum = primitiveInfo[i].primitiveNumber;
					orderedPrims.push_back(primitives[primNum]);
				}
				node->primitives = orderedPrims;

				return node;
			}
			else {
				float pmid = (centroidBounds.max.y + centroidBounds.min.y) * 0.5;
				BVHPrimInfo* midPtr = std::partition(
					&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
					[pmid](const BVHPrimInfo& pi) {
						return pi.centroid.y < pmid;
					});
				mid = midPtr - &primitiveInfo[0];
				//if (mid != start && mid != end) break;
			}
		}
		else {
			if (centroidBounds.max.z == centroidBounds.min.z) {
				node->isLeaf = true;
				std::vector<Primitive*> orderedPrims;
				for (int i = start; i < end; ++i) {
					int primNum = primitiveInfo[i].primitiveNumber;
					orderedPrims.push_back(primitives[primNum]);
				}
				node->primitives = orderedPrims;

				return node;
			}
			else {
				pmid = (centroidBounds.max.z + centroidBounds.min.z) * 0.5;
				BVHPrimInfo* midPtr = std::partition(
					&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
					[pmid](const BVHPrimInfo& pi) {
						return pi.centroid.z < pmid;
					});
				mid = midPtr - &primitiveInfo[0];
				//if (mid != start && mid != end) break;
			}
		}
		node->isLeaf = false;
		node->children[0] = recBuild(primitiveInfo, start, mid, totalNodes);
		node->children[1] = recBuild(primitiveInfo, mid, end, totalNodes);
		return node;
	}


}

BBox BVH::getBounds() const {
	BBox box = BBox::empty();
	for (auto primitive : primitives) {
		box.extend(primitive->getBounds());
	}
	return box;
}

Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {
	// std::cout<<ray.d.x<<" "<<ray.d.y<<" "<<ray.d.z<<std::endl;

	if (!root) return Intersection::failure();
	std::queue<BVHBuildNode* > intNodes;
	intNodes.push(root);
	float t_min = FLT_MAX;
	float disthit = FLT_MAX;
	Intersection intersectionHit = Intersection();
	intersectionHit.distance = FLT_MAX;
	while (!intNodes.empty()) {
		BVHBuildNode* rootNode = intNodes.front();
		intNodes.pop();
		std::pair<float,float> boxInt;
		boxInt = rootNode->bbox.intersect(ray);
		if (boxInt.second < 0) continue;

		for(int i = 0;i<2;i++){
			BVHBuildNode* child = rootNode->children[i];
			if (child->isLeaf == true) {
				for (auto primitive : child->primitives) {
					Intersection intersection = primitive->intersect(ray, previousBestDistance);
					float intdisthit = intersection.distance;
					if (intersection) {
						if (intdisthit < disthit) {
							intersectionHit = intersection;
							disthit = intdisthit;
						}
					}
				}
			}
			else {
				if(child->children[0] == NULL && child->children[1] == NULL) continue;
				else intNodes.push(child);
			}
		}
	}
	intNodes = std::queue<BVHBuildNode* >();
	if (disthit < previousBestDistance) return intersectionHit;
	else return Intersection::failure();
}

/*

bool BVHAccel::IntersectP(const Ray& ray) const {
	if (!nodes) return false;
	ProfilePhase p(Prof::AccelIntersectP);
	Vector3f invDir(1.f / ray.d.x, 1.f / ray.d.y, 1.f / ray.d.z);
	int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };
	int nodesToVisit[64];
	int toVisitOffset = 0, currentNodeIndex = 0;
	while (true) {
		const LinearBVHNode* node = &nodes[currentNodeIndex];
		if (node->bounds.IntersectP(ray, invDir, dirIsNeg)) {
			// Process BVH node _node_ for traversal
			if (node->nPrimitives > 0) {
				for (int i = 0; i < node->nPrimitives; ++i) {
					if (primitives[node->primitivesOffset + i]->IntersectP(
						ray)) {
						return true;
					}
				}
				if (toVisitOffset == 0) break;
				currentNodeIndex = nodesToVisit[--toVisitOffset];
			}
			else {
				if (dirIsNeg[node->axis]) {
					/// second child first
					nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
					currentNodeIndex = node->secondChildOffset;
				}
				else {
					nodesToVisit[toVisitOffset++] = node->secondChildOffset;
					currentNodeIndex = currentNodeIndex + 1;
				}
			}
		}
		else {
			if (toVisitOffset == 0) break;
			currentNodeIndex = nodesToVisit[--toVisitOffset];
		}
	}
	return false;
}




*/

void BVH::add(Primitive* p) {
    primitives.push_back(p);
}

void BVH::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::serialize(BVH::Output& output) {
    // To implement this function:
    // - Call output.setNodeCount() with the number of nodes in the BVH
    /* TODO */
    // - Set the root node index using output.setRootId()
    /* TODO */
    // - Write each and every one of the BVH nodes to the output using output.writeNode()
    /* TODO */ NOT_IMPLEMENTED;
}
void BVH::deserialize(BVH::Input& input) {
    // To implement this function:
    // - Allocate and initialize input.getNodeCount() nodes
    /* TODO */
    // - Fill your nodes with input.readNode(index)
    /* TODO */
    // - Use the node at index input.getRootId() as the root node
    /* TODO */ NOT_IMPLEMENTED;
}

}