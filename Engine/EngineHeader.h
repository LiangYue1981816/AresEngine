#pragma once
#include "PreHeader.h"
#include "GfxHeader.h"
#include "SceneHeader.h"

#include "Memory/include/Allocator.h"

#include "Base/include/Lock.h"
#include "Base/include/Utils.h"
#include "Base/include/TaskPool.h"
#include "Base/include/TaskGraph.h"
#include "Base/include/Stream.h"
#include "Base/include/FileManager.h"

#include "Resource/include/ResourceLoader.h"

#include "RenderSystem/include/Camera.h"
#include "RenderSystem/include/RenderSystem.h"
#include "RenderSystem/include/PassBlit.h"
#include "RenderSystem/include/PassPreZ.h"
#include "RenderSystem/include/PassSSAO.h"
#include "RenderSystem/include/PassShadow.h"
#include "RenderSystem/include/PassDefault.h"
#include "RenderSystem/include/PassForwardLighting.h"
#include "RenderSystem/include/PassColorGrading.h"
#include "RenderSystem/include/PassFinal.h"

#include "Engine/include/Engine.h"
