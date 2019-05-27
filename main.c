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
	return a << 24 | b << 16 | g << 8 | r;
}

uint8_t rgba32_channel(unsigned color, const char channel) {
	unsigned value;
	switch(channel) {
		case 'a':
			value = color >> 24;
			break;
		case 'b':
			value = color >> 16;
			break;
		case 'g':
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
	stbi_write_bmp("framebuffer.bmp", fb->width, fb->height, 4, fb->fb);
}

unsigned multiply_alpha(unsigned color, double alpha) {
	double normalized_alpha = alpha * ((double) rgba32_channel(color, 'a') / 255);
	return rgba32(rgba32_channel(color, 'r'), rgba32_channel(color, 'g'), rgba32_channel(color, 'b'), (uint8_t) (normalized_alpha * 255));
}


static inline int round_up(double x) {
	return (int) (((double) (x)) + 0.5);
}

static inline int int_part(double x) {
	return (int) x;
}

static inline double frac_part(double x) { 
	return x - int_part(x);
}

int draw_aaline_steep(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	double dx = p2->x - p1->x;
	double dy = p2->y - p1->y;
	double slope = dy / dx;
	double true_x = p1->x;
	point_t line_px1, line_px2;
	line_px1.x = p1->x;
	line_px1.y = p1->y;
	int shift;
	if(slope > 1)
		shift = 1;
	else
		shift = -1;
	line_px2.x = p1->x + shift;
	line_px2.y = p1->y;
	for(int t = p1->y; t <= p2->y; t++) {
		true_x += slope;
		line_px1.x = int_part(true_x);
		line_px1.y = t;
		line_px2.x = int_part(true_x) + shift;
		line_px2.y = t;
		set_px(fb, color, &line_px1);
		set_px(fb, color, &line_px2);
	}
	return 1;
}

int draw_aaline_shallow(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	double dx = p2->x - p1->x;
	double dy = p2->y - p1->y;
	double slope = dy / dx;
	double true_x = p1->x;
	point_t line_px1, line_px2;
	line_px1.x = p1->x;
	line_px1.y = p1->y;
	int shift;
	if(slope > 1)
		shift = 1;
	else
		shift = -1;
	line_px2.x = p1->x + shift;
	line_px2.y = p1->y;
	for(int t = p1->y; t <= p2->y; t++) {
		true_x += slope;
		line_px1.x = int_part(true_x);
		line_px1.y = t;
		line_px2.x = int_part(true_x) + shift;
		line_px2.y = t;
		set_px(fb, color, &line_px1);
		set_px(fb, color, &line_px2);
	}
	return 1;
}

int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	double dx = p2->x - p1->x;
	double dy = p2->y - p1->y;
	if(fabs(dx) < fabs(dy))
		if(p2->x < p1->x)
			return draw_aaline_shallow(fb, color, p2, p1);
		else
			return draw_aaline_shallow(fb, color, p1, p2);
	else
		if(p2->y < p1->y)
			return draw_aaline_steep(fb, color, p2, p1);
		else
			return draw_aaline_steep(fb, color, p1, p2);
}

static inline int sign(double d) {
	if(d < 0)
		return -1;
	else
		return 1;
}

int draw_line(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2) {
	double dx = p2->x - p1->x;
	double dy = p2->y - p1->y;
	double derror = abs(dy/dx);
	double error = 0;
	point_t pt;
	pt.y = p1->y;
	for(int t = p1->x; t <= p2->x; t++) {
		pt.x = t;
		set_px(fb, color, &pt);
		error += derror;
		if(error >= 0.5) {
			pt.y += sign(dy);
			error -= 1.0;
		}
	}
	return 1;
}

int main() {
	framebuffer_t* fb = framebuffer_init(100, 100);
	point_t px1 = {.x = 0, .y = 0};
	point_t px2 = {.x = 99, .y = 50};
	//make the background green
	point_t pxi;
	for(int i = 0; i < fb->width; i++) {
		for(int j = 0; j < fb->height; j++) {
			pxi.x = i;
			pxi.y = j;
			set_px(fb, rgba32(255, 0, 0, 255), &pxi);
		}
	}
	draw_aaline(fb, rgba32(255, 255, 255, 255), &px1, &px2);
	write_bmp(fb);
	return 0;
}
