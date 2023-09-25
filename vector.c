#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

vector add_vectors(vector* v1, vector* v2){
    vector result = { v1->x + v2->x, v1->y + v2->y };
    return result;
}

vector substract_vectors(vector* v1, vector* v2){
    vector result = { v1->x - v2->x, v1->y - v2->y };
    return result;
}

vector scaled_vector(vector* v, float scale){
    vector result = { v->x * scale, v->y * scale };
    return result;
}

void print_vector(vector* v){
    printf("x: %f, y: %f\n", v->x, v->y);
}

float vector_norm(vector* v){
    return sqrtf( v->x * v->x + v->y * v->y);
}

float vectors_distance(vector* v1, vector* v2){
    float x_diff = (v2->x - v1->x);
    float y_diff = (v2->y - v1->y);
    return sqrtf( x_diff * x_diff + y_diff * y_diff);
}

float distance_point_and_line(vector* p1, vector* l1, vector* l2){
    // ref : https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
    return ((float) fabs((double) (l2->x - l1->x)* (l1->y - p1->y) - (l1->x - p1->x)* (l2->y - l1->y))) / vectors_distance(l1, l2);
}

float dot_product(vector *v1, vector* v2){
    return v1->x * v2->x + v1->y * v2->y;
}

vector normalized_vector(vector* v){
    return scaled_vector(v, 1 / vector_norm(v));
}
