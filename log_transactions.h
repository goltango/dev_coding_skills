// log_transactions.h
#ifndef LOG_TRANSACTIONS_H
#define LOG_TRANSACTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define TIMESTAMP_LEN 19
#define VEH_REG_LEN 8
#define PROD_LEN 1
#define MILS_LEN 4
#define TRANSACT_LEN 2
#define MAX_TRANSACTIONS 100

// Offsets for parsing transactions
#define PRODUCT_OFFSET (TIMESTAMP_LEN + VEH_REG_LEN)
#define MILILIT_OFFSET (TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN)
#define TRANSAC_OFFSET (TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN + MILS_LEN)
#define TRANS_FRAME_SIZE (TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN + MILS_LEN + TRANSACT_LEN)

typedef struct {
    char timestamp[TIMESTAMP_LEN+1];
    time_t timestamp_unix;
    char vehicle_registration[VEH_REG_LEN+1];
    char product;
    int32_t mililiters;
    uint16_t transaction_id;
} Transaction;

void parse_transaction(const char *data, Transaction *transaction);
int compare_transactions(const void *a, const void *b);
int format_transaction_log(char *log, const Transaction *transaction);
int log_transactions(const char *data, char *log, size_t transaction_count);

#endif // LOG_TRANSACTIONS_H