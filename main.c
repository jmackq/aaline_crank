#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "framebuffer.h"

framebuffer_t* framebuffer_init(int w, int h) {
	int fb_sz = w * h * sizeof(unsigned);
	unsigned* fb_frame = malloc(fb_sz);
	memset(fb_frame, '\0', fb_sz);
	framebuffer_t* fb = malloc(sizeof(framebuffer_t));
	fb->fb = fb_frame;
	fb->width = w;
	fb->height = h;
	return fb;
}

unsigned framebuffer_px(framebuffer_t* fb, point_t* px) {
	unsigned* fbuf = (unsigned*) fb->fb;
	return fbuf[(fb->width * px->y) + px->x];
}

void set_px(framebuffer_t* fb, unsigned color, point_t* px) {
	unsigned* fbuf = (unsigned*) fb->fb;
	fbuf[(fb->width * px->y) + px->x] = color;
}

void framebuffer_repr(framebuffer_t* fb) {
	unsigned* fbuf = (unsigned*) fb->fb;
	printf("%dx%d\n", fb->width, fb->height);
	for(int i = 0; i < fb->height; i++) {
		for(int j = 0; j < fb->width; j++) {
			printf("%d", fbuf[i * fb->width + j]);
		}
		putchar('\n');
	}
}

//not actually antialiased yet
int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	int dx = (p2->x - p1->x);
	double m = (double) (p2->y - p1->y) / (double) dx;
	double true_y = 0.0;
	point_t pt = {.x = p1->x, .y = p1->y};
	for(int t = 0; t <= dx; t++) {
		set_px(fb, color, &pt);
		pt.x += 1;
		true_y += m;
		pt.y = (int) true_y;
	}
}

int main() {
	framebuffer_t* fb = framebuffer_init(10, 10);
	point_t px1 = {.x = 0, .y = 0};
	point_t px2 = {.x = 9, .y = 3};
	draw_aaline(fb, 1, &px1, &px2);
	framebuffer_repr(fb);
	return 0;
}
