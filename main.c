// RICCARDO SAMARITAN SM3201396

#include <stdio.h>
#include <stdlib.h>
#include "scene.h"
#include "ppm.h"

/**
 * Prints the program usage information in order to
 * explain users how to correctly use the program.
 * 
 * @param program_name The name of the executable.
 */
void printUsage(const char *program_name) {
    printf("Usage: %s <scene_file> <output_file> <width> <height>\n", program_name);
    printf("  <scene_file>   - Path to the input scene description file\n");
    printf("  <output_file>  - Path to the output PPM image file\n");
    printf("  <width>        - Width of the rendered image (positive integer)\n");
    printf("  <height>       - Height of the rendered image (positive integer)\n");
}

/**
 * Main function.
 * Handles scene loading, rendering, and image saving.
 */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        printUsage(argv[0]);
        return 2;
    }

    // Parse command line arguments
    const char *scene_file = argv[1];
    const char *output_file = argv[2];

    // Cast width and height to integers
    int width = atoi(argv[3]); 
    int height = atoi(argv[4]); 

    // Check if width and height are valid
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Error: Both width and height must be positive integers.\n");
        return 2;
    }

    // Load scene from file
    Scene scene;
    if (!loadScene(scene_file, &scene)) {
        fprintf(stderr, "Error: Failed to load scene from file '%s'.\n", scene_file);
        return 2;
    }

    // Allocate memory for the image
    Pixel *image = malloc(width * height * sizeof(Pixel));
    if (!image) {
        fprintf(stderr, "Error: Memory allocation for image buffer failed.\n");
        freeScene(&scene);
        return 4;
    }

    // Render the scene
    render(&scene, image, width, height);

    // Save the rendered image
    if (!savePpm(output_file, image, width, height)) {
        fprintf(stderr, "Error: Failed to save image to file '%s'.\n", output_file);
        free(image);
        freeScene(&scene);
        return 5;
    }

    free(image);
    freeScene(&scene);

    printf("Rendering completed successfully. Output saved to '%s'.\n", output_file);
    return 0;
}
