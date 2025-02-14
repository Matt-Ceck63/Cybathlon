#ifndef KINEMATICS_H
#define KINEMATICS_H
#include <Arduino.h>
#include <math.h>
#define L1 700 //mm
#define L2 500 //mm
#define MAX_LENGTH (L1+L2)

// #define TESTING

#ifdef TESTING
    #include <stdio.h>
#endif

typedef struct{
    double q1;
    double q2;
} angles;

typedef struct{
    double x;
    double y;
} coordinates;

coordinates forward_kinematics_planar2R(angles joints);
angles inverse_kinematics_planar2R(coordinates position);
angles toDegrees(angles input);
angles toRad(angles input);
bool check_circular_workspace(coordinates coordinates_to_check);

#endif