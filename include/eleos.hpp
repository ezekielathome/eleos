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
    concept signed_integral = std::is_integral_v< T > && std::is_signed_v< T >;

    template < typename T >
    concept string_like = std::is_convertible_v<T, std::string_view>;
    
    /// Interface registry
    inline static std::unordered_map< std::string, interface * > registry = { };
  } // namespace impl

  enum return_code { OK = 0, FAILED };

  /// interfaces
  class interface {
  public:
    interface( ) = default;
    template < impl::derived_of< interface > T, impl::string_like... N >
    explicit interface( T *instance, N&& ...names ) {
      (impl::registry.emplace( names, instance ), ...);
    }

    template < impl::derived_of< interface > T, impl::string_like N >
    explicit interface( T *instance, const std::vector< N > &names) {
      for ( auto const &name : names ) {
        impl::registry.emplace( name, instance );
      }
    }

    static auto find( const std::string &name ) -> void * {
      for ( auto const &interface : eleos::impl::registry ) {
        if ( interface.first == name ) {
          return interface.second;
        }
      }
      return { };
    }
  };


  /// plugin helpers
  class plugin : public interface {
  public:
    plugin( ) = default;
    template < impl::derived_of< plugin > T, impl::signed_integral... V >
    explicit plugin( T *instance, V &&...versions ) {
      std::vector< std::string > interfaces;
      for ( auto version : { versions... } ) {
        char buffer[ 128 ];
        snprintf(
            buffer, sizeof buffer, "ISERVERPLUGINCALLBACKS%03d", version );
        interfaces.emplace_back( std::string( buffer ) );
      }

      interface( instance, interfaces );
    }
  };

  class generic_plugin : public plugin {
  public:
    generic_plugin( ) = default;
    template < impl::derived_of< generic_plugin > T >
    explicit generic_plugin( T *instance )
        : plugin( instance, 1, 2, 3, 4 ) {}

    virtual auto load( void * /*unused*/, void * /*unused*/ ) -> bool {
      return false;
    }
    virtual auto unload( ) -> void {}
    virtual auto pause( ) -> void {}
    virtual auto unpause( ) -> void {}
    virtual auto description( ) -> const char * { return "generic_plugin"; }
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