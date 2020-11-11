#ifndef MINER_OWNED_STATES_H
#define MINER_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Chef class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class Chef;
struct Telegram;




//------------------------------------------------------------------------
//
//  In this state the miner will walk to a computer and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to PlayGame. If he gets thirsty he'll change state
//  to MakeFood
//------------------------------------------------------------------------
class ComputerStudy : public State<Chef>
{
private:
  
  ComputerStudy(){}

  //copy ctor and assignment should be private
  ComputerStudy(const ComputerStudy&);
  ComputerStudy& operator=(const ComputerStudy&);
 
public:

  //this is a singleton
  static ComputerStudy* Instance();

  virtual void Enter(Chef* miner);

  virtual void Execute(Chef* miner);

  virtual void Exit(Chef* miner);

  virtual bool OnMessage(Chef* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Entity will go to a playingGame and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
class PlayGame : public State<Chef>
{
private:
  
  PlayGame(){}

  //copy ctor and assignment should be private
  PlayGame(const PlayGame&);
  PlayGame& operator=(const PlayGame&);
 
public:

  //this is a singleton
  static PlayGame* Instance();

  virtual void Enter(Chef* miner);

  virtual void Execute(Chef* miner);

  virtual void Exit(Chef* miner);

  virtual bool OnMessage(Chef* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class GoHomeAndSleepTilRested : public State<Chef>
{
private:
  
  GoHomeAndSleepTilRested(){}

  //copy ctor and assignment should be private
  GoHomeAndSleepTilRested(const GoHomeAndSleepTilRested&);
  GoHomeAndSleepTilRested& operator=(const GoHomeAndSleepTilRested&);
 
public:

  //this is a singleton
  static GoHomeAndSleepTilRested* Instance();

  virtual void Enter(Chef* miner);

  virtual void Execute(Chef* miner);

  virtual void Exit(Chef* miner);

  virtual bool OnMessage(Chef* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  miner changes location to the drinking and keeps buying Whiskey until
//  his thirst is quenched. When satisfied he returns to the computer
//  and resumes his quest for nuggets.
//------------------------------------------------------------------------
class MakeFood : public State<Chef>
{
private:
  
  MakeFood(){}

  //copy ctor and assignment should be private
  MakeFood(const MakeFood&);
  MakeFood& operator=(const MakeFood&);
 
public:

  //this is a singleton
  static MakeFood* Instance();

  virtual void Enter(Chef* miner);

  virtual void Execute(Chef* miner);

  virtual void Exit(Chef* miner);

  virtual bool OnMessage(Chef* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  this is implemented as a state blip. The miner eats the stew, gives
//  Elsa some compliments and then returns to his previous state
//------------------------------------------------------------------------
class EatFood : public State<Chef>
{
private:
  
  EatFood(){}

  //copy ctor and assignment should be private
  EatFood(const EatFood&);
  EatFood& operator=(const EatFood&);
 
public:

  //this is a singleton
  static EatFood* Instance();

  virtual void Enter(Chef* miner);

  virtual void Execute(Chef* miner);

  virtual void Exit(Chef* miner);

  virtual bool OnMessage(Chef* agent, const Telegram& msg);
};




#endif