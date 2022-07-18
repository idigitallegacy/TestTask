//
// Created by elfxe on 17.07.2022.
//
#include "class/StereoSoundGenerator.h"

int main() {
    int sample_rate;
    std::string filepath;

    std::cout << "Enter preferred sample rate: ";
    std::cin >> sample_rate;

    std::cout << "Enter path to file *.wav (to save data, e.g. \"./sound.wav\"): ";
    std::cin >> filepath;

    try {
        StereoSoundGenerator generator(sample_rate, filepath);

        std::cout << "Input data format: <frequncy (Hz)> <offset (N)> (N = N * pi/6 rad)\n"
                     "\t Examples:\n"
                     "\t 400.0 2.0 // generates string 400 Hz sound, 1.0s + delay_time duration, with offset 2 * pi/6 rad to right (on the right).\n"
                     "\t 400.0 -6.0 // generates string 400 Hz sound, 1.0s + delat_time duration with offset -6 * Pi/6 rad to left (on the back).\n"
                     "Type \"exit\" to finish the program and save track.\n";

        std::string frequency;
        std::string offset;
        while (std::cin >> frequency && frequency != "exit") {
            std::cin >> offset;
            generator.generate_note(std::stod(frequency), (M_PI_2 / 3) * std::stod(offset));
        }
        if (frequency == "exit") {
            generator.save_track();
            std::cout << "Track has been saved to: " + filepath << '\n';
        }
    } catch (IOException &e) {
        std::cerr << "Unable to operate with the file located at " << filepath << '\n';
        std::cerr << "\tIOException caught: " << e.what() << '\n';
        return e.error_code();
    } catch (std::exception &e) {
        std::cerr << "Unknown exception caught\n"
                     "\te.what(): " << e.what() <<'\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}