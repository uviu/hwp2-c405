import serial

# Konfiguration
arduino_port = "/dev/ttyUSB1"  # Ändere dies je nach deinem Betriebssystem und Port
baud_rate = 9600
output_file = "../arduinoLogs/arduinoIn.txt"  # Die Datei, in der die Zustände gespeichert werden

# Serielle Verbindung herstellen
try:
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    print(f"Verbunden mit {arduino_port}")
except serial.SerialException as e:
    print(f"Fehler beim Öffnen des Ports: {e}")
    exit()

# Daten empfangen und in die Datei
previousClockState = "0"
try:
    with open(output_file, "w") as file:  # Öffnen im Anhängemodus
        print(f"Schreibe Daten in {output_file}...")
        while True:
            if ser.in_waiting > 0:  # Prüfen, ob Daten verfügbar sind
                # Daten vom Arduino empfangen (die Zustände der Pins)
                line = ser.read(4).strip().decode('latin-1')

                # Überprüfen, ob die eingehenden Daten die erwartete Länge haben (4 Zeichen)
                if len(line) == 4:
                    if previousClockState != line[0]:
                        # Speichern der Pin-Zustände in der Date
                        file.write(f"{line}")
                        print(f"{line}")  # Ausgabe für Debugging
                        previousClockState = line[0]

except KeyboardInterrupt:
    print("\nAbbruch durch Benutzer.")
except Exception as e:
    print(f"Fehler: {e}")
finally:  
    ser.close()
    print("Serielle Verbindung geschlossen.")
