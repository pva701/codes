#include "wavreader.h"



int WAVReader::field::little(const unsigned char* a) const {
    int result = 0, pw = 1;
    for (int i = 0; i < size; ++i, pw *= 256) {
        result += pw * (*(a + i));
    }
    return result;
}

int WAVReader::field::big(const unsigned char* a) const {
    int result = 0, pw = 1;
    for (int i = size - 1; i >= 0; --i, pw *= 256) {
        result += pw * (*(a + i));
    }
    return result;
}

int WAVReader::field::endian(const unsigned char* a) const {
    if (isLittle)
        return little(a);
    return big(a);
}

WAVReader::field::field(int of, int sz, bool isLit) {
    offset = of;
    size = sz;
    isLittle = isLit;
}

//WAVReader
const WAVReader::field WAVReader::FILE_ID           = field(0, 4, 0);//big
const WAVReader::field WAVReader::FILE_SIZE         = field(4, 4, 1);//little
const WAVReader::field WAVReader::FILE_FORMAT       = field(8, 4, 0);//big

const WAVReader::field WAVReader::FILE_SUB1_ID      = field(12, 4, 0);//big
const WAVReader::field WAVReader::FILE_SUB1_SIZE    = field(16, 4, 1);//little
const WAVReader::field WAVReader::FILE_AUDIO_FORMAT = field(20, 2, 1);//little
const WAVReader::field WAVReader::FILE_NUM_CHANNELS = field(22, 2, 1);//little
const WAVReader::field WAVReader::FILE_SAMPLE_RATE  = field(24, 4, 1);//little
const WAVReader::field WAVReader::FILE_BYTERATE      = field(28, 4, 1);//little
const WAVReader::field WAVReader::FILE_BLOCK_ALIGN  = field(32, 2, 1);//little
const WAVReader::field WAVReader::FILE_BITS_PER_SEC = field(34, 2, 1);//little

const WAVReader::field WAVReader::FILE_SUB2_ID      = field(36, 4, 0);//big
const WAVReader::field WAVReader::FILE_SUB2_SIZE    = field(40, 4, 1);//little
//const int WAVReader::OFFSET_DATA                   = 44;

WAVReader::WAVReader(const char *path)
{
    f = fopen(path, "r");
    readHeader();
}

WAVReader::WAVReader(const std::string& s) {
    f = fopen(s.c_str(), "r");
    readHeader();
    readData();
}

WAVReader::~WAVReader() {
    fclose(f);
}

void WAVReader::readHeader() {
    fread(fmt, 1, OFFSET_DATA, f);
    readData();
}

void WAVReader::readData() {
    length = FILE_SUB2_SIZE.endian(fmt + FILE_SUB2_SIZE.offset) / (bitpersample() / 8);
    sizeOfType = bitpersample() / 8;
    int len = channels() * length;
    if (sizeOfType == 1) {
        unsigned char* curData = new unsigned char[len];
        data = new int[len];
        fread(curData, sizeOfType, len, f);
        for (int i = 0; i < len; ++i) data[i] = curData[i];
        delete[] curData;
    } else if (sizeOfType == 2) {
        short int* curData = new short int[len];
        data = new int[len];
        fread(curData, sizeOfType, len, f);
        for (int i = 0; i < len; ++i) data[i] = curData[i];
        delete[] curData;

    } else if (sizeOfType == 4) {
        data = new int[len];
        fread(data, sizeOfType, length, f);
    }
}

std::string WAVReader::id() const {
    return std::string(fmt + FILE_ID.offset, fmt + FILE_ID.offset + FILE_ID.size);
}

int WAVReader::size() const {
    return FILE_SIZE.endian(fmt + FILE_SIZE.offset);
}

std::string WAVReader::format()  const {
    return std::string(fmt + FILE_FORMAT.offset, fmt + FILE_FORMAT.offset + FILE_FORMAT.size);
}

int WAVReader::channels() const {
    return FILE_NUM_CHANNELS.endian(fmt + FILE_NUM_CHANNELS.offset);
}

int WAVReader::samplerate() const {
    return FILE_SAMPLE_RATE.endian(fmt + FILE_SAMPLE_RATE.offset);
}

int WAVReader::byterate() const {
    return FILE_BYTERATE.endian(fmt + FILE_BYTERATE.offset);
}

int WAVReader::bitpersample() const {
    return FILE_BITS_PER_SEC.endian(fmt + FILE_BITS_PER_SEC.offset);
}

int WAVReader::samples() const {
    return length;
}

int& WAVReader::operator [] (int i) {
    return data[i];
}

const int& WAVReader::operator [] (int i) const {
    return data[i];
}

int& WAVReader::at(int channel, int i) {
    return data[2 * i + channel];
}
const int& WAVReader::at(int channel, int i) const {
    return data[2 * i + channel];
}
