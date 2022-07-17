//
// Created by elfxe on 17.07.2022.
//

#ifndef TESTTASK_TRACKCOMPLILER_H
#define TESTTASK_TRACKCOMPLILER_H

#include "TrackCompilerInterface.h"

class TrackCompiler : public TrackCompilerInterface {
private:
    std::vector<double>  noise_;

    // Generates noise sized [-1; 1], length is sampleRate / frequency
    void generate_noise();

    // First few samples are noise.
    void import_noise_to_soundwave();

    // Extends soundwave to its duration
    void z_delay_function();

    // Just a simple filter
    double string_dampling_filter(size_t i) {
        return 0.996*((1-0.5)*soundwave_[i]+0.5*soundwave_[i-1]);
    }

    // Another filter + append all the filers to whole z-delay-function output
    void filter() {
        for (size_t i = noise_.size(); i < soundwave_.size(); ++i) {
            soundwave_[i] = 0.5*(string_dampling_filter(i)-soundwave_[i-1])+string_dampling_filter(i-1);
        }
    }

    void clear() {
        noise_.clear();
        soundwave_.clear();
    }

public:
    // TrackCompiler output is only MONO sound!
    explicit TrackCompiler(sampleRate_t sampleRate, const std::string &filename = "wave.wav")
            : TrackCompilerInterface(sampleRate, filename)
            , noise_()
    {}

    // Joins generate_noise(), z_function(), etc. together
    void generate_note(frequency_t frequency, double duration) override;

    // Saves generated melody stored at WAVOutputGenerator class
    void save_track() override {
        SaveWave();
    }
};


#endif //TESTTASK_TRACKCOMPLILER_H
