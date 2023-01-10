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
    template < class T, class U >
    concept derived_of = std::is_base_of< U, T >::value;

    template < typename T >
    concept string_like = std::is_convertible_v< T, std::string_view >;

    /// Interface registry
    inline static std::unordered_map< std::string, interface * > registry = { };
  } // namespace impl

  enum return_code { OK = 0, FAILED };

  /// interfaces
  class interface {
  public:
    interface( ) = default;
    template < impl::derived_of< interface > T, impl::string_like... N >
    explicit interface( T *instance, N &&...names ) {
      ( impl::registry.emplace( names, instance ), ... );
    }

    template < impl::derived_of< interface > T, impl::string_like N >
    explicit interface( T *instance, const std::vector< N > &names ) {
      for ( auto const &name : names ) {
        impl::registry.emplace( name, instance );
      }
    }

    /// search registry for interface
    static auto find( const std::string &name ) -> void * {
      for ( auto const &interface : eleos::impl::registry ) {
        if ( interface.first == name ) {
          return interface.second;
        }
      }
      return { };
    }
  };
} // namespace eleos

extern "C" [[gnu::used]] inline auto
CreateInterface( const char *name, int *return_code ) -> void * {
  auto *interface = eleos::interface::find( name );
  if ( return_code != nullptr ) {
    *return_code = interface != nullptr ? eleos::return_code::OK
                                        : eleos::return_code::FAILED;
  }

  return interface;
}

#endif // ELEOS_HPP