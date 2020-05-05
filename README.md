Dependencies:

cpp-taskflow
  Expexted at "lib/cpp-taskflow"

ENTT
  Expected at "lib/entt/src"
  To build:
    cd lib && git clone https://github.com/skypjack/entt.git

glm, glew, glfw3
  Installed to system and available to pkg-config

to build:
  mkdir build
  cd build
  cmake ../.
  make
  ./main.o
