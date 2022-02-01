#include <rt/groups/bvh.h>
#include <stack>
#include <queue>
#include <vector>
#include <core/point.h>
#include <chrono> 
#include<unordered_map>
namespace rt {

BVH::BVH()
{
	// setting the splitting method for split in the middle set this flag to 1
	// for sah set the flag to 2
	splitMethod = 2;
}

// a struct to keep the info about the primitives, primitive bounds, primitive number, centroid
// primitive number is used to locate the position of the primitive in the list of primitives
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

// this is used for the case of SAH, where we are going to create n buckets and calcuate the cost for sah in a for loop
struct BucketInfo {
	uint32_t nPrims = 0;
	BBox bounds= BBox::empty();
};

// this is used for the creation of the main bounding volume hierarchy it holds the flag 
// isLeaf = says if the node is leaf or not
// bbox = bounds of the node 
// nPrims = no of primitives helpful for intersect
// primitiveStartInd = start of the primitive index in the list, this with nPrims help in iterating over all the primitives in the leaf nodes
struct BVHBuildNode {
	//bool isLeaf;
	BBox bbox;
	uint32_t nPrims;
	BVHBuildNode* children[2];
	uint32_t primitiveStartInd;//for leaves to store the primitives data
};

void BVH::rebuildIndex() {
    if (primitives.empty()) return;
	// std::cout << primitives.size()<<std::endl;
	//first we initiate the primitiveInfo the main vector which will be used for building our BVH
	//orderedPrims are the primitives ordered in an order based on leaf nodes, helpful for intersection
    std::vector<BVHPrimInfo> primitiveInfo(primitives.size());
	std::vector<Primitive*> orderedPrims;
	orderedPrims.reserve(primitives.size());
	for (size_t i = 0; i < primitives.size(); i++) {
		primitiveInfo[i] = { i , primitives[i]->getBounds() };
		// std::cout << i;

	}
	//std::cout << primitives.size()<<" NO of primitives in total"<<std::endl;
	//std::cout << unBoundPrimitives.size() << " NO of unbound prims in total" << std::endl;
	// std::cout << "done with building stack";
    totalNodes = 0;
	// we initiate the root and recursively build the tree
    root = recBuild(primitiveInfo, 0, primitives.size(), &totalNodes, orderedPrims);
	// swap out primitives with orderedPrims to maintin the order while doing intersect
	primitives.swap(orderedPrims);
	// cleaning some unwanted space
	orderedPrims.resize(0);
	primitiveInfo.resize(0);
}

int axisSelect(Vector d){
	// fuction used to find the axis with largest size
	if (d.x > d.y && d.x > d.z)
		return 0;
	else if (d.y > d.z)
		return 1;
	else
		return 2;
}

BVHBuildNode *BVH::recBuild(std::vector<BVHPrimInfo>& primitiveInfo, int start, int end, size_t* totalNodes, std::vector<Primitive*> &orderedPrimitives) {
	// create a dummy node and set children to null
	BVHBuildNode* node = new BVHBuildNode();
	node->children[0] = NULL;
	node->children[1] = NULL;
	(*totalNodes)++;
	float pmid;

	// set the bounds of the node by iterating from the start to the end
	BBox bounds = BBox::empty();
	for (int i = start; i < end; i++)
		bounds.extend(primitiveInfo[i].bounds);
	// set bounds of the node and nPrims
	node->bbox = bounds;
	int nPrims = end - start;
	node->nPrims = nPrims;

	// if no of prims is less than 3 then set it as a leaf node
	if (nPrims<3) {
		// set isLeaf to true, primitive start = end of the orderedPrimives list, and update the orderedPrims in order 
		//node->isLeaf = true;
		node->children[0] = nullptr;
		node->children[1] = nullptr;
		node->primitiveStartInd = orderedPrimitives.size();
		for (int i = start; i < end; i++) {
			int primNum = primitiveInfo[i].primitiveNumber;
			orderedPrimitives.push_back(primitives[primNum]);
		}		

        return node;
    }
	else {
		// find the centroid bounds by extending the centroid of each primitive
		BBox centroidBounds = BBox::empty();
		for (int i = start; i < end; i++) centroidBounds.extend(primitiveInfo[i].centroid);

		// use the diagonal of the centroidBOunds to find the axis with max length, this will be further used for splitting the axis
		Vector diag = centroidBounds.max - centroidBounds.min;
		int axisUsed = axisSelect(diag);
		int mid;
		
		switch (splitMethod) {
		case 1:
			// in this simple method we split in the middle of the centroidBounds
			// all the prims left of mid are set to left node, all right of mid are set to right node
			mid = (start + end) / 2;

			//if prmitives lie on a point then set it to a leaf node
			if (centroidBounds.max.at(axisUsed) == centroidBounds.min.at(axisUsed)) {
				//node->isLeaf = true;
				node->children[0] = nullptr;
				node->children[1] = nullptr;
				node->primitiveStartInd = orderedPrimitives.size();
				for (int i = start; i < end; i++) {
					int primNum = primitiveInfo[i].primitiveNumber;
					orderedPrimitives.push_back(primitives[primNum]);
				}

				return node;
			}
			// else shuffle the primitiveInfo vector in such a way that all the primitives info on left of mid
			// has centroid< mid point, and on the right of mid has centorid >= mid point on centroid
			else {
				// we use partition to divide the vector into 2 halves start to mid = left node, mid+1 to end = right node
				float pmid = (centroidBounds.max.at(axisUsed) + centroidBounds.min.at(axisUsed)) * 0.5;
				BVHPrimInfo* midPtr = std::partition(
					&primitiveInfo[start], &primitiveInfo[end - 1] + 1,
					[axisUsed, pmid](const BVHPrimInfo& pi) {
						return pi.centroid.at(axisUsed) < pmid;
					});
				mid = midPtr - &primitiveInfo[0];
			}
			break;
		case 2:
			// we will do SAH based on buckets, it saves us some time to create the tree
			int noBuckets = 16;
			std::vector<BucketInfo> buckets(noBuckets);

			// we create buckets and fill it with primitives info preset in that particular bucket only
			// these buckets have bounds info used to get area of the halves, and nPrims info to get the primitives in each half
			float maxDis = diag.at(axisUsed);
			if (maxDis == 0) {
				// Create leaf _BVHBuildNode_
				// similar to th way in the split in the middle method
				//std::cout << "wetfsfws";
				//node->isLeaf = true;
				node->children[0] = nullptr;
				node->children[1] = nullptr;
				node->primitiveStartInd = orderedPrimitives.size();
				for (int i = start; i < end; i++) {
					int primNum = primitiveInfo[i].primitiveNumber;
					orderedPrimitives.push_back(primitives[primNum]);
				}

				return node;
			}
//				std::cout << '\n' << '\n';
			for (int i = start; i < end; i++) {
				int bNo = noBuckets * (primitiveInfo[i].centroid.at(axisUsed) - centroidBounds.min.at(axisUsed)) / maxDis;
				if (bNo == noBuckets) bNo = noBuckets - 1;
				buckets[bNo].nPrims += 1;
				buckets[bNo].bounds.extend(primitiveInfo[i].bounds);
				//std::cout << " " << buckets[bNo].bounds.area()<< '\n';
			}
			//std::cout << bounds.area() << std::endl;
			// in cases when the prims are = 0, we set the bucket to an empty box
			for (int i = 0; i < noBuckets; i++) {
				if(buckets[i].nPrims == 0)	buckets[i].bounds = BBox::empty();
			}
			float costBucket;
			float optCost = FLT_MAX;
			int optBucket;
			BBox bboxL, bboxR;
			int nPrimsL, nPrimsR;
			// std::cout << nPrims * bounds.area() << " ";
			// here we go from left to right to find the most optimal position to split based on the
			// SAH calcuated.
			// as we go fromleft to right, the boxes are added from bboxL, and are removed from bboxR
			// giving us an optimal position for the split
			for (int i = 0; i < noBuckets - 1; i++) {
				nPrimsL = 0;
				nPrimsR = 0;
				bboxL = BBox::empty();
				bboxR = BBox::empty();
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
				//std::cout << " "<< bboxL.area()<<" "<< bboxR.area()<<" "<< costBucket << " " << optCost << '\n';
				if (optCost > costBucket) {
					optBucket = i;
					optCost = costBucket;
				}
					
					
			}

			float nodeCost = nPrims * bounds.area();
				
			int maxPrimsInNode = 2;
			// we gonna further divide them if the optimal cost < than the node cost itself or if no of prims > maximum primitive in a node
			if (nPrims > maxPrimsInNode || optCost < nodeCost) {
				// we use partition to divide the vector into 2 halves start to mid = left node, mid+1 to end = right node
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
				// similar to th way in the split in the middle method
				//node->isLeaf = true;
				node->children[0] = nullptr;
				node->children[1] = nullptr;
				node->primitiveStartInd = orderedPrimitives.size();
				for (int i = start; i < end; i++) {
					int primNum = primitiveInfo[i].primitiveNumber;
					orderedPrimitives.push_back(primitives[primNum]);
				}

				return node;
			}
			break;
		}
		//node->isLeaf = false;
		node->children[0] = recBuild(primitiveInfo, start, mid, totalNodes, orderedPrimitives);
		node->children[1] = recBuild(primitiveInfo, mid, end, totalNodes, orderedPrimitives);
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
	// in this we are going to do a BFS to traverse in the BVH
	// and check for intersection with a node
	// if the node is hit then we add the its children to the queue and 
	// continue checking the next node, else we skip it and move to the next node in the queue
	// we just change the search to DFS by changing the queue to stack.
	// the intersection hit is checked in a way similar to that of the simple group when a leaf is reached
	if (!root) return Intersection::failure();
	std::queue<BVHBuildNode* > intNodes;
	//std::stack<BVHBuildNode* > intNodes;
	intNodes.push(root);
	float t_min = FLT_MAX;
	float disthit = FLT_MAX;
	float intdisthit = FLT_MAX;
	Intersection intersectionHit = Intersection::failure();
	intersectionHit.distance = FLT_MAX;
	//if (intersectionHit.solid == nullptr) std::cout << 3;
	while (!intNodes.empty()) {
		BVHBuildNode* rootNode = intNodes.front();//top for stack
		intNodes.pop();
		std::pair<float,float> boxInt;
		
		boxInt = rootNode->bbox.intersect(ray);
		if (boxInt.second < 0) continue;
		if (rootNode->children[0] == nullptr && rootNode->children[1] == nullptr) {
			int start = rootNode->primitiveStartInd;
			for (int i = start; i < start + rootNode->nPrims;i++) {
				Intersection intersection = primitives[i]->intersect(ray, previousBestDistance);
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
	//intNodes = std::stack<BVHBuildNode* >();
	for (auto primitive : this->unBoundPrimitives) {
		Intersection intersection = primitive->intersect(ray, previousBestDistance);
		float intdisthit = intersection.distance;
		if (intersection) {
			if (intdisthit < disthit) {
				intersectionHit = intersection;
				disthit = intdisthit;
			}
			if (disthit == FLT_MAX && intersection.solid != nullptr) intersectionHit = intersection;
		}
	}
	if (disthit == FLT_MAX && intersectionHit.solid != nullptr) return intersectionHit;
	if (disthit == FLT_MAX && intersectionHit.solid == nullptr) return Intersection::failure();
	else if (disthit < previousBestDistance) return intersectionHit;
	else return Intersection::failure();
	
}

void BVH::add(Primitive* p) {

	if (p->getBounds().isUnbound()) unBoundPrimitives.push_back(p);
	else {
		primitives.push_back(p);
		BBox b = p->getBounds();
		//if(std::fabs(b.min.x - b.max.x)<0.0001)
		//std::cout << b.min.x << ' ' << b.max.x << ' ' << b.min.y << ' ' << b.max.y << ' ' << b.min.z << ' ' << b.max.z << std::endl;
	}
}

void BVH::setMaterial(Material* m) {
	for (auto primitive : primitives) {
		primitive->setMaterial(m);
	}
}

void BVH::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::serialize(BVH::Output& output) {
	// To implement this function:
	std::unordered_map<BVHBuildNode*, size_t> indMap;
	size_t ct = 0;
    // - Call output.setNodeCount() with the number of nodes in the BVH
	output.setNodeCount(totalNodes);
	// - Set the root node index using output.setRootId()
	output.setRootId(0);
	//SerializedNode* rootSerial = new SerializedNode();
	//output.setRootId(0);
    // - Write each and every one of the BVH nodes to the output using output.writeNode()
	

	std::stack<BVHBuildNode*> s;
	BVHBuildNode* currNode = root;
	while (s.empty() == false || currNode != nullptr) {
		while (currNode != nullptr) {
			indMap[currNode] = ct;
			ct++;
			//std::cout << ct<<std::endl;
			if (currNode->children[1] != nullptr) s.push(currNode->children[1]);
			currNode = currNode->children[0];

		}
		if (s.empty() == false) {
			currNode = s.top();
			s.pop();
		}
	}
	//std::cout << s.size();
	currNode = root;
	while (s.empty() == false || currNode != nullptr) {
		while (currNode != nullptr) {
			SerializedNode* serNode = new SerializedNode();
			serNode->bbox = currNode->bbox;
			if (currNode->children[0] == nullptr && currNode->children[1] == nullptr)
			{
				std::vector<Primitive*> sPrimitives;
				int start = currNode->primitiveStartInd;
				for (int i = start; i < start + currNode->nPrims; i++) {
					sPrimitives.push_back(primitives[i]);
				}
				serNode->isLeaf = true;
				serNode->primitives = sPrimitives;
				output.writeNode(indMap[currNode], *serNode);
				
			}
			else {
				serNode->isLeaf = false;
				serNode->leftChildId = indMap[currNode->children[0]];
				serNode->rightChildId = indMap[currNode->children[1]];
			}
			output.writeNode(indMap[currNode], *serNode);
			if (currNode->children[1] != nullptr) s.push(currNode->children[1]);
			currNode = currNode->children[0];

		}
		if (s.empty() == false) {
			currNode = s.top();
			s.pop();
		}
	}
     //TODO  NOT_IMPLEMENTED;
}
void BVH::deserialize(BVH::Input& input) {
    // To implement this function:
    // - Allocate and initialize input.getNodeCount() nodes
	size_t tNodes = input.getNodeCount();
    /* TODO */
    // - Fill your nodes with input.readNode(index)
    /* TODO */
    // - Use the node at index input.getRootId() as the root node
   /* TODO */ NOT_IMPLEMENTED;
}

}