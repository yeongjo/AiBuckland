#pragma once
#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

class Dish;
class ADishState : public State<Dish> {
public:
	static ADishState* Instance();
	virtual void Enter(Dish* wife) {}
	virtual void Execute(Dish* wife);
	virtual void Exit(Dish* wife) {}
	virtual bool OnMessage(Dish* wife, const Telegram& msg);
};

class NormalDish : public State<Dish> {
public:
	static NormalDish* Instance() {
		static NormalDish instance;
		return &instance;
	}
	virtual void Enter(Dish* wife){}
	virtual void Execute(Dish* wife);
	virtual void Exit(Dish* wife){}
	virtual bool OnMessage(Dish* wife, const Telegram& msg){ return false; }
};
class DirtDish : public State<Dish> {
public:
	static DirtDish* Instance() {
		static DirtDish instance;
		return &instance;
	}
	virtual void Enter(Dish* wife) {}
	virtual void Execute(Dish* wife);
	virtual void Exit(Dish* wife) {}
	virtual bool OnMessage(Dish* wife, const Telegram& msg) { return false; }
};

class Dish : public BaseGameEntity {
	StateMachine<Dish>* m_pStateMachine;
public:
	Dish(int id) :BaseGameEntity(id) {
		m_pStateMachine = new StateMachine<Dish>(this);
		m_pStateMachine->SetCurrentState(NormalDish::Instance());
		m_pStateMachine->SetGlobalState(ADishState::Instance());
	}

	void Update();
	virtual bool HandleMessage(const Telegram& msg);
	StateMachine<Dish>* GetFSM()const { return m_pStateMachine; }
};

