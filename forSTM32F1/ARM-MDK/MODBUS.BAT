SET PATH=C:\KEIL5\ARM\ARMCC\Bin;D:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\bin;D:\MentorGraphics\9.5PADS\SDD_HOME\common\win32\lib;D:\MentorGraphics\9.5PADS\MGC_HOME.ixn\bin;D:\MentorGraphics\9.5PADS\MGC_HOME.ixn\lib;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\Common Files\Autodesk Shared\;C:\Program Files\Microsoft SQL Server\80\Tools\BINN;C:\Program Files\Flash Magic;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\MATLAB7\bin\win32;C:\Program Files\IVI Foundation\VISA\WinNT\bin;C:\Program Files\IVI Foundation\IVI\bin;C:\Program Files\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files\TortoiseSVN\bin;C:\Program Files\Altium Designer Winter 09\System;;C:\PROGRA~1\FLASHM~1
SET CPU_TYPE=STM32F105RB
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=MODBUS
SET CPU_CLOCK=0x00B71B00
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\core_cm3.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\system_stm32f10x.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmAsm" --Via ".\obj\startup_stm32f10x_cl._ia"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\main.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_it.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\mylib.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\app.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\datasource.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\driver.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_cpu_c.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_dbg.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_core.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_sem.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_task.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_time.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_tmr.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmAsm" --Via ".\obj\os_cpu_a._ia"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\os_mbox.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\bsp.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_gpio.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_rcc.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\misc.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_exti.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmCC" --Via ".\obj\stm32f10x_usart.__i"
"C:\KEIL5\ARM\ARMCC\Bin\ArmLink" --Via ".\obj\STM32F10x.lnp"
"C:\KEIL5\ARM\ARMCC\Bin\fromelf.exe" ".\obj\STM32F10x.axf" --i32combined --output ".\obj\STM32F10x.hex"
fromelf --bin --output .\output\STM32F10x.bin  .\obj\STM32F10x.axf
