#pragma once
#include "Raven_Bot.h"
#include "Goals/Goal.h"
#include "Goals/Goal_Composite.h"
#include "goals/Raven_Goal_Types.h"

class Goal_Hide : public Goal_Composite<Raven_Bot> {
public:

    Goal_Hide(Raven_Bot* pOwner) :Goal_Composite<Raven_Bot>(pOwner, goal_hide) {
    	
    }

    void Activate();

    int  Process();

    void Terminate() { m_iStatus = completed; }

    Vector2D  m_vPosition;
};