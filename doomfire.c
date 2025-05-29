#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 1024
#define HEIGHT 768

#define SCALE 4
#define FIRE_WIDTH 256
#define FIRE_HEIGHT 192

uint8_t image[HEIGHT * WIDTH * 3] = {0};
uint8_t fire[FIRE_HEIGHT][FIRE_WIDTH] = {0};

const uint8_t palette[] = {
                0x07,0x07,0x07,
                0x1F,0x07,0x07,
                0x2F,0x0F,0x07,
                0x47,0x0F,0x07,
                0x57,0x17,0x07,
                0x67,0x1F,0x07,
                0x77,0x1F,0x07,
                0x8F,0x27,0x07,
                0x9F,0x2F,0x07,
                0xAF,0x3F,0x07,
                0xBF,0x47,0x07,
                0xC7,0x47,0x07,
                0xDF,0x4F,0x07,
                0xDF,0x57,0x07,
                0xDF,0x57,0x07,
                0xD7,0x5F,0x07,
                0xD7,0x5F,0x07,
                0xD7,0x67,0x0F,
                0xCF,0x6F,0x0F,
                0xCF,0x77,0x0F,
                0xCF,0x7F,0x0F,
                0xCF,0x87,0x17,
                0xC7,0x87,0x17,
                0xC7,0x8F,0x17,
                0xC7,0x97,0x1F,
                0xBF,0x9F,0x1F,
                0xBF,0x9F,0x1F,
                0xBF,0xA7,0x27,
                0xBF,0xA7,0x27,
                0xBF,0xAF,0x2F,
                0xB7,0xAF,0x2F,
                0xB7,0xB7,0x2F,
                0xB7,0xB7,0x37,
                0xCF,0xCF,0x6F,
                0xDF,0xDF,0x9F,
                0xEF,0xEF,0xC7,
                0xFF,0xFF,0xFF
};

#define PALETTE_SIZE (sizeof(palette) / sizeof(*palette) / 3)

static int max(int a, int b) {
    return a > b ? a : b;
}

static void step(void) {
    for (int row = FIRE_HEIGHT - 2; row >= 0; row--) {
        for (int column = 0; column < FIRE_WIDTH; column++) {
            uint8_t decay = rand() % 2;
            int8_t spread = (rand() % 3) - 1;
            fire[row][(column + spread + FIRE_WIDTH) % FIRE_WIDTH] = max(0, fire[row+1][column] - decay);
        }
    }
}

static uint8_t* read_doom_logo(size_t* logo_height, size_t* logo_width) {
    FILE* fp = fopen("doom.ppm", "r");
    if (!fp) {
        fprintf(stderr, "Failed to open doom.ppm\n");
        return NULL;
    }

    char header[3];
    if (fscanf(fp, "%2s", header) != 1 || strcmp(header, "P6") != 0) {
        fprintf(stderr, "Failed to find P6 header\n");
        fclose(fp);
        return NULL;
    }

    size_t w, h, tmp;
    if (fscanf(fp, "%ld", &w) != 1) {
        fprintf(stderr, "Failed to extract width\n");
        fclose(fp);
        return NULL;
    }

    if (fscanf(fp, "%ld", &h) != 1) {
        fprintf(stderr, "Failed to extract height\n");
        fclose(fp);
        return NULL;
    }

    if (fscanf(fp, "%ld", &tmp) != 1 || tmp != 255) {
        fprintf(stderr, "Failed to extract maxcolor\n");
        fclose(fp);
        return NULL;
    }

    fgetc(fp);

    size_t pixels = w * h * 3;
    uint8_t* data = malloc(pixels);
    if (!data) {
        fprintf(stderr, "Failed to allocate data\n");
        fclose(fp);
        return NULL;
    }

    if(fread(data, sizeof(uint8_t), pixels, fp) != pixels) {
        fprintf(stderr, "Failed to read %ld pixels\n", pixels);
        fclose(fp);
        free(data);
        return NULL;
    }

    *logo_height = h;
    *logo_width = w;

    return data;
}

int main(void) {
    srand(time(0));

    fprintf(stderr, "palette size %ld\n", PALETTE_SIZE);

    size_t logo_height, logo_width;
    uint8_t* logo = read_doom_logo(&logo_height, &logo_width);

    if (!logo) {
        fprintf(stderr, "Failed to read LOGO\n");
        return -1;
    }

    fprintf(stderr, "doom logo size: %ld x %ld", logo_width, logo_height);

    for (size_t column = 0; column < FIRE_WIDTH; column++) {
        fire[FIRE_HEIGHT-1][column] = PALETTE_SIZE - 1;
    }

    size_t x_offset = (WIDTH - logo_width) / 2;
    size_t y_limit = (HEIGHT - logo_height) / 2;
    size_t y_offset = y_limit + logo_height * 1.5;

    while (true) {
        step();

        memset(image, 0, sizeof(image));

        y_offset -= 5;
        y_offset = max(y_limit, y_offset);

        for (size_t row = 0; row < logo_height; row++) {
            int dst_idx = (row + y_offset) * WIDTH * 3 + x_offset * 3;
            if (dst_idx < 0 || dst_idx >= WIDTH * HEIGHT * 3) {
                continue;
            }
            uint8_t* src = &logo[row * logo_width * 3];
            uint8_t* dst = &image[dst_idx];
            memcpy(dst, src, logo_width * 3);
        }

        printf("P6\n%d %d\n255\n", WIDTH, HEIGHT);

        for (size_t row = 0; row < HEIGHT; row++) {
            for (size_t column = 0; column < WIDTH; column++) {
                size_t palette_idx = fire[row/SCALE][column/SCALE];
                const uint8_t* color = &palette[palette_idx * 3];
                uint8_t* img = &image[row * WIDTH * 3 + column * 3];

                if (img[0] + img[1] + img[2] == 0 || palette_idx > 0) {
                    img[0] = color[0];
                    img[1] = color[1];
                    img[2] = color[2];
                }
            }
        }

        fwrite(image, sizeof(uint8_t), sizeof(image), stdout);
    }

    return 0;
}
