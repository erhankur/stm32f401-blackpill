# CMake-based STM32F401 Template (Black Pill)
---
Based on https://github.com/ahessling/STM32F4Template

As an addition;
* style fixer tool (astyle) can be use with `./code-format.sh`  
* `make monitor` will open picocom serial terminal 
---
# Toolchain

- Install arm toolchain from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
- Install `cmake` and `make` build tools
- Install `openocd` to upload binaries to the Flash. https://github.com/xpack-dev-tools/openocd-xpack/releases

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
or using openocd commands directly
```
$OOCD_DIR/src/openocd -s $OOCD_DIR/tcl -f interface/stm32f4x-jlink.cfg -c "init; reset halt; stm32f2x unlock 0" -c "flash erase_sector 0 0 last" -c "flash write_bank 0 build/src/blackpill.bin 0" -c "reset; exit"
````

```
$OOCD_DIR/src/openocd -s $OOCD_DIR/tcl -f interface/stm32f4x-jlink.cfg -c "program build/src/blackpill.elf reset; exit"
```

### Serial terminal  
```
make monitor # this will run picocom with 115200 8N1 
```
Of course you can run all commands in one shot

`make flash monitor`

### Debug
OpenOCD tcl scripts can be copied from the tools directory to the openocd shared tcl directory

For the `Stlink` debugger you can find the config file inside `tools/stm32f4discovery.cfg`

For the `Segger Jlink` debugger you can find the config file inside `tools/stm32f4x-jlink.cfg`

To start debugging, run openocd with the compatible board config file. e.g.
```
$OOCD_DIR/src/openocd -s $OOCD_DIR/tcl -f board/stm32f4x-jlink.cfg
```

Then, run gdb
```
arm-none-eabi-gdb -x tools/gdbinit --se build/src/blackpill.elf
```

For the FreeRTOS thread awareness you need to add `-rtos` parameter to the target create script inside `tcl/target/stm32f4x.cfg`
```
target create $_TARGETNAME cortex_m -endian $_ENDIAN -dap $_CHIPNAME.dap -rtos FreeRTOS
```