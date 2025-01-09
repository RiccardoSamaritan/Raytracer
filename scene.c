// RICCARDO SAMARITAN SM3201396

#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

/**
 * Loads a scene from a file.
 * 
 * @param filename The path to the scene file.
 * @param scene The scene structure to store the loaded scene data.
 * @return True if the scene was loaded successfully, false otherwise.
 */
bool loadScene(const char *filename, Scene *scene) {
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
        return false;
    }

    // Read viewport size
     if (fscanf(file, "VP %f %f %f\n", &scene->viewport_size.x, &scene->viewport_size.y, &scene->viewport_size.z) != 3) {
        printf("Error: Failed to read viewport size.\n");
        fclose(file);
        return false;
    }

    // Read background color
    if (fscanf(file, "BG %f %f %f\n", &scene->background_color.x, &scene->background_color.y, &scene->background_color.z) != 3) {
        printf("Error: Failed to read background color.\n");
        fclose(file);
        return false;
    }

    // Read number of spheres
    if (fscanf(file, "OBJ_N %d\n", &scene->sphere_count) != 1) {
        printf("Error: Failed to read sphere count.\n");
        fclose(file);
        return false;
    }

    // Allocate memory for spheres
    scene->spheres = malloc(scene->sphere_count * sizeof(Sphere));
    if (!scene->spheres) {
        fprintf(stderr, "Error: Memory allocation for spheres failed.\n");
        fclose(file);
        return false;
    }

    // Read spheres data
    for (int i = 0; i < scene->sphere_count; ++i) {
        Sphere *s = &scene->spheres[i];
        unsigned int r, g, b;
        if (fscanf(file, "S %f %f %f %f %u %u %u\n", &s->center.x, &s->center.y, &s->center.z, &s->radius, &r, &g, &b) != 7) {
            fprintf(stderr, "Error: Failed to read sphere data.\n");
            free(scene->spheres);
            fclose(file);
            return false;
        }
        s->color[0] = (unsigned char)r;
        s->color[1] = (unsigned char)g;
        s->color[2] = (unsigned char)b;
    }

    fclose(file);
    return true;
}

/**
 * Frees the memory allocated for the scene.
 * 
 * @param scene The scene to free.
 */
void freeScene(Scene *scene) {
    free(scene->spheres);
}

/**
 * Normalizes a vector.
 * 
 * @param v The vector to normalize.
 */
Vector normalization(Vector v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0) {
        return (Vector){0, 0, 0};
    }
    return (Vector){v.x / length, v.y / length, v.z / length};
}



Vector subtraction(Vector a, Vector b) {
    return (Vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

float dot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * Checks if a ray intersects a sphere, that happens only if
 * the discriminant is greater or equal to 0.
 * 
 * @param origin The origin of the ray.
 * @param direction The direction of the ray.
 * @param sphere The sphere to check for intersection.
 * @return True if the ray intersects the sphere, false otherwise.
 */
bool intersectRaySphere(Vector origin, Vector direction, const Sphere *sphere) {
    Vector cs = subtraction(origin, sphere->center);
    float a = dot(direction, direction);
    float b = 2.0f * dot(cs, direction);
    float c = dot(cs, cs) - sphere->radius * sphere->radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant >= 0);
}

/**
 * Renders the scene to an image.
 * 
 * @param scene The scene to render.
 * @param image The image buffer to store the rendered image.
 * @param width The width of the image.
 * @param height The height of the image.
 */
void render(const Scene *scene, Pixel *image, int width, int height) {
    Vector camera_position = {0, 0, 0};
    float viewport_width = scene->viewport_size.x;
    float viewport_height = scene->viewport_size.y;
    float viewport_depth = scene->viewport_size.z;


    #pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float px = ((viewport_width / (width - 1)) * x) - (viewport_width / 2);
            float py = ((viewport_height / (height - 1)) * y) - (viewport_height / 2);
            Vector direction = {px, py, -viewport_depth};
            direction = normalization(direction);

            Pixel pixel_color = {
                (unsigned char)(scene->background_color.x),
                (unsigned char)(scene->background_color.y),
                (unsigned char)(scene->background_color.z)
            };

            for (int i = 0; i < scene->sphere_count; ++i) {
                Sphere *sphere = &scene->spheres[i];
                if (intersectRaySphere(camera_position, direction, sphere)) {
                    pixel_color.r = sphere->color[0];
                    pixel_color.g = sphere->color[1];
                    pixel_color.b = sphere->color[2];
                    break;
                }
            }

            image[y * width + (width - 1 - x)] = pixel_color;
        }
    }
}

