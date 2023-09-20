# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MIMX8ML8/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_driver_igpio true)
set(CONFIG_USE_driver_pdm true)
set(CONFIG_USE_utility_debug_console_lite true)
set(CONFIG_USE_utility_assert_lite true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_rdc true)
set(CONFIG_USE_driver_audiomix true)
set(CONFIG_USE_device_MIMX8ML8_CMSIS true)
set(CONFIG_USE_component_iuart_adapter true)
set(CONFIG_USE_driver_iuart true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_device_MIMX8ML8_startup true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MIMX8ML8_system true)
set(CONFIG_CORE cm7f)
set(CONFIG_DEVICE MIMX8ML8)
set(CONFIG_BOARD evkmimx8mp)
set(CONFIG_KIT evkmimx8mp)
set(CONFIG_DEVICE_ID MIMX8ML8xxxLZ)
set(CONFIG_FPU SP_FPU)
set(CONFIG_DSP NO_DSP)
