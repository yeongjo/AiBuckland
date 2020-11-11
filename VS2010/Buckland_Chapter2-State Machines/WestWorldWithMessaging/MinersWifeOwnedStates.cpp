#include "MinersWifeOwnedStates.h"
#include "MinerOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//-----------------------------------------------------------------------Global state

AState* AState::Instance()
{
  static AState instance;

  return &instance;
}


void AState::Execute(DishWasher* wife)
{
  //1 in 10 chance of needing the bathroom (provided she is not already
  //in the bathroom)
  if ( (RandFloat() < 0.1) && 
       !wife->GetFSM()->isInState(*VisitBathroom::Instance()) )
  {
    wife->GetFSM()->ChangeState(VisitBathroom::Instance());
  }
}

bool AState::OnMessage(DishWasher* wife, const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  switch(msg.Msg)
  {
  case Msg_FinishEat:
   {
       cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: " 
       << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(wife->ID()) << 
          ": 네 주인님 접시 닦을 시간이네요";

     wife->GetFSM()->ChangeState(WashDishes::Instance());
   }

   return true;

  }//end switch

  return false;
}

//-------------------------------------------------------------------------DoHouseWork

DoHouseWork* DoHouseWork::Instance()
{
  static DoHouseWork instance;

  return &instance;
}


void DoHouseWork::Enter(DishWasher* wife)
{
  cout << "\n" << GetNameOfEntity(wife->ID()) << ": 대기 시간이네";
}


void DoHouseWork::Execute(DishWasher* wife)
{
  switch(RandInt(0,2))
  {
  case 0:

    cout << "\n" << GetNameOfEntity(wife->ID()) << ": 습기 제거중..";

    break;

  case 1:

    cout << "\n" << GetNameOfEntity(wife->ID()) << ": 고장나는중..";

    break;

  case 2:

    cout << "\n" << GetNameOfEntity(wife->ID()) << ": 가만히 있는중..";

    break;
  }
}

void DoHouseWork::Exit(DishWasher* wife)
{
}

bool DoHouseWork::OnMessage(DishWasher* wife, const Telegram& msg)
{
  return false;
}

//------------------------------------------------------------------------VisitBathroom

VisitBathroom* VisitBathroom::Instance()
{
  static VisitBathroom instance;

  return &instance;
}


void VisitBathroom::Enter(DishWasher* wife)
{  
  cout << "\n" << GetNameOfEntity(wife->ID()) << ": Walkin' to the can. Need to powda mah pretty li'lle nose"; 
}


void VisitBathroom::Execute(DishWasher* wife)
{
  cout << "\n" << GetNameOfEntity(wife->ID()) << ": Ahhhhhh! Sweet relief!";

  wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(DishWasher* wife)
{
  cout << "\n" << GetNameOfEntity(wife->ID()) << ": Leavin' the Jon";
}


bool VisitBathroom::OnMessage(DishWasher* wife, const Telegram& msg)
{
  return false;
}


//------------------------------------------------------------------------WashDishes

WashDishes* WashDishes::Instance()
{
  static WashDishes instance;

  return &instance;
}


void WashDishes::Enter(DishWasher* wife)
{
  //if not already cooking put the stew in the oven
  if (!wife->Washing())
  {
    cout << "\n" << GetNameOfEntity(wife->ID()) << ": 접시를 넣어요";
  
    //send a delayed message myself so that I know when to take the stew
    //out of the oven
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,                  //time delay
                              wife->ID(),           //sender ID
                              wife->ID(),           //receiver ID
                              Msg_CleanDishes,        //msg
                              NO_ADDITIONAL_INFO); 

    wife->SetCooking(true);
  }
}


void WashDishes::Execute(DishWasher* wife)
{
  cout << "\n" << GetNameOfEntity(wife->ID()) << ": 접시 닦는중";
}

void WashDishes::Exit(DishWasher* wife)
{
  SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  
  cout << "\n" << GetNameOfEntity(wife->ID()) << ": 접시 청소끝";
}


bool WashDishes::OnMessage(DishWasher* wife, const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  switch(msg.Msg)
  {
    case Msg_CleanDishes:
    {
      cout << "\nMessage received by " << GetNameOfEntity(wife->ID()) <<
           " at time: " << Clock->GetCurrentTime();

      SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
      cout << "\n" << GetNameOfEntity(wife->ID()) << ": 삐삒!!! 접시 청소 종료!!!!!!!!";
	  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,                  //time delay
		  wife->ID(),           //sender ID
		  eDish,           //receiver ID
		  Msg_CleanDishes,        //msg
		  NO_ADDITIONAL_INFO);

      //let hubby know the stew is ready
      //Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
      //                          wife->ID(),
      //                          eChef,
      //                          Msg_CleanDishes,
      //                          NO_ADDITIONAL_INFO);

      wife->SetCooking(false);

      wife->GetFSM()->ChangeState(DoHouseWork::Instance());               
    }

    return true;

  }//end switch

  return false;
}