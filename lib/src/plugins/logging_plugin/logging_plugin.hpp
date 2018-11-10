#ifndef KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGING_PLUGIN_H_
#define KKTEST_PLUGINS_LOGGING_PLUGIN_LOGGING_PLUGIN_H_

#include <utils/plugin.hpp>
#include "logger.hpp"


namespace kktest {

extern Plugin* loggingPlugin;

class LoggingPlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override;

    void install() override;

    void uninstall() override;

private:
    Logger* logger = nullptr;
    int numPassedTests = 0;
    int numFailedTests = 0;
    int numFailedOptionalTests = 0;
};

}

#endif
