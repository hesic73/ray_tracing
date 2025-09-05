# Ray Tracing

A simple ray tracing project for CSCI 580. Currently under development.

Tested on:

- Windows 11 (MSVC 19.44.35207.1)
- Ubuntu 22.04 (GCC 11.4.0)

## Build & Run

```bash
mkdir build && cd build
cmake ..
make
```


```bash
./ray_tracing path/to/config.yaml
```

## Coordinate System Convention

- **+X**: Right
- **+Y**: Up  
- **+Z**: Toward the camera (out of the screen)
- **-Z**: Camera viewing direction (into the scene)

## Dependencies

- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

## Reference

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)