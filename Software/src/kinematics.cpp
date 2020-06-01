#include <kinematics.h>

coordinates forward_kinematics_planar2R(angles joints){
    coordinates result;
    joints = toRad(joints);
    result.x = L1 * cos(joints.q1) + L2 * cos(joints.q1 + joints.q2);
    result.y = L1 * sin(joints.q1) + L2 * sin(joints.q1 + joints.q2);
    return result;
}

angles inverse_kinematics_planar2R(coordinates position){
    angles result;
    result.q2 = acos((pow(position.x, 2) + pow(position.y, 2) - pow(L1, 2) - pow(L2, 2))/
                                (2*L1*L2));
    result.q1 = atan2(position.y, position.x) -          asin((L2*sin(result.q2))/
                                                sqrt(pow(position.x,2) + pow(position.y, 2)));
    return toDegrees(result);
}

angles toDegrees(angles input){
    return {(input.q1)*180/PI, (input.q1)*180/PI};
}

angles toRad(angles input){
    return{input.q1*DEG_TO_RAD, input.q2*DEG_TO_RAD};
}

bool check_circular_workspace(coordinates coordinates_to_check){
    double x = coordinates_to_check.x;
    double y = coordinates_to_check.y;

    if (pow(y, 2) > (pow((L1 - L2), 2) - pow(x,2)) && (pow(y,2) < (pow((L1 + L2), 2) - pow(x,2)))){
        return true;
    }
    
    return false;
}

#ifdef TESTING

int main(){

    coordinates coord;
    angles angles1;
    coordinates fk_results;
    coord.x = 1200.0;
    coord.y = 1200.0;

    printf("x: %f, y: %f", coord.x, coord.y);

    if (check_circular_workspace(coord)){
        angles1 = inverse_kinematics_planar2R(coord);
        printf("q1: %f, q2: %f", angles1.q1, angles1.q2);
        fk_results = forward_kinematics_planar2R(angles1);
    }

    printf("x: %f, y: %f", fk_results.x, fk_results.y);

}

#endif