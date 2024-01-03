#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct vector {
    float x;
    float y;
} vector;


vector add_vectors(vector* v1, vector* v2);

vector substract_vectors(vector* v1, vector* v2);

vector scaled_vector(vector* v, float scale);

void copy_vector(vector* destination, vector* source);

vector rotated_vector(vector* v, float theta);

void print_vector(vector* v);

float vector_norm(vector* v);

float vectors_distance(vector* v1, vector* v2);

float distance_point_and_line(vector* p1, vector* l1, vector* l2, bool infinite_line);

float dot_product(vector *v1, vector* v2);

vector normalized_vector(vector* v);

#endif
