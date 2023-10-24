#pragma once
#include <string>

namespace jh::graphics::keys
{
	static const std::string BASIC_3D_DIFFUSE_BOX_TEXTURE_KEY = "t1";
	static const std::string BASIC_3D_NORMAL_TEXTURE_KEY = "normalT";

#pragma region ZELDA_MODEL
	static const std::string ZELDA1_MATERIAL = "zma1";
	static const std::string ZELDA2_MATERIAL = "zma2";
	static const std::string ZELDA3_MATERIAL = "zma3";
	static const std::string ZELDA4_MATERIAL = "zma4";
	static const std::string ZELDA5_MATERIAL = "zma5";
	static const std::string ZELDA6_MATERIAL = "zma6";
	static const std::string ZELDA7_MATERIAL = "zma7";
	static const std::string ZELDA1_MESH = "zm1";
	static const std::string ZELDA2_MESH = "zm2";
	static const std::string ZELDA3_MESH = "zm3";
	static const std::string ZELDA4_MESH = "zm4";
	static const std::string ZELDA5_MESH = "zm5";
	static const std::string ZELDA6_MESH = "zm6";
	static const std::string ZELDA7_MESH = "zm7";
	static const std::string ZELDA1_TEXTURE = "zt1";
	static const std::string ZELDA2_TEXTURE = "zt2";
	static const std::string ZELDA3_TEXTURE = "zt3";
	static const std::string ZELDA4_TEXTURE = "zt4";
	static const std::string ZELDA5_TEXTURE = "zt5";
	static const std::string ZELDA6_TEXTURE = "zt6";
	static const std::string ZELDA7_TEXTURE = "zt7";
	static const std::string ZELDA_MODEL = "zelModel";
#pragma endregion



#pragma region FEMALE_SOLDER
	static const std::string FEMALE_SOLDER_MESH				= "FeMesh";
	static const std::string FEMALE_SOLDER_DIFFUSE_TEXTURE	= "FeDtex";
	static const std::string FEMALE_SOLDER_NORMAL_TEXTURE	= "FeNTex";
	static const std::string FEMALE_SOLDER_MATERIAL			= "FeMaterial";
	static const std::string FEMALE_SOLDER_MODEL			= "FeModel";
#pragma endregion

#pragma region BARNY
	static const std::string BARNY_MESH				 = "barnyMesh";
	static const std::string BARNY_SKINNED_MESH		 = "barnySMesh";
	static const std::string BARNY_DIFFUSE_TEXTURE	 = "barnyDTex";
	static const std::string BARNY_NORMAL_TEXTURE	 = "barnyNTex";
	static const std::string BARNY_SKINNNED_MATERIAL = "barnyMate";
	static const std::string BARNY_MODEL			 = "barnyModel";
	static const std::string BARNY_SKINNED_MODEL	 = "barnySkinnedModel";
#pragma endregion


	static const std::string CUBE_MAP_TEXTURE = "cubetex";
	static const std::string CUBE_MAP_MESH = "cubeMesh";
	static const std::string CUBE_MAP_MATERIAL = "cubeMat";

	static const std::string WORLD_COORD_MESH_KEY = "wc";
	static const std::string WORLD_COORD_MATERIAL = "wcMat";
	static const std::string WORLD_COORD_MODEL = "wcModel";


	static const std::string BASIC_3D_MATERIAL_KEY = "a";
	static const std::string BOX_MESH_KEY = "box";
	static const std::string BOX_MODEL = "boxModel";
	static const std::string SPEHERE_MESH_KEY = "speMesh";
	static const std::string SPEHERE_MODEL_KEY = "speModel";

	static const std::string MONKEY_MESH_KEY = "monkey";
	static const std::string MONKEY_DIFFUSE_TEXTURE_KEY = "monkeytex";


	static const std::string DEBUG_COLLIDER_BOX_MESH_KEY = "ColliderBoxMesh";
	static const std::string DEBUG_COLLIDER_BOX_MATERIAL_KEY = "ColliderBoxMaterial";
	static const std::string DEBUG_COLLIDER_BOX_MODEL_KEY = "ColliderBoxModel";


}
