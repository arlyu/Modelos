
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
    double vol;
    int id;
    vec v;
    vec a;
    vec * app;
    int * appids;
    int capp;
} Obj;

void add_vec(vec to_add, vec adder)
{
    assert(to_add != NULL && adder != NULL);
    to_add -> x += adder->x;
    to_add -> y += adder->y;
}

void scale_vec(vec w, double c)
{
    w->x *= c;
    w->y *= c;
}

static void add_scaled_vec(vec v0, vec v1, double c)
{
    v1->x = v0->x * c;
    v1->y = v0->y * c;
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

obj init_obj(double x, double y, double m, double vol, vec v, vec a, int id)
{
    obj o = malloc(sizeof(struct obj_t));

    o->x = x;
    o->y = y;
    o->m = m;
    o->vol = vol;
    o->v = v;
    o->a = a;

    o->app = NULL;
    o->appids = NULL;
    o->capp = 0;
    
    return o;
}

double vec_len(vec v)
{
    return sqrt((v->x)*(v->x)+(v->y)*(v->y));
}

void apply_vector(obj o, vec w, double t)
{
    o->x += (w->x) * t / o->m;
    o->y += (w->y) * t / o->m;
}

void apply_v(obj o, double t)
{
    apply_vector(o, o->v, t);
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

void result(obj o, vec res)
{
    for (int i = 0; i < o->capp; ++i)
    {
        add_vec(res, o->app[i]);
    }
    
    add_vec(res, o->a);
}

void atract_o(obj o, obj s)
{
    vec b = init_vec((s->x)-(o->x),(s->y)-(o->y));
    ++(o->capp);
    
    if(vec_len(b) <= 100*(s->vol+o->vol))
    {
        b->x = 0;
        b->y = 0;

        result(s,b);
        //add_vec(b,s->a);
        scale_vec(b,-1);
    }
    else
    {
        scale_vec(b,(o->m * s->m)/(obj_dist(o,s)*obj_sqdist(o,s)));

        //b->x = (o->m * s->m) * b->x/(obj_dist(o,s)*obj_sqdist(o,s));
        //b->y = (o->m * s->m) *  b->y/(obj_dist(o,s)*obj_sqdist(o,s));
    }
    
    int capp = o->capp;
    o->app = realloc(o->app, sizeof(vec)*capp);
    o->appids = realloc(o->appids, sizeof(int) * capp);

    o->app[capp-1] = b;
    o->appids[capp-1] = s->id;
}

void apply_forces(obj o, double t)
{
    vec res = init_vec(0,0);

    for (int i = 0; i < o->capp; ++i)
    {
        add_vec(res, o->app[i]);
        o->appids[i] = -1;
        free(o->app[i]);
        o->app[i] = NULL;
    }

    o->capp = 0;

    add_vec(res, o->a);
    scale_vec(res, t/o->m);
    free(o->a);

    o->a = res;
    add_vec(o->v, res);
}
