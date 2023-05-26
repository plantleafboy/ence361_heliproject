//*****************************************************************************
//
// functions for handling and setting the helicopters current state
//
//*****************************************************************************
#include "helistate.h"

flight_states_t current_state;

flight_states_t get_state(void)
{
    //returns the helicopters current state
    return current_state;
}

void set_state(flight_states_t state)
{
    //sets the helicopters current state. Takes a flight_states_t type argument`
    current_state = state;
}
