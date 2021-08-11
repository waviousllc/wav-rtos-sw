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

## FSM vs StateMachine
FSM provides a dedicated task that all FSMs share. FSM events are submitted
to a queue and processed directly by the FSM task, which move the FSM
between diferrent states. This can be nice to have when the software has
many FSMs that need to be used but doesn't need tight control of the
run loop. It can save memory by having all events processed in a single
place and dispatched to the appropriate FSM.

Some implementations, such as a firmware, need better performance and would
like tight control of their event loop and context switching. StateMachine
offers a non-threaded implementation that can be used for these cases.

### Background
Originally, FSM was the only implementation and the need for StateMachine
was realized later. For compatibility reasons, the FSM implementation
was kept around and a unique non-threaded implementation was added.
