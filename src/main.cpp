#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"

#define FILE_NAME CFG_BLACK CBG_WHITE " main " CRESET ": "

#define NUM_OF_ROWS 8
#define NUM_OF_COLS 8

#define TERMINATOR 5

struct Coordinate
{
    uint8_t row;
    uint8_t col;
};

char rijec = 'ztarjnksobižitoe';

struct Fields
{
    Coordinate coordinate;
    bool neighbour;
    char value;
};

void initFields(Fields fields[NUM_OF_ROWS][NUM_OF_COLS])
{
    // printf(FILE_NAME "initFields\n");
    char slovo = 'A';
    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            fields[r][c].value = slovo;
            fields[r][c].coordinate.row = r;
            fields[r][c].coordinate.col = c;
            fields[r][c].neighbour = false;
            slovo++;
            if (slovo > 'Z' && slovo < 'a')
            {
                slovo = 'a';
            }
            else if (slovo > 'z')
            {
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
            if (fields[r][c].neighbour)
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

uint8_t setNeighboursIntoFields(Coordinate coordinate, Fields fields[NUM_OF_ROWS][NUM_OF_COLS])
{
    uint8_t len = 0;

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            if (
                (abs(r - coordinate.row) == 0 && abs(c - coordinate.col) == 1) ||
                (abs(r - coordinate.row) == 1 && abs(c - coordinate.col) == 0) ||
                (abs(r - coordinate.row) == 1 && abs(c - coordinate.col) == 1))
            {
                fields[r][c].neighbour = true;
                len++;
            }
        }
    }

    return len;
}

bool isCoordinateUsed(Coordinate coordinate, Coordinate coordinates[], uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (memcmp(&coordinate, &coordinates[i], sizeof(Coordinate)) == 0)
        {
            return true;
        }
    }
    return false;
}

uint32_t total = 0;

void readRecursive(Coordinate coordinate, Fields fields[NUM_OF_ROWS][NUM_OF_COLS], uint8_t terminator, Coordinate usedCoordinates[])
{
    // printf("---------------------------\n");
    total++;
    // printf("value %d.%d\n", coordinate.row, coordinate.col);
    setNeighboursIntoFields(coordinate, fields);
    // drawFields(fields);

    memcpy(&usedCoordinates[terminator], &coordinate, sizeof(Coordinate));

    if (terminator >= TERMINATOR)
    {
        printf(FILE_NAME "END: ");
        for (uint8_t i = 0; i <= TERMINATOR; i++)
        {
            // printf("%d.%d, ", usedCoordinates[i].row, usedCoordinates[i].col);
            printf("%c", usedCoordinates[i].row, usedCoordinates[i].col);
        }
        printf("\n");
        return;
    }

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            if (fields[r][c].neighbour && !isCoordinateUsed(fields[r][c].coordinate, usedCoordinates, terminator + 1))
            {
                Fields fields[NUM_OF_ROWS][NUM_OF_COLS] = {0x00};
                initFields(fields);
                readRecursive(fields[r][c].coordinate, fields, terminator + 1, usedCoordinates);
            }
        }
    }
}

int main()
{
    printf(FILE_NAME "boot sequence start\n");
    Fields fields[NUM_OF_ROWS][NUM_OF_COLS] = {0x00};
    initFields(fields);

    for (uint8_t r = 0; r < NUM_OF_ROWS; r++)
    {
        for (uint8_t c = 0; c < NUM_OF_COLS; c++)
        {
            Coordinate coordinate = {r, c};
            Coordinate usedCoordinates[TERMINATOR];
            readRecursive(coordinate, fields, 0, usedCoordinates);
            // printf(FILE_NAME "total = %d\n", total);
        }
    }

    printf(FILE_NAME "boot sequence end\n");
    return 0;
}