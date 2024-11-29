#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

unsigned char crc8(const std::vector<unsigned char>& data) {
    unsigned char crc = 0xFF;
    for (unsigned char byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x80) { 
                crc = (crc << 1) ^ 0x07; 
            }
            else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void buildPackage() {
    const char* dateipfad = "100krandom.bin";
    std::ifstream datei(dateipfad, std::ios::binary);
    if (!datei) {
        std::cerr << "Fehler: Datei konnte nicht geöffnet werden!" << std::endl;
        return;
    }

    datei.seekg(0, std::ios::end);
    std::streamsize dateigroesse = datei.tellg();
    datei.seekg(0, std::ios::beg);

    if (dateigroesse == 0) {
        std::cerr << "Fehler: Datei ist leer!" << std::endl;
        datei.close();
        return;
    }

    std::vector<unsigned char> puffer;
    std::bitset<4> start(0xF);
    unsigned char startByte = static_cast<unsigned char>(start.to_ulong());
    puffer.push_back(startByte);

    unsigned char byte;
    size_t bytesRead = 0;

    std::vector<unsigned char> block;

    while (datei.read(reinterpret_cast<char*>(&byte), 1)) {
        puffer.push_back(byte);
        block.push_back(byte);
        bytesRead++;

        if (bytesRead % 8 == 0) {
            puffer.push_back(crc8(block));
            block.clear();
            puffer.push_back(startByte);
        }
    }

    std::cout << "Die ersten 100 Bytes des neuen Puffers (in Binär):" << std::endl;
    for (size_t i = 0; i < std::min<size_t>(100, puffer.size()); ++i) {
        std::bitset<8> bin(puffer[i]);
        std::cout << bin << " ";
    }
    std::cout << std::endl;

    datei.close();
}

int main() {
    buildPackage();
    return 0;
}