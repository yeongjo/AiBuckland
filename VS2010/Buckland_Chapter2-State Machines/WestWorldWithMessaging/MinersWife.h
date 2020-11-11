#ifndef MINERSWIFE_H
#define MINERSWIFE_H
//------------------------------------------------------------------------
//
//  Name: DishWasher.h
//
//  Desc: class to implement Chef Bob's wife.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinersWifeOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "Miner.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"



class DishWasher : public BaseGameEntity
{
private:

  //an instance of the state machine class
  StateMachine<DishWasher>* m_pStateMachine;

  location_type   m_Location;

  //is she presently cooking?
  bool            washing;


public:

  DishWasher(int id):m_Location(shack),
                     washing(false),
                     BaseGameEntity(id)
                                        
  {
    //set up the state machine
    m_pStateMachine = new StateMachine<DishWasher>(this);

    m_pStateMachine->SetCurrentState(DoHouseWork::Instance());

    m_pStateMachine->SetGlobalState(AState::Instance());
  }

  ~DishWasher(){delete m_pStateMachine;}


  //this must be implemented
  void          Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  StateMachine<DishWasher>* GetFSM()const{return m_pStateMachine;}

  //----------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc){m_Location=loc;}

  bool          Washing()const{return washing;}
  void          SetCooking(bool val){washing = val;}
   
};

#endif
