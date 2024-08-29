# dev_coding_skills requirements
This list should cover both the functional aspects of the code (what the function needs to do) and the non-functional aspects (how it should perform).

# Functional (F) requirements
F1: The function must process fuel transactions sent by different dispensers through a serial communication interface.

F2: Transactions are stored in a contiguous buffer without padding bytes and with the system's endianess.

F3: The function must read the following fields from the buffer:
* timestamp: 19 bytes, ASCII in the format [mm/dd/yyyy hh:mm].
* vehicle_registration: 8 bytes, ASCII in the format [AAA 1111].
* product: 1 byte, printable ASCII character.
* mililiters: 4 bytes, binary int32_t representing milliliters dispensed.
* transaction_id: 2 bytes, binary uint16_t.

F4: The function must not modify the buffer from which data is read.

F5: In case of corrupt data, the function should handle this situation aborting its execution.

F6: The function should be able to generate a log of transactions into another buffer when the function is called after a certain number of transactions or elapsed time.

F7: The log format must be in ASCII with the following structure:
* [dd/mm/yy hh:mm] id: DDDDD, reg: AAA 1111, prod: A, ltrs: ±DDDDDDD\n

F8: Transactions in the log must be ordered from least recent to most recent.

F9: The function prototype is:
int log_transactions(const char *data, char *log, size_t transaction_count);

F10: The function must process up to 100 transactions and return the total number of bytes written to the log buffer.

# Non-Functional Requirements (NF)
NF1: The function must handle up to 100 transactions efficiently (from a embedded system perspective).

NF2: The function must include error handling to manage corrupt transactions without affecting the entire buffer.

NF3: The function must not modify the original data buffer to allow re-execution in case of errors.

NF4: The function should use an intermediate buffer to sort transactions by date.

NF6: The function must handle character encoding properly and maintain the correct format for logging.

NF7: buffers shouldnt be resized in execution time.
