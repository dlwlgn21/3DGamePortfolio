#pragma once

namespace jh::enums
{

enum class eSceneType
{
	TITLE_SCENE,
	PLAY_SCENE,
	COUNT,
};

enum class eLayerType
{
	CAMERA,
	MONSTER,
	PLAYER,

	PARTICLE,
	POST_PROCESS,
	LIGHT,
	CUBE_MAP,
	UI,
	COUNT
};

enum class eComponentType
{
	NONE,
	TRANSFORM, // 위치 데이터 수정하는 컴포넌트
	CAMERA,
	ANIMATOR,
	RENDERER,
	PARTICLE_SYSTEM,
	AUDIO_LISTENER,
	AUDIO_SOURCE,
	LIGHT,
	SCRIPT,
	RIGID_BODY_2D,
	BOX_COLLIDER_2D,
	UI,
	COUNT
};

enum class eScriptType
{
	MOVING,
	HEAD,
	COUNT
};

enum class eRenererComponentType
{
	MESH_RENDERER,
	SPRITE_RENDERER,
	COUNT
};

enum class eResourceType
{
	MESH,
	TEXTURE,
	MATERIAL,
	SOUND,
	PREFAB,
	MESH_DATA,
	MODEL,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	AUDIO_CLIP,
	SCRIPT,
	COUNT
};

enum eColliderFilter
{
	NONE = 0,
	GROUND = 1 << 0,
	PLAYER = 1 << 1,
	MONSTER = 1 << 2,
	ALL = 0xffff,
};

enum class eAnimationType
{
	NONE,
	SECOND_DIMENSION,
	THIRD_DIMENSION,
	COUNT
};

enum class eLightType
{
	DIRECTIONAL,
	POINT,
	SPOT,
	COUNT
};

}