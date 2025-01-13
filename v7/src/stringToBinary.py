def binary_string_to_bytes(binary_str):
    # Teile den Binärstring in Blöcke von 8 Bits und konvertiere sie in Bytes
    byte_list = [binary_str[i:i+8] for i in range(0, len(binary_str), 8)]
    return bytes([int(byte, 2) for byte in byte_list])

def convert_file_to_binary(input_file, output_file):
    with open(input_file, 'r') as infile:
        binary_str = infile.read().strip()

    # Konvertiere den Binärstring in Bytes
    byte_data = binary_string_to_bytes(binary_str)

    # Schreibe die Bytes in die Binärdatei
    with open(output_file, 'wb') as outfile:
        outfile.write(byte_data)

# Beispielaufruf
input_file = '../arduinoLogs/arduinoIn.txt'  # Die Datei, die den Binärstring enthält
output_file = '../arduinoLogs/arduinoIn.bin'  # Die Datei, in die die Binärdaten geschrieben werden

convert_file_to_binary(input_file, output_file)
