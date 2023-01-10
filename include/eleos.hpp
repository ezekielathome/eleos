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
  };

  extern "C" [[gnu::used]] inline auto
  CreateInterface( const char *name, int *return_code ) -> void * {
    // search through interface registry
    auto interface =  [name]() -> void* {
      if ( auto interface = impl::registry.find( name );
           interface != impl::registry.end( ) )
        return interface->second;
      else
        return { };
    } ();
    
    // set return_code if it's valid
    if ( return_code != nullptr ) {
      *return_code = interface != nullptr ? eleos::return_code::OK
                                          : eleos::return_code::FAILED;
    }

    return interface;
  }
} // namespace eleos


#endif // ELEOS_HPP