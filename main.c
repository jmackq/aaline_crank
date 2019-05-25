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

unsigned rgba32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return r << 24 | g << 16 | b << 8 | a;
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
			printf("%u", fbuf[i * fb->width + j]);
		}
		putchar('\n');
	}
}

void write_bmp(framebuffer_t* fb) {
	stbi_write_jpg("framebuffer.jpg", fb->width, fb->height, 4, fb->fb, 100);
}

int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	double dx = p2->x - p1->x;
	double dy = p2->y - p1->y;
	double m;
	double y_t = p1->y;
	if(dx == 0.0)
		m = 1;
	else
		m = dy/dx;
	point_t top_subpx, bot_subpx;
	for(int t = p1->x; t <= p2->x; t++) {
		//color should be alpha * frac part
		//TODO: add OVER blending
		top_subpx.x = t;
		top_subpx.y = (int) y_t;
		bot_subpx.x = t; 
		bot_subpx.y = (int) y_t - 1;	
		set_px(fb, color, &top_subpx);
		set_px(fb, color, &bot_subpx);
		y_t += m;
	}
	return 1;
}

int main() {
	framebuffer_t* fb = framebuffer_init(100, 100);
	point_t px1 = {.x = 25, .y = 25};
	point_t px2 = {.x = 75, .y = 75};
	draw_aaline(fb, rgba32(255, 0, 255, 255), &px1, &px2);
	framebuffer_repr(fb);
	write_bmp(fb);
	return 0;
}
