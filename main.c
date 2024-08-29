#include "log_transactions.h"

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
        printf(",");
    }
    printf("]\n");
}

int main() {
    // Lets simulate a buffer with raw transactions
    const char buffer[] =
        "08/27/2024 12:34:56AAA 1234P\x01\x00\x01\x00\x01\x00"  // transaction 1
        "08/26/2024 09:12:45BBB 5678D\x01\x00\x01\x00\x01\x00"  // transaction 2
        "08/27/2024 14:00:00CCC 9012G\x01\x00\x01\x00\x01\x00"; // transaction 3

    int bytes_written = 0;
    size_t transaction_count = 3;
    
    // Print raw buffer
    //print_buffer(buffer, sizeof(buffer) - 1);
    
    // Lets define an output buffer for logging
    char log[1024] = {0};

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