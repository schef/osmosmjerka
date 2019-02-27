#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "colors.h"

#define FILE_NAME CFG_BLACK CBG_WHITE " main " CRESET ": "

#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8

#define TERMINATOR 1

struct Fields
{
    uint8_t row;
    uint8_t col;
    bool used;
    char value;
};

void initFields(Fields fields[NUM_OF_ROWS][NUM_OF_COLS])
{
    printf(FILE_NAME "initFields\n");
    char slovo = 'A';
    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            fields[r][c].value = slovo;
            fields[r][c].row = r;
            fields[r][c].col = c;
            fields[r][c].used = false;
            slovo++;
            if (slovo > 'Z' && slovo < 'a'){
                slovo = 'a';
            } else if (slovo > 'z'){
                slovo = '0';
            }
        }
    }
}

void drawFields(Fields fields[NUM_OF_ROWS][NUM_OF_COLS])
{
    printf(FILE_NAME "drawFields\n");

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        printf("  ");
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            if (fields[r][c].used)
            {
                printf(CBG_BLUE "%c" CRESET " ", fields[r][c].value);
            }
            else
            {
                printf("%c ", fields[r][c].value);
            }
        }
        printf("\n");
    }
}

uint8_t getNeighbours(Fields field, Fields fields[NUM_OF_ROWS][NUM_OF_COLS])
{
    uint8_t len = 0;

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            if (
                (abs(r - field.row) == 0 && abs(c - field.col) == 1) ||
                (abs(r - field.row) == 1 && abs(c - field.col) == 0) ||
                (abs(r - field.row) == 1 && abs(c - field.col) == 1))
            {
                fields[r][c].used = true;
                len++;
            }
        }
    }

    return len;
}


void readRecursive(Fields field, Fields fields[NUM_OF_ROWS][NUM_OF_COLS], uint8_t terminator, char rijec[])
{
    // printf("value %02x\n", field.id);
    getNeighbours(field, fields);
    // drawFields(fields);
    // printf("---------------------------\n");

    if (terminator > TERMINATOR)
    {
        printf("%s\n", rijec);
        return;
    }

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            if (fields[r][c].used)
            {
                rijec[terminator] = fields[r][c].value;
                Fields fields[NUM_OF_ROWS][NUM_OF_COLS] = {0x00};
                initFields(fields);
                readRecursive(fields[r][c], fields, terminator + 1, rijec);
            }
        }
    }
}

int main()
{
    printf(FILE_NAME "boot sequence start\n");
    Fields fields[NUM_OF_ROWS][NUM_OF_COLS] = {0x00};
    initFields(fields);
    Fields field = {0, 1};

    char rijec[TERMINATOR];

    readRecursive(field, fields, 0, rijec);


    printf(FILE_NAME "boot sequence end\n");
    return 0;
}