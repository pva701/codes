#ifndef WAVREADER_H
#define WAVREADER_H
#include <iostream>
#include <cstring>
#include <cstdio>

class WAVReader
{
public:
    WAVReader(const char* path);
    WAVReader(const std::string& s);
    ~WAVReader();

    std::string id() const;
    int size() const;
    std::string format() const;
    int channels() const;
    int samplerate() const;
    int byterate() const;
    int bitpersample() const;
    int samples() const;

    int& operator [] (int i);
    const int& operator [] (int i) const;

    int& at(int channel, int i);
    const int& at(int channel, int i) const;
private:
    struct field {
        int offset, size;
        bool isLittle;
        field(int of, int sz, bool isLit);
        int endian(const unsigned char* a) const;
        int little(const unsigned char* a) const;
        int big(const unsigned char* a) const;
    };

    static const field FILE_ID;
    static const field FILE_SIZE;
    static const field FILE_FORMAT;

    static const field FILE_SUB1_ID;
    static const field FILE_SUB1_SIZE;
    static const field FILE_AUDIO_FORMAT;
    static const field FILE_NUM_CHANNELS;
    static const field FILE_SAMPLE_RATE;
    static const field FILE_BYTERATE;
    static const field FILE_BLOCK_ALIGN;
    static const field FILE_BITS_PER_SEC;

    static const field FILE_SUB2_ID;
    static const field FILE_SUB2_SIZE;
    static const int OFFSET_DATA = 44;

    FILE *f;
    void readHeader();
    void readData();
    unsigned char fmt[OFFSET_DATA];

    int length;
    int sizeOfType;
    int *data;

};

#endif // WAVREADER_H
