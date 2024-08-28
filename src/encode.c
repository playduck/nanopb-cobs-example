#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <pb.h>
#include <pb_encode.h>
#include <cobs.h>
#include "simple.pb.h"

#include "common.h"

#define BUFFER_SIZE SIMPLE_PB_H_MAX_SIZE + COBS_ENCODE_SRC_OFFSET(SIMPLE_PB_H_MAX_SIZE) + 1
#define PROTOBUF_ENCODED_SIZE SIMPLE_PB_H_MAX_SIZE

int main() {
    printf("--- Encoder ---\n");

    // Create out buffer
    uint8_t buffer[BUFFER_SIZE];

    // Initialize random number generator
    srand(time(NULL));

    // Create a SimpleMessage message
    simple_message_t message = SIMPLE_MESSAGE_INIT_DEFAULT;

    /* Create a stream that will write to our buffer, offset it by the required source offset for COBS later */
    pb_ostream_t stream = pb_ostream_from_buffer(buffer + COBS_ENCODE_SRC_OFFSET(SIMPLE_PB_H_MAX_SIZE), PROTOBUF_ENCODED_SIZE);

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
    size_t message_length = stream.bytes_written;

    printf("Protobuf encoded message size: %zu bytes\n", message_length);
    for (size_t i = 0; i < message_length; i++) {
        printf("%02x ", buffer[i + COBS_ENCODE_SRC_OFFSET(SIMPLE_PB_H_MAX_SIZE)]);
    }
    printf("\n");

    /* apply COBS encoding */
    cobs_encode_result encode_result = cobs_encode(
        buffer, BUFFER_SIZE,   /* size of the entire buffer */
        buffer + COBS_ENCODE_SRC_OFFSET(SIMPLE_PB_H_MAX_SIZE), message_length /* location and size of the encoded data */
    );

    if (encode_result.status != COBS_ENCODE_OK)   {
        printf("COBS Error %d", encode_result.status);
        return 1;
    }
    /* add termination null byte at the end */
    buffer[encode_result.out_len] = 0x00;
    size_t encoded_length = encode_result.out_len + 1;

    /* print encoded message */
    printf("COBS encoded message size: %zu bytes\n", encoded_length);
    for (size_t i = 0; i < encoded_length; i++) {
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
    fwrite(buffer, sizeof(buffer[0]), encoded_length, file);
    fclose(file);

    return 0;
}
