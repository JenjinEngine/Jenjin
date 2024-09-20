#ifndef JENJIN_EDITOR_STATE_H
#define JENJIN_EDITOR_STATE_H

class State_t {
public:
	bool running = false; // Marks when the game is running continuously
	bool stepping = false; // Marks when the game is running and should pause after one frame
	bool live = false; // Marks when the game is running and edits should be temporary
} extern State;

#endif // JENJIN_EDITOR_STATE_H
