# eleos
c++20 header-only library for exposing source engine interfaces  
& plugin helpers

## example
```cpp
/// named interface
class sample_interface : public eleos::interface {
public:
    sample_interface() : eleos::interface(this, "ISAMPLEINTERFACE001") {}
    ~sample_interface()
    
    // ...
};

/// plugin by version
class sample_plugin_v4 : public eleos::plugin {
public:
    sample_plugin_v4() : eleos::plugin(this, 4) {}
    ~sample_plugin_v4()

    // ...
};

/// plugin multiple versions
class sample_plugin_multiversion : public eleos::plugin {
public:
    sample_plugin_multiversion() : eleos::plugin(this, 3, 4) {}
    ~sample_plugin_multiversion()

    // ...
};

/// generic plugin
// exposes IServerPluginCallbacks versions 1 through 4
// only exposes the callbacks load, unload, pause, unpause and description.
class sample_generic_plugin : public eleos::generic_plugin {
public:
    sample_generic_plugin() : eleos::generic_plugin(this) {}
    ~sample_generic_plugin()

public:
    virtual bool load(void*, void*) {}
    virtual const char* description() { return "sample_generic_plugin"; }
};
```