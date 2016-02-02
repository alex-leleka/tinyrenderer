#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "MathTemplates.h"
#include <vector>
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

float inline lerp(float v0, float v1, float t) {
  return (1-t)*v0 + t*v1;
}
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;

        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

void lineBresenham(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	int D = 2 * dy - dx;
	//image.set(x1, y1, color);

	if(dx > dy)
	{
		if(x0 > x1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		int y = y0;
		if (D > 0)
		{
			y = y + 1;
			D = D - 2*dx;
		}
		image.set(x0, y0, color);
		for(int x = x0 + 1; x <= x1; ++x )
		{
			image.set(x, y, color);
			D = D + 2*dy;
			if (D > 0)
			{
				y = y + 1;
				D = D - 2*dx;
			}
		}
	}
	else
	{
		if(y0 > y1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		int x = x0;
		if (D > 0)
		{
			x = x + 1;
			D = D - 2*dy;
		}
		image.set(x0, y0, color);
		for(int y = y0 + 1; y <= y1; ++y )
		{
			image.set(x, y, color);
			D = D + 2*dx;
			if (D > 0)
			{
				x = x + 1;
				D = D - 2*dy;
			}
		}
	}
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

