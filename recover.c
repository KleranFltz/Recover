#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
if (argc != 2)
{
    printf("error: correct usage: ./recover IMAGE\n");
    return 1;
}

 //open memory card
FILE *input = fopen(argv[1], "r");
if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
uint8_t buffer[BLOCK_SIZE];
int iteration = 0;
string filename;
string * pfile = &filename;
int filenum = -1;
FILE * output;
while (fread(&buffer, sizeof(uint8_t), BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        //printf("iteration: %i\n", iteration);

        iteration++;
        //detecting the start of a new jpeg
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0)) //this condition will return true if it is the start of a new file
        {
            filenum++;

            //printf("file: %i\n", filenum);

            //this is the start of a new file

            //if this is the first file, make the output file and start writing
            if (filenum == 0)
            {
                sprintf(filename, "%03i.jpg", filenum);
                output = fopen(filename, "w");
                fwrite(&buffer, 1, BLOCK_SIZE, output);
            }

            //if this isn't the first file, close the previous file and start writing to a new file
            else
            {
                fclose(output);
                sprintf(filename, "%03i.jpg", filenum);
                output = fopen(filename, "w");
                fwrite(&buffer, 1, BLOCK_SIZE, output);
            }
        }
        else //this condition will return true if it is not the start of a new file
        {
            //if this isn't the start of a new file and we haven't found a file yet, continue?
            if (filenum == -1)
            {
                continue;
            }
            //if we found a jpeg last time, (just write the information from this block to same file as last time)
            //printf("beaan");
            fwrite(&buffer, 1, BLOCK_SIZE, output);
            //printf("bon");
        }
    }
fclose(input);
fclose(output);
}