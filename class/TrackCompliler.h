//
// Created by elfxe on 17.07.2022.
//

#ifndef TESTTASK_TRACKCOMPLILER_H
#define TESTTASK_TRACKCOMPLILER_H

#include "TrackCompilerInterface.h"

class TrackCompiler : public TrackCompilerInterface {
private:
    std::vector<double>  noise_;

    void generate_noise() {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(-1.0, 1.0);

        for (auto &i : noise_) {
            i = dist(mt);
        }
    }

    void import_noise_to_soundwave() {
        for (size_t i = 0; i < noise_.size(); ++i) {
            soundwave_[i] = noise_[i];
        }
    }

    void z_delay_function() {
        for (size_t i = noise_.size(); i < soundwave_.size(); ++i) {
            soundwave_[i] = 0.999 * (soundwave_[i-noise_.size()]+soundwave_[i-noise_.size()-1])/2;
        }
    }

    double string_dampling_filter(size_t i) {
        return 0.996*((1-0.5)*soundwave_[i]+0.5*soundwave_[i-1]);
    }

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

    explicit TrackCompiler(sampleRate_t sampleRate, const std::string &filename = "wave.wav")
            : TrackCompilerInterface(sampleRate, filename)
            , noise_()
    {}

    void generate_note(frequency_t frequency, double duration) override;

    void save_track() override {
        SaveWave();
    }
};


#endif //TESTTASK_TRACKCOMPLILER_H
