<div align="center">
  <h3><a href="https://github.com/ezekielathome">
    ~ezekielathome/</a>eleos – έ‧λε‧ος ( mercy, compassion )
  </h3>
c++20 header-only library for exposing source engine interfaces
</div>

## usage
```cpp
/// interface constructors
template < impl::derived_of< interface > T, impl::string_like... N >
explicit interface( T *instance, N &&...names );

template < impl::derived_of< interface > T, impl::string_like N >
explicit interface( T *instance, const std::vector< N > &names );

/// example usages
// sample interface
class sample_interface : public eleos::interface {
public:
  sample_interface( )
      : eleos::interface( this, "ISAMPLEINTERFACE001" ) {}
  ~sample_interface( ) = default;

  // ...
};

// sample plugin
class sample_plugin
    : public eleos::interface
    , valve::plugin_callbacks::v4 {
public:
  sample_plugin( )
      : eleos::interface( this, "ISERVERPLUGINCALLBACKS004" ) {}
  ~sample_plugin( ) = default;

  // ...
  virtual auto load( void * /*unused*/, void * /*unused*/ ) -> bool {
    return false;
  }
  virtual auto unload( ) -> void {}
  virtual auto pause( ) -> void {}
  virtual auto unpause( ) -> void {}
  virtual auto description( ) -> const char * { return "sample_plugin"; }
}
```