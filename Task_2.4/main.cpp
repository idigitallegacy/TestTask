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
    typedef int16_t data_t;
    typedef double  frequency_t;
    typedef int32_t sampleRate_t;
    typedef std::vector<raw_data_t> soundwave_t;

    sampleRate_t sampleRate_;
    std::string filename_;
    std::string temp_filename_ = "task.bin";

    static double format_angle_2PI(angle_t angle) {
        // correct angle to be at [-PI; PI]
        while (angle > 2 * M_PI)
            angle -= 2 * M_PI;
        while (angle < -2 * M_PI)
            angle += 2 * M_PI;
        return angle;
    }

    static double format_angle_PI2(angle_t angle) {
        // Making angle to be at [-PI/2; PI/2]
        if (angle < -3 * M_PI_2)
            angle += 2 * M_PI;
        if (angle > 3 * M_PI_2)
            angle -= 2 * M_PI;

        if ((angle < -M_PI_2 && angle >= - M_PI) || (angle > M_PI_2 && angle <= M_PI))
            angle = -std::acos(std::abs(std::cos(angle)));
        if ((angle < -M_PI && angle >= -3 * M_PI_2) || (angle > M_PI && angle <= -3 * M_PI_2))
            angle = std::acos(std::abs(std::cos(angle)));
    }

    inline static double calculate_delay(angle_t angle) {
        // According to the Woodworth's Formula for Interaural Time Delay (head radius is 10.2 cm = .102 m, V = 340.29 m/s):
        // https://www.researchgate.net/figure/Woodworths-Formula-for-Interaural-Time-Delay-ITD-This-model-is-a-simplification-The_fig3_247930825
        // https://www.researchgate.net/publication/247930825_Under-explored_dimensions_in_spatial_sound

        angle = format_angle_PI2(angle);
        // Delay is calculated for left ear
        return 0.102 * (angle + std::sin(angle))/(340.29);
    }

    // To make it sound more realistic, I divide one of the channels by this coefficient.
    inline static double calculate_coefficient(angle_t angle) {
        if (angle == 0.0)
            return 1;
//        return 1;
        return std::abs(2.0 * std::sin(angle));
    }

    soundwave_t generate_soundwave(frequency_t frequency, double delay) {
        TrackCompiler compiler(sampleRate_, temp_filename_);
        compiler.generate_note(frequency, std::abs(delay));
        return compiler.get_note();
    }

    void append_channels(const soundwave_t &left, const soundwave_t &right) {
        for (size_t i = 0, j = 0; i < left.size(); ++i, ++j) {
            soundwave_.push_back(left[i]);
            soundwave_.push_back(right[j]);
        }
    }

    void format_channels(soundwave_t &left_channel, soundwave_t &right_channel, angle_t angle, double delay) {
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

    void generate_channels(frequency_t frequency, soundwave_t &left_channel, soundwave_t &right_channel, double delay) {
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

public:
    StereoSoundGenerator(sampleRate_t sampleRate, const std::string &filename)
    : StereoSoundGeneratorInterface(sampleRate, filename)
    , sampleRate_(sampleRate)
    , filename_()
    {}

    // 0 angle - in front of you; -PI - at the backside
    void generate_note(frequency_t frequency, angle_t angle) override {
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

    void save_track() override {
        SaveWave();
    }
};

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