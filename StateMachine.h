#pragma once
#include "Common.h"
#include "State.h"

namespace jh::fsm
{

template<class T>
class StateMachine
{
public:
	StateMachine() = default;
	virtual ~StateMachine() = default;

	void Init(State<T>& entryState);

	void Excute();

	void ChangeState(State<T>& newState);

protected:
	State<T>* mpCurrenState = nullptr;
};


template<class T>
void StateMachine<T>::Init(State<T>& entryState)
{
	mpCurrenState = &entryState;
}

template<class T>
void StateMachine<T>::Excute()
{
	if (mpCurrenState != nullptr)
	{
		mpCurrenState->Excute();
	}
}

template<class T>
void StateMachine<T>::ChangeState(State<T>& newState)
{
	if (mpCurrenState != nullptr)
	{
		mpCurrenState->Exit();
	}

	mpCurrenState = &newState;
	mpCurrenState->Enter();
}

}
