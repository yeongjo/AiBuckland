#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_FinishEat,
  Msg_CleanDishes,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_FinishEat:
    
    return "FinishEat"; 

  case Msg_CleanDishes:
    
    return "CleanDishes";

  default:

    return "Not recognized!";
  }
}

#endif