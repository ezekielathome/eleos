#ifndef ELEOS_HPP
#define ELEOS_HPP

#include <concepts>
#include <string>
#include <unordered_map>

namespace eleos {
  // forward declarations
  class interface;

  namespace impl {
    // concept that matches anything that inherits from U
    template < class T, class U >
    concept derived_of = std::is_base_of< U, T >::value;

    // concept that matches anything that can be converted
    // to a std::string_view (ie. a std::string)
    template < typename T >
    concept string_like = std::is_convertible_v< T, std::string_view >;

    // interface registry
    inline static std::unordered_map< std::string, interface * > registry = { };
  } // namespace impl

  // CreateInterface return code
  enum return_code { OK = 0, FAILED };

  // interfaces
  class interface {
  public:
    interface( ) = default;
    template < impl::string_like... N > explicit interface( N &&...names ) {
      ( impl::registry.emplace( names, this ), ... );
    }
  };

  // exported function called by source engine
  // to find a given interface
  extern "C" __attribute__( ( used ) ) inline auto
  CreateInterface( const char *name, int *return_code ) -> void * {
    // search through interface registry
    auto *interface = [ name ]( ) -> void * {
      if ( auto interface = impl::registry.find( name );
           interface != impl::registry.end( ) )
        return interface->second;
      return { };
    }( );

    // set return_code if applicable
    if ( return_code != nullptr ) {
      *return_code =
          interface != nullptr ? return_code::OK : return_code::FAILED;
    }

    return interface;
  }
} // namespace eleos

#endif // ELEOS_HPP