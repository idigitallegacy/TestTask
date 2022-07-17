#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>
#include <stdexcept>

class WAVOutputGenerator {
private:
    typedef double  raw_data_t;
    typedef short   data_t;
    typedef int     sampleRate_t;
    typedef std::vector<double> soundwave_t;

    sampleRate_t    sampleRate_;
    std::vector<double>     soundwave_;

    std::ofstream   stream_;

    union BinaryHeader {
        struct {
            int     RIFF        : 32;
            int     fileSize    : 32;
            int     WAVE        : 32;
            int     frm_        : 32;
            int     blockSize   : 32;
            short   audioType   : 16;
            short   nChannels   : 16;
            int     sampleRate  : 32;
            int     byteRate    : 32;
            short   frameSize   : 16;
            short   bitDepth    : 16;
            int     DATA        : 32;
            int     dataSize    : 32;
        } TagList {
                .RIFF = 0x46464952,
                .WAVE = 0x45564157,
                .frm_ = 0x20746D66,
                .DATA = 0x61746164
        };

        char data[44];
    };

    union DataByte {
        short   value;
        char    data[2];
    };

protected:
    explicit WAVOutputGenerator(const std::string &filename, sampleRate_t sampleRate = 48000)
    : stream_(filename, std::ios::out | std::ios::binary)
    , sampleRate_(sampleRate)
    , soundwave_() {
        if (!stream_.is_open())
            throw std::exception();
    }

    void addNote(const soundwave_t &note) {
        for (auto waveRate : note)
            soundwave_.push_back(waveRate);
    }

    void write_wav_header() {
        BinaryHeader header;
        auto frameSize = (short)(16 / 8);
        header.TagList.fileSize = 36 + soundwave_.size() * frameSize;
        header.TagList.blockSize = 16;
        header.TagList.audioType = 1;
        header.TagList.nChannels = 1;
        header.TagList.sampleRate = sampleRate_;
        header.TagList.byteRate = sampleRate_ * frameSize;
        header.TagList.frameSize = frameSize;
        header.TagList.bitDepth = 16;
        header.TagList.dataSize = soundwave_.size() * frameSize;

        for (char element : header.data)
            stream_.put(element);
    }

    void SaveWave() {
        write_wav_header();

        for (auto &element : soundwave_) { // Начинаем записывать данные из нашего массива.
            DataByte dataByte { .value = (short) (element * 0x7FFF) };
            for (char byte : dataByte.data)
                stream_.put(byte);
        }
    }

    ~WAVOutputGenerator() {
        stream_.close();
    }
};

class TrackCompiler : public WAVOutputGenerator {
private:
    typedef short   data_t;
    typedef double  frequency_t;
    typedef int     sampleRate_t;
    typedef std::vector<double> soundwave_t;

    std::vector<double>  noise_;
    std::vector<double>  soundwave_;
    sampleRate_t sampleRate_;

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



    void normalize_data() {
        double max_element = 0;
        for (auto &i : soundwave_) {
            if (std::abs(i) > max_element)
                max_element = std::abs(i);
        }
        for (auto &i : soundwave_) {
            i /= max_element;
        }
    }

    void clear() {
        noise_.clear();
        soundwave_.clear();
    }

public:

    explicit TrackCompiler(sampleRate_t sampleRate, const std::string &filename = "wave.wav")
    : WAVOutputGenerator(filename, sampleRate)
    , soundwave_()
    , noise_()
    , sampleRate_(sampleRate)
    {}

    void generate_note(frequency_t frequency, double duration) {
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

    void save_track() {
        SaveWave();
    }
};

int main() {
    
    TrackCompiler a(44100, "wave.wav");
    a.generate_note(246.94, 3.0);
    a.save_track();

    return 0;
}