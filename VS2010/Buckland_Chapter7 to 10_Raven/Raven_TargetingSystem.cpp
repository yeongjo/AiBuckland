#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"
#include "Raven_Game.h"
#include "debug/DebugConsole.h"
#include <algorithm>


//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner):m_pOwner(owner),
                                                               m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------

//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update()
{
  double ClosestDistSoFar = MaxDouble;
  bool isCertainTarget = false;
  m_pCurrentTarget       = 0;  
  //grab a list of all the opponents the owner can sense
  std::list<Raven_Bot*> SensedBots;
  SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();
  std::vector< Raven_Bot*> HitBots;
  HitBots = m_pOwner->GetSensoryMem()->GetHitbots();
  std::sort(HitBots.begin(), HitBots.end(),
      [](Raven_Bot* a, Raven_Bot* b) { return a->Health() > b->Health(); });

  std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
  for (curBot; curBot != SensedBots.end(); ++curBot)
  {
    //make sure the bot is alive and that it is not the owner
    if ((*curBot)->isAlive() && (*curBot != m_pOwner) )
    {
      double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());

     /* if (m_pOwner->ID() == 401)
      {
          debug_con << m_pOwner->ID() << "{";
          for (int i = 0; i < HitBots.size(); i++){ debug_con << HitBots[i]->ID() << ", "; }
          debug_con << "}" << "";
      }         */

      //���� ����ִ°�?, ���� �ٶ󺸰��ִ°�?, ���� ���� ���ΰ�?, ����, ���� ��Ÿ��� ������ ª����,
      if (dist < ClosestDistSoFar)
      {          
          for (int i = 0; i < HitBots.size(); i++)
          {             
              if (HitBots[i] != m_pOwner && HitBots[i] == *curBot)
              {                  
                  ClosestDistSoFar = dist;
                  m_pCurrentTarget = *curBot;
                  isCertainTarget = true;
              }
          }        
          //Ÿ�پ��� �����Ÿ� ������ Ÿ��
          if (!isCertainTarget) {              
              ClosestDistSoFar = dist;
              m_pCurrentTarget = *curBot;
          }                   
      }
    }
  }
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
  return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
  return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
  return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}

std::vector<Raven_Bot*> Raven_TargetingSystem::GetTargetBots() const {
	std::vector<Raven_Bot*> hitmeBots;
	Raven_Game* m_world = m_pOwner->GetWorld();
	std::list<Raven_Bot*> allBots = m_world->GetAllBots();
	for (auto i = allBots.begin(); i != allBots.end(); ++i){
		if ((*i)->GetTargetBot() == m_pOwner){
			hitmeBots.push_back(*i);
		}
	}
	return hitmeBots;
}
