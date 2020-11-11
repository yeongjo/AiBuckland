#ifndef MINERSWIFE_OWNED_STATES_H
#define MINERSWIFE_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   All the states that can be assigned to the DishWasher class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"

class DishWasher;



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class AState : public State<DishWasher>
{  
private:
  
  AState(){}

  //copy ctor and assignment should be private
  AState(const AState&);
  AState& operator=(const AState&);
 
public:

  //this is a singleton
  static AState* Instance();
  
  virtual void Enter(DishWasher* wife){}

  virtual void Execute(DishWasher* wife);

  virtual void Exit(DishWasher* wife){}

  virtual bool OnMessage(DishWasher* wife, const Telegram& msg);
};


//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class DoHouseWork : public State<DishWasher>
{
private:

  DoHouseWork(){}
  
  //copy ctor and assignment should be private
  DoHouseWork(const DoHouseWork&);
  DoHouseWork& operator=(const DoHouseWork&);

public:

  //this is a singleton
  static DoHouseWork* Instance();
  
  virtual void Enter(DishWasher* wife);

  virtual void Execute(DishWasher* wife);

  virtual void Exit(DishWasher* wife);
  
  virtual bool OnMessage(DishWasher* wife, const Telegram& msg);

};



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class VisitBathroom : public State<DishWasher>
{
private:
  
  VisitBathroom(){}

  //copy ctor and assignment should be private
  VisitBathroom(const VisitBathroom&);
  VisitBathroom& operator=(const VisitBathroom&);
 
public:

  //this is a singleton
  static VisitBathroom* Instance();
  
  virtual void Enter(DishWasher* wife);

  virtual void Execute(DishWasher* wife);

  virtual void Exit(DishWasher* wife);

  virtual bool OnMessage(DishWasher* wife, const Telegram& msg);

};


//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class WashDishes : public State<DishWasher>
{
private:
  
  WashDishes(){}

  //copy ctor and assignment should be private
  WashDishes(const WashDishes&);
  WashDishes& operator=(const WashDishes&);
 
public:

  //this is a singleton
  static WashDishes* Instance();
  
  virtual void Enter(DishWasher* wife);

  virtual void Execute(DishWasher* wife);

  virtual void Exit(DishWasher* wife);

  virtual bool OnMessage(DishWasher* wife, const Telegram& msg);
};


#endif