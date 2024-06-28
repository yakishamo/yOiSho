[Defines]
  PLATFORM_NAME              = HelloPkg
  PLATFORM_GUID              = 3EB4E374-4231-4FB1-A18F-EC8574943AFB
  PLATFORM_VERSION           = 0.1
  DSC_SPECIFICATION          = 0x00010005
  OUTPUT_DIRECTORY           = Build/HelloPkg/$(ARCH)
  SUPPORTED_ARCHITECTURES    = IA32|X64
  BUILD_TARGETS              = DEBUG|RELEASE|NOOPT

  DEFINE DEBUG_ENABLE_OUTPUT = FALSE

[LibraryClasses]
  # Entry point
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf

  # Common Libraries
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  !if $(DEBUG_ENABLE_OUTPUT)
    DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
    DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  !else ## DEBUG_ENABLE_OUTPUT
    DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  !endif ## DEBUG_ENABLE_OUTPUT

  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

[BuildOptions]
  RELEASE_XCODE5_X64_CC_FLAGS += -Wno-unused-but-set-variable -hoge

[Components]
  HelloPkg/Hello/Hello.inf
