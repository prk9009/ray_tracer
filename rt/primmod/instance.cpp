#include <rt/primmod/instance.h>
#include <core/matrix.h>

namespace rt {

Instance::Instance(Primitive* content):
    archetype(content), transformations(Matrix::identity()), inverse_transforms(transformations)
{}

Primitive* Instance::content() {
    return archetype;
}

void Instance::reset() {
    transformations = Matrix::identity();
    inverse_transforms = transformations;
}

void Instance::translate(const Vector& t) {
    Matrix matmul = Matrix::identity();
    matmul[0][3] = t.x;
    matmul[1][3] = t.y;
    matmul[2][3] = t.z;
    transformations = product( matmul, transformations);
    inverse_transforms = transformations.invert();
}

void Instance::rotate(const Vector& nnaxis, float angle) {
    Vector sprime;
    float argmin = std::fmin(std::fmin(nnaxis.x, nnaxis.y), nnaxis.z);
    if (nnaxis.x == argmin) sprime = Vector(0.0f, -nnaxis.z, nnaxis.y).normalize();
    else if (nnaxis.y == argmin) sprime = Vector(-nnaxis.z, 0.0f, nnaxis.x).normalize();
    else sprime = Vector(-nnaxis.y, nnaxis.x, 0.0f).normalize();

    Vector t = cross(nnaxis, sprime).normalize();
    Matrix txmx = Matrix::identity();
    txmx.r1 = Float4(nnaxis);
    txmx.r2 = Float4(sprime);
    txmx.r3 = Float4(t);
    txmx = txmx.transpose();

    Matrix rx = Matrix::identity();
    rx[1][1] = cos(angle);
    rx[1][2] = -sin(angle);
    rx[2][1] = sin(angle);
    rx[2][2] = cos(angle);

    Matrix rotate_mat = product(txmx, product(rx, txmx.invert()));
    transformations = product(rotate_mat, transformations);
    inverse_transforms = transformations.invert();

}

void Instance::scale(float f) {
    Matrix matmul = Matrix::identity();
    matmul[0][0] = f;
    matmul[1][1] = f;
    matmul[2][2] = f;
    transformations = product(matmul, transformations);
    inverse_transforms = transformations.invert();
}

void Instance::scale(const Vector& s) {
    Matrix matmul = Matrix::identity();
    matmul[0][0] = s.x;
    matmul[1][1] = s.y;
    matmul[2][2] = s.z;
    transformations = product(matmul, transformations);
    inverse_transforms = transformations.invert();
}

void Instance::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {
    /* TODO */
    Point tranformed_ori = inverse_transforms * ray.o;
    Vector transformed_dir = inverse_transforms * ray.d;
    float scaling_factor = transformed_dir.length();  // required for scaling the previous best distance
    Ray transformed_ray = Ray(tranformed_ori, transformed_dir.normalize());

    Intersection intersect_obj = archetype->intersect(transformed_ray, previousBestDistance * scaling_factor);

    if (intersect_obj) {
        // Normals are transformed by the transpose of the inverse of the 4x4 transformation matrix 
        return Intersection(intersect_obj.distance / scaling_factor, ray, intersect_obj.solid, (inverse_transforms.transpose() * intersect_obj.normal()).normalize(), intersect_obj.hitPoint());
    }
    else {
        return Intersection::failure();
    }
}

BBox Instance::getBounds() const {
    BBox box = archetype->getBounds();
    Point min_box = transformations* box.min;
    Point max_box = transformations*box.max;

    return BBox(min(min_box, max_box), max(min_box, max_box));
}

}