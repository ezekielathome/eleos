#ifndef ELEOS_HPP
#define ELEOS_HPP

#include <concepts>
#include <string>
#include <unordered_map>
#include <vector>

namespace eleos {
// forward declarations
class interface;

namespace impl {
template <class T, class U>
concept derived_of = std::is_base_of<U, T>::value;

inline static std::unordered_map<std::string, interface *> registry = {};
} // namespace impl

enum return_code { OK = 0, FAILED };

/// interfaces
class interface {
public:
  interface() = default;
  template <impl::derived_of<interface> T>
  explicit interface(T *instance, std::string name) {
    impl::registry.emplace(name, instance);
  }

  static auto find(const std::string &name) -> void * {
    for (auto const &interface : eleos::impl::registry) {
      if (interface.first == name) {
        return interface.second;
      }
    }
    return {};
  }
};

class variadic_interface : public interface {
public:
  variadic_interface() = default;
  template <impl::derived_of<variadic_interface> T, typename... N>
  explicit variadic_interface(T *instance, N &&...names) {
    for (auto name : {names...}) {
      impl::registry.emplace(name, instance);
    }
  }
};

class multi_interface : public interface {
public:
  multi_interface() = default;
  template <impl::derived_of<multi_interface> T, typename... N>
  multi_interface(T *instance, const std::vector<std::string> &names) {
    for (auto const &name : names) {
      impl::registry.emplace(name, instance);
    }
  }
};

/// plugin helpers
class plugin : public multi_interface {
public:
  plugin() = default;
  template <impl::derived_of<plugin> T, typename... V>
  explicit plugin(T *instance, V &&...versions) {
    std::vector<std::string> interfaces;
    for (auto version : {versions...}) {
      char buffer[128];
      snprintf(buffer, sizeof buffer, "ISERVERPLUGINCALLBACKS%03d", version);
      interfaces.emplace_back(std::string(buffer));
    }

    multi_interface(instance, interfaces);
  }
};

class generic_plugin : public plugin {
public:
  generic_plugin() = default;
  template <impl::derived_of<generic_plugin> T>
  explicit generic_plugin(T *instance) : plugin(instance, 1, 2, 3, 4) {}

  virtual auto load(void * /*unused*/, void * /*unused*/) -> bool {
    return false;
  }
  virtual auto unload() -> void {}
  virtual auto pause() -> void {}
  virtual auto unpause() -> void {}
  virtual auto description() -> const char * { return "generic_plugin"; }
};
} // namespace eleos

extern "C" [[gnu::used]] inline auto CreateInterface(const char *name,
                                                     int *return_code)
    -> void * {
  auto *interface = eleos::interface::find(name);
  if (return_code != nullptr) {
    *return_code = interface != nullptr ? eleos::return_code::OK
                                        : eleos::return_code::FAILED;
  }

  return interface;
}

#endif // ELEOS_HPP