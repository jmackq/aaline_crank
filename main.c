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

unsigned framebuffer_px(framebuffer_t* fb, int x, int y) {
	void* fb_frame = fb->fb;
	fb_frame += RGBA_SZ * fb->width * y;
	fb_frame += RGBA_SZ + x;
	return *((unsigned*) fb_frame);
}

void framebuffer_repr(framebuffer_t* fb) {
	printf("%dx%d\n", fb->width, fb->height);
	for(int i = 0; i < fb->height; i++) {
		for(int j = 0; j < fb->width; j++) {
			putchar(framebuffer_px(fb, j, i));
		}
		putchar('\n');
	}
}

int main() {
	framebuffer_t* fb = framebuffer_init(10, 15);
	framebuffer_repr(fb);
	return 0;
}
