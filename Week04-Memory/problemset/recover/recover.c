#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;
typedef uint8_t BYTE;
bool valid_sign(BYTE buffer[BLOCK_SIZE]);

int main(int argc, char *argv[])
{
    // Check for valid command line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open image
    FILE *file = fopen(argv[1], "r");
    // Check if file exists
    if (file == NULL)
    {
        printf("No such file exists!\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];          // read 1 block at a time
    int img_num = 0;                  // To keep track of image number
    char *img_name = malloc((7 + 1)); // ###.jpg (length = 7) + 1 ('\0')
    FILE *img = NULL;

    // Iterate over and read block(512 bytes) of file
    while (fread(buffer, 1, BLOCK_SIZE, file))
    {
        // Check for valid signatures
        if (valid_sign(buffer))
        {
            // if img is not null, close the previous image file
            if (img != NULL)
            {
                fclose(img);
                img_num++;
            }

            // Write to a new image
            sprintf(img_name, "%03i.jpg", img_num);
            img = fopen(img_name, "w");
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }

        // if valid signature is not found
        else
        {
            // if img is not null, continue writing to this img
            if (img != NULL)
            {
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
    }
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(file);
    free(img_name);
}

bool valid_sign(BYTE buffer[BLOCK_SIZE])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}