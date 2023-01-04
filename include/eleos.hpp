#include <concepts>
#include <string>
#include <unordered_map>
#include <vector>

namespace eleos {
// forward declarations
class interface;

namespace impl {
template <class T, class U> concept derived_of = std::is_base_of<U, T>::value;

inline static std::unordered_map<std::string, interface *> registry = {};
} // namespace impl

enum result_code
{
    OK = 0,
    FAILED
};


/// interfaces
class interface {
public:
  interface() = default;
  template <impl::derived_of<interface> T>
  interface(T *instance, std::string name) {
    impl::registry.emplace(name, dynamic_cast<interface *>(instance));
  }

  static auto find(std::string name) -> void* {
    for (auto interface: eleos::impl::registry)
        if (interface.first == name)
            return interface.second;
    return {};
  }
};

class variadic_interface : public interface {
public:
  variadic_interface() = default;
  template <impl::derived_of<variadic_interface> T, typename... N>
  variadic_interface(T *instance, N &&... names) {
    auto interface = dynamic_cast<variadic_interface *>(instance);
    for (auto name : {names...})
      impl::registry.emplace(name, interface);
  }
};

class multi_interface : public interface {
public:
  multi_interface() = default;
  template <impl::derived_of<multi_interface> T, typename... N>
  multi_interface(T *instance, std::vector<std::string> names) {
    auto interface = dynamic_cast<multi_interface *>(instance);
    for (auto name : names)
      impl::registry.emplace(name, interface);
  }
};

/// plugin helpers
class plugin : public multi_interface {
public:
  plugin() = default;
  template <impl::derived_of<plugin> T, typename... V>
  plugin(T *instance, V &&... v) {
    std::vector<std::string> interfaces;
    for (auto version : {v...}) {
      char buf[128];
      snprintf(buf, sizeof buf, "ISERVERPLUGINCALLBACKS%03d", version);
      interfaces.push_back(std::string(buf));
    }

    multi_interface(dynamic_cast<plugin *>(instance), interfaces);
  }
};

class generic_plugin : public plugin {
public:
  generic_plugin() = default;
  template <impl::derived_of<generic_plugin> T>
  generic_plugin(T *instance) : plugin(instance, 1, 2, 3, 4) {}

  virtual bool load(void *, void *) { return 0; }
  virtual void unload() {}
  virtual void pause() {}
  virtual void unpause() {}
  virtual const char *description() { return "generic_plugin"; }
};
} // namespace eleos


extern "C" [[gnu::used]] inline void* 
CreateInterface(const char* name, u32* return_code) {
    auto interface = eleos::interface::find(name);
    if (return_code)
        *return_code = interface ? ReturnCode::OK : ReturnCode::FAILED;

    return interface;
}