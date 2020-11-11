#pragma once
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

class Miner;

template <class T>
class State
{
public:

  virtual ~State(){}

  //this will execute when the state is entered
  virtual void Enter(T*)=0;

  //this is the state's normal update function
  virtual void Execute(T*)=0;

  //this will execute when the state is exited. (My word, isn't
  //life full of surprises... ;o))
  virtual void Exit(T*)=0;

};

template <class T>
class StateMachine {
public:
	void Update();
	void SetGlobalState(State<T> *target);
	void SetCurrentState(State<T> *target);
	void ChangeState(State<T> *target);
	void RevertToPreviousState();
};


template<class T>
inline void StateMachine<T>::Update() {
}

template<class T>
inline void StateMachine<T>::SetGlobalState(State<T>* target) {
}

template<class T>
inline void StateMachine<T>::SetCurrentState(State<T>* target) {
}

template<class T>
inline void StateMachine<T>::ChangeState(State<T>* target) {
}

template<class T>
inline void StateMachine<T>::RevertToPreviousState() {
}
