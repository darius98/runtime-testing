#ifndef KKTEST_EXTENSIONS_DEATH_KKTEST_EXT_DEATH_IMPL_CHECK_DEATH_HPP_
#define KKTEST_EXTENSIONS_DEATH_KKTEST_EXT_DEATH_IMPL_CHECK_DEATH_HPP_

#include <kktest_ext/death_impl/death_status.hpp>

namespace kktest {
namespace death {

DeathStatus checkDeath(const std::function<void()>& func);

}
}

#endif