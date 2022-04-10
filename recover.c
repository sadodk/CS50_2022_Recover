#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    else
    {
        // Opens file
        char *input_file_name = argv[1]; //pointer to filename
        FILE *input_pointer = fopen(input_file_name, "r"); // pointer to file.

        if (input_pointer == NULL)
        {
            printf("Error: cannot open %s\n", input_file_name);
            return 2;
        }

        // Variables
        FILE *img_pointer = NULL; // pointer to the written file
        char jpg_name[8]; // name of jpegs
        BYTE buffer[512]; // butter of 512 bytes to be loaded each time
        int imgCount = 0; // counts number of images

        // Read and write to files
            while (fread(&buffer, 512, 1, input_pointer) == 1)
            {
                // if beginning of firs jpeg file:
                if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
                {
                    // If not first JPEG, close previous jpeg
                    if (!(imgCount == 0))
                    {
                        fclose(img_pointer);
                    }
                    // Initialise file
                    sprintf(jpg_name, "%03i.jpg", imgCount); //define jpeg
                    img_pointer = fopen(jpg_name, "w"); // open each jpeg file destination
                    imgCount++; // keep looping
                }
                // If JPEG has been found, write to file
                if (!(imgCount == 0))
                {
                    fwrite(&buffer, 512, 1, img_pointer);
                }
            }
            fclose(input_pointer);
            fclose(img_pointer);
            return 0;
    }
}