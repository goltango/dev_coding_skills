// log_transactions.c
#include "log_transactions.h"


void parse_transaction(const char *data, Transaction *transaction) {
    memcpy(transaction->timestamp, data, TIMESTAMP_LEN);
    transaction->timestamp[TIMESTAMP_LEN] = '\0';  // Final NULL character
    memcpy(transaction->vehicle_registration, data + TIMESTAMP_LEN, VEH_REG_LEN);
    transaction->vehicle_registration[VEH_REG_LEN] = '\0';
    transaction->product = *(data + PRODUCT_OFFSET);
    memcpy(&transaction->mililiters, data + MILILIT_OFFSET, sizeof(int32_t));
    memcpy(&transaction->transaction_id, data + TRANSAC_OFFSET, sizeof(uint16_t));
}

int compare_transactions(const void *a, const void *b) {
    const Transaction *trans_a = (const Transaction *)a;
    const Transaction *trans_b = (const Transaction *)b;
    return strcmp(trans_a->timestamp, trans_b->timestamp);
}

int format_transaction_log(char *log, const Transaction *transaction) {
    return sprintf(log, "[%.2s/%.2s/%.4s %.8s] id: %05d, reg: %.8s, prod: %c, ltrs: %+.7d\n",
                   transaction->timestamp + 3, transaction->timestamp, transaction->timestamp + 6,
                   transaction->timestamp + 11, transaction->transaction_id,
                   transaction->vehicle_registration, transaction->product,
                   transaction->mililiters / 1000);
}

int log_transactions(const char *data, char *log, size_t transaction_count) {
    if (transaction_count > MAX_TRANSACTIONS) {
        return -1; // Error: cannot process more than 100 transactions
    }

    Transaction transactions[MAX_TRANSACTIONS];

    // Parse transactions
    for (size_t i = 0; i < transaction_count; i++) {
        parse_transaction(data + i * TRANS_FRAME_SIZE, &transactions[i]);
    }

    // Order transactions by date
    qsort(transactions, transaction_count, sizeof(Transaction), compare_transactions);

    // Generate log
    int total_bytes_written = 0;
    for (size_t i = 0; i < transaction_count; i++) {
        int bytes_written = format_transaction_log(log + total_bytes_written, &transactions[i]);
        total_bytes_written += bytes_written;
    }


    return 0;
}