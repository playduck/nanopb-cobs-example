#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <pb.h>
#include <pb_encode.h>

#include "simple.pb.h"

#include "common.h"

int main() {
    printf("--- Encoder ---\n");

    // Create out buffer
    uint8_t buffer[SIMPLE_PB_H_MAX_SIZE + HEADROOM_BYTES];

    // Initialize random number generator
    srand(time(NULL));

    // Create a SimpleMessage message
    simple_message_t message = SIMPLE_MESSAGE_INIT_DEFAULT;

    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    /* Fill in the lucky number */
    message.lucky_number = rand() % 100 + 1;
    printf("Chose lucky number %d\n", message.lucky_number);

    /* Now we are ready to encode the message! */
    uint8_t status = pb_encode(&stream, SIMPLE_MESSAGE_FIELDS, &message);
    if (!status)
    {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return 1;
    }

    /* print encoded message */
    size_t message_length = stream.bytes_written;
    printf("Encoded message size: %zu bytes\n", message_length);
    for (size_t i = 0; i < message_length; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");

    /* save the message to a file */
    printf("Writing to file %s\n", MESSAGE_FILENAME);
    FILE *file = fopen(MESSAGE_FILENAME, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    fwrite(buffer, sizeof(buffer[0]), message_length, file);
    fclose(file);

    return 0;
}
