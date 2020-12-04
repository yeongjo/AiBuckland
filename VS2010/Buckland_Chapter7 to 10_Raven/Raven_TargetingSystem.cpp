#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"
#include "debug/DebugConsole.h"



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
  m_pCurrentTarget       = 0;  
  //grab a list of all the opponents the owner can sense
  std::list<Raven_Bot*> SensedBots;
  std::vector< Raven_Bot*> HitBots;
  bool isCertainTarget = false;
  SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();
  HitBots = m_pOwner->GetSensoryMem()->GetHitbots();

  std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
  for (curBot; curBot != SensedBots.end(); ++curBot)
  {
    //make sure the bot is alive and that it is not the owner
    if ((*curBot)->isAlive() && (*curBot != m_pOwner) )
    {
      double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());
      
      //나를 쏘고있는가?, 나를 바라보고있는가?, 내가 쐈던 봇인가?, 가까운가, 적의 사거리가 나보다 짧은가,
      if (dist < ClosestDistSoFar)
      {
          for (int i = 0; i < HitBots.size(); i++)
          {
              if (HitBots[i] == *curBot)
              {
                  debug_con << m_pOwner ->ID()<<": 맞췄던놈"<< HitBots[i]->ID() <<" 부터 공격해야지" << "";
                  ClosestDistSoFar = dist;
                  m_pCurrentTarget = *curBot;
                  isCertainTarget = true;
              }
          }
          if (!isCertainTarget) {
              debug_con << m_pOwner->ID() << "아무나쏴야지" << "";
              ClosestDistSoFar = dist;
              m_pCurrentTarget = *curBot;
          }          
          //Vector2D toTarget = Vec2DNormalize((*curBot)->Pos() - m_pOwner->Pos());
          //double dot = (*curBot)->Facing().Dot(toTarget);
          ////1에 가까울수록 나랑 마주보고있음
          ////TODO 타겟팅 시스템          
          //debug_con << m_pOwner->ID()<<": "<<(*curBot)->ID()<<" = " <<dot<<"";
          //if (dot > 0) 
          //{
          //    ClosestDistSoFar = dist;
          //    m_pCurrentTarget = *curBot;
          //}          
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
