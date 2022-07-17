//
// Created by elfxe on 17.07.2022.
//
#include <string>
#include <iostream>
#include <cmath>


#include "class/StereoSoundGeneratorInterface.h"

#include "../Task_2.6/class/TrackCompliler.h"

class StereoSoundGenerator : public StereoSoundGeneratorInterface {
private:
    typedef double  raw_data_t;
    typedef double  angle_t;
    typedef double  distance_t;
    typedef int16_t data_t;
    typedef double  frequency_t;
    typedef int32_t sampleRate_t;
    typedef std::vector<raw_data_t> soundwave_t;

    sampleRate_t sampleRate_;
public:
    StereoSoundGenerator(sampleRate_t sampleRate, const std::string &filename)
    : StereoSoundGeneratorInterface(sampleRate, filename)
    , sampleRate_(sampleRate)
    {}

    // 0 angle - in front of you
    void generate_note(frequency_t frequency, angle_t angle = 0.0) override {
        // According to the Woodworth's Formula for Interaural Time Delay (head radius is 10.2 = 102 mm, V = 340.29 m/s):
        // https://www.researchgate.net/figure/Woodworths-Formula-for-Interaural-Time-Delay-ITD-This-model-is-a-simplification-The_fig3_247930825
        // https://www.researchgate.net/publication/247930825_Under-explored_dimensions_in_spatial_sound

        // Delay is calculated for left ear
        double delay = 10.2 * (angle + std::sin(angle))/340.29;
        soundwave_t left_sound;
        soundwave_t right_sound;
        soundwave_t delay_sound;

        TrackCompiler compiler(sampleRate_, "./a.wav");
        compiler.generate_note(0, std::abs(delay));
        delay_sound = compiler.get_note();

        compiler.generate_note(frequency, 1.0);

        double coefficient = (angle == 0.0 ? 1 : angle * 5);

        if (delay < 0) {
            left_sound = compiler.get_note();
            for (auto &i : delay_sound)
                left_sound.push_back(i);

            right_sound = delay_sound;
            for (auto &i : compiler.get_note())
                right_sound.push_back(i / coefficient);
        } else {
            right_sound = compiler.get_note();
            for (auto &i : delay_sound)
                right_sound.push_back(i);

            left_sound = delay_sound;
            for (auto &i : compiler.get_note())
                left_sound.push_back(i / coefficient);
        }

        for (size_t i = 0, j = 0; i < left_sound.size(); ++i, ++j) {
            soundwave_.push_back(left_sound[i]);
            soundwave_.push_back(right_sound[j]);
        }

        addNote(soundwave_);
        soundwave_.clear();
    }

    void save_track() override {
        SaveWave();
    }
};

int main() {
    StereoSoundGenerator a(48000, "./a.wav");

    a.generate_note(440.00, -M_PI/2);
    a.generate_note(440.00, 0.0);
    a.save_track();

//    int sample_rate;
//    std::string filepath;
//
//    std::cout << "Enter preferred sample rate: ";
//    std::cin >> sample_rate;
//
//    std::cout << "Enter path to file *.wav (to save data, e.g. \"./sound.wav\"): ";
//    std::cin >> filepath;

    return 0;
}