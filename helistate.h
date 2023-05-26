enum flight_states{LANDED, TAKING_OFF, FLYING, LANDING, OFF,ON};
typedef enum flight_states flight_states_t;


flight_states_t get_state(void);

void set_state(flight_states_t state);

