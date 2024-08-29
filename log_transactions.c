// log_transactions.c
#include "log_transactions.h"

/**
 * @brief Parses a transaction from raw data into a `Transaction` structure.
 *
 * This function extracts and converts the timestamp from ASCII format to a Unix timestamp.
 * It then parses additional transaction details such as vehicle registration, product,
 * mililiters, and transaction ID.
 *
 * @param[in]  data       Pointer to the raw data buffer containing transaction information.
 * @param[out] transaction Pointer to the `Transaction` structure to be filled with parsed data.
 */
void parse_transaction(const char *data, Transaction *transaction) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    char timestamp[TIMESTAMP_LEN+1];

    // Copy timestamp ASCII format
    memcpy(timestamp, data, TIMESTAMP_LEN);
    timestamp[TIMESTAMP_LEN] = '\0';  // Final NULL character

    // Convert timestamp ASCII to time_t (timestamp Unix)
    sscanf(timestamp, "%2d/%2d/%4d %2d:%2d:%2d",
           &tm.tm_mday,  // Day of a month
           &tm.tm_mon,   // Month (0-11 en struct tm)
           &tm.tm_year,  // Year since 1900
           &tm.tm_hour,  // Hour
           &tm.tm_min,   // Minute
           &tm.tm_sec);  // Second

    // Adjust year and month for tm struct
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;     // (0-11)

    // Convert struct tm to Unix timestamp
    transaction->timestamp_unix = mktime(&tm);

    // Parse other data
    memcpy(transaction->vehicle_registration, data + TIMESTAMP_LEN, VEH_REG_LEN);
    transaction->vehicle_registration[VEH_REG_LEN] = '\0';
    transaction->product = *(data + PRODUCT_OFFSET);
    memcpy(&transaction->mililiters, data + MILILIT_OFFSET, sizeof(int32_t));
    memcpy(&transaction->transaction_id, data + TRANSAC_OFFSET, sizeof(uint16_t));
}

/**
 * @brief Compares two transactions based on their Unix timestamps.
 *
 * This function is used for sorting transactions by their timestamp. It compares the Unix
 * timestamp values of two `Transaction` structures and returns the difference.
 *
 * @param[in] a Pointer to the first `Transaction` structure.
 * @param[in] b Pointer to the second `Transaction` structure.
 * 
 * @return An integer less than, equal to, or greater than zero, depending on whether the
 *         first transaction's timestamp is less than, equal to, or greater than the second
 *         transaction's timestamp.
 */
int compare_transactions(const void *a, const void *b) {
    const Transaction *trans_a = (const Transaction *)a;
    const Transaction *trans_b = (const Transaction *)b;
    return (trans_a->timestamp_unix - trans_b->timestamp_unix);
}

/**
 * @brief Formats a transaction record into a human-readable log string.
 *
 * This function converts a Unix timestamp from a `Transaction` structure to a formatted
 * date and time string, and then writes the transaction details into the provided log buffer.
 *
 * @param[out] log Pointer to the buffer where the formatted log string will be written.
 * @param[in] transaction Pointer to the `Transaction` structure containing the data to be formatted.
 * 
 * @return The number of bytes written to the `log` buffer.
 */
int format_transaction_log(char *log, const Transaction *transaction) {
    // Buffer for formatted date and time
    char formatted_time[20];
    
    // Convert unix timestamp to tm struct
    struct tm *time_info = localtime(&transaction->timestamp_unix);
    
    // Format to dd/mm/yy hh:mm:ss
    strftime(formatted_time, sizeof(formatted_time), "%d/%m/%y %H:%M:%S", time_info);
    
    // Write-down to log
    return sprintf(log, "[%s] id: %05d, reg: %.8s, prod: %c, ltrs: %+.7d\n",
                   formatted_time,
                   transaction->transaction_id,
                   transaction->vehicle_registration,
                   transaction->product,
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