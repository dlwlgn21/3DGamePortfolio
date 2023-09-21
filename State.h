#pragma once
namespace jh::fsm
{

template<class T>
class State
{
public:
	State(T* pScript)
		: mpOwnerScript(pScript)
	{
		assert(mpOwnerScript != nullptr);
	}
	virtual ~State() = default;
	virtual void Enter() = 0;
	virtual void Excute() = 0;
	virtual void Exit() = 0;
protected:
	T* mpOwnerScript;
};

}
