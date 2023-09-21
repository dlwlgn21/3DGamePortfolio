#include "Component.h"

using namespace jh::enums;
namespace jh
{

Component::Component(eComponentType eType)
	: Entity()
	, mpOwner(nullptr)
	, meType(eType)
{
}

void Component::Initialize()
{
}
void Component::Update()
{
}
void Component::FixedUpdate()
{
}
void Component::Render()
{
}


}