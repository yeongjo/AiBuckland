#include "DodgeBulletGoal_Evaluator.h"
#include "../Raven_ObjectEnumerations.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../Raven_SensoryMemory.h"

//---------------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double DodgeBulletGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
    //pBot 사정거리에 2명 이상이 있고 그 2명의 타겟이 나일 경우 숫자 높아짐
    double ClosestDistSoFar = MaxDouble;
    double Desirability = 0;
    std::list<Raven_Bot*> SensedBots;    
    SensedBots = pBot->GetSensoryMem()->GetListOfRecentlySensedOpponents();
    std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
    for (curBot; curBot != SensedBots.end(); ++curBot)
    {   
        
        double dist = Vec2DDistanceSq((*curBot)->Pos(), pBot->Pos());
        if (dist < ClosestDistSoFar)
        {
            ClosestDistSoFar = dist;
            if (pBot == (*curBot)->GetTargetBot())
            {
                Desirability+= 0.3;
            }
        }
    }

    //ensure the value is in the range 0 to 1
    Clamp(Desirability, 0, 1);

    //bias the value according to the personality of the bot
    Desirability *= m_dCharacterBias;


    return Desirability;
}



//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void DodgeBulletGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
  pBot->GetBrain()->AddGoal_GetItem(type_health); 
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void DodgeBulletGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
  gdi->TextAtPos(Position, "Hide: " + ttos(CalculateDesirability(pBot), 2));
  return;
  
  std::string s = ttos(1-Raven_Feature::Health(pBot)) + ", " + ttos(Raven_Feature::DistanceToItem(pBot, type_health));
  gdi->TextAtPos(Position+Vector2D(0,15), s);
}