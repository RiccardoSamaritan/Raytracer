# Raytracing implementation in C

## Introduction

This project is a simple ray tracing implementation that renders images based on a scene defined with spheres and colors.
It was carried out for the final exam of "Advanced and Parallel Programming" at the University of Trieste.

---
## Compilation

Thanks to the presence of a makefile, it is sufficient to type "make" in the terminal to compile the program.
```bash
make
```
---

## Run the program
To run the program you need to type the following line in your terminal:

```bash
./raytracer <scene_file> <output_file> <width> <height>
```

Where:
- `<scene_file>` is the file containing the scene description.
- `<output_file>` is the file where the image will be saved.
- `<width>` is the width of the image.
- `<height>` is the height of the image.

---

## Code errors

| Error Code | Description                |
|------------|----------------------------|
| 0          | Success                    |
| 2          | Invalid usage              |
| 3          | Scene load failure         |
| 4          | Memory allocation failure  |
| 5          | Image save failure         |

## Scene file format
The scene file is a text file containing the description of the scene. 
To test the program, you can use the provided `scene.txt` file.
