#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_PLUGIN_API_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_PLUGIN_API_H_

#include <functional>

#include <kktest_impl/info.hpp>

namespace kktest {

typedef std::function<void(const TestInfo&)> TestHook;
typedef std::function<void(const GroupInfo&)> GroupHook;
typedef std::function<void()> AfterInitHook;
typedef std::function<void()> BeforeDestroyHook;
typedef std::function<void(const std::exception&)> BeforeForceDestroyHook;

class PluginApi {
public:
    virtual void addBeforeTestHook(const TestHook& hook) = 0;
    virtual void addAfterTestHook(const TestHook& hook) = 0;
    virtual void addBeforeGroupHook(const GroupHook& hook) = 0;
    virtual void addAfterGroupHook(const GroupHook& hook) = 0;
    virtual void addAfterInitHook(const AfterInitHook& hook) = 0;
    virtual void addBeforeDestroyHook(const BeforeDestroyHook& hook) = 0;
    virtual void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook) = 0;

};

class Plugin {
public:
    virtual ~Plugin();

    virtual void install(PluginApi* api);

    virtual void uninstall();
};

class ExtensionManager {
public:
    virtual ~ExtensionManager();

    virtual void init(const std::function<void(Plugin*)>& addPlugin);

    virtual void destroy();
};

}

#endif
