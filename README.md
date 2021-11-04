# Wavious RTOS SW

Wavious RTOS Base Software project. This project contains all code
that runs on the various wavious cores utilizing FreeRTOS

## Build Configuration
When the project is first clone, the project needs to be configured using the
following command:
~~~
./configure --board <bsp-board>
~~~
where bsp-board is the targeted supported wavious core.

Since Wavious RTOS Base Software project is meant to be used as a submodule
for other Wavious projects, the configure script has been designed such that
it can be sourced and expanded for other projects. Arguments specific to
wav-rtos-sw configuration script are passed when invoking the parent's configure
script. Additionally, "extra" cmake arguments can be passed directly to this
projects configure script so that the cmake project is fully configured.

An example from Wavious LPDDR SW project:
~~~
  # Invoke RTOS configure script with addtional parameters
  args=(
      -DCONFIG_TARGET_PHY=${CONFIG_TARGET_PHY}
      -DCONFIG_CALIBRATE_PLL=${CONFIG_CAL_PLL}
      -DCONFIG_CALIBRATE_ZQCAL=${CONFIG_CAL_ZQCAL}
      -DCONFIG_CALIBRATE_SA=${CONFIG_CAL_SA}
      -DCONFIG_DRAM_TRAIN=${CONFIG_DRAM_TRAIN}
      -DCONFIG_CAL_PERIODIC=${CONFIG_CAL_PERIODIC}
  )
  source ${WAV_RTOS_DIR}/configure ${PARAMS} ${args[@]}

  To invoke: ./configure --board wavious-mcu

  board argument is passed to wav-rtos-sw configure script via ${PARAMS} argument
~~~

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
