#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//prototype
char* iteration(char* previous, int times);

int totalTimes = 0;

int main(void)
{
    //gets user inputted number of iterations
    int num = 0;
    printf("number of iterations: ");
    scanf("%i", &num);

    int num2 = 0;
    printf("output options (0: print, 1: to file): ");
    scanf("%i", &num2);
    
    totalTimes = num;
    
    char first[2] = { "#" };
    char *final = iteration(first, num);

    if (num2 == 1) {
        //outputs information to file
        FILE* file;

        file = fopen("output.txt", "w+");

        if (file == NULL) {
            printf("file can't be opened\n");
            exit(1);
        }

        fprintf(file, final);

        fclose(file);
        
    }
    else {
        printf("\n%s\n", final);
    }

    printf("exiting...\n");
    return 0;
}

//r e cu r s i o n
char* iteration(char* previous, int times)
{
    if (times > 0)
    {
        int width = pow(2, (totalTimes - times));

        char *current = malloc((width * (width + 1)) * sizeof(char*));
        char currentSplit[width][width];

        current[width * (width + 1)] = '\0';

        //row and column counter
        int r = 0;
        int c = 0;
        
        //converts string into an array
        for (int i = 0; i < strlen(previous); i++)
        {
            if (previous[i] == '\n')
            {
                r++;
                c = 0;
            }
            else
            {
                currentSplit[r][c] = previous[i];
                c++;
            }
        }
        
        //generates iteration
        for (int i = 0; i < width; i++)
        {
            //adds padding to original triangle
            if (i < width / 2)
            {
                c = 1;
                while (i + c < width)
                {
                    currentSplit[i][i + c] = ' ';
                    c++;
                }
            }
            //duplicates original triangle twice for iteration
            else
            {
                c = 0;
                while (c < width)
                {
                    currentSplit[i][c] = currentSplit[i - width / 2][c >= width / 2 ? c - width / 2 : c];
                    c++;
                }
            }
        }

        //converts array back into a string
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                current[i * width + i + j] = currentSplit[i][j];
            }
            current[(i + 1) * (width + 1) - 1] = '\n';
        }

        free(currentSplit);

        //returns next iteration
        return(iteration(current, times - 1));
    }
    else
    {
        //returns completed string at end
        return(previous);
    }
}