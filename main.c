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

uint8_t rgba32_channel(unsigned color, char channel) {
	unsigned value;
	switch(channel) {
		case 'r':
			value = color >> 24;
			break;
		case 'g':
			value = color >> 16;
			break;
		case 'b':
			value = color >> 8;
			break;
		default:
			value = color;
	}
	value = value & 0xff;
	return (uint8_t) value;
}

unsigned alpha_over(unsigned top, unsigned bot) {
	//linear alpha blending
	double normalized_alpha  = (double) rgba32_channel(top, 'a') / 255;
	double blended_red, blended_green, blended_blue;
	blended_red = (rgba32_channel(top, 'r') * normalized_alpha);
	blended_red += (rgba32_channel(bot, 'r') * (1 - normalized_alpha));
	blended_green = (rgba32_channel(top, 'g') * normalized_alpha);
	blended_green += (rgba32_channel(bot, 'g') * (1 - normalized_alpha));
	blended_blue = (rgba32_channel(top, 'b') * normalized_alpha);
	blended_blue += (rgba32_channel(bot, 'b') * (1 - normalized_alpha));
	return rgba32((uint8_t) blended_red, (uint8_t) blended_green, (uint8_t) blended_blue, 0xff);
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
	unsigned top_color, bot_color;
	for(int t = p1->x; t <= p2->x; t++) {
		top_subpx.x = t;
		top_subpx.y = (int) y_t;
		bot_subpx.x = t; 
		bot_subpx.y = (int) y_t - 1;	
		top_color = alpha_over(color, framebuffer_px(fb, &top_subpx));
		bot_color = alpha_over(color, framebuffer_px(fb, &bot_subpx));
		set_px(fb, top_color, &top_subpx);
		set_px(fb, bot_color, &bot_subpx);
		y_t += m;
	}
	return 1;
}

int main() {
	/*
	framebuffer_t* fb = framebuffer_init(100, 100);
	point_t px1 = {.x = 25, .y = 25};
	point_t px2 = {.x = 75, .y = 75};
	//make the background green
	point_t pxi;
	for(int i = 0; i < fb->width; i++) {
		for(int j = 0; j < fb->height; j++) {
			pxi.x = i;
			pxi.y = j;
			set_px(fb, rgba32(0, 255, 0, 0), &pxi);
		}
	}
	draw_aaline(fb, rgba32(255, 255, 255, 255), &px1, &px2);
	write_bmp(fb);
	return 0;*/
	framebuffer_t* fb = framebuffer_init(256, 256);
	unsigned bg, fg;
	bg = rgba32(255, 5, 10, 255); // should be red
	assert(rgba32_channel(bg, 'r') == 255);
	assert(rgba32_channel(bg, 'g') == 5);
	assert(rgba32_channel(bg, 'b') == 10);
	assert(rgba32_channel(bg, 'a') == 255);
	fg = rgba32(5, 10, 255, 100); // should be translucent blue
	unsigned blended = alpha_over(fg, bg);
	printf("(%u, %u, %u, %u)\n", rgba32_channel(blended, 'r'),
			rgba32_channel(blended, 'g'),
			rgba32_channel(blended, 'b'),
			rgba32_channel(blended, 'a'));
}
