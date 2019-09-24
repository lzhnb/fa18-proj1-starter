/*
 * Project 1-1: Computing a Displacement Map
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement)
            / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;
}

/* Helper function to return the square euclidean distance between two values. */
unsigned int square_euclidean_distance(unsigned char a, unsigned char b) {
   /*
    * This is an optional helper function which you may find useful. It
    * currently has an incomplete example CUnit test to help with debugging.
    * This test is not graded and is merely for your convenience. See the spec
    * for details on how to use CUnit.
    */
    return pow(a-b, 2);
}

void fill_feature(unsigned char *compare, unsigned char *feature, unsigned char *right,
                  int *min_dis, int *dx, int *dy,
                  int feature_width, int feature_height, int feature_area,
                  int image_height, int image_width, int right_index, int center,
                  int x_index, int y_index, 
                  int j_index, int k_index){
    int a, b, i, dis = 0;
    for(b = -feature_height; b<=feature_height; b++){
        for(a = -feature_width; a<=feature_width; a++){
            if(y_index + k_index + b < 0 ||
               y_index + k_index + b >= image_height ||
               x_index + j_index + a < 0 ||
               x_index + j_index + a >= image_width){
                   feature[center + b * (2 * feature_width + 1) + a] = 0;
            }
            else{
                   feature[center + b * (2 * feature_width + 1) + a] = 
                    right[right_index + (k_index + b) * image_width + (j_index + a)];
            }
        }
    }
     // finish a sample
    for(i=0; i<feature_area; i++){ //accumulation s_e_dis
        dis += square_euclidean_distance(compare[i], feature[i]);
    }

    if( *min_dis == -1 || // first sample point
        dis < *min_dis || // best e_l_distance
        (dis == *min_dis && pow(j_index, 2) + pow(k_index, 2) < pow(*dx, 2) + pow(*dy, 2))|| // same dis but closer
        (dis == *min_dis && (pow(j_index, 2) + pow(k_index, 2) == pow(*dx, 2) + pow(*dy, 2)) // same dis but inside
        && (x_index + *dx < 0 || x_index + *dx >= image_width ||
        y_index + *dy < 0 || y_index + *dy >= image_height))){ 
        *min_dis = dis;
        *dx = j_index;
        *dy = k_index;
    }
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {
    /* YOUR CODE HERE */
    int x, y, j, k, a, b, i,
        dx = 0, dy = 0; // record the most similar point's bias
    int feature_area = (2 * feature_height + 1) * (2 * feature_width + 1);
    int left_index,  // point to center in left
        right_index, // point to center in right
        center = (feature_area - 1)/2, // center of feature
        min_dis;     // record min dis
        // init left's compare
    unsigned char compare[feature_area];
        // init right's feature
    unsigned char feature[feature_area];
    if(maximum_displacement == 0){
        for(i = 0; i < image_width*image_height; i++)
        depth_map[i] = 0;
    }
    else{
        for(y = 0; y < image_height; y++) {
            for(x = 0; x < image_width; x++) {
                if(y - feature_height < 0 || y + feature_height >= image_height 
                    || x - feature_width < 0 || x + feature_width >= image_width){
                    depth_map[y * image_width + x] = 0;
                    continue;
                }
                left_index = y * image_width + x; // confirm the center in left
                right_index = y * image_width + x;
                min_dis = -1;
                // first collect the feature form left(compare)
                for(b = -feature_height; b <= feature_height; b++){
                    for(a = -feature_width; a <= feature_width; a++){
                        if(y+b<0 || y+b>=image_height || x+a<0 || x+a>=image_width){ // left sample out of height bounds
                            compare[center + b * (2 * feature_width + 1) + a] = 0;
                        }
                        else{
                            compare[center + b * (2 * feature_width + 1) + a] = // left sample inside
                            left[left_index + b * image_width + a];
                        }
                    }
                }

                for(k = -maximum_displacement; k <= maximum_displacement; k++){
                    for(j = -maximum_displacement; j <= maximum_displacement; j++){
                        // add bias to confirm the center in right
                        fill_feature(compare, feature, right, &min_dis, &dx, &dy,
                                     feature_width, feature_height, feature_area,
                                     image_height, image_width, right_index, 
                                     center, x, y, j, k);
                    }
                }
                if(y + dy < 0 || y + dy >= image_height || x + dx < 0 || x + dx >= image_width){
                    depth_map[y * image_width + x] = 0;
                }
                else{
                    depth_map[y * image_width + x] = normalized_displacement(dx, dy, maximum_displacement);
                }
            }
        }
    }
}
