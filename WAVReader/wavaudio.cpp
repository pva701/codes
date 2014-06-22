#include "wavaudio.h"

//FORMAT
FormatAudio::FormatAudio(std::string vid, std::string vformat, int vsamplerate, int vbitpersample, int vchannels) {
    vId = vid;
    vFormat = vformat;
    vSamplerate = vsamplerate;
    vBitPerSample = vbitpersample;
    vChannels = vchannels;
}

std::string FormatAudio::id() const {
    return vId;
}

std::string FormatAudio::format() const {
    return vFormat;
}

std::string FormatAudio::fullFormat() const {
    return vId + " " + vFormat;
}

int FormatAudio::samplerate() const {
    return vSamplerate;
}

int FormatAudio::channels() const {
    return vChannels;
}

int FormatAudio::bitspersample() const {
    return vBitPerSample;
}

int FormatAudio::byterate() const {
    return vSamplerate * vChannels * vBitPerSample / 8;
}

FormatAudio FormatAudio::setChannel(int ch) {
    vChannels = ch;
    return *this;
}

//ENDFORMAT

//FIELD
int WAVAudio::field::little(const unsigned char* a) const {
    int result = 0, pw = 1;
    for (int i = 0; i < size; ++i, pw *= 256) {
        result += pw * (*(a + i));
    }
    return result;
}

int WAVAudio::field::big(const unsigned char* a) const {
    int result = 0, pw = 1;
    for (int i = size - 1; i >= 0; --i, pw *= 256) {
        result += pw * (*(a + i));
    }
    return result;
}

int WAVAudio::field::endian(const unsigned char* a) const {
    if (isLittle)
        return little(a);
    return big(a);
}

WAVAudio::field::field(int of, int sz, bool isLit) {
    offset = of;
    size = sz;
    isLittle = isLit;
}
//ENDFIELD

//WAVAudio
const WAVAudio::field WAVAudio::FILE_ID           = field(0, 4, 0);//big
const WAVAudio::field WAVAudio::FILE_SIZE         = field(4, 4, 1);//little
const WAVAudio::field WAVAudio::FILE_FORMAT       = field(8, 4, 0);//big

const WAVAudio::field WAVAudio::FILE_SUB1_ID      = field(12, 4, 0);//big
const WAVAudio::field WAVAudio::FILE_SUB1_SIZE    = field(16, 4, 1);//little
const WAVAudio::field WAVAudio::FILE_AUDIO_FORMAT = field(20, 2, 1);//little
const WAVAudio::field WAVAudio::FILE_NUM_CHANNELS = field(22, 2, 1);//little
const WAVAudio::field WAVAudio::FILE_SAMPLE_RATE  = field(24, 4, 1);//little
const WAVAudio::field WAVAudio::FILE_BYTERATE      = field(28, 4, 1);//little
const WAVAudio::field WAVAudio::FILE_BLOCK_ALIGN  = field(32, 2, 1);//little
const WAVAudio::field WAVAudio::FILE_BITS_PER_SAM = field(34, 2, 1);//little

const WAVAudio::field WAVAudio::FILE_SUB2_ID      = field(36, 4, 0);//big
const WAVAudio::field WAVAudio::FILE_SUB2_SIZE    = field(40, 4, 1);//little
//const int WAVReader::OFFSET_DATA                   = 44;

WAVAudio::WAVAudio(const char *path)
{
    FILE *f;
    f = fopen(path, "r");
    readHeader(f);
    readData(f);
    fclose(f);
    //for (int i = 0; i < OFFSET_DATA; ++i) std::cout << (int)fmt[i] << " ";std::cout << "\n";
}

WAVAudio::WAVAudio(const FormatAudio& audioFmt, int* dt, int len) {
    const int SIZE_OF_SUB1 = 16;
    memcpy(fmt, "RIFF", 4);
    writeLittleEndian(fmt + FILE_SIZE.offset, FILE_SIZE.size, SIZE_OF_SUB1 + len * audioFmt.bitspersample() / 8);
    memcpy(fmt + FILE_FORMAT.offset, "WAVE", 4);
    memcpy(fmt + FILE_SUB1_ID.offset, "fmt ", 4);
    writeLittleEndian(fmt + FILE_SUB1_SIZE.offset, FILE_SUB1_SIZE.size, SIZE_OF_SUB1);
    writeLittleEndian(fmt + FILE_AUDIO_FORMAT.offset, FILE_AUDIO_FORMAT.size, 1);//PCM
    writeLittleEndian(fmt + FILE_NUM_CHANNELS.offset, FILE_NUM_CHANNELS.size, audioFmt.channels());
    writeLittleEndian(fmt + FILE_SAMPLE_RATE.offset, FILE_SAMPLE_RATE.size, audioFmt.samplerate());
    writeLittleEndian(fmt + FILE_BYTERATE.offset, FILE_BYTERATE.size, audioFmt.byterate());
    writeLittleEndian(fmt + FILE_BLOCK_ALIGN.offset, FILE_BLOCK_ALIGN.size, audioFmt.bitspersample() * audioFmt.channels() / 8);
    writeLittleEndian(fmt + FILE_BITS_PER_SAM.offset, FILE_BITS_PER_SAM.size, audioFmt.bitspersample());
    memcpy(fmt + FILE_SUB2_ID.offset, "data", 4);
    writeLittleEndian(fmt + FILE_SUB2_SIZE.offset, FILE_SUB2_SIZE.size, len * audioFmt.bitspersample() / 8);
    //for (int i = 0; i < OFFSET_DATA; ++i) std::cout << (int)fmt[i] << " ";std::cout << "\n";
    data = new int[len];
    sizeOfType = audioFmt.bitspersample() / 8;
    length = len;
    int lefbound, rigbound;
    if (sizeOfType == 1) {
        lefbound = 0;
        rigbound = 255;
    } else {
        lefbound = -(1<<(audioFmt.bitspersample() - 1));
        rigbound = (1<<(audioFmt.bitspersample() - 1)) - 1;
    }
    for (int i = 0; i < len; ++i) {
        if (lefbound <= dt[i] && dt[i] <= rigbound)
            data[i] = dt[i];
        else
            throw std::exception();
    }
}

WAVAudio& WAVAudio::operator = (const WAVAudio& oth) {
    if (&oth == this)
        return *this;
    delete[] data;
    data = oth.data;
    for (int i = 0; i < OFFSET_DATA; ++i)
        fmt[i] = oth.fmt[i];
    length = oth.length;
    sizeOfType = oth.sizeOfType;
    return *this;
}

WAVAudio::~WAVAudio() {
    delete[] data;
}

void WAVAudio::readHeader(FILE *f) {
    fread(fmt, 1, OFFSET_DATA, f);
}

void WAVAudio::readData(FILE *f) {
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

std::string WAVAudio::id() const {
    return std::string(fmt + FILE_ID.offset, fmt + FILE_ID.offset + FILE_ID.size);
}

int WAVAudio::size() const {
    return FILE_SIZE.endian(fmt + FILE_SIZE.offset);
}

std::string WAVAudio::format()  const {
    return std::string(fmt + FILE_FORMAT.offset, fmt + FILE_FORMAT.offset + FILE_FORMAT.size);
}

int WAVAudio::channels() const {
    return FILE_NUM_CHANNELS.endian(fmt + FILE_NUM_CHANNELS.offset);
}

int WAVAudio::samplerate() const {
    return FILE_SAMPLE_RATE.endian(fmt + FILE_SAMPLE_RATE.offset);
}

int WAVAudio::byterate() const {
    return FILE_BYTERATE.endian(fmt + FILE_BYTERATE.offset);
}

int WAVAudio::bitpersample() const {
    return FILE_BITS_PER_SAM.endian(fmt + FILE_BITS_PER_SAM.offset);
}

int WAVAudio::samples() const {
    return length;
}

int& WAVAudio::operator [] (int i) {
    return data[i];
}

const int& WAVAudio::operator [] (int i) const {
    return data[i];
}

int& WAVAudio::at(int channel, int i) {
    return data[2 * i + channel];
}

const int& WAVAudio::at(int channel, int i) const {
    return data[2 * i + channel];
}

void WAVAudio::save(const char* name) {
    FILE *f = fopen(name, "w");
    fwrite(fmt, sizeof(char), sizeof(fmt), f);
    if (sizeOfType == 1) {
        unsigned char *outdata = new unsigned char[length];
        for (int i = 0; i < length; ++i)
            outdata[i] = data[i];
        fwrite(outdata, sizeof(unsigned char), length, f);
    } else if (sizeOfType == 2) {
        short int *outdata = new short int[length];
        for (int i = 0; i < length; ++i)
            outdata[i] = data[i];
        fwrite(outdata, sizeof(short int), length, f);
    } else if (sizeOfType == 4) {
        int *outdata = new int[length];
        for (int i = 0; i < length; ++i)
            outdata[i] = data[i];
        fwrite(outdata, sizeof(int), length, f);
    }
    fclose(f);
}

FormatAudio WAVAudio::formatOfAudio() {
    return FormatAudio(id(), format(), samplerate(), bitpersample(), channels());
}

double WAVAudio::duration() {
    return samples() * 1.0 / samplerate() / channels();
}
