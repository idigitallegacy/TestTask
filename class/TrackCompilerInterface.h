//
// Created by elfxe on 17.07.2022.
//

#ifndef TESTTASK_TRACKCOMPILERINTERFACE_H
#define TESTTASK_TRACKCOMPILERINTERFACE_H

#include <random>
#include <vector>

#include "WAVOutputGenerator.h"

class TrackCompilerInterface : public WAVOutputGenerator {
protected:
    typedef double  raw_data_t;
    typedef int16_t data_t;
    typedef double  frequency_t;
    typedef int32_t sampleRate_t;
    typedef std::vector<raw_data_t> soundwave_t;

    soundwave_t soundwave_;
    sampleRate_t sampleRate_;

    virtual void normalize_data();

public:
    explicit TrackCompilerInterface(sampleRate_t sampleRate, const std::string &filename = "wave.wav");

    virtual void generate_note(frequency_t frequency, double duration) = 0;

    virtual void save_track() = 0;
};

#endif //TESTTASK_TRACKCOMPILERINTERFACE_H
