#include "Goal_Hide.h"

#include "Raven_Game.h"
#include "Raven_Map.h"
#include "Raven_SensoryMemory.h"
#include "debug/DebugConsole.h"
#include "goals/Goal_MoveToPosition.h"
#include "goals/Goal_SeekToPosition.h"
#include "navigation/Raven_PathPlanner.h"
#include "time/CrudeTimer.h"

void Goal_Hide::Activate() {
	m_iStatus = active;

	//record the time the bot starts this goal
	m_dStartTime = Clock->GetCurrentTime();

	//This value is used to determine if the bot becomes stuck 
	m_dTimeToReachPos = m_pOwner->CalculateTimeToReachPosition(m_vPosition) + 3;

	RemoveAllSubgoals();
	
	auto& targetBots = m_pOwner->GetSensoryMem()->GetHitbots();
	auto map = m_pOwner->GetWorld()->GetMap();
	if (!m_bDestinationIsSet)
	{
		// 적이 갈수없는 위치라면
		while (true) {
			for (int i = 0; i < targetBots.size(); i++)
			{
				if (!m_pOwner->canWalkBetween(targetBots[i]->Pos(), m_vPosition)) {
					// 위치로 이동
					m_vPosition = targetBots[i]->Pos();
					break;
				}
				m_vPosition = map->GetRandomNodeLocation();
			}
		}
	}
	m_pOwner->GetPathPlanner()->RequestPathToPosition(m_vPosition);
	AddSubgoal(new Goal_SeekToPosition(m_pOwner, m_vPosition));
}

int Goal_Hide::Process() {
	ActivateIfInactive();

	//test to see if the bot has become stuck
	if (isStuck())
	{
		m_iStatus = failed;
	}

	//test to see if the bot has reached the waypoint. If so terminate the goal
	else
	{
		//process the subgoals
		m_iStatus = ProcessSubgoals();
	}
	return m_iStatus;
}

//--------------------------- isBotStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently active waypoint
//-----------------------------------------------------------------------------
bool Goal_Hide::isStuck()const {
	double TimeTaken = Clock->GetCurrentTime() - m_dStartTime;

	if (TimeTaken > m_dTimeToReachPos)
	{
		debug_con << "BOT " << m_pOwner->ID() << " IS STUCK!!" << "";

		return true;
	}

	return false;
}
