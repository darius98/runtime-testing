#ifndef KKTEST_EXTENSIONS_FEEDBACK_SRC_PIPE_MESSAGE_TYPE_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_SRC_PIPE_MESSAGE_TYPE_HPP_

namespace kktest {
namespace feedback {

enum PipeMessageType {
    TEST,
    GROUP,
    DONE,
    ERROR
};

}
}

#endif