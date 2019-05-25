#include <stdint.h>

typedef struct {
	void* fb;
	int width;
	int height;
} framebuffer_t;

typedef struct {
	int x;
	int y;
} point_t;

unsigned rgba32(int8_t r, int8_t g, int8_t b, int8_t a);

void set_px(framebuffer_t* fb, unsigned color, point_t* px);
int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2);

unsigned framebuffer_px(framebuffer_t* fb, point_t* px);
void framebuffer_repr(framebuffer_t* fb);
void framebuffer_dump(framebuffer_t* fb);
