## Dependencies
- cpp-taskflow

  Expected at `lib/cpp-taskflow`

- ENTT

  Expected at `lib/entt/src`

  To build:
    `cd lib && git clone https://github.com/skypjack/entt.git`

- glm, glew, glfw3

  Installed to system and available to `pkg-config`

## Building
```
  mkdir build
  cd build
  cmake ../.
  make
  ./main.o
```
