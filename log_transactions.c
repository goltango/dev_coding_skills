// log_transactions.c
#include "log_transactions.h"

void parse_transaction(const char *data, Transaction *transaction) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    // Copiar timestamp en formato cadena
    memcpy(transaction->timestamp, data, TIMESTAMP_LEN);
    transaction->timestamp[TIMESTAMP_LEN] = '\0';  // Final NULL character

    // Convertir fecha en formato cadena a time_t (timestamp Unix)
    sscanf(transaction->timestamp, "%2d/%2d/%4d %2d:%2d:%2d",
           &tm.tm_mday,  // Día del mes
           &tm.tm_mon,   // Mes (0-11 en struct tm)
           &tm.tm_year,  // Año desde 1900
           &tm.tm_hour,  // Hora
           &tm.tm_min,   // Minuto
           &tm.tm_sec);  // Segundo

    // Ajustar el año y mes para struct tm
    tm.tm_year -= 1900; // Ajuste del año
    tm.tm_mon -= 1;     // Ajuste del mes (0-11)

    // Convertir struct tm a timestamp Unix
    transaction->timestamp_unix = mktime(&tm);

    // Parse other data
    memcpy(transaction->vehicle_registration, data + TIMESTAMP_LEN, VEH_REG_LEN);
    transaction->vehicle_registration[VEH_REG_LEN] = '\0';
    transaction->product = *(data + PRODUCT_OFFSET);
    memcpy(&transaction->mililiters, data + MILILIT_OFFSET, sizeof(int32_t));
    memcpy(&transaction->transaction_id, data + TRANSAC_OFFSET, sizeof(uint16_t));
}

int compare_transactions(const void *a, const void *b) {
    const Transaction *trans_a = (const Transaction *)a;
    const Transaction *trans_b = (const Transaction *)b;
    return (trans_a->timestamp_unix - trans_b->timestamp_unix);
}

int format_transaction_log(char *log, const Transaction *transaction) {
    // Buffer para la fecha y hora formateada
    char formatted_time[20];
    
    // Convertir timestamp Unix a struct tm
    struct tm *time_info = localtime(&transaction->timestamp_unix);
    
    // Formatear la fecha y hora en dd/mm/yy hh:mm:ss
    strftime(formatted_time, sizeof(formatted_time), "%d/%m/%y %H:%M:%S", time_info);
    
    // Escribir la transacción en el log con el formato deseado
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