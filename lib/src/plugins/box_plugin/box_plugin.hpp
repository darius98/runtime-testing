#ifndef KKTEST_PLUGINS_BOX_PLUGIN_BOX_PLUGIN_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_BOX_PLUGIN_H_

#include <utils/plugin.hpp>
#include "box_executor.hpp"


namespace kktest {

extern Plugin* boxPlugin;

class BoxPlugin: public Plugin {
public:
    using Plugin::Plugin;

    bool isEnabled() const override;

    void install() override;

    void uninstall() override;

private:
    BoxExecutor* boxExecutor;
};

}

#endif
