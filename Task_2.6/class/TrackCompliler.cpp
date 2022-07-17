//
// Created by elfxe on 17.07.2022.
//

#include "TrackCompliler.h"

void TrackCompiler::generate_note(TrackCompilerInterface::frequency_t frequency, double duration) {
    noise_.resize((size_t) (sampleRate_ / frequency));
    soundwave_.resize((size_t) (sampleRate_ * duration));

    generate_noise();
    import_noise_to_soundwave();
    z_delay_function();
    filter();
    normalize_data();

    addNote(soundwave_);

    clear();
}

void TrackCompiler::generate_noise() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (auto &i : noise_) {
        i = dist(mt);
    }
}

void TrackCompiler::import_noise_to_soundwave() {
    for (size_t i = 0; i < noise_.size(); ++i) {
        soundwave_[i] = noise_[i];
    }
}

void TrackCompiler::z_delay_function() {
    for (size_t i = noise_.size(); i < soundwave_.size(); ++i) {
        soundwave_[i] = 0.999 * (soundwave_[i-noise_.size()]+soundwave_[i-noise_.size()-1])/2;
    }
}
