引擎对Vulkan使用基于名称管理各类型Uniform变量等, 于此同时Vulkan对Uniform具有非常严格的使用要求(比如DescriptorSet中不能绑定在Shader中未被使用的Uniform). 
Shaderc在最优化编译时会将没有使用的Uniform优化掉, 从而造成DescritporSet与Shader不匹配进而导致崩溃.

基于此原因对Shaderc进行如下修改
1. 禁用对变量名进行扰码
/libshaderc_util/src/compiler.cc
  switch (level) {
    case OptimizationLevel::Size:
      //if (!generate_debug_info_) {
      //  enabled_opt_passes_.push_back(PassId::kStripDebugInfo);
      //}
      enabled_opt_passes_.push_back(PassId::kSizePasses);
      break;
    case OptimizationLevel::Performance:
      //if (!generate_debug_info_) {
      //  enabled_opt_passes_.push_back(PassId::kStripDebugInfo);
      //}
      enabled_opt_passes_.push_back(PassId::kPerformancePasses);
      break;
    default:
      break;
  }

2. 禁止优化掉不使用的变量
/third_party/spirv-tools/source/opt/aggressive_dead_code_elim_pass.cpp
  // Process module-level instructions. Now that all live instructions have
  // been marked, it is safe to remove dead global values.
  // modified |= ProcessGlobalValues();
