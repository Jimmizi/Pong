#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <sstream>

sf::RenderWindow window(sf::VideoMode(900, 500), "tPong! - Multiplayer");
sf::Font font;

void gameClient(sf::TcpSocket &socket);
void gameHost(sf::TcpSocket &socket);
int menu();

void victory(int player);
void pointTo(sf::Text &text, int player);
bool inBounds(sf::RectangleShape entity, std::string dir);

void hostAGame();
void joinAGame();

std::string convertInt(int number)
{
   std::stringstream ss;
   ss << number;
   return ss.str();
}

int main()
{
	window.setFramerateLimit(60);
	font.loadFromFile("visitor.ttf");

    menu();

    return 0;
}

int menu()
{
    sf::Text title;
	title.setFont(font);
	title.setString("tPong! - Multiplayer");
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

	sf::Text joinGame;
	joinGame.setFont(font);
	joinGame.setString("Join a game");
	joinGame.setPosition(250,300);
	joinGame.setColor(sf::Color::White);

	sf::Text hostGame;
	hostGame.setFont(font);
	hostGame.setString("Host a game");
	hostGame.setPosition(250,325);
	hostGame.setColor(sf::Color::White);

	sf::Text goBack;
	goBack.setFont(font);
	goBack.setString("exit");
	goBack.setPosition(250,360);
	goBack.setColor(sf::Color::White);

	int menu = 0;

	bool running;

	bool keyPressed = false;

	while(running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !keyPressed)
        {
             if(menu != 0)
             {
                 menu -= 1;
             }

             int i = 0;
             do
             {
                 i++;
                 std::cout << " ";
             } while (i < 1000);

             keyPressed = true;
        }



		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && !keyPressed)
        {

            if(menu != 2)
            {
                menu += 1;
            }

            int i = 0;
            do
            {
                i++;
                std::cout << " ";
            } while (i < 1000);

            keyPressed = true;
        }

        if(!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            keyPressed = false;
        }

        if(menu == 0)
        {
            joinGame.setColor(sf::Color::Red);
            hostGame.setColor(sf::Color::White);
            goBack.setColor(sf::Color::White);

            joinGame.setString("* Join a game");
            hostGame.setString("Host a game");
            goBack.setString("Exit");
        }
        else if(menu == 1)
        {
            joinGame.setColor(sf::Color::White);
            hostGame.setColor(sf::Color::Red);
            goBack.setColor(sf::Color::White);

            joinGame.setString("Join a game");
            hostGame.setString("* Host a game");
            goBack.setString("Exit");
        }
        else if(menu == 2)
        {
            joinGame.setColor(sf::Color::White);
            hostGame.setColor(sf::Color::White);
            goBack.setColor(sf::Color::Red);

            joinGame.setString("Join a game");
            hostGame.setString("Host a game");
            goBack.setString("* Exit");
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if(menu == 0)
            {
                joinAGame();
                //return 0;
            }
            else if(menu == 1)
            {
                hostAGame();
                //return 0;
            }
            else if(menu == 2)
            {
                return 2;
            }
        }

		window.clear();
		window.draw(title);
		window.draw(ball);
		window.draw(joinGame);
		window.draw(hostGame);
		window.draw(goBack);
		window.draw(name);
		window.draw(version);
		window.display();
    }
}

void gameClient(sf::TcpSocket &socket)
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

	sf::Int32 p1Speed = 0;
	sf::Int32 p2Speed = 0;
	sf::Vector2f ballVelocity;//(7.40,2.0);

	sf::Int32 p1Score = 0;
	sf::Int32 p2Score = 0;

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

		if(p1Score == 3 || p2Score == 3)
		{
			if(p1Score == 3)
            {
                return;
            }
            else return;
			//when a player's score reaches 3 they lose
		}

            //getting p1Position and ballVelocity to p2
            sf::Packet packet;
            socket.receive(packet);

            sf::Int32 x;
            sf::Vector2f i;

            packet >> x >> i.x >> i.y;

            p1Speed = x;
            ballVelocity = i;
            packet.clear();
            //sending p2's position

            packet << p2Speed;
            std::cout << p2Speed << std::endl;

            socket.send(packet);
            packet.clear();

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

	return;
}

void gameHost(sf::TcpSocket &socket)
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

	sf::Int32 p1Speed = 0;
	sf::Int32 p2Speed = 0;
	sf::Vector2f ballVelocity(7.40,2.0);

	sf::Int32 p1Score = 0;
	sf::Int32 p2Score = 0;

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
	sf::Clock clock;

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

		if(p1Score == 3 || p2Score == 3)
		{
			if(p1Score == 3)
            {
                return;
            }
            else return;
			//when a player's score reaches 3 they lose
		}

            //Sending p1Position and ballVelocity to p2
            sf::Packet packet;
            packet << p1Speed << ballVelocity.x << ballVelocity.y;

            socket.send(packet);
            packet.clear();

            //Getting p2's position
            socket.receive(packet);

            sf::Int32 i;

            packet >> i;

            p2Speed = i;
            std::cout << i << std::endl;
            packet.clear();


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

	return;
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

void hostAGame()
{
    sf::Event event;
    window.clear();

    sf::Text waiting;
	waiting.setFont(font);
	waiting.setString("waiting for a connection...");
	waiting.setPosition(250,225);
	waiting.setColor(sf::Color::White);

    sf::TcpListener listener;
    sf::TcpSocket socket;

    do
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    return;
                }
            }
        }

        window.clear();
        window.draw(waiting);
        window.display();

    } while (listener.listen(50001) != sf::Socket::Done);

    if (listener.accept(socket) != sf::Socket::Done)
        return;
    waiting.setString("Client connected!");

    gameHost(socket);
}

void joinAGame()
{
    sf::Event event;
    window.clear();

    std::string str;
    sf::String text;

    sf::Text title;
	title.setFont(font);
	title.setString("tPong! - Multiplayer");
	title.setPosition(250,175);
	title.setColor(sf::Color::White);

    sf::Text ip;
	ip.setFont(font);
	ip.setString("enter ip address");
	ip.setPosition(250,225);
	ip.setColor(sf::Color::Red);

	sf::Text goBack;
	goBack.setFont(font);
	goBack.setString("press escape to go back");
	goBack.setPosition(250,450);
	goBack.setColor(sf::Color::White);

    int menu = 0;

    sf::IpAddress server;

    do
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::TextEntered)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                {
                    if(!str.empty())
                    {
                        str.pop_back();
                        ip.setString(str);

                        if(str.empty())
                        {
                            ip.setString("enter ip address");
                        }
                    }

                }

                // Handle ASCII characters only
                if (event.text.unicode < 128 &&
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) &&
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                {
                    str += static_cast<char>(event.text.unicode);
                    ip.setString(str);
                }
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            return;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            server = str;
        }

        window.clear();
        window.draw(title);
        window.draw(ip);
        window.draw(goBack);
        window.display();

    } while (server == sf::IpAddress::None);

    const unsigned short port = 50001;

    // Create a socket for communicating with the server
    sf::TcpSocket socket;

    // Connect to the server
    if (socket.connect(server, port) == sf::Socket::Done)
    {
        ip.setColor(sf::Color::White);
        ip.setString("Connected successfully!");

        window.clear();
        window.draw(ip);
        window.display();

        int i = 0;
        do
        {
            i++;
            std::cout << " ";
        } while (i < 20000);

        gameClient(socket);
    }
    else
    {
        ip.setColor(sf::Color::White);
        ip.setString("Connection error!");

        window.clear();
        window.draw(ip);
        window.display();

        int i = 0;
        do
        {
            i++;
            std::cout << " ";
        } while (i < 20000);

        return;
    }


}

/* // Receive a message
    char in[128];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return;
    std::cout << "Message received from the server: \"" << in << "\"" << std::endl;

    // Send a message
    const char out[] = "Hi, I'm a client";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
*/
