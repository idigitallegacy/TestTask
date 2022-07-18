//
// Created by elfxe on 17.07.2022.
//

#include "TrackCompilerInterface.h"

void TrackCompilerInterface::normalize_data() {
    double max_element = 0;
    for (auto &i: soundwave_) {
        if (std::abs(i) > max_element)
            max_element = std::abs(i);
    }
    for (auto &i: soundwave_) {
        i /= max_element;
    }
}

TrackCompilerInterface::TrackCompilerInterface(TrackCompilerInterface::sampleRate_t sampleRate,
                                               const std::string &filename)
        : WAVOutputGenerator(filename, sampleRate)
        , soundwave_()
        , sampleRate_(sampleRate)
        , last_note_()
{}
