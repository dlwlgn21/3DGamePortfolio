#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Common.h"
#include "Entity.h"


namespace jh::graphics
{
	class GraphicResource : public Entity
	{
	public:
		GraphicResource(const jh::enums::eResourceType eType);
		virtual ~GraphicResource() = default;

		jh::enums::eResourceType GetResourceType() const { return meType; }

		virtual HRESULT Load(const std::wstring& path);

		const std::string& GetKey() const { return mKey; }
		const std::wstring& GetPath() const { return mPath; }

		void SetKey(const std::string& keyName) { mKey = keyName; }
		void SetPath(const std::wstring& path) { mPath = path; }

	protected:
		const jh::enums::eResourceType meType;
		std::string mKey;
		std::wstring mPath;
	};

}

