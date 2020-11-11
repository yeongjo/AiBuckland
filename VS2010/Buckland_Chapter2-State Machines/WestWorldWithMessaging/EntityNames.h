#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  eChef,

  eDishWasher,
  eDish
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case eChef:

    return "HouseChef";

  case eDishWasher:
    
    return "DishWasher";
  case eDish:
	  return "HappyDish";

  default:

    return "UNKNOWN!";
  }
}

#endif