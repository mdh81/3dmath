# 3dmath

A header-only 3D math library. 

## Conventions
This library uses the standard right-handed coordinate system 

<img alt="Right-handed system" src="https://github.com/mdh81/3dmath/blob/master/Right_handed_system.jpg?raw=true" width="25%" height="25%" class="center">

## Derivations

This section lists proof for formulas used for different features implemented by this library 

### Conversion of spherical coordinates to cartesian coordinates
[Sphere::generateGeometry](https://tinyurl.com/sphere-geometry)  

![Derivation of spherical to cartesian](derivations/Spherical_to_Cartesian.jpg)

### Distance between a point and a ray
[Ray::distanceToPoint](https://tinyurl.com/distanceToRay)  

![Point ray distance](derivations/PointDistanceToRay.jpg)

### Ray-ray intersection
[Ray::intersectWithRay](https://tinyurl.com/rayRayIntersection)  

![Ray-Ray intersection](derivations/RayRayIntersection.jpg)

### Ray-plane intersection
[Plane::intersectWithRay](https://tinyurl.com/PlaneRayIntersection)  

![Ray-Ray intersection](derivations/RayPlaneIntersection.jpg)

### Ray-sphere intersection
[Sphere::intersectWithRay](https://tinyurl.com/SphereRayIntersection)  

![Ray-Sphere intersection](derivations/RaySphereIntersection_1.jpg)
![Ray-Sphere intersection](derivations/RaySphereIntersection_2.jpg)


### Orthographic Projection Matrix

Here is a derivation for the orthographic projection matrix built by <insert link to class here>


