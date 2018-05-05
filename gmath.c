#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  //normalize
  normalize(view);
  normalize(normal);
  normalize(light[LOCATION]);
  //colors!
  color ambient = calculate_ambient(alight, areflect);
  color diffuse = calculate_diffuse(light, dreflect, normal);
  color specular = calculate_specular(light, sreflect, view, normal);
  i.red = ambient.red + diffuse.red + specular.red;
  i.green = ambient.green + diffuse.green + specular.green;
  i.blue = ambient.blue + diffuse.blue + specular.blue;
  
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * *areflect;
  a.green = alight.green * *areflect;
  a.blue = alight.blue * *areflect;
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double tmp = dot_product(normal, light[LOCATION]);
  d.red = light[COLOR][RED] * *dreflect * tmp;
  d.green = light[COLOR][GREEN] * *dreflect * tmp;
  d.blue = light[COLOR][BLUE] * *dreflect * tmp;
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double *tmp = scalarize(dot_product(normal, light[LOCATION])*2, normal);
  tmp = subtract(tmp, light[LOCATION]);
  double tmp2 = pow(dot_product(tmp, view), 8);
  s.red = light[COLOR][RED] * *sreflect * tmp2;
  s.green = light[COLOR][GREEN] * *sreflect * tmp2;
  s.blue = light[COLOR][BLUE] * *sreflect * tmp2;
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  //ternary ops
  c->red = c->red > 255 ? 255 : c->red;
  c->green = c->green > 255 ? 255 : c->green;
  c->blue = c->blue > 255 ? 255 : c->blue;
}

//vector functions
//normalize vector, should modify the parameter
void normalize( double *vector ) {
  double mag = 0;
  int i;
  for (i = 0; i < 3; i ++){
    mag += vector[i] * vector[i]; 
  }
  mag = sqrt(mag);
  for (i = 0; i < 3; i ++){
    vector[i] = vector[i] /mag;
  }
}

//Return the dot product of a . b
double dot_product( double *a, double *b ) {
  double dot = 0;
  int i;
  for (i = 0; i < 3; i ++){
    dot += a[i] * b[i];
  }
  return dot;
}

//scalarize a vector
double *scalarize( double x, double *v ) {
    int i;
    for (i = 0; i < 3; i++) {
        v[i] = x * v[i];
    }
    return v;
}

//vector subtraction
double *subtract( double *v0, double *v1 ) {
    int i;
    for (i = 0; i < 3; i++) {
        v0[i] = v0[i] - v1[i];
    }
    return v0;
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
