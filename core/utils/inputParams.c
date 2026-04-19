#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "ioUtils.h"
#include "inputParams.h"

const int FILES_COUNT = 2;


enum ReadInputState
{
    READ_FILE,
    PENDING_READ_MODE,
    READ_MODE,
    PENDING_READ_RANDOM_SIZE,
    READ_RANDOM_SIZE
};

typedef enum ReadInputState ReadInputState;

ReadInputState inputState;


#pragma region private methods prototypes

ReadInputState GetCurrentState(char* arg, ReadInputState lastState);

#pragma endregion


Params ReadInputParams(int argc, char* argv[])
{
    
    Params t = {NULL, NULL, 0, 0};

    int currentIndex = 0;
    char** fileNames = (char**)malloc(FILES_COUNT*sizeof(char*));
    if (fileNames == NULL) {return t; };

    for (int i = 0; i < FILES_COUNT; i++) {
        fileNames[i] = NULL;
    }

    int mode = 0;    
    int randomSize = 0;

    ReadInputState state = READ_FILE;
    
    //skip 0: it's executable name
    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];

        state = GetCurrentState(arg, state);

        switch (state)
        {

            case READ_FILE:
                copyString(&fileNames[currentIndex], arg);
                currentIndex++;
            break;

            case READ_MODE:
                if (strcmp(arg, "opt") == 0)
                {   
                    mode = 1;
                }
            break;

            case READ_RANDOM_SIZE:
                randomSize = atoi(arg);
            break;

            default:
            break;
        }
    }

    //if has files to read
    if (currentIndex == FILES_COUNT)
    {
        copyString(&t.fileA, fileNames[0]);
        copyString(&t.fileB, fileNames[1]);
    }

    t.mode = mode;
    t.randomSize = randomSize;

    for (int i = 0; i < FILES_COUNT; i++) {
        if (fileNames[i] != NULL) free(fileNames[i]);
    }
    free(fileNames);

    return t;
}


void PrintParams(Params params)
{
    printf("\n");
    printf("fileA: %s\n", params.fileA);
    printf("fileB: %s\n", params.fileB);
    printf("mode: %d\n", params.mode);
    printf("randomSize: %d\n", params.randomSize);
    printf("\n");
}



#pragma region private methods implementation

ReadInputState GetCurrentState(char* arg, ReadInputState state)
{
    if (strcmp(arg, "-m") == 0)
    {
        return PENDING_READ_MODE;
    }
    else if (state == PENDING_READ_MODE)
    {
        return READ_MODE;
    }

    if (strcmp(arg, "-r") == 0)
    {
        return PENDING_READ_RANDOM_SIZE;
    }
    else if (state == PENDING_READ_RANDOM_SIZE)
    {
        return READ_RANDOM_SIZE;
    }

    return READ_FILE;
}

#pragma endregion