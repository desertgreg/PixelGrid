#include "PGSnakeApp.h"
#include "PixelGrid.h"


static int wrap(int val) { if (val < 0) return 12; if (val>12) return 0; return val; }
static int random_pos() { return rand() % 13; }

PGSnakeApp::PGSnakeApp() :
	m_icon(0x00,0x38,0xc4,0x04,0x09,0x7d,0x82,0x7c)
{
}
		
void PGSnakeApp::start()
{
	m_snakeHead = 0;
	m_snakeSegs = 0;
	m_moveDelay = 50;
	m_moveCounter = m_moveDelay;
	m_speedupCounter = 200;
	m_direction = 0;
	for (int i=0; i<MAX_SEGMENTS; ++i)
	{
		m_snake[i].reset();
	}
	
	m_snakeHead = 0;
	m_snake[m_snakeHead].m_x = 6;
	m_snake[m_snakeHead].m_y = 6;
	m_snake[m_snakeHead].m_flags |= F_ALIVE;

	m_direction = 0;
	m_snakeSegs = 4;
	m_moveCounter = m_moveDelay;
	
	// create an apple
	m_apples[0].m_x = random_pos();
	m_apples[0].m_y = random_pos();
	m_apples[0].m_flags = F_ALIVE;

	m_apples[1].m_x = random_pos();
	m_apples[1].m_y = random_pos();
	m_apples[1].m_flags = F_ALIVE;
}

#define APPLE_COLOR PGCOLOR(14,1,1)
#define SNAKE_COLOR(i) PGCOLOR(5,12,5)


void PGSnakeApp::check_new_head_location(int x,int y)
{
	// does this collide with the snake?
	int seg = m_snakeHead;
	for (int i=0; i<m_snakeSegs; i++)
	{
		if (m_snake[seg].m_flags & F_ALIVE)
		{
			if ((m_snake[seg].m_x == x) && (m_snake[seg].m_y == y))
			{
				//DEAD!
				PixelGrid.playSound(TakeDamageSnd);
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



void PGSnakeApp::update()
{

	PixelGrid.clear();

	// draw the snake
	int seg = m_snakeHead;
	for (int i=0; i<m_snakeSegs; i++)
	{
		if (m_snake[seg].m_flags & F_ALIVE)
		{
			PixelGrid.setPixel(m_snake[seg].m_x,m_snake[seg].m_y,SNAKE_COLOR(i));
		}
		seg++;
		if (seg >= MAX_SEGMENTS) seg = 0;
	}

	// draw the apples
	for (int i=0; i<MAX_APPLES; i++)
	{
		if (m_apples[i].m_flags & F_ALIVE)
		{
			PixelGrid.setPixel(m_apples[i].m_x,m_apples[i].m_y,APPLE_COLOR);
		}
	}
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
		check_new_head_location(x,y);

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
	if (PixelGrid.isDown(PGButton::U)) { m_direction = 0; }
	if (PixelGrid.isDown(PGButton::R)) { m_direction = 1; }
	if (PixelGrid.isDown(PGButton::D)) { m_direction = 2; }
	if (PixelGrid.isDown(PGButton::L)) { m_direction = 3; }
}



PGBitmap8 & PGSnakeApp::getIcon()
{
	return m_icon;
}