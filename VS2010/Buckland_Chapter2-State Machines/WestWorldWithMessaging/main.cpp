#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Dish.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Chef* b = new Chef(eChef);

  //create his wife
  DishWasher* a = new DishWasher(eDishWasher);
  Dish* d = new Dish(eDish);

  //register them with the entity manager
  EntityMgr->RegisterEntity(b);
  EntityMgr->RegisterEntity(a);
  EntityMgr->RegisterEntity(d);

  //run b and a through a few Update calls
  for (int i=0; i<30; ++i)
  { 
    b->Update();
    a->Update();
	d->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete b;
  delete a;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






