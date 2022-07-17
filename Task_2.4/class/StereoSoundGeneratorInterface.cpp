//
// Created by elfxe on 17.07.2022.
//

#include "StereoSoundGeneratorInterface.h"

StereoSoundGeneratorInterface::StereoSoundGeneratorInterface(StereoSoundGeneratorInterface::sampleRate_t sampleRate,
                                                             const std::string &filename) : WAVOutputGenerator(filename, sampleRate, true)
{}
