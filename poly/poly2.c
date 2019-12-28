#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "poly.h"

struct poly_t {
    int size;
    struct pair *pairs;
};

struct pair {
    int f;
    int e;
};

poly_t* new_poly_from_string(const char* str) {
    struct pair *pairs = malloc(sizeof(struct pair) * 10);
    int f = 0;
    int e = 0;
    int sign = 1;
    bool before = true;
    int counter = 0;
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (isdigit(c)) {
            if (before) {
                f = 10*f + (c - '0');
            } else {
                e = 10*e + (c - '0');
            }
        } else if (c == '-' || c == '+') {
            sign = -(c - 44);
        } else if (c == 'x') {
            if (f == 0)
                f = 1;
            before = false;
        } else if (f != 0 && c == ' ') {
            if (!before && e == 0)
                e = 1;
            f = sign*f;
            struct pair p = {f, e};
            pairs[counter] = p;
            f = 0;
            e = 0;
            before = true;
            counter++;
        }
    }
    f = sign*f;
    struct pair p = {f, e};
    pairs[counter] = p;
    counter++;
    struct poly_t* ret = (struct poly_t*) malloc(sizeof(struct poly_t));
    ret->pairs = pairs;
    ret->size = counter;
    return ret;
}

void free_poly(poly_t *poly) {
    free(poly->pairs);
    free(poly);
}

poly_t* mul(poly_t* a, poly_t* b) {
    struct pair *pairs = NULL;
    int size = 0;
    for(int i = 0; i < a->size; i++) {
        for(int j = 0; j < b->size; j++) {
            int e = a->pairs[i].e + b->pairs[j].e;
            int f = a->pairs[i].f * b->pairs[j].f;
            bool exists = false;
            for(int k = 0; k < size; k++) {
                if ((*(pairs + k)).e == e) {
                    (*(pairs + k)).f += f;
                    exists = true;
                }
            }
            if (!exists) {
                pairs = realloc(pairs, sizeof(struct pair) *(size + 1));
                struct pair p = {f, e};
                *(pairs + size++) = p;
            }
        }
    }
    struct poly_t* ret = (struct poly_t*) malloc(sizeof(struct poly_t));
    ret->pairs = pairs;
    ret->size = size;
    return ret;
}

void print_poly(poly_t *poly) {
    for (int i = 0; i < poly->size; i++) {
        if (i != 0) {
            printf(" %c ", (poly->pairs[i].f > 0) ? '+' : '-');
        }
        if (abs(poly->pairs[i].f) != 1 || poly->pairs[i].e == 0) printf("%d", abs(poly->pairs[i].f));
        if (poly->pairs[i].e > 1) {
            printf("x^%d", poly->pairs[i].e);
        } else if (poly->pairs[i].e == 1) {
            printf("x");
        }
    }
    printf("\n");
}