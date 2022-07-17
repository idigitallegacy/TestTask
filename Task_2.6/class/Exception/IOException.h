//
// Created by elfxe on 17.07.2022.
//

#ifndef TESTTASK_IOEXCEPTION_H
#define TESTTASK_IOEXCEPTION_H

#include <stdexcept>

class IOException : public std::exception {
private:
    const char *message_;
    int error_code_;
public:
    enum ECODE {
        UNABLE_TO_OPEN_FILE = 1,
        UNABLE_TO_READ_FILE,
        UNABLE_TO_WRITE_FILE
    };

    IOException(const char * msg, ECODE eCode) : message_(msg), error_code_(eCode) {}

    const char * what() {
        return message_;
    }

    [[nodiscard]] ECODE error_code() const {
        return ECODE(error_code_);
    }
};


#endif //TESTTASK_IOEXCEPTION_H
