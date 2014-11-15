#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <sstream>

sf::RenderWindow window(sf::VideoMode(900, 500), "tPong!");
sf::Font font;

bool game(int toWin);
int menu();

void victory(int player);
void pointTo(sf::Text &text, int player);
bool inBounds(sf::RectangleShape entity, std::string dir);

std::string convertInt(int number)
{
   std::stringstream ss;
   ss << number;
   return ss.str();
}

int main()
{
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	font.loadFromFile("visitor.ttf");

    while(1 == 1)
    {
        switch(menu())
        {
        case 1:
            if(game(3))
            {
                victory(1);
            }
            else victory(2);

            break;
        case 0:
            return 0;
            break;
        default:
            return 0;
            break;
        }
    }

    return 0;
}

int menu()
{
    sf::Text title;
	title.setFont(font);
	title.setString("tPong!");
	title.setPosition(250,175);
	title.setColor(sf::Color::White);

	sf::Text name;
	name.setFont(font);
	name.setString("datMoka");
	name.setPosition(835,475);
	name.setScale(0.6,0.6);
	name.setColor(sf::Color::White);

	sf::Text version;
	version.setFont(font);
	version.setString("v1.0");
	version.setPosition(10,475);
	version.setScale(0.6,0.6);
	version.setColor(sf::Color::White);

	sf::RectangleShape ball(sf::Vector2f(6,6));
    ball.setFillColor(sf::Color::Green);
	ball.setPosition(235,196);

	sf::Text localCOOP;
	localCOOP.setFont(font);
	localCOOP.setString("Local Co-op");
	localCOOP.setPosition(250,300);
	localCOOP.setColor(sf::Color::White);

	sf::Text exit;
	exit.setFont(font);
	exit.setString("Exit game");
	exit.setPosition(250,325);
	exit.setColor(sf::Color::White);

	int menu = 0;

	bool running;

	while(running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
             if(menu != 0)
             {
                 menu -= 1;
             }
        }


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if(menu != 1)
            {
                menu += 1;
            }
        }

        if(menu == 0)
        {
            localCOOP.setColor(sf::Color::Red);
            exit.setColor(sf::Color::White);

            localCOOP.setString("* Local Co-op");
            exit.setString("Exit game");
        }
        else if(menu == 1)
        {
            localCOOP.setColor(sf::Color::White);
            exit.setColor(sf::Color::Red);

            localCOOP.setString("Local Co-op");
            exit.setString("* Exit game");
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if(menu == 0)
            {
                return 1;
            }
            else if(menu == 1)
            {
                return 0;
            }
        }

		window.clear();
		window.draw(title);
		window.draw(ball);
		window.draw(localCOOP);
		window.draw(exit);
		window.draw(name);
		window.draw(version);
		window.display();
    }
}

bool game(int toWin)
{
	sf::RectangleShape p1Paddle(sf::Vector2f(2,75));
    p1Paddle.setFillColor(sf::Color::Red);
	p1Paddle.setPosition(50,218);

	sf::RectangleShape p2Paddle(sf::Vector2f(2,75));
    p2Paddle.setFillColor(sf::Color::Blue);
	p2Paddle.setPosition(825,218);

	sf::RectangleShape ball(sf::Vector2f(12,12));
    ball.setFillColor(sf::Color::Green);
	ball.setPosition(438,150);

	sf::RectangleShape p1Goal(sf::Vector2f(80,500));
	p1Goal.setFillColor(sf::Color::Black);
	p1Goal.setPosition(833,0);

	sf::RectangleShape p2Goal(sf::Vector2f(44,500));
    p2Goal.setFillColor(sf::Color::Black);
	p2Goal.setPosition(0,0);

	float p1Speed = 0;
	float p2Speed = 0;
	sf::Vector2f ballVelocity(7.40,2.0);

	int p1Score = 0;
	int p2Score = 0;

	sf::Text p1ScoreDraw;
	sf::Text p2ScoreDraw;

	sf::Text awardPoint;

	p1ScoreDraw.setFont(font);
	p2ScoreDraw.setFont(font);
	awardPoint.setFont(font);

	awardPoint.setString("+1");

	p1ScoreDraw.setPosition(400,20);
	p2ScoreDraw.setPosition(500,20);
	awardPoint.setPosition(500,-20);

	p1ScoreDraw.setColor(sf::Color::White);
	p2ScoreDraw.setColor(sf::Color::White);
	awardPoint.setColor(sf::Color::Green);

	sf::FloatRect p1GoalBB = p1Goal.getGlobalBounds();
	sf::FloatRect p2GoalBB = p2Goal.getGlobalBounds();

	bool inPlay = true;

    while (inPlay)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


		sf::FloatRect ballBB = ball.getGlobalBounds();
		sf::FloatRect p1BB = p1Paddle.getGlobalBounds();
		sf::FloatRect p2BB = p2Paddle.getGlobalBounds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && inBounds(p1Paddle, "up"))
		{
			if(p1Speed > -5)
			{
				p1Speed -= 1;
			}
			else if(p1Speed <= -5)
			{
				//Do nothing
			}
		}
		else
		{
			if(p1Speed < 0)
			{
				p1Speed += 1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && inBounds(p1Paddle, "down"))
		{
			if(p1Speed < 5)
			{
				p1Speed += 1;
			}
			else if(p1Speed >= 5)
			{
				//Do nothing
			}
		}
		else
		{
			if(p1Speed > 0)
			{
				p1Speed -= 1;
			}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && inBounds(p2Paddle, "up"))
		{
			if(p2Speed > -5)
			{
				p2Speed -= 1;
			}
			else if(p2Speed <= -5)
			{
				//Do nothing
			}
		}
		else
		{
			if(p2Speed < 0)
			{
				p2Speed += 1;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && inBounds(p2Paddle, "down"))
		{
			if(p2Speed < 5)
			{
				p2Speed += 1;
			}
			else if(p2Speed >= 5)
			{
				//Do nothing
			}

		}
		else
		{
			if(p2Speed > 0)
			{
				p2Speed -= 1;
			}
		}



		if(ball.getPosition().y < 15 || ball.getPosition().y > 473)
		{
			ballVelocity.y = ballVelocity.y - (ballVelocity.y * 2);
		}

		if (ballBB.intersects(p1BB) || ballBB.intersects(p2BB))
		{
			ballVelocity.x = ballVelocity.x - (ballVelocity.x * 2);
			ballVelocity.x += 0.4;

			if(ballBB.intersects(p1BB) && p1Speed != 0)
			{
				ballVelocity.y += p1Speed / 2;
			}
			else if(ballBB.intersects(p2BB) && p2Speed != 0)
			{
				ballVelocity.y += p2Speed / 2;
			}
		}

		if(ballBB.intersects(p1GoalBB))
		{
			p1Score += 1;
			pointTo(awardPoint, 1);

			p1Paddle.setPosition(50,218);
			p2Paddle.setPosition(825,218);
			ball.setPosition(438,150);
			ballVelocity = sf::Vector2f(7.40,2.0);
		}
		else if(ballBB.intersects(p2GoalBB))
		{
			p2Score += 1;
			pointTo(awardPoint, 2);

			p1Paddle.setPosition(50,218);
			p2Paddle.setPosition(825,218);
			ball.setPosition(438,150);
			ballVelocity = sf::Vector2f(7.40,2.0);
		}

		if(p1Score == toWin || p2Score == toWin)
		{
			if(p1Score == toWin)
            {
                return true;
            }
            else return false;
			//when a player's score reaches 3 they lose
		}

		p1Paddle.move(0, p1Speed);
		p2Paddle.move(0, p2Speed);

		p1ScoreDraw.setString(convertInt(p1Score));
        p2ScoreDraw.setString(convertInt(p2Score));

		ball.move(ballVelocity.x, ballVelocity.y);

        window.clear();
		window.draw(p1Goal);
		window.draw(p2Goal);
		window.draw(p1ScoreDraw);
		window.draw(p2ScoreDraw);
        window.draw(p1Paddle);
		window.draw(p2Paddle);
		window.draw(ball);
        window.display();
    }

	return true;
}

bool inBounds(sf::RectangleShape entity, std::string dir)
{
	if(dir == "up")
	{
		if(entity.getPosition().y > 20)
		{
			return true;
		}
		else return false;
	}
	else if(dir == "down")
	{
		if(entity.getPosition().y < 405)
		{
			return true;
		}
		else return false;
	}
}

void pointTo(sf::Text &text, int player)
{
	if(player == 1)
    {
        text.setPosition(395,45);
    }
    else if(player == 2)
    {
        text.setPosition(495,45);
    }

    window.draw(text);
    window.display();

    for(int i = 0; i < 2540; i++)
    {
        std::cout << " " << std::endl;
        //making the program wait
    }

    text.setPosition(500,-20);

    return;
}

void victory(int player)
{
    sf::Text victory;
    victory.setPosition(370,200);
    victory.setFont(font);
    victory.setColor(sf::Color::White);
    while (1 == 1)
    {
        if(player == 1)
        {
            victory.setString("Player 1 wins!");
        }
        else if(player == 2)
        {
            victory.setString("Player 2 wins!");
        }

    window.clear();
    window.draw(victory);
    window.display();

    for(int i = 0; i < 10002; i++)
    {
        std::cout << " " << std::endl;
        if(i > 10000)
        {
            return;
        }
    }
    }

    return;
}
