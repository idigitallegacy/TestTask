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
