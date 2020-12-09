#include "Goal_SeekToPosition.h"

#include "../Raven_Game.h"
#include "..\Raven_Bot.h"
#include "..\Raven_SteeringBehaviors.h"
#include "time/CrudeTimer.h"
#include "../navigation/Raven_PathPlanner.h"
#include "misc/cgdi.h"



#include "debug/DebugConsole.h"



//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
Goal_SeekToPosition::Goal_SeekToPosition(Raven_Bot* pBot,
                                         Vector2D   target):

                                Goal<Raven_Bot>(pBot,
                                                goal_seek_to_position),
                                 m_vPosition(target),
                                 m_dTimeToReachPos(0.0)
{}

                                             
//---------------------------- Activate -------------------------------------
//-----------------------------------------------------------------------------  
void Goal_SeekToPosition::Activate()
{
  m_iStatus = active;
  
  //record the time the bot starts this goal
  m_dStartTime = Clock->GetCurrentTime();
  m_dLastSeekTime = m_dStartTime - 5;
  
  //This value is used to determine if the bot becomes stuck 
  m_dTimeToReachPos = m_pOwner->CalculateTimeToReachPosition(m_vPosition);
  
  //factor in a margin of error for any reactive behavior
  const double MarginOfError = 1.0;

  m_dTimeToReachPos += MarginOfError;

  m_pOwner->GetSteering()->SeekOn();
}


//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_SeekToPosition::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();
    
  //test to see if the bot has become stuck
  if (isStuck())
  {
    m_iStatus = failed;
  }
  
  //test to see if the bot has reached the waypoint. If so terminate the goal
  else
  { 
    if (m_pOwner->isAtPosition(m_vPosition))
    {
      m_iStatus = completed;
    }
  }

	if(Clock->GetCurrentTime() - m_dLastSeekTime > 1.f){
	    m_dLastSeekTime = Clock->GetCurrentTime();
        m_vDirec = GetTargetApproximateDirection();
	    debug_con << "BOT " << m_pOwner->ID() << " IS Seek Without Brain..." << "";
	}
	m_pOwner->GetSteering()->SetTarget(m_pOwner->Pos() + m_vDirec * 100);
  return m_iStatus;
}

//--------------------------- isBotStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently active waypoint
//-----------------------------------------------------------------------------
bool Goal_SeekToPosition::isStuck()const
{  
  double TimeTaken = Clock->GetCurrentTime() - m_dStartTime;

  if (TimeTaken > m_dTimeToReachPos)
  {
    debug_con << "BOT " << m_pOwner->ID() << " IS STUCK!!" << "";

    return true;
  }

  return false;
}

Vector2D Goal_SeekToPosition::GetTargetApproximateDirection() {
    int playerClosestNode = m_pOwner->GetPathPlanner()->GetClosestNodeToPosition(m_pOwner->Pos());
    int TargetClosestNode = m_pOwner->GetPathPlanner()->GetClosestNodeToPosition(m_vPosition);
    auto& graph = m_pOwner->GetWorld()->GetMap()->GetNavGraph();
	
    double smallestCost = 100000;
    int targetNodeIdx = -1;
	
    typedef typename Raven_Map::NavGraph::EdgeType Edge;
    Raven_Map::NavGraph::ConstEdgeIterator ConstEdgeItr(graph, playerClosestNode);
    for (const Edge* pE = ConstEdgeItr.begin(); !ConstEdgeItr.end(); pE = ConstEdgeItr.next())
    {
        double HCost = Heuristic_Euclid::Calculate(graph, TargetClosestNode, pE->To());
        double GCost = pE->Cost();
        if (HCost + GCost < smallestCost) {
            smallestCost = HCost + GCost;
            targetNodeIdx = pE->To();
        }
    }
    auto pos = graph.GetNode(targetNodeIdx).Pos();
    auto direc = pos - m_pOwner->Pos();
    direc.Normalize();
    return direc;
}


//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_SeekToPosition::Terminate()
{
  m_pOwner->GetSteering()->SeekOff();
  m_pOwner->GetSteering()->ArriveOff();

  m_iStatus = completed;
}

//----------------------------- Render ----------------------------------------
//-----------------------------------------------------------------------------
void Goal_SeekToPosition::Render()
{
  if (m_iStatus == active)
  {
    gdi->GreenBrush();
    gdi->BlackPen();
    gdi->Circle(m_vPosition, 3);
  }

  else if (m_iStatus == inactive)
  {

   gdi->RedBrush();
   gdi->BlackPen();
   gdi->Circle(m_vPosition, 3);
  }
}

