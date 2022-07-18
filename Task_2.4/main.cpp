//
// Created by elfxe on 17.07.2022.
//
#include "class/StereoSoundGenerator.h"

int main() {
    StereoSoundGenerator a(48000, "./a.wav");

    a.generate_note(400.0, 0.0);
    a.generate_note(400.0, -M_PI_4);
    a.generate_note(400.0, -M_PI_2);
    a.generate_note(400.0, -3 * M_PI_4);
    a.generate_note(400.0, -M_PI);
    a.generate_note(400.0, -5 * M_PI_4);
    a.generate_note(400.0, -3 * M_PI_2);
    a.generate_note(400.0, -7 * M_PI_4);
    a.generate_note(400.0, -2 * M_PI);

    a.generate_note(400.0, 0.0);
    a.generate_note(400.0, M_PI_4);
    a.generate_note(400.0, M_PI_2);
    a.generate_note(400.0, 3 * M_PI_4);
    a.generate_note(400.0, M_PI);
    a.generate_note(400.0, 5 * M_PI_4);
    a.generate_note(400.0, 3 * M_PI_2);
    a.generate_note(400.0, 7 * M_PI_4);
    a.generate_note(400.0, 2 * M_PI);
    a.save_track();

    return 0;
}