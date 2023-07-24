# 3dmath

A header-only 3D math library. 

## Conventions
This library uses the standard right-handed coordinate system 

<img alt="Right-handed system" src="https://github.com/mdh81/3dmath/blob/master/Right_handed_system.jpg?raw=true" width="25%" height="25%" class="center">

Matrices are column major. Columns of transform matrices are the basis vectors of the new frame 
![Matrix format](derivations/Matrix_Format.jpg)

## Derivations

This section lists proof for formulas used for different features implemented by this library 

### Conversion of spherical coordinates to cartesian coordinates
Used in [Sphere::generateGeometry](https://tinyurl.com/sphere-geometry)  

![Derivation of spherical to cartesian](derivations/Spherical_to_Cartesian.jpg)

### Distance between a point and a ray
Used in [Ray::distanceToPoint](https://tinyurl.com/distanceToRay)  

![Point ray distance](derivations/PointDistanceToRay.jpg)

### Ray-ray intersection
Used in [Ray::intersectWithRay](https://tinyurl.com/rayRayIntersection)  

![Ray-Ray intersection](derivations/RayRayIntersection.jpg)

### Ray-plane intersection
Used in [Plane::intersectWithRay](https://tinyurl.com/PlaneRayIntersection)  

![Ray-Ray intersection](derivations/RayPlaneIntersection.jpg)

### Ray-sphere intersection
Used in [Sphere::intersectWithRay](https://tinyurl.com/SphereRayIntersection)  

![Ray-Sphere intersection](derivations/RaySphereIntersection_1.jpg)
![Ray-Sphere intersection](derivations/RaySphereIntersection_2.jpg)


### Rotation Matrices

### Rotation About Z
Used in [RotationMatrix::rotateAboutZ](https://tinyurl.com/rotateAboutZAxis)
![Rotate about z](derivations/Rotation_About_Z.jpg)

### Rotation About Y
Used in [RotationMatrix::rotateAboutY](https://tinyurl.com/2dkvawxx)
![Rotate about y](derivations/Rotation_About_Y.jpg)

### Rotation About X
Used in [RotationMatrix::rotateAboutX](https://tinyurl.com/23kf8w2q)
![Rotate about x](derivations/Rotation_About_X.jpg)

### Orthographic Projection Matrix

Here is a derivation for the orthographic projection matrix built by <insert link to class here>


