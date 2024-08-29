// log_transactions.h
#ifndef LOG_TRANSACTIONS_H
#define LOG_TRANSACTIONS_H

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define TIMESTAMP_LEN 19
#define VEH_REG_LEN 8
#define PROD_LEN 1
#define MILS_LEN 4
#define TRANSACT_LEN 2
#define FRAME_SIZE TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN + MILS_LEN + TRANSACT_LEN
#define MAX_TRANSACTIONS 100

typedef struct {
    char timestamp[TIMESTAMP_LEN+1];
    char vehicle_registration[VEH_REG_LEN+1];
    char product;
    int32_t mililiters;
    uint16_t transaction_id;
} Transaction;

int log_transactions(const char *data, char *log, size_t transaction_count);

#endif // LOG_TRANSACTIONS_H