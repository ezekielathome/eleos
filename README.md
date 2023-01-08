<div align="center">
  <h3><a href="https://github.com/ezekielathome">
    ~ezekielathome/</a>eleos –
    <span style="font-size: 1.25vw">έ‧λε‧ος ( mercy, compassion )</span>
  </h3>
c++20 header-only library for exposing source engine interfaces
</div>



## example
```cpp
/// interface
class sample_interface : public eleos::interface {
public:
  sample_interface( )
      : eleos::interface( this, "ISAMPLEINTERFACE001" ) {}
  ~sample_interface( ) = default;

  // ...
};

/// variadic interface
class sample_variadic_interface : public eleos::interface {
public:
  sample_variadic_interface( )
      : eleos::interface(
            this,
            "ISAMPLEINTERFACE001",
            "ISAMPLEINTERFACE002",
            "ISAMPLEINTERFACE003" ) {}
  ~sample_variadic_interface( ) = default;

  // ...
};

/// multi interface
class sample_multi_interface : public eleos::interface {
public:
  sample_multi_interface( )
      : eleos::interface(
            this,
            { "ISAMPLEINTERFACE001",
              "ISAMPLEINTERFACE002",
              "ISAMPLEINTERFACE003" } ) {}
  ~sample_multi_interface( ) = default;

  // ...
};

/// plugin by version
class sample_plugin_v4 : public eleos::plugin {
public:
  sample_plugin_v4( )
      : eleos::plugin( this, 4 ) {}
  ~sample_plugin_v4( ) = default;

  // ...
};

/// plugin multiple versions
class sample_plugin_multiversion : public eleos::plugin {
public:
  sample_plugin_multiversion( )
      : eleos::plugin( this, 3, 4 ) {}
  ~sample_plugin_multiversion( ) = default;

  // ...
};

/// generic plugin
// exposes IServerPluginCallbacks versions 1 through 4
// only exposes the callbacks load, unload, pause, unpause and description.
class sample_generic_plugin : public eleos::generic_plugin {
public:
  sample_generic_plugin( )
      : eleos::generic_plugin( this ) {}
  ~sample_generic_plugin( ) = default;

  virtual auto load( void * /*unused*/, void * /*unused*/ ) -> bool {
    return false;
  }
  virtual auto unload( ) -> void {}
  virtual auto pause( ) -> void {}
  virtual auto unpause( ) -> void {}
  virtual auto description( ) -> const char * { return "generic_plugin"; }
}
```