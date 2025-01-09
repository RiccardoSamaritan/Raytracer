# RICCARDO SAMARITAN SM3201396

TARGET = raytracer

COMPILER = gcc
CFLAGS = -Wall -Wextra -O2 -fopenmp 
LINKER_FLAGS =  -lm

SOURCE = main.c scene.c ppm.c
OBJECT = $(SOURCE:.c=.o)

$(TARGET): $(OBJECT)
	$(COMPILER) $(CFLAGS) -o $@ $^ $(LINKER_FLAGS)

%.o: %.c
	$(COMPILER) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECT) $(TARGET)