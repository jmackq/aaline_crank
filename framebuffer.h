#define RGBA_SZ sizeof(int) 

typedef struct {
	void* fb;
	int width;
	int height;
} framebuffer_t;

typedef struct {
	int x;
	int y;
} point_t;

int draw_aaline(framebuffer_t* fb, unsigned color, point_t* p1, point_t* p2);

framebuffer_t* framebuffer_init(int w, int h);
unsigned framebuffer_px(framebuffer_t* fb, int x, int y);
void framebuffer_repr(framebuffer_t* fb);
void framebuffer_dump(framebuffer_t* fb);
