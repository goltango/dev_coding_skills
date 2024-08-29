"""
This Python script generates a C source code file that defines a buffer containing 100 raw transaction records.
Each transaction is formatted with a random date, time, a three-letter label, a four-digit number, and a 
random single-letter code, followed by a fixed byte sequence.

The purpose of this script is to provide a sample buffer for testing a transaction processing function.
This function reads raw transactions from the buffer and processes them to generate a log file.

The generated C source code is saved as 'buffer_code.c'.
"""

import random
import datetime

NUM_TRANSACTIONS = 50

def random_date(start, end):
    return start + datetime.timedelta(days=random.randint(0, (end - start).days))

def random_time():
    return f"{random.randint(0, 23):02}:{random.randint(0, 59):02}:{random.randint(0, 59):02}"

def generate_random_transaction(index):
    date = random_date(datetime.date(2023, 1, 1), datetime.date(2024, 12, 31))
    time = random_time()
    label = ''.join(chr(65 + random.randint(0, 25)) for _ in range(3))  # Generates a 3 letters label
    number = f"{random.randint(1000, 9999)}"  # 4 digits number
    code = chr(65 + random.randint(0, 25))  # Random character from A to Z

    # Format MM/DD/YYYY
    date_str = date.strftime("%m/%d/%Y")
    
    # Generates transaction in specific format
    return f'"{date_str} {time}{label} {number}{code}\\x01\\x00\\x01\\x00\\x01\\x00"'

def generate_buffer():
    transactions = [generate_random_transaction(i) for i in range(NUM_TRANSACTIONS)]
    buffer = "const char buffer[] =\n" + "\n".join("        " + transaction for transaction in transactions)
    buffer += ";"
    return buffer

def main():
    buffer_code = generate_buffer()
    buffer_code += f'\nint transactions_generated = {NUM_TRANSACTIONS};'

    with open("buffer_code.c", "w") as file:
        file.write(buffer_code)
    
    print("Source code for buffer stored in 'buffer_code.c'.")

if __name__ == "__main__":
    main()
