
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
