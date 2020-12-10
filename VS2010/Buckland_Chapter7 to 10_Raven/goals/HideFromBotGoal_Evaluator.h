#ifndef RAVEN_DODGE_EVALUATOR
#define RAVEN_DODGE_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GetHealthGoal_Evaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to calculate how desirable the goal of fetching a health item
//          is
//-----------------------------------------------------------------------------

#include "Goal_Evaluator.h"
#include "../Raven_Bot.h"

class HideFromBotGoal_Evaluator : public Goal_Evaluator
{
public:

    HideFromBotGoal_Evaluator(double bias) :Goal_Evaluator(bias) {}

    double CalculateDesirability(Raven_Bot* pBot);

    void  SetGoal(Raven_Bot* pEnt);

    void RenderInfo(Vector2D Position, Raven_Bot* pBot);
};

#endif
    
