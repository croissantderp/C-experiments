#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//prototype
char* iteration(char* input, int count);
char* copyWithOffset(int xOffset, int yOffset, int totalCount, int width, int height, char* input);

//total number of iterations
int totalIterations = 0;

//options for output
int outputIterations = 0;

int main(void) {
  //get amount of iterations specified by user
  printf("Number of iterations:\n> ");
  int iterations = -1;
  scanf("%i", &iterations);

  //check if input is valid
  if (iterations < 0){
    printf("Number of iterations has to be a positive number\n");
    return 1;
  }

  totalIterations = iterations;

  //get output style specified by user
  printf("Output Iterations? (0 = output to file, 1 = output final to cmd, 2 = output individual iterations to cmd)\n> ");
  scanf("%i", &outputIterations);

  //defaults invalid responses to 0
  if (outputIterations != 1 && outputIterations != 2) {
      outputIterations = 0;
  }

  if (outputIterations == 0) {
      //outputs information to file
      FILE *file;

      file = fopen("output.txt", "w+");

      if (file == NULL) {
          printf("file can't be opened\n");
          exit(1);
      }

      fprintf(file, iteration("_##_\n#___\n#___\n____", iterations));

      fclose(file);
  }
  else {
      printf("\nIteration %i:\n%s\n", iterations, iteration("_##_\n#___\n#___\n____", iterations));
  }
  return 0;
}

//rotates an object 90 degrees counter-clockwise
char* rotate(char *input) {
  //dimensions of input string
  int width = 0;
  int height = 0;

  //if the first row is over
  int reachedRow = 0;

  //input without newlines
  char *cleanedInput = malloc(strlen(input) * sizeof(char));
  int cleanedIndex = 0;
  
  //finding various variables
  for (int i = 0; i < strlen(input); i++) {
    //finds height by counting newlines
    if (input[i] == '\n'){
      height++;
      reachedRow = 1;
    }
    else{
      //copies input except for newlines
      cleanedInput[cleanedIndex] = input[i];
      cleanedIndex++;
    }

    //counts characters in first row as width
    if (reachedRow == 0){
      width++;
    }
  }

  //final rotated output
  char *output = malloc((width * height) * sizeof(char*));

  //current index of the output string
  int outputIndex = 0;

  //current item index in a row
  int itemCount = 0;

  //new line count
  int nlCount = 0;

  int len = strlen(cleanedInput);
  
  for (int i = 0; i < len; i++){
    output[outputIndex] = cleanedInput[(itemCount * width) + abs(nlCount - (width - 1))];
    itemCount++;
    outputIndex++;

    //if a newline should be added
    if (itemCount > height && i != len - 1){
      itemCount = 0;
      output[outputIndex] = '\n';
      outputIndex++;
      nlCount++;
    }
  }

  free(cleanedInput);

  return output;
}

//recursive function which generates an iteration
char* iteration(char* input, int count){
  //if there are still iterations to go
  if (count > 0){
      if (outputIterations == 2) {
          printf("Iteration %i:\n%s\n\n", totalIterations - count, input);
      }
      else {
          printf("%i", totalIterations - count);
      }

    //produces a rotated version of the input
    char* rotated = rotate(input);
    
    //figure out width using same method in rotate
    int width = 1;
    int height = 1;

    int reachedRow = 0;
    for (int i = 0; i < strlen(input); i++){
      if (input[i] == '\n'){
        height++;
        reachedRow = 1;
        continue;
      }
      
      if (reachedRow == 0){
        width++;
      }
    }
  
    int inputLen = strlen(input);

    //locate rotation point on input and rotated text
    int inputPos = 0;
    int inputPosFromTop = 0;
    int inputPosFromBottom = 0;
    int inputPosFromRight = 0;
    int inputPosFromLeft = 0;

    //checks every square and adjacent squares for the input string to find the rotation point
    //checks for:
    //
    //##x
    //
    int currentDistanceFromTop = 0;
    int currentDistanceFromLeft = 0;
    for (int i = 0; i < inputLen; i++){
      //check square itself
      if (input[i] == '_'){
        //check square left and square 2 to the left
        if (i - 2 > 0 && (input[i - 1] == '#' && input[i - 2] == '#')){
          //check square to the right
          if (i + 1 > inputLen || input[i + 1] != '#' || input[i + 1] == '\n'){
            //check square to the bottom
            if (i - width < 0 || input[i - width] != '#'){
              //check square to the top
              if (i + width > inputLen || input[i + width] != '#'){
                //logs information and exits loop once found
                inputPos = i;
                inputPosFromTop = currentDistanceFromTop;
                inputPosFromLeft = currentDistanceFromLeft;
                
                inputPosFromRight = width - inputPosFromLeft - 1;
                inputPosFromBottom = inputLen / width - inputPosFromTop;
                break;
              }
            }
          }
        }
      }

      //updates current position
      currentDistanceFromLeft++;
      
      if (input[i] == '\n'){
        currentDistanceFromTop++;
        currentDistanceFromLeft = 0;
      }
    }

    //repeating the process for rotated string
    int rotatePos = 0;
    int rotatePosFromTop = 0;
    int rotatePosFromBottom = 0;
    int rotatePosFromRight = 0;
    int rotatePosFromLeft = 0;

    int rotateWidth = height + 1;

    //checks for:
    // x
    // #
    // #
    currentDistanceFromTop = 0;
    currentDistanceFromLeft = 0;
    for (int i = 0; i < inputLen; i++){
      //check square itself
      if (rotated[i] == '_'){
        //check bottom square
        if (i + rotateWidth < inputLen && rotated[i + rotateWidth] == '#'){
          //check square 2 down
          if (i + 2 * rotateWidth < inputLen && rotated[i + 2 * rotateWidth] == '#'){
            //check right square
            if (i + 1 > inputLen || rotated[i + 1] != '#' || rotated[i + 1] == '\n'){
              //check left square
              if (i - 1 < 0 || rotated[i - 1] !='#' || rotated[i - 1] == '\n'){
                //check top square
                if (i - rotateWidth < 0 || rotated[i - rotateWidth] != '#'){
                  rotatePos = i;
                  rotatePosFromTop = currentDistanceFromTop;
                  rotatePosFromLeft = currentDistanceFromLeft;
      
                  rotatePosFromRight = rotateWidth - rotatePosFromLeft;
                  rotatePosFromBottom = width - rotatePosFromTop;
                  break;
                }
              }
            }
          }
        }
      }
      currentDistanceFromLeft++;
      
      if (rotated[i] == '\n'){
        currentDistanceFromTop++;
        currentDistanceFromLeft = 0;
      }
    }
    
    //finds offset of rotation point from top-left corner in the final string. This uses the whichever position has the greater value between the input and rotated strings
    int xOffset = (inputPosFromLeft > rotatePosFromLeft ? inputPosFromLeft : rotatePosFromLeft);
    int yOffset = (inputPosFromTop > rotatePosFromTop ? inputPosFromTop : rotatePosFromTop);
    
    //finds width and height of the final string
    int finalWidth = xOffset + (inputPosFromRight > rotatePosFromRight - 1 ? inputPosFromRight : rotatePosFromRight - 1);
    int finalHeight = yOffset + (inputPosFromBottom + 1 > rotatePosFromBottom - 1 ? inputPosFromBottom + 1 : rotatePosFromBottom - 1);

    //total number of characters in the final string, +1 for newlines, -1 for lack of last newline
    int totalCount = (finalWidth + 1) * finalHeight - 1;

    //copies input and rotated strings to the final strings
    char* finalInput = copyWithOffset(xOffset - inputPosFromLeft, yOffset - inputPosFromTop, totalCount, finalWidth, finalHeight, input);
    char* finalRotate = copyWithOffset(xOffset - rotatePosFromLeft, yOffset - rotatePosFromTop, totalCount, finalWidth, finalHeight, rotated);

    //combine both input and rotated strings into the same string
    for (int i = 0; i < totalCount; i++){
      if (finalRotate[i] == '#' || finalInput[i] == '#'){
        finalRotate[i] = '#';
      }
    }

    //passes final string onto the next iteration
    return iteration(finalRotate, count - 1);
  }
  else {
    return input;
  }
}

//copies an input string to the larger final string with a set offset from the top-left corner
char *copyWithOffset(int xOffset, int yOffset, int totalCount, int width, int height, char* input){
  //generates string
  char* finalString = malloc((totalCount + 1) * sizeof(char));

  //current index of the input which is being copied
  int inputIndex = 0;
  //current y position in the final string
  int currentY = 0;
  int currentX = 0;

  //whether the input is done being copied for the current row
  int hitEnd = 0;

  //whether the input is done being copied overall
  int terminated = 0;

  //populates final string
  for (int i = 0; i < totalCount; i++){
    //fills value with _ by default
    finalString[i] = '_';

    //if the current position is beyond the offset
    if (currentY >= yOffset){
      //If input is still being copied and current position is beyond xOffset
      if (currentX >= xOffset && hitEnd == 0 && terminated == 0){
        //terminated copying if the input is completely done being copied
        if (inputIndex >= strlen(input)){
          terminated = 1;
        }
        else {
          //if current input is a newline, meaning that it is done being copied for the row
          if (input[inputIndex] == '\n'){
            //ends copying for the row
            hitEnd = 1;
            inputIndex++;
          }
          else{
            //copies values
            finalString[i] = input[inputIndex];
            inputIndex++;
          }
        }
      }
    }
    currentX++;

    //if a new line should be added and variables reset
    if (currentX > width){
      hitEnd = 0;
      currentY++;
      currentX = 0;
      finalString[i] = '\n';
    }
  }
  
  return finalString;
}
