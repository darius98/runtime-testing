#include <iostream>

#include <kktest_extension_api.hpp>
#include <kktest_ext/feedback_impl/ext.hpp>
#include <kktest_ext/feedback_impl/pipe_message_type.hpp>

using kktest::arguments::ArgumentsApi;
using kktest::arguments::FlagBuilder;
using kktest::arguments::ArgumentBuilder;
using kktest::interproc::Message;
using kktest::interproc::PipeWriter;
using kktest::interproc::openNamedPipeForWriting;
using std::cout;
using std::exception;

namespace kktest {
namespace feedback {

void FeedbackExtension::registerCommandLineArgs(ArgumentsApi* argumentsApi) {
    quietFlag = argumentsApi->addFlag(FlagBuilder(
        "quiet",
        "Disable STDOUT logging for this test run")
        .withShortName("q"));
    pipeNameArgument = argumentsApi->addArgument(ArgumentBuilder(
        "pipe-to",
        "A file with write access for piping the test results as they become "
        "available."));
}

void FeedbackExtension::init(ExtensionApi* api) {
    if (!quietFlag->get()) {
        initLogging(api);
    }
    if (!pipeNameArgument->get().empty()) {
        initPipe(api, pipeNameArgument->get());
    }
}

void FeedbackExtension::destroy() {
    delete logger;
    delete pipe;
}

void FeedbackExtension::initLogging(ExtensionApi* api) {
    logger = new TestLogger(cout);

    api->beforeGroup([this](const GroupInfo& groupInfo) {
        logger->addGroupInfo(groupInfo);
    });

    api->afterTest([this](const TestInfo& testInfo) {
        logger->logTest(testInfo);
    });

    api->beforeDestroy([this]() {
        logger->logFinalInformation();
    });

    api->beforeForceDestroy([this](const exception& error) {
        logger->logFatalError(error.what());
    });
}

void FeedbackExtension::initPipe(ExtensionApi* api, const String& pipeName) {
    pipe = openNamedPipeForWriting(pipeName);

    api->beforeGroup([this](const GroupInfo& groupInfo) {
        pipe->sendMessage(
                PipeMessageType::GROUP,
                groupInfo.parentGroupIndex,
                groupInfo.index,
                groupInfo.description);
    });

    api->afterTest([this](const TestInfo& testInfo) {
        pipe->sendMessage(
                PipeMessageType::TEST,
                testInfo.groupIndex,
                testInfo.index,
                testInfo.optional,
                testInfo.description,
                testInfo.passed,
                testInfo.failureMessage);
    });

    api->beforeDestroy([this]() {
        pipe->sendMessage(PipeMessageType::DONE);
    });

    api->beforeForceDestroy([this](const exception& error) {
        pipe->sendMessage(PipeMessageType::ERROR, String(error.what()));
    });
}

}  // namespace feedback
}  // namespace kktest
