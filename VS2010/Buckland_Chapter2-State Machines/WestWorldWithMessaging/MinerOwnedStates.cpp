#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for ComputerStudy
ComputerStudy* ComputerStudy::Instance() {
	static ComputerStudy instance;

	return &instance;
}


void ComputerStudy::Enter(Chef* pMiner) {
	//if the miner is not already located at the computer, he must
	//change location to the gold mine
	if (pMiner->Location() != computer) {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "컴터 켜야지";

		pMiner->ChangeLocation(computer);
	}
}


void ComputerStudy::Execute(Chef* pMiner) {
	//Now the miner is at the computer he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the drinking for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "과제해야하는데 유튭 재밋당";
	pMiner->DoRemainHomework(1);

	//if enough gold mined, go and put it in the playingGame
	if (pMiner->PocketsFull()) {
		pMiner->GetFSM()->ChangeState(PlayGame::Instance());
	}

	if (pMiner->IsHungry()) {
		pMiner->GetFSM()->ChangeState(MakeFood::Instance());
	}
}


void ComputerStudy::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "과제는 내일해야지...";
}


bool ComputerStudy::OnMessage(Chef* pMiner, const Telegram& msg) {
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for PlayGame

PlayGame* PlayGame::Instance() {
	static PlayGame instance;

	return &instance;
}

void PlayGame::Enter(Chef* pMiner) {
	//on entry the miner makes sure he is located at the playingGame
	if (pMiner->Location() != playingGame) {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "야 영웅문 온라인 들어와라";

		pMiner->ChangeLocation(playingGame);
	}
}


void PlayGame::Execute(Chef* pMiner) {
	//deposit the gold
	pMiner->IncreaseFatigue();

	pMiner->SetGoldCarried(0);

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "게임 재밋넹 과제 몇일 남았지?: " << pMiner->RemainHomeworkDates();

	//wealthy enough to have a well earned rest?
	if (pMiner->RemainHomeworkDates() > ComfortRemainHomeworkDates) {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "아직 시간 많이 남았넹 ㅎㅎ";

		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "과제 내일까지네 빨리 해야지";
		pMiner->GetFSM()->ChangeState(ComputerStudy::Instance());
	}
}


void PlayGame::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "게임 재밌었다 얘들아 안녕";
}


bool PlayGame::OnMessage(Chef* pMiner, const Telegram& msg) {
	//send msg to global message handler
	return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance() {
	static GoHomeAndSleepTilRested instance;

	return &instance;
}

void GoHomeAndSleepTilRested::Enter(Chef* pMiner) {
	if (pMiner->Location() != shack) {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "씻고자야지";

		pMiner->ChangeLocation(shack);

		//let the wife know I'm home
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			eDishWasher,            //ID of recipient
			Msg_FinishEat,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Chef* pMiner) {
	//if miner is not fatigued start to dig for nuggets again.
	if (!pMiner->IsFatigued()) {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "다 잤네, 또 컴터하러 가볼까";

		pMiner->GetFSM()->ChangeState(ComputerStudy::Instance());
	}

	else {
		//sleep
		pMiner->DecreaseFatigue();

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "드르렁쿨... ";
	}
}

void GoHomeAndSleepTilRested::Exit(Chef* pMiner) {
}


bool GoHomeAndSleepTilRested::OnMessage(Chef* pMiner, const Telegram& msg) {
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg) {
	case Msg_CleanDishes:

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pMiner->ID())
			<< ": 오늘 메뉴 뭐임?";

		pMiner->GetFSM()->ChangeState(EatFood::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------------------------------MakeFood

MakeFood* MakeFood::Instance() {
	static MakeFood instance;

	return &instance;
}

void MakeFood::Enter(Chef* pMiner) {
	if (pMiner->Location() != drinking) {
		pMiner->ChangeLocation(drinking);

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "넘 배고픈데 맘마 해야지";
	}
}

void MakeFood::Execute(Chef* pMiner) {
	pMiner->Eat();

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "또 밥없네 밥부터 해야지";
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		eDish,            //ID of recipient
		Msg_FinishEat,   //the message
		NO_ADDITIONAL_INFO);
	pMiner->GetFSM()->ChangeState(EatFood::Instance());
}


void MakeFood::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "맘마 준비완료";
}


bool MakeFood::OnMessage(Chef* pMiner, const Telegram& msg) {
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------EatFood

EatFood* EatFood::Instance() {
	static EatFood instance;

	return &instance;
}


void EatFood::Enter(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "(먹기전)맛있겠넹";
}

void EatFood::Execute(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "(먹는중)소금넘많이넣었다";
	pMiner->Eat();
	pMiner->GetFSM()->ChangeState(ComputerStudy::Instance());;

}

void EatFood::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "비싼 식기 세척기 써볼까";
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		eDishWasher,            //ID of recipient
		Msg_FinishEat,   //the message
		NO_ADDITIONAL_INFO);
}


bool EatFood::OnMessage(Chef* pMiner, const Telegram& msg) {
	//send msg to global message handler
	return false;
}


