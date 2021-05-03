# Wavious RTOS SW

Wavious RTOS Base Software project. This project contains all code
that runs on the various wavious cores utilizing FreeRTOS

## Building with CMAKE
When the project is first clone, the project needs to be configured using the
following command:
~~~
./configure --board <bsp-board>
~~~
where bsp-board is the targeted supported wavious core.

## Building with CMAKE ##
Once the environment is configured, software can be
built using the make command when in the build directory.
~~~
cd build
make
~~~

---
**NOTE**

It is recommended that you use Docker to build this project. A Docker container
can be started by running the following command from the root of this project:
`./docker/run.sh`. It is important to run the configure command within the
Docker container.

---
