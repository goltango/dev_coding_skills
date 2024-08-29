// log_transactions.c
#include "log_transactions.h"

#define SUCCESS 0
#define ERROR_INVALID_DATE -1

/**
 * @brief Parses transaction data from a given input string.
 *
 * This function extracts and parses various fields, including a timestamp,
 * vehicle registration, product code, mililiters, and transaction ID from
 * the provided `data` string. It converts the timestamp from ASCII format
 * to a Unix timestamp and validates the parsed date and time. If the date
 * or time is invalid, the function returns an error code.
 *
 * @param[in] data The input string containing the transaction data.
 * @param[out] transaction A pointer to a Transaction structure where the
 *                         parsed data will be stored.
 * 
 * @return SUCCESS if the transaction is successfully parsed, or 
 *         ERROR_INVALID_DATE if the date or time is invalid.
 */
int parse_transaction(const char *data, Transaction *transaction) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    char timestamp_ascii[TIMESTAMP_LEN+1];

    // Copy timestamp ASCII format
    memset(timestamp_ascii, '\0', sizeof(timestamp_ascii));
    memcpy(timestamp_ascii, data, TIMESTAMP_LEN);
    timestamp_ascii[TIMESTAMP_LEN] = '\0';  // Final NULL character

    // Convert timestamp ASCII to struct tm
    int parsed_fields = sscanf(timestamp_ascii, "%2d/%2d/%4d %2d:%2d:%2d",
                               &tm.tm_mon,   // Month (1-12 in input)
                               &tm.tm_mday,  // Day of the month
                               &tm.tm_year,  // Year (full, e.g., 2023)
                               &tm.tm_hour,  // Hour
                               &tm.tm_min,   // Minute
                               &tm.tm_sec);  // Second

    // Check if all fields were parsed correctly
    if (parsed_fields != 6) {
        return ERROR_INVALID_DATE;
    }

    // Validate the date and time ranges
    if (tm.tm_mon < 1 || tm.tm_mon > 12 ||
        tm.tm_mday < 1 || tm.tm_mday > 31 ||
        tm.tm_hour < 0 || tm.tm_hour > 23 ||
        tm.tm_min < 0 || tm.tm_min > 59 ||
        tm.tm_sec < 0 || tm.tm_sec > 59) {
        return ERROR_INVALID_DATE;
    }

    // Adjust year and month for tm struct
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;  // Convert month to 0-11 range

    // Convert struct tm to Unix timestamp
    time_t timestamp = mktime(&tm);
    if (timestamp == -1) {
        return ERROR_INVALID_DATE;
    }

    transaction->timestamp_unix = timestamp;

    // Parse other data
    memcpy(transaction->vehicle_registration, data + TIMESTAMP_LEN, VEH_REG_LEN);
    transaction->vehicle_registration[VEH_REG_LEN] = '\0';
    transaction->product = *(data + PRODUCT_OFFSET);
    memcpy(&transaction->mililiters, data + MILILIT_OFFSET, sizeof(int32_t));
    memcpy(&transaction->transaction_id, data + TRANSAC_OFFSET, sizeof(uint16_t));

    return SUCCESS;
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
 * @brief Parses, sorts, and logs transaction data.
 *
 * This function processes a block of transaction data by first parsing each
 * transaction, sorting them by their timestamp, and then formatting them into
 * a log. The transactions are limited by `MAX_TRANSACTIONS`. If the transaction
 * count exceeds this limit or if a parsing error occurs, the function will 
 * return an error code.
 *
 * @param[in]  data              Pointer to the raw transaction data.
 * @param[out] log               Pointer to the buffer where the formatted log will be stored.
 * @param[in]  transaction_count Number of transactions to process.
 *
 * @return The total number of bytes written to the log on success, or -1 on error.
 */

int log_transactions(const char *data, char *log, size_t transaction_count) {
    int total_bytes_written = 0;
    int bytes_written = 0;
    int result = SUCCESS;

    if (transaction_count > MAX_TRANSACTIONS) {
        return -1; // Error: cannot process more than 100 transactions
    }

    Transaction transactions[MAX_TRANSACTIONS];

    // Parse transactions
    for (size_t i = 0; i < transaction_count; i++) {
        result = parse_transaction(data + i * TRANS_FRAME_SIZE, &transactions[i]);
        if (result != SUCCESS) {
            fprintf(stderr, "Error parsing transaction at index %zu: %d\n", i, result);
            return -1;
        }
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