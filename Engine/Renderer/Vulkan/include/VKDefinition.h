#pragma once


#define CALL_VK_FUNCTION_RETURN(func)        \
{                                            \
	VkResult err = func;                     \
	if (err != VK_SUCCESS) {                 \
		CVKInstance::SetLastError(err);      \
		return err;                          \
	}                                        \
}

#define CALL_VK_FUNCTION_RETURN_BOOL(func)   \
{                                            \
	VkResult err = func;                     \
	if (err != VK_SUCCESS) {                 \
		CVKInstance::SetLastError(err);      \
		return false;                        \
	}                                        \
}


class CVKRenderer;

class CVKAllocator;
class CVKInstance;
