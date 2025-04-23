// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <sstream>
#include<SFML/Audio.hpp>
// Make code easier to type with "using namespace"
using namespace sf;
//Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES=6;
Sprite branches[NUM_BRANCHES];

//position of branches,left or right
enum class side{LEFT,RIGHT,NONE};
side branchPosition[NUM_BRANCHES];
int main()
{
	// Create a video mode object
	// VideoMode vm(1920, 1080);

	// Low res code
	VideoMode vm(960, 540);

	// Create and open a window for the game
	// RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Low res code
	RenderWindow window(vm, "Timber!!!");
	View view(sf::FloatRect(0, 0, 1920, 1080));
	window.setView(view);
	//End of low res code!! Everything else is the same!!

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	// Is the bee currently moving?
	bool beeActive = false;

	// How fast can the bee fly
	float beeSpeed = 0.0f;
	
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	chop.setVolume(50);
	chop.setLoop(true);
	chop.play();

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("death.wav");
	Sound death;
	death.setBuffer(deathBuffer);
	death.setVolume(50);
	death.setLoop(true);
	death.play();

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("out_of_time.wav");
	Sound oot;
	oot.setBuffer(ootBuffer);
	oot.setVolume(50);
	oot.setLoop(true);
	oot.play();

	Texture texturePlayer;
	texturePlayer.loadFromFile("player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580,720);

	//Player starts on left
	side playerSide=side::LEFT;

	//control the player input
	bool acceptInput=false;


	//add gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600,860);

	//add axe
	Texture textureAxe;
	textureAxe.loadFromFile("axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700,830);

	//line the axe up with the tree
	const float AXE_POSITION_LEFT=700;
	const float AXE_POSITION_RIGHT=1075;

	//add log
	Texture textureLog;
	textureLog.loadFromFile("log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810,720);

	//some other useful log realted variables
	bool logActive=false;
	float logSpeedX=1000;
	float logSpeedY=-1500;// when player moves from left to right and bottom to top,x increase and y decrease

	// make 3 cloud sprites from 1 texture
	Texture textureCloud;

	// Load 1 new texture
	textureCloud.loadFromFile("cloud.png");

	// 3 New sprites withe the same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	// Position the clouds off screen
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 150);
	spriteCloud3.setPosition(0, 300);

	// Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	// How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Variables to control time itself
	Clock clock;

	// Time Bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 40;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1250 / 2) - timeBarStartWidth / 2, 600);

    float timeRemaining = 8.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

	int score=0;
	sf::Text scoreText;
	sf::Text messageText;
	sf::Font font;
	font.loadFromFile("/fonts/KOMIKAP_.ttf");
	
	//set the font to the msg
	scoreText.setFont(font);
	messageText.setFont(font);

	//set the specific msg
	scoreText.setString("score=0");
	messageText.setString("Press Enter to Start!!!");

	//set size
	scoreText.setCharacterSize(100);
	messageText.setCharacterSize(200);

	//set color
	scoreText.setFillColor(Color::White);
	messageText.setFillColor(Color::White);

	//set position
	scoreText.setPosition(20,20);
	FloatRect textRect=messageText.getLocalBounds();
	messageText.setOrigin(textRect.left+ textRect.width/2.0f,textRect.top+ textRect.height/2.0f);
	messageText.setPosition(1920/2.0f,1080/2.0f);

	// create 5 branches
	Texture textureBranch;

	textureBranch.loadFromFile("/home/student/c++_2241016101/2241016101/Project1/graphics/branch.png");
	
	//set the texture for each branch sprite
	for(int i=0;i<NUM_BRANCHES;i++){
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000,-2000);

		//set the sprite origin to centre
		branches[i].setOrigin(220,20);
	}
	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);
	
	//main loop
	while (window.isOpen())
	{		
		Event event;
		while(window.pollEvent(event)){
			if(event.type==Event::KeyReleased && !paused){
				acceptInput=true;

				spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;
			//reset the time and score
			score=0;
			timeRemaining=6;
			
			for(int i=1;i<NUM_BRANCHES;i++){ //disable all branches
				branchPosition[i]=side::NONE;
			}

			//make the gravestone hidden
			spriteRIP.setPosition(675,2000);

			//move the player into left position
			spritePlayer.setPosition(580,720);

			acceptInput=true;
        }
		if(acceptInput){
			if(Keyboard::isKeyPressed(Keyboard::Right)){
				playerSide=side::RIGHT;//makesure the player is on the right

				score++;

				//add the amount to the time remaining
				timeRemaining += (2/score) + 15;

				//set axe position
				spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);

				//set player position
				spritePlayer.setPosition(1200,720);

				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810,720);
				logSpeedX=-5000;
				logActive=true;

				acceptInput=false;

				//play a chop sound
				chop.play();
			}
			if(Keyboard::isKeyPressed(Keyboard::Left)){
				playerSide=side::LEFT;//makesure the player is on the right

				score++;

				//add the amount to the time remaining
				timeRemaining += (2/score) + 15;

				//set axe position
				spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);

				//set player position
				spritePlayer.setPosition(580,720);

				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810,720);
				logSpeedX=5000;
				logActive=true;

				acceptInput=false;

				//play a chop sound
				chop.play();
			}
		}
		if (!paused) {
            Time dt = clock.restart();
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining < 0.0f) {
				paused = true;
				//change the messgae shown to player
				messageText.setString("Out of Time!!");

				FloatRect textRect=messageText.getLocalBounds();
				messageText.setOrigin(textRect.left+ textRect.width/2.0f,
				textRect.top+ textRect.height/2.0f);
				messageText.setPosition(1920/2.0f,1080/2.0f);
                
            }
		
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			
			window.close();
		}

		/*/home/student/Desktop/gpwc_2241004021/Project1 (1)/graphics/branch.png
		*/
		// Measure time
		//Time dt = clock.restart();

		// Setup the bee
		if (!beeActive)
		{

			// How fast is the bee
			srand((int)time(0) * 10);
			beeSpeed = (rand() % 200) + 200;
			
			// How high is the bee
			srand((int)time(0) * 10);
			float height = (rand() % 500) + 500;
			spriteBee.setPosition(2000, height);
			beeActive = true;

		}
		else
			// Move the bee
		{

			spriteBee.setPosition(
				spriteBee.getPosition().x -
				(beeSpeed * dt.asSeconds()),
				spriteBee.getPosition().y);

			// Has the bee reached the right hand edge of the screen?
			if (spriteBee.getPosition().x < -100)
			{
				// Set it up ready to be a whole new cloud next frame
				beeActive = false;
			}
		}

		// Manage the clouds
		// Cloud 1
		if (!cloud1Active)
		{

			// How fast is the cloud
			srand((int)time(0) * 10);
			cloud1Speed = (rand() % 200);

			// How high is the cloud
			srand((int)time(0) * 10);
			float height = (rand() % 150);
			spriteCloud1.setPosition(-200, height);
			cloud1Active = true;


		}
		else
		{

			spriteCloud1.setPosition(
				spriteCloud1.getPosition().x +
				(cloud1Speed * dt.asSeconds()),
				spriteCloud1.getPosition().y);

			// Has the cloud reached the right hand edge of the screen?
			if (spriteCloud1.getPosition().x > 1920)
			{
				// Set it up ready to be a whole new cloud next frame
				cloud1Active = false;
			}
		}
		// Cloud 2
		if (!cloud2Active)
		{

			// How fast is the cloud
			srand((int)time(0) * 20);
			cloud2Speed = (rand() % 200);

			// How high is the cloud
			srand((int)time(0) * 20);
			float height = (rand() % 300) - 150;
			spriteCloud2.setPosition(-200, height);
			cloud2Active = true;


		}
		else
		{

			spriteCloud2.setPosition(
				spriteCloud2.getPosition().x +
				(cloud2Speed * dt.asSeconds()),
				spriteCloud2.getPosition().y);

			// Has the cloud reached the right hand edge of the screen?
			if (spriteCloud2.getPosition().x > 1920)
			{
				// Set it up ready to be a whole new cloud next frame
				cloud2Active = false;
			}
		}

		if (!cloud3Active)
		{

			// How fast is the cloud
			srand((int)time(0) * 30);
			cloud3Speed = (rand() % 200);

			// How high is the cloud
			srand((int)time(0) * 30);
			float height = (rand() % 450) - 150;
			spriteCloud3.setPosition(-200, height);
			cloud3Active = true;


		}
		else
		{

			spriteCloud3.setPosition(
				spriteCloud3.getPosition().x +
				(cloud3Speed * dt.asSeconds()),
				spriteCloud3.getPosition().y);

			// Has the cloud reached the right hand edge of the screen?
			if (spriteCloud3.getPosition().x > 1920)
			{
				// Set it up ready to be a whole new cloud next frame
				cloud3Active = false;
			}
		}

		std::stringstream ss;
		ss<<"score="<<score;
		scoreText.setString(ss.str());

		//update the branch sprite
		for(int i=0;i<NUM_BRANCHES;i++){
			float height=i*150;
			if(branchPosition[i]==side::LEFT)
			{	//move the sprite to the left
				branches[i].setPosition(600,height);
				branches[i].setRotation(180);
			}
			else if(branchPosition[i]==side::RIGHT)
			{
				branches[i].setPosition(1330,height);
				branches[i].setRotation(0);	
			}
			else
			{	//hide the branch
				branches[i].setPosition(3000,height);
			}
		}
		//Handle the top log flying
if(logActive){
	spriteLog.setPosition(spriteLog.getPosition().x+(logSpeedX*dt.asSeconds()),
	spriteLog.getPosition().y+(logSpeedY*dt.asSeconds()));
}
if(spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000){
	logActive=false;
	spriteLog.setPosition(810,720);
}
//Had a player squished by branch
if(branchPosition[5] == playerSide){
	//death
	paused=true;
	acceptInput=false;
	//draw the gravestone
	spriteRIP.setPosition(525,760);
	//hide the player
	spritePlayer.setPosition(2000,660);
	//change the text msg
	messageText.setString("SQUISHED!!");
	//center of the screen
	FloatRect textRect=messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width/2.0f, 
		textRect.top + textRect.height/2.0f);
	messageText.setPosition(1920/2,1000/2);
	death.play();
}

	}
		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		// Draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for(int i=0;i<NUM_BRANCHES;i++){
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);
		window.draw(scoreText);
		window.draw(timeBar);
		if(paused){
			window.draw(messageText);
		}
		
		window.display();


		
}
		return 0;
}
//function definition
void updateBranches(int seed){
	//move the branches down one place
	for(int j=NUM_BRANCHES-1;j>0;j--){
		branchPosition[j]=branchPosition[j-1];
	}
		//position LEFT,RIGHT or NONE
		srand((int)time(0)+seed);
		int r=(rand()%5);
		switch(r){
			case 0:
				branchPosition[0]=side::LEFT;
				break;
			case 1:
				branchPosition[0]=side::RIGHT;
				break;
			default:
				branchPosition[0]=side::NONE;
				break;
		}
	
}

