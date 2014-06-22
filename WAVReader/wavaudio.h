#ifndef WAVREADER_H
#define WAVREADER_H
#include <iostream>
#include <cstring>
#include <cstdio>

class FormatAudio {
    std::string vId, vFormat;
    int vSamplerate;
    int vBitPerSample;
    int vChannels;
public:
    std::string id() const;
    std::string format() const;
    std::string fullFormat() const;
    int samplerate() const;
    int channels() const;
    int bitspersample() const;
    int byterate() const;
    FormatAudio setChannel(int ch);
    FormatAudio(std::string vid, std::string vformat, int vsamplerate, int vbitpersample, int vchannels);
};

class WAVAudio
{
public:
    WAVAudio(const char* path);
    WAVAudio(const FormatAudio& audioFmt, int* dt, int len);
    WAVAudio& operator = (const WAVAudio& oth);
    ~WAVAudio();

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
    void save(const char* name);
    FormatAudio formatOfAudio();
    double duration();
private:
    static void writeLittleEndian(unsigned char *strt, int l, int number) {
        for (int i = 0; i < l; ++i, number /= 256)
            *(strt + i) = number % 256;
    }

    static void writeBigEndian(unsigned char *strt, int l, int number) {
        for (int i = l - 1; i >= 0; --i, number /= 256)
            *(strt + i) = number % 256;
    }

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
    static const field FILE_BITS_PER_SAM;

    static const field FILE_SUB2_ID;
    static const field FILE_SUB2_SIZE;
    static const int OFFSET_DATA = 44;

    void readHeader(FILE *f);
    void readData(FILE *f);

    unsigned char fmt[OFFSET_DATA];
    int length;
    int sizeOfType;
    int *data;

};
#endif // WAVREADER_H
