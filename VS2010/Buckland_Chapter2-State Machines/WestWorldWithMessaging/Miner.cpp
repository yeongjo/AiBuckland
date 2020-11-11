#include "Miner.h"

bool Chef::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void Chef::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);

  hungry += 1;
  
  m_pStateMachine->Update();
}



void Chef::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Chef::DoRemainHomework(const int val)
{
  remainHomework -= val;

  if (remainHomework < 0) remainHomework = 0;
}

bool Chef::IsHungry()const
{
  if (hungry >= HungryLevel){return true;}

  return false;
}

bool Chef::IsFatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}
