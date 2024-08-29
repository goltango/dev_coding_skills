#include "log_transactions.h"
#include <time.h>

// Define constants for transaction size
#define TIMESTAMP_LEN 19
#define VEH_REG_LEN 8
#define PROD_LEN 1
#define MILS_LEN 4
#define TRANSACT_LEN 2
#define TRANS_FRAME_SIZE (TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN + MILS_LEN + TRANSACT_LEN)
#define NUM_TRANSACTIONS 100
#define MAX_LOG_BUFFER_SIZE (NUM_TRANSACTIONS * 72)

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

// Function to generate raw transaction data
void generate_transaction_data(char *buffer, size_t buffer_size) {
    // Ensure buffer size is sufficient
    if (buffer_size < NUM_TRANSACTIONS * TRANS_FRAME_SIZE) {
        fprintf(stderr, "Buffer size is too small.\n");
        return;
    }

    // Generate a timestamp in the format "mm/dd/yyyy hh:mm:ss"
    time_t rawtime;
    struct tm * timeinfo;
    char timestamp[TIMESTAMP_LEN + 1]; // Extra byte for null terminator
    int i;

    // Set up a starting time point
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    for (i = 0; i < NUM_TRANSACTIONS; ++i) {
        // Update timestamp
        strftime(timestamp, TIMESTAMP_LEN + 1, "%m/%d/%Y %H:%M:%S", timeinfo);
        // Increment time by 1 minute for each transaction
        timeinfo->tm_min += 1;
        mktime(timeinfo);

        // Fill in the transaction data
        memcpy(buffer + i * TRANS_FRAME_SIZE, timestamp, TIMESTAMP_LEN);
        memcpy(buffer + i * TRANS_FRAME_SIZE + TIMESTAMP_LEN, "AAA 1234", VEH_REG_LEN);
        buffer[i * TRANS_FRAME_SIZE + TIMESTAMP_LEN + VEH_REG_LEN] = 'P'; // Product

        // Example mililiters (e.g., 1000 milliliters)
        uint32_t mililiters = 1000;
        memcpy(buffer + i * TRANS_FRAME_SIZE + TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN, &mililiters, MILS_LEN);

        // Example transaction_id (e.g., 1)
        uint16_t transaction_id = (uint16_t)(i + 1);
        memcpy(buffer + i * TRANS_FRAME_SIZE + TIMESTAMP_LEN + VEH_REG_LEN + PROD_LEN + MILS_LEN, &transaction_id, TRANSACT_LEN);
    }
}

int main() {
    // Lets simulate a buffer with raw transactions
    //char buffer[NUM_TRANSACTIONS * TRANS_FRAME_SIZE];
    //generate_transaction_data(buffer, sizeof(buffer));
    const char buffer[] =
        "11/09/2023 21:52:44BTQ 6704M\x01\x00\x01\x00\x01\x00"  // Transacción 1
        "12/26/2024 15:49:54HRA 2988R\x01\x00\x01\x00\x01\x00"  // Transacción 2
        "03/14/2023 02:54:22TWM 3861T\x01\x00\x01\x00\x01\x00"  // Transacción 3
        "02/10/2023 10:59:51XQC 7972A\x01\x00\x01\x00\x01\x00"  // Transacción 4
        "02/18/2023 06:15:18WNO 5861Z\x01\x00\x01\x00\x01\x00"  // Transacción 5
        "09/04/2024 04:59:32ETJ 3582C\x01\x00\x01\x00\x01\x00"  // Transacción 6
        "03/21/2023 02:38:34IUT 4787Z\x01\x00\x01\x00\x01\x00"  // Transacción 7
        "09/12/2024 20:59:28AKW 7325N\x01\x00\x01\x00\x01\x00"  // Transacción 8
        "05/25/2024 19:16:43UWS 8618B\x01\x00\x01\x00\x01\x00"  // Transacción 9
        "10/23/2024 16:27:25QBX 2592T\x01\x00\x01\x00\x01\x00"  // Transacción 10
        "11/03/2024 02:57:29BOS 4038A\x01\x00\x01\x00\x01\x00"  // Transacción 11
        "05/08/2023 14:16:39EWU 9937P\x01\x00\x01\x00\x01\x00"  // Transacción 12
        "01/01/2024 12:04:24ILL 2545R\x01\x00\x01\x00\x01\x00"  // Transacción 13
        "03/04/2024 12:53:15JPU 2552K\x01\x00\x01\x00\x01\x00"  // Transacción 14
        "02/09/2023 15:25:10XBC 6574F\x01\x00\x01\x00\x01\x00"  // Transacción 15
        "08/12/2023 12:12:47LPF 7285G\x01\x00\x01\x00\x01\x00"  // Transacción 16
        "08/11/2024 13:31:32XWF 7614U\x01\x00\x01\x00\x01\x00"  // Transacción 17
        "08/02/2023 04:48:53YTJ 4391A\x01\x00\x01\x00\x01\x00"  // Transacción 18
        "06/23/2024 02:04:52JHP 4426F\x01\x00\x01\x00\x01\x00"  // Transacción 19
        "09/14/2024 12:05:28ZPE 3485I\x01\x00\x01\x00\x01\x00"  // Transacción 20
        "12/12/2024 08:46:53WOM 3911U\x01\x00\x01\x00\x01\x00"  // Transacción 21
        "12/24/2023 13:08:51CCR 2075E\x01\x00\x01\x00\x01\x00"  // Transacción 22
        "12/03/2024 10:04:15AOI 8403I\x01\x00\x01\x00\x01\x00"  // Transacción 23
        "04/11/2024 17:07:59PEN 3986G\x01\x00\x01\x00\x01\x00"  // Transacción 24
        "01/20/2023 16:12:14ZZU 4099Z\x01\x00\x01\x00\x01\x00"  // Transacción 25
        "05/15/2024 13:04:54QSU 1834Q\x01\x00\x01\x00\x01\x00"  // Transacción 26
        "05/22/2023 23:54:29AMA 2854V\x01\x00\x01\x00\x01\x00"  // Transacción 27
        "10/01/2024 21:39:25CXN 2166Y\x01\x00\x01\x00\x01\x00"  // Transacción 28
        "04/21/2023 12:55:07OKX 8313D\x01\x00\x01\x00\x01\x00"  // Transacción 29
        "11/04/2024 09:05:40KVA 1284J\x01\x00\x01\x00\x01\x00"  // Transacción 30
        "12/03/2024 04:29:16HLB 9701O\x01\x00\x01\x00\x01\x00"  // Transacción 31
        "09/29/2023 01:21:07LCJ 7212O\x01\x00\x01\x00\x01\x00"  // Transacción 32
        "06/23/2023 18:43:11FBJ 2419H\x01\x00\x01\x00\x01\x00"  // Transacción 33
        "03/29/2023 11:31:32ILW 1027L\x01\x00\x01\x00\x01\x00"  // Transacción 34
        "08/09/2023 07:05:07KLG 9433U\x01\x00\x01\x00\x01\x00"  // Transacción 35
        "08/24/2024 23:39:47JRE 1823P\x01\x00\x01\x00\x01\x00"  // Transacción 36
        "04/03/2024 12:56:51ICO 6966C\x01\x00\x01\x00\x01\x00"  // Transacción 37
        "06/12/2024 11:55:03DCJ 2302G\x01\x00\x01\x00\x01\x00"  // Transacción 38
        "05/18/2024 06:49:48JHZ 7320H\x01\x00\x01\x00\x01\x00"  // Transacción 39
        "03/27/2024 09:04:20MMQ 2740C\x01\x00\x01\x00\x01\x00"  // Transacción 40
        "04/06/2024 19:18:08MAZ 8231A\x01\x00\x01\x00\x01\x00"  // Transacción 41
        "03/01/2023 01:13:02QTB 6831A\x01\x00\x01\x00\x01\x00"  // Transacción 42
        "02/18/2024 17:54:00IGM 6755S\x01\x00\x01\x00\x01\x00"  // Transacción 43
        "10/23/2024 10:25:39UMG 8285T\x01\x00\x01\x00\x01\x00"  // Transacción 44
        "02/19/2024 23:20:32ZGB 4695N\x01\x00\x01\x00\x01\x00"  // Transacción 45
        "02/15/2023 11:58:28SHD 7137V\x01\x00\x01\x00\x01\x00"  // Transacción 46
        "12/13/2024 11:15:51VKG 4378X\x01\x00\x01\x00\x01\x00"  // Transacción 47
        "06/20/2024 01:29:49CFG 7353J\x01\x00\x01\x00\x01\x00"  // Transacción 48
        "03/12/2024 07:47:11NZN 8279F\x01\x00\x01\x00\x01\x00"  // Transacción 49
        "07/09/2024 10:30:48ZFZ 5751S\x01\x00\x01\x00\x01\x00"  // Transacción 50
        "01/15/2023 11:14:54ALF 3250G\x01\x00\x01\x00\x01\x00"  // Transacción 51
        "07/16/2023 03:15:25XUO 1708H\x01\x00\x01\x00\x01\x00"  // Transacción 52
        "07/09/2023 22:30:06NZQ 7232U\x01\x00\x01\x00\x01\x00"  // Transacción 53
        "09/17/2023 03:29:50UML 8538S\x01\x00\x01\x00\x01\x00"  // Transacción 54
        "07/01/2023 22:07:46CGX 5017N\x01\x00\x01\x00\x01\x00"  // Transacción 55
        "07/09/2023 11:45:57UBT 9000X\x01\x00\x01\x00\x01\x00"  // Transacción 56
        "11/09/2023 03:23:52ZWP 6136N\x01\x00\x01\x00\x01\x00"  // Transacción 57
        "05/23/2024 15:56:53UUY 5441H\x01\x00\x01\x00\x01\x00"  // Transacción 58
        "02/15/2023 17:27:44UXJ 2620U\x01\x00\x01\x00\x01\x00"  // Transacción 59
        "08/08/2023 08:10:34LDM 9776I\x01\x00\x01\x00\x01\x00"  // Transacción 60
        "11/09/2023 14:20:06AUG 3569P\x01\x00\x01\x00\x01\x00"  // Transacción 61
        "08/28/2024 00:32:53CKW 3838E\x01\x00\x01\x00\x01\x00"  // Transacción 62
        "01/11/2024 00:06:29BUY 7855F\x01\x00\x01\x00\x01\x00"  // Transacción 63
        "10/29/2023 22:33:16OXS 1482M\x01\x00\x01\x00\x01\x00"  // Transacción 64
        "04/01/2023 12:07:01HNR 3211O\x01\x00\x01\x00\x01\x00"  // Transacción 65
        "08/18/2024 17:33:03SMW 6630P\x01\x00\x01\x00\x01\x00"  // Transacción 66
        "10/13/2023 06:11:04YJQ 9339N\x01\x00\x01\x00\x01\x00"  // Transacción 67
        "02/23/2023 21:29:32NML 4087I\x01\x00\x01\x00\x01\x00"  // Transacción 68
        "12/03/2023 12:47:12TWP 8513F\x01\x00\x01\x00\x01\x00"  // Transacción 69
        "03/20/2024 09:00:31VUB 3782O\x01\x00\x01\x00\x01\x00"  // Transacción 70
        "06/19/2024 06:31:42IQO 3339O\x01\x00\x01\x00\x01\x00"  // Transacción 71
        "08/28/2024 19:03:48WSZ 6319E\x01\x00\x01\x00\x01\x00"  // Transacción 72
        "04/19/2024 16:38:48ZDK 9736Q\x01\x00\x01\x00\x01\x00"  // Transacción 73
        "11/21/2023 20:35:05WGX 5743Y\x01\x00\x01\x00\x01\x00"  // Transacción 74
        "06/11/2024 01:29:06SAG 1494S\x01\x00\x01\x00\x01\x00"  // Transacción 75
        "12/29/2024 09:36:03SIR 6451E\x01\x00\x01\x00\x01\x00"  // Transacción 76
        "07/20/2024 10:25:18EVI 4565W\x01\x00\x01\x00\x01\x00"  // Transacción 77
        "04/20/2024 20:18:56LKH 2100R\x01\x00\x01\x00\x01\x00"  // Transacción 78
        "06/02/2023 10:17:30PLF 5393E\x01\x00\x01\x00\x01\x00"  // Transacción 79
        "09/25/2023 20:06:42NOH 7726S\x01\x00\x01\x00\x01\x00"  // Transacción 80
        "11/12/2024 15:48:41PUK 4346A\x01\x00\x01\x00\x01\x00"  // Transacción 81
        "04/10/2023 17:52:37DVE 4652Y\x01\x00\x01\x00\x01\x00"  // Transacción 82
        "04/17/2023 10:19:12HOR 6949V\x01\x00\x01\x00\x01\x00"  // Transacción 83
        "07/28/2023 16:59:49EDI 3955H\x01\x00\x01\x00\x01\x00"  // Transacción 84
        "04/20/2024 05:06:41RBJ 3153M\x01\x00\x01\x00\x01\x00"  // Transacción 85
        "08/10/2023 02:02:32NGG 5404B\x01\x00\x01\x00\x01\x00"  // Transacción 86
        "07/13/2023 09:09:19XXD 3590U\x01\x00\x01\x00\x01\x00"  // Transacción 87
        "11/17/2024 19:44:33HTG 4171Z\x01\x00\x01\x00\x01\x00"  // Transacción 88
        "02/07/2023 06:45:14HTH 1291V\x01\x00\x01\x00\x01\x00"  // Transacción 89
        "07/12/2023 03:03:29GJZ 3622A\x01\x00\x01\x00\x01\x00"  // Transacción 90
        "11/19/2023 22:04:27SWS 4864N\x01\x00\x01\x00\x01\x00"  // Transacción 91
        "07/18/2023 16:25:42BXL 2929R\x01\x00\x01\x00\x01\x00"  // Transacción 92
        "12/11/2024 13:37:41FKH 5242R\x01\x00\x01\x00\x01\x00"  // Transacción 93
        "07/08/2023 23:44:43WEF 3942W\x01\x00\x01\x00\x01\x00"  // Transacción 94
        "08/12/2023 12:38:45ALF 8398R\x01\x00\x01\x00\x01\x00"  // Transacción 95
        "03/24/2023 02:03:17IOG 3631A\x01\x00\x01\x00\x01\x00"  // Transacción 96
        "03/30/2023 09:26:46LKH 8185S\x01\x00\x01\x00\x01\x00"  // Transacción 97
        "06/17/2024 20:37:15ZLU 5004E\x01\x00\x01\x00\x01\x00"  // Transacción 98
        "05/11/2024 20:09:15XWZ 7052C\x01\x00\x01\x00\x01\x00"  // Transacción 99
        "01/05/2023 05:11:20FWJ 3757I\x01\x00\x01\x00\x01\x00";  // Transacción 100;
        int bytes_written = 0;
    size_t transaction_count = 100;
    
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