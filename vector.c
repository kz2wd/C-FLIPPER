#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

void copy_vector(vector* destination, vector* source){
    destination->x = source->x;
    destination->y = source->y;
}

vector rotated_vector(vector* v, float theta){
    float cos_theta = cosf(theta);
    float sin_theta = sinf(theta);
    vector rotated;
    rotated.x = v->x * cos_theta - v->y * sin_theta;
    rotated.y = v->x * sin_theta + v->y * cos_theta;
    return rotated;
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

float distance_point_and_line(vector* p1, vector* l1, vector* l2, bool infinite_line){
    if (infinite_line){
        // ref : https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
        return ((float) fabs((double) (l2->x - l1->x)* (l1->y - p1->y) - (l1->x - p1->x)* (l2->y - l1->y))) / vectors_distance(l1, l2);
    } else {
        // ref : https://www.youtube.com/watch?v=egmZJU-1zPU
        vector ab = substract_vectors(l2, l1);
        vector ap = substract_vectors(p1, l1);
        float proj = dot_product(&ap, &ab);
        float abLen = vectors_distance(l1, l2);
        float d = proj / (abLen * abLen);
        if (d < 0){
            return vectors_distance(l1, p1);
        } else if (d > 1){
            return vectors_distance(l2, p1);
        } 
        vector scaled = scaled_vector(&ab, d);
        vector closest = add_vectors(l1, &scaled);
        return vectors_distance(&closest, p1);
    }
}

float dot_product(vector *v1, vector* v2){
    return v1->x * v2->x + v1->y * v2->y;
}

vector normalized_vector(vector* v){
    return scaled_vector(v, 1 / vector_norm(v));
}
