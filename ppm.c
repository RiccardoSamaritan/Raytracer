// RICCARDO SAMARITAN SM3201396

#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/**
 * Saves an image in PPM format.
 * 
 * @param filename: Name of the file where the image will be saved.
 * @param image: Pointer to the array of image pixels.
 * @param width Image width.
 * @param height Image height.
 * @return true if the image is successfully saved, false otherwise.
 */
bool savePpm(const char *filename, const Pixel *image, int width, int height) {
    
    // Open file with read/write permissions, create it if it doesn't exist, and truncate it
    int filedata = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    
    if (filedata < 0) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
        return false;
    }

    // Compute sizes for the header and the image data
    size_t header_size = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
    size_t image_size = width * height * sizeof(Pixel);
    size_t total_size = header_size + image_size;

    // Resize the file to the total size needed
    if (ftruncate(filedata, total_size) == -1) {
        fprintf(stderr, "Error: Failed to resize file '%s'\n", filename);
        close(filedata);
        return false;
    }

    // Map the file into memory
    char *data = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, filedata, 0);
    if (data == MAP_FAILED) {
        fprintf(stderr, "Error: Failed to map file '%s'\n", filename);
        close(filedata);
        return false;
    }

    // Write the header to the mapped memory
    snprintf(data, header_size + 1, "P6\n%d %d\n255\n", width, height);

    // Write the image data to the mapped memory
    memcpy(data + header_size, image, image_size);

    // Unmap the file from memory
    if (munmap(data, total_size) == -1) {
        fprintf(stderr, "Error: Failed to unmap file '%s'\n", filename);
        close(filedata);
        return false;
    }

    close(filedata);
    return true;
}

