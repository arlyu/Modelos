#ifndef OBJS_H
#define OBJS_H

typedef struct obj_t * obj;
typedef struct vec_t * vec;

void add_vec(vec to_add, vec adder);
void add_app(vec v, vec * app, int capp);
vec init_vec(double x, double y);
double vec_len(vec v);
obj init_obj(double x, double y, double m, vec v);
void apply_v(obj o, double t);
double obj_dist(obj o, obj s);

#endif
