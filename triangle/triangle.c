#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//prototype
string iteration(char* previous, int times);

int totalTimes = 0;

int main(void)
{
    //gets user inputted number of iterations
    int num = 0;
    printf("number of iterations: ");
    scanf("%i", &num);
    
    totalTimes = num;
    
    printf("\n%s\n", iteration("#", num));
    return 0;
}

//r e cu r s i o n
string iteration(char* previous, int times)
{
    if (times > 0)
    {
        int width = pow(2, (totalTimes - times));
        char* current = malloc((width * width) * sizeof(char*));
        char currentSplit[width][width];
        
        //row and column counter
        int r = 0;
        int c = 0;
        
        //converts string into an array
        for (int i = 0; i < strlen(previous); i++)
        {
            if (previous[i] == 10)
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
            strcat(current, "\n");
        }
        
        //returns next iteration
        return(iteration(current, times - 1));
    }
    else
    {
        //returns completed string at end
        return(previous);
    }
}