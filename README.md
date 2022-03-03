# CMake-based STM32F103C8 Template (Blue Pill)
---
Based on https://github.com/ahessling/STM32F4Template

As an addition;
* style fixer tool (astyle) can be use with `./code-format.sh`  
* `make monitor` will open picocom serial terminal 
---
### Build 
```
./build.sh
cd build
make  
```
### Download binary
```
make flash  # this will run openocd if it is found during cmake process
```
### Serial terminal  
```
make monitor # this will run picocom with 115200 8N1 
```
Of course you can run all commands in one shot

`make flash monitor`

### Debug
OpenOCD tcl scripts can be copied from the tools directory to the openocd shared tcl directory.
For stlink debugger tools/stm32f4discovery.cfg can be used. 
For Segger Jlink tools/stm32f4x-jlink.cfg can be used. 

First, run openocd with the compatible board config file.e.g.
```
$OOCD_DIR/src/openocd -s $OOCD_DIR/tcl -f board/stm32f4x-jlink.cfg
```

Then, run gdb. e.g
```
arm-none-eabi-gdb -x tools/gdbinit --se build/src/blackpill.elf
```

For the FreeRTOS thread awareness you need to add `-rtos` parameter to the target create script inside tcl/target/stm32f4x.cfg 
```
target create $_TARGETNAME cortex_m -endian $_ENDIAN -dap $_CHIPNAME.dap -rtos FreeRTOS
```