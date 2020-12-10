#include "HideFromBotGoal_Evaluator.h"
#include "../Raven_ObjectEnumerations.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "misc/Stream_Utility_Functions.h"
#include "Raven_Feature.h"
#include "../Raven_SensoryMemory.h"

//---------------------- CalculateDesirability -------------------------------------
//-----------------------------------------------------------------------------
double HideFromBotGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
    //pBot �����Ÿ��� 2�� �̻��� �ְ� �� 2���� Ÿ���� ���� ��� ���� ������
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
void HideFromBotGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
  pBot->GetBrain()->AddGoal_Hide();
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void HideFromBotGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
  gdi->TextAtPos(Position, "Hide: " + ttos(CalculateDesirability(pBot), 2));  
}