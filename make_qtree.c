/*
 * Project 1-1: Quadtree Compression
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    int i, j;
    unsigned char value = depth_map[y*map_width + x];
    for(j=y; j<y+section_width; j++){
        for(i=x; i<x+section_width; i++){
            if(depth_map[j*map_width + i] != value){
                return -1;
            }
        }
    }
    return value;
}

qNode *make_quad_tree(unsigned char *depth_map, int map_width, int x, int y, int section_width){
    qNode *node = (qNode*)malloc(sizeof(qNode));
    if(node == NULL){allocation_failed();}
    node->x = x;
    node->y = y;
    node->size = section_width;
    // if it is leaf
    node->gray_value = homogenous(depth_map, map_width, x, y, section_width);
    if(node->gray_value == -1){
        node->leaf = 0;
        node->child_NW = make_quad_tree(depth_map, map_width, x, y, section_width/2);
        node->child_NE = make_quad_tree(depth_map, map_width, x+section_width/2, y, section_width/2);
        node->child_SE = make_quad_tree(depth_map, map_width, x+section_width/2, y+section_width/2, section_width/2);
        node->child_SW = make_quad_tree(depth_map, map_width, x, y+section_width/2, section_width/2);
    }else{
        node->leaf = 1;
        node->child_NW = NULL;
        node->child_NE = NULL;
        node->child_SE = NULL;
        node->child_SW = NULL;
    }
    return node;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    /* YOUR CODE HERE */
    qNode *root = make_quad_tree(depth_map, map_width, 0, 0, map_width);
    return root;
}

void free_qtree(qNode *qtree_node) {
    /* YOUR CODE HERE */
    if(qtree_node->leaf == 1){
        free(qtree_node);
    }else{
        free_qtree(qtree_node->child_NW);
        free_qtree(qtree_node->child_NE);
        free_qtree(qtree_node->child_SE);
        free_qtree(qtree_node->child_SW);
        free(qtree_node);
    }
}
