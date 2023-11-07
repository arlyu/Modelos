
#include <stdio.h>
#include "objs.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

typedef struct vec_t
{
    double x;
    double y;
} Vec;

typedef struct obj_t {
    double x;
    double y;
    double m;
    vec v;
    vec * app;
    int capp;
} Obj;

void add_vec(vec to_add, vec adder)
{
    assert(to_add != NULL && adder != NULL);
    to_add -> x += adder->x;
    to_add -> y += adder->y;
}

void add_app(vec v, vec * app, int capp)
{
    for (int i = 0; i < capp; ++i)
    {
        add_vec(v, app[i]);   
    }
}

vec init_vec(double x, double y)
{
    vec v = malloc(sizeof(struct vec_t));
    v->x = x;
    v->y = y;

    return v;
}

obj init_obj(double x, double y, double m, vec v)
{
    obj o = malloc(sizeof(struct obj_t));

    o->x = x;
    o->y = y;
    o->m = m;
    o->v = v;

    o->app = NULL;
    o->capp = 0;
    
    return o;
}

double vec_len(vec v)
{
    return sqrt((v->x)*(v->x)+(v->y)*(v->y));
}

void apply_v(obj o, double t)
{
    o->x += (o->v->x) * t / o->m;
    o->y += (o->v->y) * t / o->m;
}

static double obj_sqdist(obj o, obj s)
{
    double x = o->x - s->x;
    double y = o->y - s->y;

    return x*x+y*y;
}

double obj_dist(obj o, obj s)
{
    return sqrt(obj_sqdist(o,s));
}

void atract_o(obj o, obj s)
{
    vec b = init_vec((o->x)-(s->x),(o->y)-(s->y));
    o->app = realloc()


}
