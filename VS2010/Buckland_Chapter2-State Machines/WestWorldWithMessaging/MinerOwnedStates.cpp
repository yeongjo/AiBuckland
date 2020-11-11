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
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� �Ѿ���";

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

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�����ؾ��ϴµ� ���Z ��Դ�";
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
		<< "������ �����ؾ���...";
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
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�� ������ �¶��� ���Ͷ�";

		pMiner->ChangeLocation(playingGame);
	}
}


void PlayGame::Execute(Chef* pMiner) {
	//deposit the gold
	pMiner->IncreaseFatigue();

	pMiner->SetGoldCarried(0);

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
		<< "���� ��Գ� ���� ���� ������?: " << pMiner->RemainHomeworkDates();

	//wealthy enough to have a well earned rest?
	if (pMiner->RemainHomeworkDates() > ComfortRemainHomeworkDates) {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "���� �ð� ���� ���ҳ� ����";

		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else {
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
			<< "���� ���ϱ����� ���� �ؾ���";
		pMiner->GetFSM()->ChangeState(ComputerStudy::Instance());
	}
}


void PlayGame::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� ��վ��� ���� �ȳ�";
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
		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�İ��ھ���";

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
			<< "�� ���, �� �����Ϸ� ������";

		pMiner->GetFSM()->ChangeState(ComputerStudy::Instance());
	}

	else {
		//sleep
		pMiner->DecreaseFatigue();

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�帣����... ";
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
			<< ": ���� �޴� ����?";

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

		cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�� ����µ� ���� �ؾ���";
	}
}

void MakeFood::Execute(Chef* pMiner) {
	pMiner->Eat();

	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "�� ����� ����� �ؾ���";
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		eDish,            //ID of recipient
		Msg_FinishEat,   //the message
		NO_ADDITIONAL_INFO);
	pMiner->GetFSM()->ChangeState(EatFood::Instance());
}


void MakeFood::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "���� �غ�Ϸ�";
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
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "(�Ա���)���ְڳ�";
}

void EatFood::Execute(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "(�Դ���)�ұݳѸ��̳־���";
	pMiner->Eat();
	pMiner->GetFSM()->ChangeState(ComputerStudy::Instance());;

}

void EatFood::Exit(Chef* pMiner) {
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "��� �ı� ��ô�� �Ẽ��";
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


