#include <rt/groups/bvh.h>
#include <queue>
#include <vector>
#include <core/point.h>
#include <chrono> 

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

struct BucketInfo {
	int nPrims = 0;
	BBox bounds= BBox::empty();
};

struct BVHBuildNode {
	bool isLeaf;
	BBox bbox;
	int nPrims;
	BVHBuildNode* children[2];
	std::vector<int> primitivesInd;//for leaves to store the primitives data
};

struct LinearBVHNode {
	BBox bounds;
	bool isLeaf;
	union {
		int primitivesOffset;   // leaf
		int secondChildOffset;  // interior
	};
	uint16_t nPrims;  // 0 -> interior node
	uint8_t pad[1];        // ensure 32 byte total size
	std::vector<int> primitivesInd;
};

void BVH::rebuildIndex() {

    if (primitives.empty()) return;
	// std::cout << primitives.size()<<std::endl;
	auto start = std::chrono::high_resolution_clock::now();
    std::vector<BVHPrimInfo> primitiveInfo(primitives.size());
	for (size_t i = 0; i < primitives.size(); i++) {
		primitiveInfo[i] = { i , primitives[i]->getBounds() };
		// std::cout << i;

	}
	//std::cout << primitives.size()<<" NO of primitives in total"<<std::endl;
	//std::cout << unBoundPrimitives.size() << " NO of unbound prims in total" << std::endl;
	// std::cout << "done with building stack";
    int totalNodes = 0;

    root = recBuild(primitiveInfo, 0, primitives.size(), &totalNodes);
	std::cout << totalNodes<<" total no of nodes"<<std::endl;
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";
	std::cout << "done with building the tree"<<std::endl;
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

int axisSelect(Vector d){
	if (d.x > d.y && d.x > d.z)
		return 0;
	else if (d.y > d.z)
		return 1;
	else
		return 2;
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
	node->nPrims = nPrims;
	//std::cout << nPrims << bounds.min.x<<" " << bounds.min.y << " " << bounds.min.z << " " << bounds.max.x << " " << bounds.max.y << " " << bounds.max.z << " " << std::endl;
	if (nPrims<3) {
		node->isLeaf = true;
        std::vector<int> orderedPrims;
		for (int i = start; i < end; i++) {
			int primNum = primitiveInfo[i].primitiveNumber;
			orderedPrims.push_back(primNum);
		}		
        node->primitivesInd = orderedPrims;

        return node;

    }

	else {
		if (bounds.isUnbound()) {
			node->isLeaf = true;
			std::vector<int> orderedPrims;
			for (int i = start; i < end; i++) {
				int primNum = primitiveInfo[i].primitiveNumber;
				orderedPrims.push_back(primNum);
			}
			node->primitivesInd = orderedPrims;
		}
		BBox centroidBounds = BBox::empty();
		for (int i = start; i < end; i++) centroidBounds.extend(primitiveInfo[i].centroid);


		Vector diag = centroidBounds.max - centroidBounds.min;
		//std::cout << diag.x<<' '<<diag.y<<' '<<diag.z<<std::endl;
		int axisUsed = axisSelect(diag);
		int mid;
		//std::cout << axisUsed<<nPrims<<std::endl;
		int splitMethod = 2;
			switch (splitMethod) {
			case 1:
				mid = (start + end) / 2;
				if (centroidBounds.max.at(axisUsed) == centroidBounds.min.at(axisUsed)) {
					node->isLeaf = true;
					std::vector<int> orderedPrims;
					for (int i = start; i < end; ++i) {
						int primNum = primitiveInfo[i].primitiveNumber;
						orderedPrims.push_back(primNum);
					}
					node->primitivesInd = orderedPrims;

					return node;
				}
				else {
					float pmid = (centroidBounds.max.at(axisUsed) + centroidBounds.min.at(axisUsed)) * 0.5;
					BVHPrimInfo* midPtr = std::partition(
						&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
						[axisUsed, pmid](const BVHPrimInfo& pi) {
							return pi.centroid.at(axisUsed) < pmid;
						});
					mid = midPtr - &primitiveInfo[0];
					//if (mid != start && mid != end) break;
				}
				break;
			case 2:
				if (nPrims <= 2) {
					// Partition primitives into equally-sized subsets
					mid = (start + end) / 2;
					std::nth_element(&primitiveInfo[start], &primitiveInfo[mid],
						&primitiveInfo[end - 1] + 1,
						[axisUsed](const BVHPrimInfo& a,
							const BVHPrimInfo& b) {
								return a.centroid.at(axisUsed) <
									b.centroid.at(axisUsed);
						});
				}
				else {

					int noBuckets = 12;
					std::vector<BucketInfo> buckets(noBuckets);
					float maxDis = diag.at(axisUsed);
					for (int i = start; i < end; i++) {
						//std::cout << ' ' << i << std::endl;
						int bNo = noBuckets * (primitiveInfo[i].centroid.at(axisUsed) - centroidBounds.min.at(axisUsed)) / maxDis;
						//std::cout << bNo<<' '<<i << std::endl;
						if (bNo == noBuckets) bNo = noBuckets - 1;
						buckets[bNo].nPrims += 1;
						buckets[bNo].bounds.extend(primitiveInfo[i].bounds);
					}
					float costBucket;
					float optCost = FLT_MAX;
					int optBucket;
					for (int i = 0; i < noBuckets - 1; i++) {
						int nPrimsL, nPrimsR;
						nPrimsL = 0;
						nPrimsR = 0;
						BBox bboxL, bboxR;
						bboxL = BBox::empty();
						for (int j = 0; j < i+1; j++) {
							bboxL.extend(buckets[j].bounds);
							nPrimsL += buckets[j].nPrims;
						}
						for (int j = i+1; j < noBuckets; j++) {
							bboxR.extend(buckets[j].bounds);
							nPrimsR += buckets[j].nPrims;
						}
						//considering all the costs for hitting the box take a constant time of 1
						costBucket = nPrimsL * bboxL.area() + nPrimsR * bboxR.area();
						if (optCost > costBucket) {
							optBucket = i;
							optCost = costBucket;
						}
					}
					float nodeCost = nPrims * bounds.area();
					int maxPrimsInNode = 255;
					if (nPrims > maxPrimsInNode || optCost < nodeCost) {
						BVHPrimInfo* pmid = std::partition(
							&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
							[=](const BVHPrimInfo& pi) {
								int b = noBuckets * (pi.centroid.at(axisUsed) - centroidBounds.min.at(axisUsed)) / maxDis;
								if (b == noBuckets) b = noBuckets - 1;
								return b <= optBucket;
							});
						mid = pmid - &primitiveInfo[0];
					}
					else {
						// Create leaf _BVHBuildNode_
						node->isLeaf = true;
						std::vector<int> orderedPrims;
						for (int i = start; i < end; ++i) {
							int primNum = primitiveInfo[i].primitiveNumber;
							orderedPrims.push_back(primNum);
						}
						node->primitivesInd = orderedPrims;

						return node;
					}
				}
				break;
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
	float intdisthit = FLT_MAX;
	Intersection intersectionHit = Intersection();
	intersectionHit.distance = FLT_MAX;
	while (!intNodes.empty()) {
		BVHBuildNode* rootNode = intNodes.front();
		intNodes.pop();
		std::pair<float,float> boxInt;
		
		boxInt = rootNode->bbox.intersect(ray);
		// std::cout << boxInt.first << " " << boxInt.second << " " << std::endl;
		if (boxInt.second < 0) continue;
		if (rootNode->isLeaf == true) {
			for (auto primitiveInds : rootNode->primitivesInd) {
				// std::cout << primitiveInds<<std::endl;
				Intersection intersection = primitives[primitiveInds]->intersect(ray, previousBestDistance);
				intdisthit = intersection.distance;
				if (intersection) {
					if (intdisthit < disthit) {
						intersectionHit = intersection;
						disthit = intdisthit;
					}
				}
			}
		}
		else{
			for(int i = 0;i<2;i++)
				intNodes.push(rootNode->children[i]);
		}
	}
	intNodes = std::queue<BVHBuildNode* >();
	for (auto primitive : this->unBoundPrimitives) {
		Intersection intersection = primitive->intersect(ray, previousBestDistance);
		float intdisthit = intersection.distance;
		if (intersection) {
			if (intdisthit < disthit) {
				intersectionHit = intersection;
				disthit = intdisthit;
			}
		}
	}

	// std::cout << disthit<<std::endl;
	if (disthit == FLT_MAX) return Intersection::failure();
	else if (disthit < previousBestDistance) return intersectionHit;
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
    
	if (p->getBounds().isUnbound()) unBoundPrimitives.push_back(p);
	else primitives.push_back(p);
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

/*
SAH:
Warning: Unsupported OBJ instruction encountered at models/bitcoin.obj:3.1, ignoring.
Warning: Unsupported OBJ instruction encountered at models/bitcoin.obj:123819.1, ignoring.
120096 faces loaded.
Elapsed time: 0.409027 s
done with building the tree
Elapsed time for scene1: 148.834 s
Elapsed time for scene2: 134.671 s

Centroid
Warning: Unsupported OBJ instruction encountered at models/bitcoin.obj:3.1, ignoring.
Warning: Unsupported OBJ instruction encountered at models/bitcoin.obj:123819.1, ignoring.
120096 faces loaded.
Elapsed time: 0.530268 s
done with building the tree
Elapsed time for scene1: 42.6169 s
Elapsed time for scene2: 33.9594 s
*/

/*
SAH
Warning: Unsupported OBJ instruction encountered at models/dragon.obj:3.1, ignoring.
Warning: Unsupported OBJ instruction encountered at models/dragon.obj:435549.1, ignoring.
871306 faces loaded.
Elapsed time: 5.66354 s
done with building the tree
Elapsed time for scene1: 70.3829 s
Elapsed time for scene2: 35.0337 s

Centroid
Warning: Unsupported OBJ instruction encountered at models/dragon.obj:3.1, ignoring.
Warning: Unsupported OBJ instruction encountered at models/dragon.obj:435549.1, ignoring.
871306 faces loaded.
Elapsed time: 5.75634 s
done with building the tree
Elapsed time for scene1: 38.8372 s
Elapsed time for scene2: 19.1232 s
*/