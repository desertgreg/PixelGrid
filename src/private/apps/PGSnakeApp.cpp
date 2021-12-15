#include "PGSnakeApp.h"
#include "PixelGrid.h"


#define APPLE_COLOR PGCOLOR(200,30,30)
#define SNAKE_HEAD_COLOR PGCOLOR(100,190,90)
#define SNAKE_COLOR PGCOLOR(32,170,20)

#define WALLS_APPEAR 8
#define FLOOR_APPEAR 16

static int wrap(int val) { if (val < 0) return 12; if (val>12) return 0; return val; }
static int random_pos() { return rand() % 11 + 1; }
static pgcolor snakeColor(int i)
{
	if (i==0) return SNAKE_HEAD_COLOR;
	return SNAKE_COLOR;
}


PGSnakeApp::PGSnakeApp() 
{
}
		
void PGSnakeApp::start()
{
	init_starting();
}


void PGSnakeApp::checkNewHeadLocation(int x,int y)
{
	// does this collide with the wall?
	if (m_snakeSegs > WALLS_APPEAR)
	{
		if ((x==0) || (x==12) || (y==0)||(y==12))
		{
			init_lost();
			return;
		}
	}
	
	// does this collide with the snake?
	int seg = m_snakeHead;
	for (int i=0; i<m_snakeSegs; i++)
	{
		if (m_snake[seg].m_flags & F_ALIVE)
		{
			if ((m_snake[seg].m_x == x) && (m_snake[seg].m_y == y))
			{
				init_lost();
				return;
			}
		}
		seg++;
		if (seg > MAX_SEGMENTS) seg = 0;
	}

	// does this collide with an apple
	for (int i=0; i<MAX_APPLES; i++)
	{
		if (m_apples[i].m_flags & F_ALIVE)
		{
			if ((m_apples[i].m_x == x) && (m_apples[i].m_y == y))
			{
				PixelGrid.playSound(PickupCoinSnd);
				if (m_snakeSegs < MAX_SEGMENTS-1) m_snakeSegs++;

				// spawn new apple
				m_apples[i].m_x = random_pos();
				m_apples[i].m_y = random_pos();
				m_apples[i].m_flags = F_ALIVE;
			}
		}
	}
}

void PGSnakeApp::drawSnake()
{
	// draw the snake
	int seg = m_snakeHead;
	for (int i=0; i<m_snakeSegs; i++)
	{
		if (m_snake[seg].m_flags & F_ALIVE)
		{
			PixelGrid.setPixel(m_snake[seg].m_x,m_snake[seg].m_y,snakeColor(i));
		}
		seg++;
		if (seg >= MAX_SEGMENTS) seg = 0;
	}
}

void PGSnakeApp::drawApples()
{
	// draw the apples
	for (int i=0; i<MAX_APPLES; i++)
	{
		if (m_apples[i].m_flags & F_ALIVE)
		{
			PixelGrid.setPixel(m_apples[i].m_x,m_apples[i].m_y,APPLE_COLOR);
		}
	}
}

void PGSnakeApp::drawWalls()
{
	if (m_snakeSegs<= WALLS_APPEAR) return;
	static float t = 0.0f;
	t+=0.05f;
	int offset = (int)(30.0f*sin(t));
	PixelGrid.drawBox(0,0,12,12,PGCOLOR(80,30+offset,30-offset));  //make the wall change colors for fun
}

void PGSnakeApp::drawFloor()
{
	if (m_snakeSegs < FLOOR_APPEAR) return;
	pgcolor c0 = PGCOLOR(32,32,32);
	pgcolor c1 = PGCOLOR(17,15,32);
	static float offset_t = 0.0;
	offset_t += 0.02;
	float radius = 10.0f;
	int x_offset = (int)(radius*sin(offset_t) + radius);
	int y_offset = (int)(radius*cos(offset_t) + radius);
	
	for (int j=0; j<13; ++j)
	{
		int sy = (j+y_offset)/3;
		for (int i=0; i<13;++i)
		{
			int sx = (i+x_offset)/3;
			pgcolor c = c0;
			if (sx + sy & 0x01)
			{
				c = c1;
			}
			PixelGrid.setPixel(i,j,c);
		}
	}
}
bool PGSnakeApp::check_buttons_and_update_direction()
{
	if (PixelGrid.isDown(PGButton::U)) { m_direction = 0; return true;}
	else if (PixelGrid.isDown(PGButton::R)) { m_direction = 1; return true;}
	else if (PixelGrid.isDown(PGButton::D)) { m_direction = 2; return true;}
	else if (PixelGrid.isDown(PGButton::L)) { m_direction = 3; return true;}
	return false;
}

void PGSnakeApp::init_starting()
{
	m_stateCounter = 0;
	m_state = STATE_STARTING;
	m_moveDelay = 50;
	m_moveCounter = m_moveDelay;
	m_speedupCounter = 200;
	m_direction = 0;

	for (int i=0; i<MAX_SEGMENTS; ++i)
	{
		m_snake[i].reset();
	}

	// put the snake in the center of the screen with a couple segments
	m_snakeHead = 0;
	m_snake[0].m_x = 6;
	m_snake[0].m_y = 6;
	m_snake[0].m_flags |= F_ALIVE;
	
	m_snake[1].m_x = 6;
	m_snake[1].m_y = 7;
	m_snake[1].m_flags |= F_ALIVE;

	m_snakeSegs = 2;
}


void PGSnakeApp::update_starting()
{
	drawSnake();
		
	bool keypress = false;
	keypress = check_buttons_and_update_direction();
	if (keypress)
	{
		init_playing();
	}
}

void PGSnakeApp::init_playing()
{
	m_stateCounter = 0;
	m_state = STATE_PLAYING;
	// create an apple
	m_apples[0].m_x = random_pos();
	m_apples[0].m_y = random_pos();
	m_apples[0].m_flags = F_ALIVE;

	m_apples[1].m_x = random_pos();
	m_apples[1].m_y = random_pos();
	m_apples[1].m_flags = F_ALIVE;
}

void PGSnakeApp::update_playing()
{
	PixelGrid.clear();
	
	drawFloor();
	drawSnake();
	drawApples();
	drawWalls();
	
	// speedup
	m_speedupCounter--;
	if (m_speedupCounter <= 0)
	{
		if (m_moveDelay > 15) m_moveDelay--;
		m_speedupCounter = SPEEDUP_COUNT;
	}
	
	// if its time to move the head, 
	m_moveCounter--;
	if (m_moveCounter <= 0)
	{
		PixelGrid.playSound(MenuMoveSnd);
		m_moveCounter = m_moveDelay;

		int x = m_snake[m_snakeHead].m_x + m_moveX[m_direction];
		int y = m_snake[m_snakeHead].m_y + m_moveY[m_direction];
		x = wrap(x);
		y = wrap(y);

		// game logic for moving into new location
		checkNewHeadLocation(x,y);

		m_snakeHead--;
		if (m_snakeHead < 0) m_snakeHead = MAX_SEGMENTS-1;
		m_snake[m_snakeHead].m_x = x;
		m_snake[m_snakeHead].m_y = y;
		m_snake[m_snakeHead].m_flags |= F_ALIVE;

		// erase the tail segment
		int tail = m_snakeHead + m_snakeSegs;
		if (tail >= MAX_SEGMENTS)
		{
			tail = tail - MAX_SEGMENTS;
		}
		m_snake[tail].m_flags &= ~F_ALIVE;
	}

	// update direction based on inputs
	check_buttons_and_update_direction();
}

void PGSnakeApp::init_lost()
{
	m_state = STATE_LOST;
	m_stateCounter = 0;
	PixelGrid.playSound(TakeDamageSnd);
	m_blowUpCounter = 130;
	m_score = 0;
}

void PGSnakeApp::update_lost()
{
	// slowly blow up the snake segments, then go to 'waiting for start'?
	if (m_stateCounter & 0x10)
	{
		drawSnake();
	}
	drawWalls();
	//drawApples();
	
	m_blowUpCounter--;
	if(m_blowUpCounter < 0)
	{
		m_blowUpCounter = 10;
		if (m_snakeSegs > 0)
		{
			m_score++;
			m_snakeSegs--;
			PixelGrid.playSound(TakeDamageSnd);
		}
		else
		{
			if (PixelGrid.anyPressed())
			{
				init_starting();
			}
		}
	}
	int tmp = m_score;
	int h = tmp / 100;
	tmp = tmp - h*100;
	int t = tmp / 10;
	tmp = tmp - t*10;
	int o = tmp;
	PixelGrid.setBlendMode(PGBlendMode::ALPHA);
	PixelGrid.drawDigit(1,3,h);
	PixelGrid.drawDigit(5,3,t);
	PixelGrid.drawDigit(9,3,o);
	PixelGrid.resetRenderStates();
}

void PGSnakeApp::update()
{
	PixelGrid.clear();
	switch (m_state)
	{
		case STATE_STARTING:
			update_starting();
			break;
		case STATE_PLAYING:
			update_playing();
			break;
		case STATE_LOST:
			update_lost();
			break;
	}
	m_stateCounter++;
}



PGImage & PGSnakeApp::getIcon()
{
	return IconSnakeImg;
}