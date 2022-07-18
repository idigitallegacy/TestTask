//
// Created by elfxe on 19.07.2022.
//

#include "StereoSoundGenerator.h"

double StereoSoundGenerator::format_angle_2PI(StereoSoundGenerator::angle_t angle) {
    while (angle > 2 * M_PI)
        angle -= 2 * M_PI;
    while (angle < -2 * M_PI)
        angle += 2 * M_PI;
    return angle;
}

double StereoSoundGenerator::format_angle_PI2(StereoSoundGenerator::angle_t angle) {
    if (angle < -3 * M_PI_2)
        angle += 2 * M_PI;
    if (angle > 3 * M_PI_2)
        angle -= 2 * M_PI;

    if ((angle < -M_PI_2 && angle >= - M_PI) || (angle > M_PI && angle <= 3 * M_PI_2))
        angle = -std::acos(std::abs(std::cos(angle)));
    if ((angle < -M_PI && angle >= -3 * M_PI_2) || (angle > M_PI_2 && angle <= M_PI))
        angle = std::acos(std::abs(std::cos(angle)));
    return angle;
}

double StereoSoundGenerator::calculate_delay(StereoSoundGenerator::angle_t angle) {
    angle = format_angle_PI2(angle);
    // Delay is calculated for left ear
    return 0.102 * (angle + std::sin(angle))/(340.29);
}

double StereoSoundGenerator::calculate_coefficient(StereoSoundGenerator::angle_t angle) {
    if (angle == 0.0)
        return 1;
    return std::abs(2.0 * std::sin(angle));
}

StereoSoundGenerator::soundwave_t
StereoSoundGenerator::generate_soundwave(StereoSoundGenerator::frequency_t frequency, double delay) {
    TrackCompiler compiler(sampleRate_, temp_filename_);
    compiler.generate_note(frequency, std::abs(delay));
    return compiler.get_note();
}

void StereoSoundGenerator::append_channels(const StereoSoundGenerator::soundwave_t &left,
                                           const StereoSoundGenerator::soundwave_t &right) {
    for (size_t i = 0, j = 0; i < left.size(); ++i, ++j) {
        soundwave_.push_back(left[i]);
        soundwave_.push_back(right[j]);
    }
}

void StereoSoundGenerator::format_channels(StereoSoundGenerator::soundwave_t &left_channel,
                                           StereoSoundGenerator::soundwave_t &right_channel,
                                           StereoSoundGenerator::angle_t angle, double delay) {
    // quiet coefficient
    double coefficient = calculate_coefficient(angle);

    if (delay < 0) {
        for (auto &i : right_channel)
            i /= coefficient;
    }
    if (delay > 0) {
        for (auto &i : left_channel)
            i /= coefficient;
    }

    // at the front side sound is louder, so to make it feel more realistic, I divide all the backside sounds by 2.
    if ((-M_PI_2 > angle && angle > -3 * M_PI_2) || (M_PI_2 < angle && angle < 3 * M_PI_2)) {
        for (auto &i : left_channel)
            i /= 2.0;
        for (auto &i : right_channel)
            i /= 2.0;
    }
}

void StereoSoundGenerator::generate_channels(StereoSoundGenerator::frequency_t frequency,
                                             StereoSoundGenerator::soundwave_t &left_channel,
                                             StereoSoundGenerator::soundwave_t &right_channel, double delay) {
    soundwave_t soundwave_buffer;
    soundwave_t delay_sound;

    delay_sound = generate_soundwave(0.0, delay);

    if (delay < 0) {
        soundwave_buffer = generate_soundwave(frequency, 1.0);

        left_channel = soundwave_buffer;
        for (auto &i : delay_sound)
            left_channel.push_back(i);

        right_channel = delay_sound;
        for (auto &i : soundwave_buffer)
            right_channel.push_back(i);
    } else {
        soundwave_buffer = generate_soundwave(frequency, 1.0);

        right_channel = soundwave_buffer;
        for (auto &i : delay_sound)
            right_channel.push_back(i);

        left_channel = delay_sound;
        for (auto &i : soundwave_buffer)
            left_channel.push_back(i);
    }
}

void
StereoSoundGenerator::generate_note(StereoSoundGenerator::frequency_t frequency, StereoSoundGenerator::angle_t angle) {
    angle = format_angle_2PI(angle);

    // ITD time
    double delay = calculate_delay(angle);

    soundwave_t left_channel;
    soundwave_t right_channel;

    generate_channels(frequency, left_channel, right_channel, delay);
    format_channels(left_channel, right_channel, angle, delay);
    append_channels(left_channel, right_channel);

    addNote(soundwave_);
    soundwave_.clear();
}
