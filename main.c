#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <time.h>
#include "objs.h"
#include <stdlib.h>

char *base;
int w;
int h;

int x_c;
int y_c;

typedef struct vec_t
{
    double x;
    double y;
} Vec;

typedef struct obj_t {
    double x;
    double y;
    double m;
    int id;
    vec v;
    vec * app;
    int * appids;
    int capp;
} Obj;

int car(int x, int y)
{
    return 4*((h-y)*w + x);
}

void pix(int x, int y, int color)
{
    int *real = (int *) (base + car(x,y));
    *real = color;
}

int is_inside(int x, int y)
{
    return (x*x+y*y<= 100*100 && x*x+y*y>=50*50);
}

int is_safe(int x, int y)
{
    return (x < w && x >= 0 && y < h && y >= 0);
}

void print_circle(double x, double y, double r, unsigned int color)
{
    int x1 = 1;
    int y1 = 1;

    for (double y0 = -r; y0 < r; y0 += 0.01)
    {
        for (double x0 = -r; x0 < r; x0 += 0.01)
        {
            x1 = (int)(x+x0*100);
            y1 = (int)(y+y0*100);

            if(is_safe(x1, y1) && x0*x0+y0*y0<=r*r)
            {
                pix(x1, y1, color);
            }
        }
    }

}

void draw_obj(obj o, unsigned int color)
{
    print_circle(o->x, o->y, 0.1, color);
}

void clear()
{
    int *real = NULL;
    real = (int *) base;
    
    while ( real < (int *) (base + 4*h*w))
    {
        *real = 0;
        ++real;
    }
}

void clear_obj(obj o)
{
    draw_obj(o, 0x0);
}

int main()
{
    int fb = open("/dev/fb0", O_RDWR);

    if(fb == -1)
    {
        printf("Shit\n");
        return -1;
    }

    struct fb_var_screeninfo vin;

    if(ioctl(fb, FBIOGET_VSCREENINFO, &vin))
    {
        printf("Fuck2\n");
        close(fb);
        return -1;
    }

    int qsize = 200;
    x_c = vin.xres/2;
    y_c = vin.yres/2;

    int ssize = vin.yres_virtual * vin.xres_virtual * vin.bits_per_pixel / 8;
    char *ptr = (char *) mmap(0, ssize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

    if((int)ptr == -1)
    {
        printf("Fuck3\n");
        close(fb);
        return -1;
    }

    base = ptr;
    w = vin.xres;
    h = vin.yres;

    printf("bpp: %d\tyv:%d\ty:%d\n", vin.bits_per_pixel, vin.yres_virtual, vin.yres);

    ptr += car(20,20);
    int offx = x_c;
    int offy = y_c;
    struct timespec delay;
    delay.tv_nsec = 1000000;
    delay.tv_sec = 0;
    vec v = init_vec(0.2, 1);
    obj b[10];
    b[0] = init_obj(150.0, 150.0, 20.0, v, 0);
    b[1] = init_obj(210.0, 310.0, 220.0, NULL, 1);

    double t = 0.0;

    clear();
    
    while(t < 100)
    {
        atract_o(b[0], b[1]);
        apply_forces(b[0], 0.01);
        draw_obj(b[0], 0x00ff0000); 
        draw_obj(b[1], 0x0000ff00);

        clear_obj(b[0]);
        clear_obj(b[1]);

        //nanosleep(&delay, NULL);
        //++t;
    }
    
    return 0;
}
