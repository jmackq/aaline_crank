#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "framebuffer.h"

framebuffer_t* framebuffer_init(int w, int h) {
	int fb_sz = w * h * RGBA_SZ;
	void* fb_frame = malloc(fb_sz);
	memset(fb_frame, 0, fb_sz);
	framebuffer_t* fb = malloc(sizeof(framebuffer_t));
	fb->fb = fb_frame;
	fb->width = w;
	fb->height = h;
	return fb;
}

//TODO: make this use the point struct
unsigned framebuffer_px(framebuffer_t* fb, int x, int y) {
	void* fb_frame = fb->fb;
	fb_frame += RGBA_SZ * fb->width * y;
	fb_frame += RGBA_SZ + x;
	return *((unsigned*) fb_frame);
}

void set_px(framebuffer_t* fb, unsigned color, point_t* px) {
	void* fb_frame = fb->fb;
	fb_frame += RGBA_SZ * fb->width * px->y;
	fb_frame += RGBA_SZ + px->x;
	* (unsigned*) fb_frame = color;
}

void framebuffer_repr(framebuffer_t* fb) {
	printf("%dx%d\n", fb->width, fb->height);
	for(int i = 0; i < fb->height; i++) {
		for(int j = 0; j < fb->width; j++) {
			if(framebuffer_px(fb, j, i) != 0)
				putchar('X');
			else
				putchar('0');
		}
		putchar('\n');
	}
}

int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	int dx = (p2->x - p1->x);
	double m = (double) (p2->y - p1->y) / (double) dx;
	double true_y = 0.0;
	point_t pt = {.x = p1->x, .y = p1->y};
	for(int t = 0; t <= dx; t++) {
		pt.x += 1;
		true_y += m;
		pt.y += (int) m;
		set_px(fb, color, &pt);
	}
}

int main() {
	framebuffer_t* fb = framebuffer_init(10, 15);
	point_t px1 = {.x = 5, .y = 5};
	point_t px2 = {.x = 10, .y = 10};
	set_px(fb, 255, &px1);
	framebuffer_repr(fb);
	return 0;
}
