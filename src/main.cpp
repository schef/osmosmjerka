#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "colors.h"

#define FILE_NAME CFG_BLACK CBG_WHITE " main " CRESET ": "

#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8

struct Fields {
    uint8_t row;
    uint8_t col;
    uint8_t id;
    uint8_t used;
};


void printFields(Fields fields[NUM_OF_ROWS][NUM_OF_COLS]){
    printf(FILE_NAME "printFields\n");

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++){
        printf("  ");
        for (uint8_t c = 0; c < NUM_OF_COLS; c++){
            fields[r][c].id = r * 0x10 + c * 0x01;
            fields[r][c].row = r;
            fields[r][c].col = c;
            if (fields[r][c].used){
                printf(CBG_BLUE "%02x" CRESET " ", fields[r][c].id);
            } else {
                printf("%02x ", fields[r][c].id);
            }
        }
        printf("\n");
    }
}

uint8_t getNeighbours(Fields field, Fields fields[NUM_OF_ROWS][NUM_OF_COLS]){
    uint8_t len = 0;

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++){
        for (uint8_t c = 0; c < NUM_OF_COLS; c++){
            if (
                (abs(r - field.row) == 0 && abs(c - field.col) == 1) ||
                (abs(r - field.row) == 1 && abs(c - field.col) == 0) ||
                (abs(r - field.row) == 1 && abs(c - field.col) == 1)
            ){
                fields[r][c].used = true;
                len++;
            }
        }
    }

    return len;
}

int main() {
    printf(FILE_NAME "boot sequence start\n");
    Fields fields[NUM_OF_ROWS][NUM_OF_COLS] = {0x00};
    Fields field = {0, 1};
    getNeighbours(field, fields);
    printFields(fields);
    printf(FILE_NAME "boot sequence end\n");
    return 0;
}