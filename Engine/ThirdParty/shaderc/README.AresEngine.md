�����Vulkanʹ�û������ƹ��������Uniform������, �ڴ�ͬʱVulkan��Uniform���зǳ��ϸ��ʹ��Ҫ��(����DescriptorSet�в��ܰ���Shader��δ��ʹ�õ�Uniform). 
Shaderc�����Ż�����ʱ�Ὣû��ʹ�õ�Uniform�Ż���, �Ӷ����DescritporSet��Shader��ƥ��������±���.

���ڴ�ԭ���Shaderc���������޸�
1. ���öԱ�������������
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

2. ��ֹ�Ż�����ʹ�õı���
/third_party/spirv-tools/source/opt/aggressive_dead_code_elim_pass.cpp
  // Process module-level instructions. Now that all live instructions have
  // been marked, it is safe to remove dead global values.
  // modified |= ProcessGlobalValues();
