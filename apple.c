#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//random structs copied from cs50
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;

typedef struct
{
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

typedef struct
{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

//total number of frames plus one
const int FRAMECOUNT = 6513 + 1;

//int to char*
char *itoa(long n)
{
    int len = n==0 ? 1 : floor(log10l(labs(n)))+1;
    if (n<0) len++; // room for negative sign '-'

    char    *buf = calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%ld", n);
    return   buf;
}

int main(void)
{
    //dimensions of image with scaling
    int height = 368 / 4;
    int width = 496 / 2;
    
    // Allocate memory for image
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    
    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
    
    
    //iterates over every frame
    for (int frameNum = 0; frameNum < FRAMECOUNT; frameNum++)
    {
        //printf("%i\n", frameNum);
        
        //generates path
        char *result = malloc(strlen("./frames/") + strlen(itoa(frameNum)) + strlen(".bmp") + 1); // +1 for the null-terminator
        
        // in real code you would check for errors in malloc here
        //unfortunately, this is not real code
        strcpy(result, "./frames/");
        strcat(result, itoa(frameNum));
        strcat(result, ".bmp");
        
        FILE *frame = fopen(result, "r");
        
        free(result);
        
        // Iterate over infile's scanlines
        for (int i = 0; i < height; i++)
        {
            // Read row into pixel array
            fread(image[i], sizeof(RGBTRIPLE), width, frame);

            // Skip over padding
            fseek(frame, padding, SEEK_CUR);
        }
        
        //allocates memory for current frame
        char* currentFrame = malloc(height * (width + 1) + 1);
        
        //generates frame based on average color
        strcpy(currentFrame, "");
        
        for (int i = height - 1; i >= 0; i--)
        {
            for (int j = 0; j < width; j++)
            {
                int rgb = round( (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0 );
                if (rgb <= 50)
                {
                    strcat(currentFrame, ".");
                }
                else if (rgb <= 100)
                {
                    strcat(currentFrame, "-");
                }
                else if (rgb <= 150)
                {
                    strcat(currentFrame, "#");
                }
                else if (rgb <= 255)
                {
                    strcat(currentFrame, "@");
                }
            }
            strcat(currentFrame, "\n");
        }
        
        //prints frame to console
        printf("\n\n\n\n\n\n%s", currentFrame);
        
        //closes frame
        fclose(frame);
    }
    
    // Free memory for image
    free(image);
    
    printf("\ndone\n");
}