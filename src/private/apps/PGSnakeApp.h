#include "PixelGrid.h"
#ifndef PGSNAKEAPP_H
#define PGSNAKEAPP_H

#include "PixelGridTypes.h"



class PGSnakeApp : public PGApp
{
public:
	PGSnakeApp();
	void start() override;
	void update() override;
	PGImage & getIcon() override;
	
protected:

	void checkNewHeadLocation(int x,int y);
	void drawFloor();
	void drawSnake();
	void drawApples();
	void drawWalls();
	bool check_buttons_and_update_direction();
	
	void init_starting();
	void update_starting();
	
	void init_playing();
	void update_playing();
	
	void init_lost();
	void update_lost();

	enum SNAKE_FLAGS
	{
	  F_ALIVE = 0x01
	};

	struct SnakeSegStruct
	{
		SnakeSegStruct() { reset(); }
		void reset() { m_x = 6; m_y = 6; m_flags = 0; }

		int8_t m_x;
		int8_t m_y;
		int8_t m_flags;
	};

	struct AppleStruct
	{
		int8_t m_x;
		int8_t m_y;
		int8_t m_flags;
	};


	static const int MAX_SEGMENTS = 256;
	SnakeSegStruct m_snake[MAX_SEGMENTS];
	
	static const int MAX_APPLES = 4;
	AppleStruct m_apples[MAX_APPLES];

	static const int SPEEDUP_COUNT = 200;
	
	int m_snakeHead = 0;
	int m_snakeSegs = 0;
	int m_moveDelay = 50;
	int m_moveCounter = m_moveDelay;
	int m_speedupCounter = SPEEDUP_COUNT;
	int m_direction = 0;
	const int m_moveX[4] = { 0,1,0,-1 };
	const int m_moveY[4] = { -1,0,1,0 };
	PGBitmap8 m_icon;
	int m_score = 0;
	
	enum
	{
		STATE_STARTING = 0,	// apples form, snake forms, wait for keypress
		STATE_PLAYING,		// play snake until you hit a wall or yourself
		STATE_LOST,			// blink snake, explode parts, hide apples, go to starting
	};
	int m_state;
	int m_stateCounter;
	int m_blowUpCounter;
};


#endif //PGSNAKEAPP_H
