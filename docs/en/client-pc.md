[English](client-pc.md) | [Русский](../ru/client-pc.md) | [Contents](../en/README.md)

# C++ client (`SVClient`)

Header: [`src/SVClient/SVClient.h`](../../src/SVClient/SVClient.h). The client **sends** values; it does not subscribe or read back.

```cpp
namespace SV {
  bool svConnect(const char *moduleName, const char *ipAddrServ, int portServ);
  void svDisconnect();
  bool svAddBoolValue(const char *name, bool Value, bool onlyFront = false);
  bool svAddIntValue(const char *name, int Value);
  bool svAddFloatValue(const char *name, float Value);
  bool svSetParam(int cycleRecMs, int packetSz);
}
```

Module and value names: max **24** characters. Do not put `=begin=` or `=end=` in names.

Example from [`src/SVClientExample/src/main.cpp`](../../src/SVClientExample/src/main.cpp): connect to `127.0.0.1:2144` (Monitor’s default TCP port), `svSetParam(100, 10)` unless you pass other CLI args.

```cpp
SV::svSetParam(100, 10);
if (SV::svConnect("client", "127.0.0.1", 2144)) {
  SV::svAddIntValue("sin", value);
  SV::svAddBoolValue("flag", value > 0);
}
```

`cycleRecMs` / `packetSz` must match what you intend on the server (Monitor settings). For rates above ~1 kHz also set the server **time offset**.
