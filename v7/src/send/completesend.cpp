#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <b15f/b15f.h>

unsigned char crc8(const std::vector<unsigned char>& data) {
    unsigned char crc = 0xFF;
    for (unsigned char byte : data) {
        crc ^= byte;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int getUpcomingBlockLength(int dateigroesse) {
    return (dateigroesse >= 8) ? 8 : dateigroesse;
}

void buildPackage(std::istream& input, const std::string& outputPath) {
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    size_t dateigroesse = buffer.size();

    if (dateigroesse == 0) {
        std::cerr << "Fehler: Eingabedatei ist leer!" << std::endl;
        return;
    }

    std::vector<unsigned char> puffer;
    unsigned char lengthHeader = getUpcomingBlockLength(dateigroesse);
    puffer.push_back(lengthHeader);

    std::vector<unsigned char> block;

    for (size_t i = 0; i < buffer.size(); ++i) {
        unsigned char byte = buffer[i];
        puffer.push_back(byte);
        block.push_back(byte);

        if (block.size() == 8 || i == buffer.size() - 1) {
            puffer.push_back(crc8(block));
            block.clear();
            dateigroesse -= getUpcomingBlockLength(dateigroesse);
            if (dateigroesse > 0) {
                lengthHeader = getUpcomingBlockLength(dateigroesse);
                puffer.push_back(lengthHeader);
            }
        }
    }

    std::ofstream ausgabeDatei(outputPath, std::ios::binary);
    if (!ausgabeDatei) {
        std::cerr << "Fehler: Ausgabedatei konnte nicht geschrieben werden!" << std::endl;
        return;
    }

    ausgabeDatei.write(reinterpret_cast<const char*>(puffer.data()), puffer.size());
    ausgabeDatei.close();
}

B15F& drv = B15F::getInstance();

void outputDataOnClock(const std::string& binaryFile) {
    std::ifstream file(binaryFile, std::ios::binary);
    if (!file) {
        std::cerr << "Fehler beim Öffnen der Datei: " << binaryFile << std::endl;
        return;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    size_t bitIndex = 0;
    bool clockState = 1;

    int binaryPackage = 0;

    while (bitIndex < buffer.size() * 8) {
        binaryPackage = (binaryPackage << 1 | clockState);
        binaryPackage = (binaryPackage << 1 | 0);
        for (int i = 0; i < 2; ++i) {
            size_t nibbleIndex = (bitIndex + i) / 8;
            size_t bitOffset = (bitIndex + i) % 8;

            if (nibbleIndex < buffer.size()) {
                bool bit = (buffer[nibbleIndex] >> (7 - bitOffset)) & 1;
                binaryPackage = (binaryPackage << 1 | bit);
            } else {
                binaryPackage = (binaryPackage << 1 | 0);
            }
        }
        bitIndex += 2;
        clockState = !clockState;
        drv.setRegister(&PORTA, binaryPackage);
        std::cout << std::bitset<4>(binaryPackage) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(int argc, char* argv[]) {
    std::istream* input = &std::cin;
    bool fromFile = false;
    std::string inputPath;
    std::string outputPath = "../../encodedTestfiles/encoded.bin";

    if (argc > 1) {
        inputPath = argv[1];
        fromFile = true;
    }

    if (fromFile) {
        std::ifstream file(inputPath, std::ios::binary);
        if (!file) {
            std::cerr << "Fehler beim Öffnen der Datei: " << inputPath << std::endl;
            return 1;
        }
        buildPackage(file, outputPath);
    } else {
        buildPackage(*input, outputPath);
    }

    unsigned char outputMask = 0b00001111;
    drv.setRegister(&DDRA, drv.getRegister(&DDRA) | outputMask);

    outputDataOnClock(outputPath);

    drv.setRegister(&PORTA, 0b0000);
    return 0;
}
