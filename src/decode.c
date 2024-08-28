#include <stdio.h>
#include <stdlib.h>

#include <pb.h>
#include <pb_decode.h>
#include <cobs.h>
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

    unsigned char *buffer = (unsigned char *)malloc(file_size); // Allocate memory for the buffer
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

    /* Decode COBS */
    cobs_decode_result decode_result = cobs_decode(buffer, sizeof(buffer), buffer, file_size - 1);
    if (decode_result.status != COBS_ENCODE_OK)   {
        printf("COBS Error %d", decode_result.status);
        ret = 1;
        goto exit;
    }

    /* Print the buffer to the console */
    printf("COBS decoded message size: %zu bytes\n", decode_result.out_len);
    for (int i = 0; i < decode_result.out_len; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    /* Create a SimpleMessage message */
    simple_message_t message = SIMPLE_MESSAGE_INIT_DEFAULT;

    /* Create a stream that will write to our buffer. Note: don't pass the buffer size, but the file size */
    pb_istream_t stream = pb_istream_from_buffer(buffer, decode_result.out_len);

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
