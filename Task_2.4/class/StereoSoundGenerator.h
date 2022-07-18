//
// Created by elfxe on 19.07.2022.
//

#ifndef TESTTASK_STEREOSOUNDGENERATOR_H
#define TESTTASK_STEREOSOUNDGENERATOR_H

#include <string>
#include <iostream>
#include <cmath>


#include "StereoSoundGeneratorInterface.h"

#include "../../Task_2.6/class/TrackCompliler.h"


class StereoSoundGenerator : public StereoSoundGeneratorInterface {
private:
    typedef double  raw_data_t;
    typedef double  angle_t;
    typedef int16_t data_t;
    typedef double  frequency_t;
    typedef int32_t sampleRate_t;
    typedef std::vector<raw_data_t> soundwave_t;

    sampleRate_t sampleRate_;
    std::string  filename_;
    std::string  temp_filename_ = "task.bin";

    // correct angle to be at [-PI; PI]
    static double format_angle_2PI(angle_t angle);

    // Making angle to be at [-PI/2; PI/2]
    static double format_angle_PI2(angle_t angle);

    // According to the Woodworth's Formula for Interaural Time Delay (head radius is 10.2 cm = .102 m, V = 340.29 m/s):
    // https://www.researchgate.net/figure/Woodworths-Formula-for-Interaural-Time-Delay-ITD-This-model-is-a-simplification-The_fig3_247930825
    // https://www.researchgate.net/publication/247930825_Under-explored_dimensions_in_spatial_sound
    inline static double calculate_delay(angle_t angle);

    // To make it sound more realistic, I divide one of the channels by this coefficient.
    inline static double calculate_coefficient(angle_t angle);

    soundwave_t generate_soundwave(frequency_t frequency, double delay);

    void append_channels(const soundwave_t &left, const soundwave_t &right);

    void format_channels(soundwave_t &left_channel, soundwave_t &right_channel, angle_t angle, double delay);

    void generate_channels(frequency_t frequency, soundwave_t &left_channel, soundwave_t &right_channel, double delay);

public:
    StereoSoundGenerator(sampleRate_t sampleRate, const std::string &filename)
            : StereoSoundGeneratorInterface(sampleRate, filename)
            , sampleRate_(sampleRate)
            , filename_()
    {}

    // 0 angle - in front of you; +/- PI - at the backside
    void generate_note(frequency_t frequency, angle_t angle) override;

    void save_track() override {
        SaveWave();
    }
};


#endif //TESTTASK_STEREOSOUNDGENERATOR_H
