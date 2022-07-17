#include <iostream>
#include <string>

#include "class/TrackCompliler.h"

int main() {
    int sample_rate;
    std::string filepath;

    std::cout << "Enter preferred sample rate: ";
    std::cin >> sample_rate;

    std::cout << "Enter path to file *.wav (to save data, e.g. \"./sound.wav\"): ";
    std::cin >> filepath;


    try {
        TrackCompiler compiler(sample_rate, filepath);

        std::cout << "Input data format: <frequncy (Hz)> <duration (s)>\n"
                     "\t Example: 246.94 3.0 // generates string 3.0s sound with frequency 246.94 Hz\n"
                     "Type \"exit\" to finish the program and save track.\n";

        std::string frequency;
        std::string duration;
        while (std::cin >> frequency && frequency != "exit") {
            std::cin >> duration;
            compiler.generate_note(std::stod(frequency), std::stod(duration));
        }
        if (frequency == "exit") {
            compiler.save_track();
            std::cout << "Track has been saved to: " + filepath << '\n';
        }
    } catch (IOException &e) {
        std::cerr << "Unable to operate with the file located at " << filepath << '\n';
        std::cerr << "\tIOException caught: " << e.what() << '\n';
        return e.error_code();
    } catch (std::exception &e) {
        std::cerr << "Unknown exception caught\n"
                     "\te.what(): " << e.what() <<'\n';
        return -1;
    }

    return 0;
}