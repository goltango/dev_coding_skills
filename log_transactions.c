// log_transactions.c
#include "log_transactions.h"

/**
 * @brief Parses a raw transaction data buffer into a Transaction structure.
 *
 * This function extracts and copies specific fields from a buffer containing
 * transaction data into a `Transaction` structure. The buffer is assumed to
 * be in a contiguous format without padding bytes. It only fill one transaction,
 * so, multiples calls to this function has to be made if multiples transactions
 * need to be parsed.
 *
 * @param[in]  data        Pointer to the raw data buffer containing transaction information.
 * 
 * @param[out] transaction Pointer to a `Transaction` structure where the parsed data will be stored.
 *
 * @note The `data` buffer should be aligned and formatted according to the defined lengths:
 *       - Timestamp: `TIMESTAMP_LEN` bytes (ASCII)
 *       - Vehicle Registration: `VEH_REG_LEN` bytes (ASCII)
 *       - Product: `PROD_LEN` byte (ASCII)
 *       - Milliliters: `MILS_LEN` bytes (binary, `int32_t`)
 *       - Transaction ID: `TRANSACT_LEN` bytes (binary, `uint16_t`)
 *
 */
void parse_transaction(const char *data, Transaction *transaction) {
    memcpy(transaction->timestamp, data, TIMESTAMP_LEN);
    transaction->timestamp[TIMESTAMP_LEN] = '\0';  // Final NULL character
    memcpy(transaction->vehicle_registration, data + TIMESTAMP_LEN, VEH_REG_LEN);
    transaction->vehicle_registration[VEH_REG_LEN] = '\0';
    transaction->product = *(data + PRODUCT_OFFSET);
    memcpy(&transaction->mililiters, data + MILILIT_OFFSET, sizeof(int32_t));
    memcpy(&transaction->transaction_id, data + TRANSAC_OFFSET, sizeof(uint16_t));
}

/**
 * @brief Compares two transactions based on their timestamps for sorting.
 *
 * This function is used by the `qsort` function to compare two `Transaction` structures.
 * It compares the timestamps of the transactions to determine their order.
 *
 * @param[in]  a  Pointer to the first `Transaction` structure to be compared.
 * @param[in]  b  Pointer to the second `Transaction` structure to be compared.
 *
 * @return An integer less than, equal to, or greater than zero, depending on whether
 *         the timestamp of the first transaction is less than, equal to, or greater than
 *         the timestamp of the second transaction, respectively.
 *
 * @note The function assumes that the timestamps in the `Transaction` structures are
 *       null-terminated strings in a consistent format that can be compared using `strcmp`.
 *       Ensure that the timestamps are properly formatted for correct comparison.
 */
int compare_transactions(const void *a, const void *b) {
    const Transaction *trans_a = (const Transaction *)a;
    const Transaction *trans_b = (const Transaction *)b;
    return strcmp(trans_a->timestamp, trans_b->timestamp);
}

/**
 * @brief Formats a transaction into a human-readable log string.
 *
 * This function formats the details of a `Transaction` structure into a log string
 * with a specific format. The resulting string is written to the `log` buffer.
 *
 * @param[out] log         Pointer to a buffer where the formatted log string will be written.
 *                         The buffer should be large enough to hold the formatted log entry.
 * @param[in]  transaction Pointer to a `Transaction` structure containing the transaction data
 *                         to be formatted.
 *
 * @return The number of characters written to the `log` buffer, excluding the null terminator.
 *
 * @note The format of the log string is as follows:
 *       - Date and time: [dd/mm/yyyy hh:mm:ss]
 *       - Transaction ID: id: DDDDD
 *       - Vehicle Registration: reg: AAA 1111
 *       - Product: prod: X
 *       - Liters: ltrs: ±DDDDDDD
 *       The `timestamp` field is parsed to extract the day, month, year, and time components.
 *       Ensure that the `log` buffer is appropriately sized to prevent buffer overflow.
 */
int format_transaction_log(char *log, const Transaction *transaction) {
    return sprintf(log, "[%.2s/%.2s/%.4s %.8s] id: %05d, reg: %.8s, prod: %c, ltrs: %+.7d\n",
                   transaction->timestamp + 3, transaction->timestamp, transaction->timestamp + 6,
                   transaction->timestamp + 11, transaction->transaction_id,
                   transaction->vehicle_registration, transaction->product,
                   transaction->mililiters / 1000);
}

/**
 * @brief Parses and logs a specified number of transactions from a data buffer.
 *
 * This function processes a given number of transactions from the input data buffer,
 * sorts them by date, and writes the formatted log entries to the output buffer.
 * If the number of transactions exceeds the maximum allowed, an error is returned.
 *
 * @param[in]  data            Pointer to the buffer containing raw transaction data.
 *                             The data must be in the format specified by the transaction structure.
 * @param[out] log             Pointer to the buffer where the formatted log entries will be written.
 *                             The buffer should be large enough to accommodate all log entries.
 * @param[in]  transaction_count The number of transactions to process from the data buffer.
 *
 * @return The total number of bytes written to the `log` buffer, excluding the null terminator.
 *         If `transaction_count` exceeds `MAX_TRANSACTIONS`, returns -1 to indicate an error.
 *
 * @note The function assumes that the raw transaction data is stored in a contiguous block
 *       with each transaction having a fixed size defined by `TRANS_FRAME_SIZE`.
 *       Ensure that the `log` buffer is sufficiently sized to prevent overflow, as
 *       the total length of the formatted log entries depends on the number of transactions.
 */
int log_transactions(const char *data, char *log, size_t transaction_count) {
    int total_bytes_written = 0;
    int bytes_written = 0;

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
    total_bytes_written = 0;
    for (size_t i = 0; i < transaction_count; i++) {
        bytes_written = format_transaction_log(log + total_bytes_written, &transactions[i]);
        total_bytes_written += bytes_written;
    }

    return total_bytes_written;
}