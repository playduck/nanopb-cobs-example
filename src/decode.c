#include <stdio.h>
#include <stdlib.h>

#include <pb.h>
#include <pb_decode.h>

#include "simple.pb.h"

#include "common.h"

int main() {
    int ret = 0;

    printf("--- Decoder ---\n");

    FILE *file = fopen(MESSAGE_FILENAME, "rb"); // Open the file in binary read mode
    if (file == NULL) {
        printf("Error opening file\n");
        ret = 1;
        goto exit;
    }

    fseek(file, 0, SEEK_END); // Move to the end of the file
    long file_size = ftell(file); // Get the size of the file
    rewind(file); // Move back to the beginning of the file

    printf("Encoded message from file size: %zu bytes\n", file_size);

    unsigned char *buffer = (unsigned char *)malloc(file_size + HEADROOM_BYTES); // Allocate memory for the buffer
    if (buffer == NULL) {
        printf("Error allocating memory\n");
        fclose(file);
        ret = 1;
        goto exit;
    }

    fread(buffer, 1, file_size, file); // Read the file into the buffer
    fclose(file);

    /* Print the buffer to the console */
    for (int i = 0; i < file_size; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    // Create a SimpleMessage message
    simple_message_t message = SIMPLE_MESSAGE_INIT_DEFAULT;

    /* Create a stream that will write to our buffer. Note: don't pass the buffer size, but the file size */
    pb_istream_t stream = pb_istream_from_buffer(buffer, file_size);

    uint8_t status = pb_decode(&stream, SIMPLE_MESSAGE_FIELDS, &message);
    if (!status)
    {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        ret = 1;
        goto exit;
    }   else    {
        printf("Your lucky number was %d!\n", (int)message.lucky_number);
    }

exit:
    free(buffer); // Free the buffer
    return ret;
}
