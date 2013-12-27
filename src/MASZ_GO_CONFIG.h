enum {
	PURSUIT_target_visible,
	PURSUIT_target_not_visible,
	PURSUIT_radar,
	ESCAPE_turn,
	ESCAPE_run_away
};

#define LSPEED_DEF 230
#define RSPEED_DEF 180
#define VISIBILITY_THRESHOLD 170

#define DELAY_PER_TICK_MS 40

#define REMOTE_DISABLED_TICKS 7
#define INVISIBILITY_PATIENCE_TICKS 25
#define RADAR_TO_DO_TICKS 16
#define RADAR_DISABLED_TICKS 50

#define TURN_PHASE_COUNT 5
const int turn_in_place_durations[TURN_PHASE_COUNT] = { 5, 2, 7, 2, 5 };
typedef void (*action)(void);
const action turn_in_place_actions[TURN_PHASE_COUNT] = { Cofaj, Stop, Prawo,
		Stop, Jedz };

