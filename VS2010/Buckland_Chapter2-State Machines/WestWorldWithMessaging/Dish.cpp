#include "Dish.h"
#include <iostream>
#include "MessageTypes.h"
#include "MessageDispatcher.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

using namespace std;

ADishState* ADishState::Instance() {
	static ADishState instance;
	return &instance;
}


void ADishState::Execute(Dish* wife) {
	//1 in 10 chance of needing the bathroom (provided she is not already
	//in the bathroom)
	//if ((RandFloat() < 0.1) &&
	//	!wife->GetFSM()->isInState(*VisitBathroom::Instance())) {
	//	wife->GetFSM()->ChangeState(VisitBathroom::Instance());
	//}
}

bool ADishState::OnMessage(Dish* wife, const Telegram& msg) {
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: "
		<< Clock->GetCurrentTime();

	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	switch (msg.Msg) {
	case Msg_CleanDishes:
	{
		cout << "\n" << GetNameOfEntity(wife->ID()) <<
			": ¿Í ±ú²ýÇØÁø´Ù";

		wife->GetFSM()->ChangeState(NormalDish::Instance());
	}
	case Msg_FinishEat:
	{
		cout << "\n" << GetNameOfEntity(wife->ID()) <<
			": ¸ÔÀ»°Å¹¯À½";

		wife->GetFSM()->ChangeState(DirtDish::Instance());
	}

	return true;

	}//end switch

	return false;
}

void NormalDish::Execute(Dish * wife) {
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": ±ú²ýÁ¢½Ã";
}

void Dish::Update() {
	//set text color to green
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	m_pStateMachine->Update();
}

bool Dish::HandleMessage(const Telegram & msg) {
	return false;
}

void DirtDish::Execute(Dish * wife) {
	cout << "\n" << GetNameOfEntity(wife->ID()) << ": ´õ·¯¿îÁ¢½Ã";
}
