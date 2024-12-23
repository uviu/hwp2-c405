#include <fstream>
#include <iostream>
#include <b15f/b15f.h>

B15F& drv = B15F::getInstance();

void receiveData(const std::string &binaryFile){
    std::ofstream outFile(binaryFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }
    
    while (true) {
      unsigned char mask = 0b00001111;
      unsigned char portValue = drv.getRegister(&PORTA);
      unsigned char value = portValue & mask;
      std::cout << "PORTA (Bits 1-4): " << std::bitset<4>(value) << std::endl;

    }
}

int main() {
    unsigned char inputMask = 0b11110000;
    drv.setRegister(&DDRA, drv.getRegister(&DDRA) & ~inputMask);

    const std::string outputFile = "../receivedb15f.bin";
    receiveData(outputFile);

    return 0;
}
