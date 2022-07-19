//
// Created by elfxe on 19.07.2022.
//

#include <iostream>
#include <utility>
#include <vector>

class LCGPredictor {
private:
    std::vector<int> seed_;
    std::vector<std::pair<int, int>> possible_variables_;
    std::vector<int> possible_additives_;

    int last_index_ = 0;

    static inline int LCG_control(int seed, int a, int c, int m) {
        return (a * seed + c) % m;
    }

    static int modular_inverse(int a, int m) {
        // Based on code from http://rosettacode.org/wiki/Modular_inverse#C
        int t, nt, r, nr, q, tmp;
        t = 0;  nt = 1;  r = m;  nr = a % m;
        while (nr != 0) {
            q = r/nr;
            tmp = nt;  nt = t - q*nt;  t = tmp;
            tmp = nr;  nr = r - q*nr;  r = tmp;
        }
        if (r > 1) return -1;
        if (t < 0) t += m;
        return t;
    }

    void find_additives() {
        for (auto &i : possible_variables_) {
            int c = seed_[2] - ((seed_[1] * i.first) % i.second);
            if (c < 0)
                c += i.second;
            possible_additives_.emplace_back(c);
        }
    }

    void generate_possible_variables() {
        int m_start = 0;
        for (auto &i : seed_) {
            if (i > m_start)
                m_start = i + 1;
        }

        for (int m = m_start; m < 10 * m_start; ++m) {
            int diff0 = (seed_[1] - seed_[0]);
            int diff1 = (seed_[2] - seed_[1]);
            int diff2 = (seed_[3] - seed_[2]);

            if (diff0 < 0)
                diff0 += m;
            if (diff1 < 0)
                diff1 += m;
            if (diff2 < 0)
                diff2 += m;

            // diff1 = a * diff0   =>   1/diff0 * diff1 = a
            int inv_diff0 = modular_inverse(diff0, m);
            if (inv_diff0 < 0)
                continue;
            int a = (diff1 * inv_diff0) % m;

            int my_diff1 = (a * diff0) % m;
            int my_diff2 = (a * diff1) % m;

            if (my_diff1 != diff1)
                continue;

            if (my_diff2 == diff2)
                possible_variables_.emplace_back(a, m);
        }
    }
public:
    explicit LCGPredictor(std::vector<int> seed)
    : seed_(std::move(seed)) {
        generate_possible_variables();
        find_additives();
    }

    int predict() {
        if (possible_variables_.empty())
            return -1;

        int a = possible_variables_[last_index_].first;
        int m = possible_variables_[last_index_].second;
        int c = possible_additives_[last_index_];

        for (int i = 0; i < seed_.size() - 1; ++i) {
            if (LCG_control(seed_[i], a, c, m) != seed_[i + 1]) {
                fail();
                i = 0;
            }
        }

        return LCG_control(seed_[seed_.size() - 1], a, c, m);
    }

    void fail() {
        ++last_index_;
        if (last_index_ == seed_.size())
            last_index_ = 0;
    }

    std::vector<int> get_vector() {
        return std::vector<int>{possible_variables_[last_index_].first,
                                possible_variables_[last_index_].second,
                                possible_additives_[last_index_]};
    }
};

int main() {
    LCGPredictor predictor({157,  5054, 25789, 13214});
    std::vector<int> seed;
    std::cout << "Enter 4 seed values with the following format: <seed_n1> <seed_n2> <...>\n"
                 "E. g.: 157 5054 25789 13214\n";

    int tmp;
    for (int i = 0; i < 4; ++i)
        std::cin >> tmp;

    std::cout << "Controls:\n"
                 "\tpredict - predicts the next number after the last in seed\n"
                 "\tfail - command to update coefficients a, c and m (if you are not satisfied with the answer given)\n"
                 "\tlist - display the calculated coefficients\n"
                 "\texit - finish the program\n";

    while (true) {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "exit")
            break;

        if (cmd == "predict")
            std::cout << predictor.predict() << '\n';
        else if (cmd == "fail") {
            predictor.fail();
            std::cout << "Coefficients updated.\n";
        } else if (cmd == "list") {
            std::vector<int> vector = predictor.get_vector();
            std::cout << "a = " << vector[0] << ", c = " << vector[2] << ", m = " << vector[1] << '\n';
        }
        else
            std::cout << "Unrecognized command\n";
    }

    return EXIT_SUCCESS;
}