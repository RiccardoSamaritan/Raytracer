// RICCARDO SAMARITAN SM3201396

#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    float x, y, z;
} Vector;

typedef struct {
    Vector center;
    float radius;
    unsigned char color[3];
} Sphere;

typedef struct {
    Vector viewport_size;
    Vector background_color;
    Sphere *spheres;
    int sphere_count;
} Scene;

typedef struct {
    unsigned char r, g, b;
} Pixel;

bool loadScene(const char *filename, Scene *scene);
void freeScene(Scene *scene);
Vector normalization(Vector v);
Vector subtraction(Vector a, Vector b);
float dot(Vector a, Vector b);
bool intersectRaySphere(Vector origin, Vector direction, const Sphere *sphere);
void render(const Scene *scene, Pixel *image, int width, int height);

#endif