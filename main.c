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
    double vol;
    int id;
    vec v;
    vec a;
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
    print_circle(o->x, o->y, o->vol, color);
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
    delay.tv_nsec = 1100000;
    delay.tv_sec = 0;
    vec v0 = init_vec(0, 0);
    vec v1 = init_vec(1, -1);
    vec v2 = init_vec(2,-1);
    vec a0 = init_vec(0, 0);
    vec a1 = init_vec(2, -5);
    vec a2 = init_vec(8,8);
    obj b[10];
    b[0] = init_obj((double) x_c,(double) y_c, 1000.0, 0.3,  v0, a0, 0);
    b[1] = init_obj(310.0, 210.0, 2.0, 0.2,v1, a1, 1);
    b[2] = init_obj(150.0, 250.0, 2.0, 0.1, v2, a2, 2);
 
    double t = 0.0;

    clear();
    
    while(1)
    {
        atract_o(b[0], b[1]);
        //atract_o(b[1], b[0]);
        atract_o(b[0], b[2]);
        atract_o(b[1], b[0]);
        atract_o(b[1], b[2]);
        atract_o(b[2], b[0]);
        atract_o(b[2], b[1]);

        apply_forces(b[0], 0.1);
        apply_forces(b[1], 0.1);
        apply_forces(b[2], 0.1);
        
        apply_v(b[0], 0.1);
        apply_v(b[1], 0.1);
        apply_v(b[2], 0.1);
        
        draw_obj(b[0], 0x00ff0000); 
        draw_obj(b[1], 0x0000ff00);
        draw_obj(b[2], 0x000000ff);
        nanosleep(&delay, NULL);
        //clear_obj(b[0]);
        //clear_obj(b[1]);
        //clear_obj(b[2]);

        t+=1;
    }
    
    return 0;
}
