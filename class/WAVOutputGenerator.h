//
// Created by elfxe on 17.07.2022.
//

#ifndef TESTTASK_WAVOUTPUTGENERATOR_H
#define TESTTASK_WAVOUTPUTGENERATOR_H

#include <fstream>
#include <vector>
#include <stdexcept>
#include "Exception/IOException.h"

class WAVOutputGenerator {
private:
    typedef double  raw_data_t;
    typedef int16_t data_t;
    typedef int32_t sampleRate_t;

    typedef std::vector<raw_data_t> soundwave_t;

    std::ofstream   stream_;
    sampleRate_t    sampleRate_;
    soundwave_t     soundwave_;

    union BinaryHeader {
        struct TL_s {
            int32_t  RIFF        : 32;
            int32_t  fileSize    : 32;
            int32_t  WAVE        : 32;
            int32_t  frm_        : 32;
            int32_t  blockSize   : 32;
            int16_t  audioType   : 16;
            int16_t  nChannels   : 16;
            int32_t  sampleRate  : 32;
            int32_t  byteRate    : 32;
            int16_t  frameSize   : 16;
            int16_t  bitDepth    : 16;
            int32_t  DATA        : 32;
            int32_t  dataSize    : 32;
        };

        TL_s TagList {
                .RIFF = 0x46464952,
                .fileSize = 0x0,
                .WAVE = 0x45564157,
                .frm_ = 0x20746D66,
                .blockSize = 0x0,
                .audioType = 0x0,
                .nChannels = 0x0,
                .sampleRate = 0x0,
                .byteRate = 0x0,
                .frameSize = 0x0,
                .bitDepth = 0x0,
                .DATA = 0x61746164,
                .dataSize = 0x0
        };

        int8_t data[44];
    };

    // Structure to convert int16_t into 2 bytes of int8_t
    union DataByte {
        int16_t  value;
        int8_t   data[2];
    };

protected:
    explicit WAVOutputGenerator(const std::string &filename, sampleRate_t sampleRate = 48000);

    void addNote(const soundwave_t &note);

    void write_wav_header();

    void SaveWave();

    ~WAVOutputGenerator() {
        stream_.close();
    }
};


#endif //TESTTASK_WAVOUTPUTGENERATOR_H
