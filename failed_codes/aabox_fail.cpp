
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vector N; //normal;
	Point loc_hit_pt;
	Point bounds[2];
	bounds[0] = corner1;
	bounds[1] = corner2;
	int sign[3];
	Vector inv_d = Vector(1.0f / ray.d.x, 1.0f / ray.d.y, 1.0f / ray.d.z);
	sign[0] = (inv_d.x < 0);
	sign[1] = (inv_d.y < 0);
	sign[2] = (inv_d.z < 0);

	tmin = (bounds[sign[0]].x - ray.o.x) * inv_d.x;
	tmax = (bounds[1 - sign[0]].x - ray.o.x) * inv_d.x;
	tymin = (bounds[sign[1]].y - ray.o.y) * inv_d.y;
	tymax = (bounds[1 - sign[1]].y - ray.o.y) * inv_d.y;

	Vector low_corner = Vector(tmin, tymin, 0.0f);
	Vector far_corner = Vector(tmax, tymax, 0.0f);

	if ((tmin > tymax) || (tymin > tmax)) return Intersection::failure();
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	tzmin = (bounds[sign[2]].z - ray.o.z) * inv_d.z;
	tzmax = (bounds[1 - sign[2]].z - ray.o.z) * inv_d.z;
	low_corner.z = tzmin;
	far_corner.z = tzmax;

	if ((tmin > tzmax) || (tzmin > tmax)) return Intersection::failure();
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
