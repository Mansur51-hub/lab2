//
// Created by Mansur51 on 26.10.2020.
//
#include <math.h>
#include <assert.h>
#ifndef LAB11_CALC_H
#define LAB11_CALC_H

typedef struct {
    double first_length;
    double second_length;
    double sin_fi;
} Parallelogram;
double perimeter(Parallelogram parallelogram){
    return 2 * (parallelogram.first_length + parallelogram.second_length);
}
double square (Parallelogram parallelogram){
    return parallelogram.first_length * parallelogram.second_length * parallelogram.sin_fi;
}

typedef struct {
    double x;
    double y;
} Cor;

Parallelogram create_parallelogram (Cor cor_1, Cor cor_2, Cor cor_3){
    Parallelogram parallelogram;
    double dx1 = cor_1.x - cor_2.x;
    double dy1 = cor_1.y - cor_2.y;
    parallelogram.first_length = sqrt(dx1 * dx1 + dy1 * dy1);
    double dx2 = cor_3.x - cor_2.x;
    double dy2 = cor_3.y - cor_2.y;
    parallelogram.second_length = sqrt(dx2 * dx2 + dy2 * dy2);
    assert(parallelogram.first_length > 0.00000000001);
    assert(parallelogram.second_length > 0.00000000001);
    double cos_fi = (dx1 * dx2 + dy1 * dy2) / (parallelogram.first_length * parallelogram.second_length);
    parallelogram.sin_fi = sqrt(1.0 - cos_fi * cos_fi);
    return parallelogram;
}
#endif //LAB11_CALC_H
