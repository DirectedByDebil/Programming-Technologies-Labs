struct Params 
{
    char* fileA;
    char* fileB;

    int mode;
    int randomSize;
};

typedef struct Params Params;


Params ReadInputParams(int argc, char* argv[]);

void PrintParams(Params params);