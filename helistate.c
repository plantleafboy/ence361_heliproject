//*****************************************************************************
//
// functions to support flight states of the code
//
//*****************************************************************************
#include "helistate.h"

flight_states_t current_state;

flight_states_t get_state(void)
{
    return current_state;
}

void set_state(flight_states_t state)
{
    current_state = state;
}
