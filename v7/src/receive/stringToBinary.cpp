#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>

// Funktion, um einen Binärstring in Bytes zu konvertieren
std::vector<uint8_t> binaryStringToBytes(const std::string &binaryStr) {
    std::vector<uint8_t> byteList;
    for (size_t i = 0; i < binaryStr.size(); i += 8) {
        std::string byteString = binaryStr.substr(i, 8);
        uint8_t byte = std::bitset<8>(byteString).to_ulong();
        byteList.push_back(byte);
    }
    return byteList;
}

// Funktion, um eine Datei von Binärstring zu Binärdatei zu konvertieren
void convertFileToBinary(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream infile(inputFile);
    if (!infile) {
        std::cerr << "Fehler beim Öffnen der Eingabedatei." << std::endl;
        return;
    }

    std::string binaryStr;
    infile >> binaryStr;
    infile.close();

    std::vector<uint8_t> byteData = binaryStringToBytes(binaryStr);

    std::ofstream outfile(outputFile, std::ios::binary);
    if (!outfile) {
        std::cerr << "Fehler beim Öffnen der Ausgabedatei." << std::endl;
        return;
    }
    outfile.write(reinterpret_cast<const char*>(byteData.data()), byteData.size());
    outfile.close();
}

// Beispielaufruf
int main() {
    std::string inputFile = "../arduinoLogs/arduinoIn.txt";  // Die Datei, die den Binärstring enthält
    std::string outputFile = "../arduinoLogs/arduinoIn.bin"; // Die Datei, in die die Binärdaten geschrieben werden

    convertFileToBinary(inputFile, outputFile);

    return 0;
}
