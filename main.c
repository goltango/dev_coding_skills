#include "log_transactions.h"
#include "buffer_code.c"
#include <time.h>

// Define constants for transaction size
#define TIMESTAMP_LEN 19
#define VEH_REG_LEN 8
#define PROD_LEN 1
#define MILS_LEN 4
#define TRANSACT_LEN 2
#define TRANS_FRAME_SIZE (TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN + MILS_LEN + TRANSACT_LEN)
#define MAX_LOG_BUFFER_SIZE (MAX_TRANSACTIONS * 72)

void print_buffer(const char *buffer, size_t length) {
    printf("[");
    for (size_t i = 0; i < length; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            // Print printable ascii characters
            printf("%c", buffer[i]);
        } else {
            // Print non-printable characters in hexadecimal format
            printf("\\x%02X", (unsigned char)buffer[i]);
        }
    }
    printf("]\n");
}

int main() {
    // Lets use a buffer with raw transactions from buffer_code.c
    
    int bytes_written = 0;
    size_t transaction_count = transactions_generated;
    
    // Print raw buffer
    //print_buffer(buffer, sizeof(buffer) - 1);
    
    // Lets define an output buffer for logging
    char log[MAX_LOG_BUFFER_SIZE] = {0};

    // Core function
    bytes_written = log_transactions(buffer, log, transaction_count);

    // Output verification
    if (bytes_written >= 0) {
        printf("Log generated (%d bytes):\n%s", bytes_written, log);
    } else {
        printf("Error generating log.\n");
    }

    return 0;
}