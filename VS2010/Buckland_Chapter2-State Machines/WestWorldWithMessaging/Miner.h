#ifndef MINER_H
#define MINER_H
//------------------------------------------------------------------------
//
//  Name:   Chef.h
//
//  Desc:   DishWasher class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "MinerOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//the amount of gold a miner must have before he feels he can go home
const int ComfortRemainHomeworkDates       = 1;
//the amount of nuggets a miner can carry
const int MaxNuggets         = 3;
//above this value a miner is thirsty
const int HungryLevel        = 5;
//above this value a miner is sleepy
const int TirednessThreshold = 5;



class Chef : public BaseGameEntity
{
private:

  //an instance of the state machine class
  StateMachine<Chef>*  m_pStateMachine;
  
  location_type         m_Location;

  //how many nuggets the miner has in his pockets
  int                   m_iGoldCarried;

  int                   remainHomework;

  //the higher the value, the thirstier the miner
  int                   hungry;

  //the higher the value, the more tired the miner
  int                   m_iFatigue;

public:

  Chef(int id):m_Location(shack),
                          m_iGoldCarried(0),
                          remainHomework(0),
                          hungry(0),
                          m_iFatigue(0),
                          BaseGameEntity(id)
                               
  {
    //set up state machine
    m_pStateMachine = new StateMachine<Chef>(this);
    
    m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested::Instance());

    /* NOTE, DishWasher GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
  }

  ~Chef(){delete m_pStateMachine;}

  //this must be implemented
  void Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  
  StateMachine<Chef>* GetFSM()const{return m_pStateMachine;}


  
  //-------------------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc){m_Location=loc;}
    
  int           GoldCarried()const{return m_iGoldCarried;}
  void          SetGoldCarried(int val){m_iGoldCarried = val;}
  void          AddToGoldCarried(int val);
  bool          PocketsFull()const{return m_iGoldCarried >= MaxNuggets;}

  bool          IsFatigued()const;
  void          DecreaseFatigue(){m_iFatigue -= 1;}
  void          IncreaseFatigue(){m_iFatigue += 1;}

  int           RemainHomeworkDates()const{return remainHomework;}
  void          AddHomeworkDate(int val){remainHomework = val;}
  void          DoRemainHomework(int val);

  bool          IsHungry()const; 
  void          Eat(){hungry = 0;}

};



#endif
