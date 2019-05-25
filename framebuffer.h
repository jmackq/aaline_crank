#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

typedef struct {
	void* fb;
	int width;
	int height;
} framebuffer_t;

typedef struct {
	int x;
	int y;
} point_t;

/**
 * @brief Pack rgba values into one int
 *
 * @param r red intensity from 0 to 255
 * @param g green intensity from 0 to 255
 * @param b blue intensity from 0 to 255
 * @param a alpha factor from 0 to 255
 *
 * @return a single uint32 representing the arguments
 */
unsigned rgba32(int8_t r, int8_t g, int8_t b, int8_t a);

/**
 * @brief Set the value of a pixel in a framebuffer
 *
 * @param fb framebuffer to operate on
 * @param color uint32 representing an rgba value
 * @param px pixel to set the value of
 */
void set_px(framebuffer_t* fb, unsigned color, point_t* px);

/**
 * @brief Write framebuffer to image file
 *
 * @param fb framebuffer to operate on
 */
void write_bmp(framebuffer_t* fb);

/**
 * @brief Draw antialiased line into framebuffer
 *
 * @param fb framebuffer to operate one
 * @param color color to draw line with
 * @param p1 start position of line
 * @param p2 stop position of line
 */
int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2);

/**
 * @brief Create a new empty framebuffer
 *
 * @param w width of framebuffer
 * @param h height of framebuffer
 *
 * @return a pointer to the new framebuffer
 */
framebuffer_t* framebuffer_init(int w, int h);

/**
 * @brief Get the value of a pixel in a framebuffer
 *
 * @param fb framebuffer to operate on
 * @param px pixel to get the value of
 *
 * @return the RGBA value of px in fb
 */
unsigned framebuffer_px(framebuffer_t* fb, point_t* px);

/**
 * @brief Print framebuffer contents and size to stdout
 *
 * @param fb framebuffer to operate on
 */
void framebuffer_repr(framebuffer_t* fb);
