//
// Created by elfxe on 17.07.2022.
//

#include "WAVOutputGenerator.h"

WAVOutputGenerator::WAVOutputGenerator(const std::string &filename, WAVOutputGenerator::sampleRate_t sampleRate, bool stereo)
        : stream_(filename, std::ios::out | std::ios::binary)
        , sampleRate_(sampleRate)
        , WAV_soundwave_()
        , stereo_(stereo) {
    if (!stream_.is_open())
        throw IOException("Unable to open file", IOException::ECODE::UNABLE_TO_OPEN_FILE);

    try {
        stream_ << "Trying to write into file" << '\n';
    } catch (std::exception &e) {
        stream_.close();
        stream_.open(filename, std::ios::out | std::ios::binary);
        stream_.close();
        throw IOException(("Unable to write to file " + filename).c_str(), IOException::ECODE::UNABLE_TO_WRITE_FILE);
    }

    stream_.close();
    stream_.open(filename, std::ios::out | std::ios::binary);
}

void WAVOutputGenerator::addNote(const WAVOutputGenerator::soundwave_t &note) {
    for (auto waveRate : note)
        WAV_soundwave_.push_back(waveRate);
}

void WAVOutputGenerator::write_wav_header() {
    BinaryHeader header;
    auto frameSize = (int8_t)(16 / 8);
    header.TagList.fileSize   = (int32_t) (36 + WAV_soundwave_.size() * frameSize);
    header.TagList.blockSize  = 16;
    header.TagList.audioType  = 1;
    header.TagList.nChannels  = stereo_ ? 2 : 1;
    header.TagList.sampleRate = sampleRate_;
    header.TagList.byteRate   = sampleRate_ * frameSize;
    header.TagList.frameSize  = (uint8_t) frameSize;
    header.TagList.bitDepth   = 16;
    header.TagList.dataSize   = (int32_t) (WAV_soundwave_.size() * frameSize);

    for (char element : header.data)
        stream_.put(element);
}

void WAVOutputGenerator::SaveWave() {
    write_wav_header();

    for (auto &element : WAV_soundwave_) { // Начинаем записывать данные из нашего массива.
        DataByte dataByte { .value = (short) (element * 0x7FFF) };
        for (char byte : dataByte.data)
            stream_.put(byte);
    }
}
