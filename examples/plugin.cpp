#include <eleos.hpp> // iwyu: for interface
#include <stdio.h>   // iwyu: for printf

class sample_plugin : public eleos::interface {
public:
  sample_plugin( )
      : eleos::interface( this, "ISERVERPLUGINCALLBACKS004" ) {}

  virtual auto load( void * /*unused*/, void * /*unused*/ ) -> bool {
    printf( "sample_plugin!\n" );
    return false;
  }
  virtual auto unload( ) -> void {}
  virtual auto pause( ) -> void {}
  virtual auto unpause( ) -> void {}
  virtual auto description( ) -> const char * { return "sample_plugin"; };

  // NOTE: This is an incomplete VMT for ISERVERPLUGINCALLBACKS004
  // SRCDS will load it, but will crash shortly after the server initializes.
};

inline sample_plugin g_sample_plugin;
