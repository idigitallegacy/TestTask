//
// Created by elfxe on 17.07.2022.
//

#ifndef TESTTASK_STEREOSOUNDGENERATORINTERFACE_H
#define TESTTASK_STEREOSOUNDGENERATORINTERFACE_H

#include <vector>
#include <cmath>
#include "../../Task_2.6/class/WAVOutputGenerator.h"


class StereoSoundGeneratorInterface : public WAVOutputGenerator {
protected:
    typedef double  raw_data_t;
    typedef double  angle_t;
    typedef double  distance_t;
    typedef int16_t data_t;
    typedef double  frequency_t;
    typedef int32_t sampleRate_t;
    typedef std::vector<raw_data_t> soundwave_t;

    soundwave_t soundwave_;

    StereoSoundGeneratorInterface(sampleRate_t sampleRate, const std::string &filename);

    virtual void normalize_data() {
        double max_element_left = 0;
        double max_element_right = 0;

        for (size_t i = 0; i < soundwave_.size(); i += 2) {
            if (std::abs(soundwave_[i]) > max_element_left)
                max_element_left = std::abs(soundwave_[i]);
        }
        for (size_t i = 1; i < soundwave_.size(); i += 2) {
            if (std::abs(soundwave_[i]) > max_element_left)
                max_element_right = std::abs(soundwave_[i]);
        }

        for (size_t i = 0; i < soundwave_.size(); i += 2) {
            soundwave_[i] /= max_element_left;
        }
        for (size_t i = 1; i < soundwave_.size(); i += 2) {
            soundwave_[i] /= max_element_right;
        }
    }

public:
    virtual void generate_note(frequency_t frequency, angle_t angle) = 0;

    virtual void save_track() = 0;
};


#endif //TESTTASK_STEREOSOUNDGENERATORINTERFACE_H
