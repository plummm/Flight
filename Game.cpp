#include "stdafx.h"
#include "Game.h"
#include "Aircraft.h"
#include "Socket.h"

SYSTEMTIME FireStartTime, FireEndTime, EnemyStartTime, EnemyEndTime, BossStartTime, BossEndTime, EnemyFireStartTIme, EnemyFireEndTime, AttackStartTime, AttackEndTime;

DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
	)
{
	Socket socket;
	socket.getRank(*(int *)lpParameter);
	return 0;
}


int compareTime()
{
	return (AttackEndTime.wHour * 3600000 + AttackEndTime.wSecond * 1000 + AttackEndTime.wMilliseconds) - (AttackEndTime.wHour * 3600000 + AttackStartTime.wSecond * 1000 + AttackStartTime.wMilliseconds);
}


Game::Game()
:mWindow(sf::VideoMode(BoundaryWidth,BoundaryHigh),"Flight")
,mPlayer(Player)
,commonBullet(Bullet)
,jbBullet(JbBullet)
,bomb(Bomb)
,enemy(Enemy)
,boss(Boss)
,explodeWave(ExplodeWave)
,parachuteBomb(ParachuteBomb)
,parachuteBullet(ParachuteBullet)
,laser(Laser)
,parachuteCure(ParachuteCure)
,caiDan(CaiDan)
,font()
,music()
,enemyNum(0)
,bossNum(0)
,sumScore(0)
,count(0)
,parachuteBombScore(0)
, parachuteBulletScore(0)
,lastHP(PlayerHP)
,limitBoss(MaxNumOfBoss)
,limitEnemy(MaxNumOfenemy)
,gameOverFlag(Alive)
{
	mIsMovingDown = false;
	mIsMovingUp = false;
	mIsMovingLeft = false;
	mIsMovingRight = false;
	mFire = false;
	mBullet = false;
	mJbBullet = false;
	mBomb = false;
	mReturn = false;
	mEsc = false;
	bombstop = false;
	upstop = false;
	downstop = false;
	if_menu = true;
	if_pause = false;
	if_caiDan = false;
	bombType = Bullet;
	bullet = &commonBullet;

	buttonList[1].currentNum = 1;
	buttonList[1].positionX = NewGameButtonX - 30;
	buttonList[1].positionY = NewGameButtonY + 35;
	buttonList[2].currentNum = 2;
	buttonList[2].positionX = HelpButtonX - 30;
	buttonList[2].positionY = HelpButtonY + 35;
	buttonList[3].currentNum = 3;
	buttonList[3].positionX = ExitButtonX - 30;
	buttonList[3].positionY = ExitButtonY + 35;
	currentButton = buttonList[1];

	

	//font
	if (!font.loadFromFile("resources/font/HYYouRanTiJ.ttf"))
	{
		printf("Load font failed!\n");
	}


	//common bullet ammo
	jbBulletAmmo.setFont(font);
	std::string jbnum = std::to_string(InitialJbBullet);
	jbBulletAmmo.setString(jbnum);
	jbBulletAmmo.setColor(sf::Color::Black);
	jbBulletAmmo.setCharacterSize(20);
	jbBulletAmmo.setPosition(JbBulletPositionX+8, JbBulletPositionY+8-offset);
	jbBulletAmmo.setStyle(sf::Text::Italic);

	//bomb ammo
	bombAmmo.setFont(font);
	jbnum = std::to_string(InitialBomb);
	bombAmmo.setString(jbnum);
	bombAmmo.setColor(sf::Color::Black);
	bombAmmo.setCharacterSize(20);
	bombAmmo.setPosition(BombPositionX + 8, BombPositionY + 8- offset);
	bombAmmo.setStyle(sf::Text::Italic);

	//Bomb explode
	if (!bombExplode.openFromFile("resources/sound/use_bomb.ogg"))
	{
		printf("Open bomb_explode failed!\n");
	}

	//Boss destroy
	if (!bossDestroy.openFromFile("resources/sound/enemy2_down.ogg"))
	{
		printf("Open enemy2_down failed!\n");
	}


	if (!bossSound.openFromFile("resources/sound/big_spaceship_flying.ogg"))
	{
		printf("Open big_spaceship_flying sound failed!\n");
	}
	bossSound.setVolume(500);

	//bulletSound
	if (!bulletSound.openFromFile("resources/sound/bullet.ogg"))
	{
		printf("Open bullet sound failed!\n");
	}
	bulletSound.setVolume(40);

	//buttonsound
	if (!buttonSound.openFromFile("resources/sound/button.ogg"))
	{
		printf("Open button sound failed!\n");
	}
	buttonSound.setVolume(200);

	//cai dan
	if (!caidanSound.openFromFile("resources/sound/caidan.ogg"))
	{
		printf("Open caidan sound failed!\n");
	}

	//attack enemies
	if (!enemyDestroySound.openFromFile("resources/sound/enemy1_down.ogg"))
	{
		printf("Opne bullet sound failed!\n");
	}

	//cure sound
	if (!cure.openFromFile("resources/sound/cure.ogg"))
	{
		printf("Open cure sound failed!\n");
	}

	//destroy enemies
	if (!playerAttackSound.openFromFile("resources/sound/enemy3_down.ogg"))
	{
		printf("Opne bullet sound failed!\n");
	}


	//score
	text.setFont(font);
	text.setColor(sf::Color::Black);
	text.setCharacterSize(40);
	text.setStyle(sf::Text::Italic);

	//gameover
	if (!gameOverSound.openFromFile("resources/sound/game_over.ogg"))
	{
		printf("Opne game_over sound failed!\n");
	}
	gameOverSound.setVolume(170);
	

	//get bomb
	if (!getJbBullet.openFromFile("resources/sound/get_jb.ogg"))
	{
		printf("Open get-bomb sound failed!\n");
	}


	//get jb bullet
	if (!getBomb.openFromFile("resources/sound/get_bomb.ogg"))
	{
		printf("Open get-bomb sound failed!\n");
	}


	//laser sound
	if (!laserSound.openFromFile("resources/sound/camera_click.ogg"))
	{
		printf("Open laser sound failed!\n");
	}
	//laserSound.setVolume(30);


	//menu sound
	if (!menuSound.openFromFile("resources/sound/menu.ogg"))
	{
		printf("Open menu sound failed!\n");
	}
	menuSound.play();
	menuSound.setLoop(true);

	//Music
	if (!music.openFromFile("resources/sound/game_music.ogg"))
	{
		printf("Opne music failed!\n");
	}
	music.setLoop(true);
	music.setVolume(45);


	// arrow
	if (!tArrow.loadFromFile("resources/image/arrow.png"))
	{
		printf("load arrow failed!\n");
	}
	sArrow.setTexture(tArrow);
	sArrow.setPosition(buttonList[1].positionX,buttonList[1].positionY);


	//attack side left
	if (!tAttackSideLeft[1].loadFromFile("resources/image/attacksideleft1.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[1].setTexture(tAttackSideLeft[1]);
	sAttackSideLeft[1].setPosition(0, 0);

	if (!tAttackSideLeft[2].loadFromFile("resources/image/attacksideleft2.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[2].setTexture(tAttackSideLeft[2]);
	sAttackSideLeft[2].setPosition(0, 0);

	if (!tAttackSideLeft[3].loadFromFile("resources/image/attacksideleft3.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[3].setTexture(tAttackSideLeft[3]);
	sAttackSideLeft[3].setPosition(0, 0);
	
	if (!tAttackSideLeft[4].loadFromFile("resources/image/attacksideleft4.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[4].setTexture(tAttackSideLeft[4]);
	sAttackSideLeft[4].setPosition(0, 0);

	if (!tAttackSideLeft[5].loadFromFile("resources/image/attacksideleft5.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[5].setTexture(tAttackSideLeft[5]);
	sAttackSideLeft[5].setPosition(0, 0);

	if (!tAttackSideLeft[6].loadFromFile("resources/image/attacksideleft6.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[6].setTexture(tAttackSideLeft[6]);
	sAttackSideLeft[6].setPosition(0, 0);

	if (!tAttackSideLeft[7].loadFromFile("resources/image/attacksideleft7.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[7].setTexture(tAttackSideLeft[7]);
	sAttackSideLeft[7].setPosition(0, 0);

	if (!tAttackSideLeft[8].loadFromFile("resources/image/attacksideleft8.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[8].setTexture(tAttackSideLeft[8]);
	sAttackSideLeft[8].setPosition(0, 0);

	if (!tAttackSideLeft[9].loadFromFile("resources/image/attacksideleft9.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[9].setTexture(tAttackSideLeft[9]);
	sAttackSideLeft[9].setPosition(0, 0);

	if (!tAttackSideLeft[10].loadFromFile("resources/image/attacksideleft10.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[10].setTexture(tAttackSideLeft[10]);
	sAttackSideLeft[10].setPosition(0, 0);

	if (!tAttackSideLeft[11].loadFromFile("resources/image/attacksideleft11.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[11].setTexture(tAttackSideLeft[11]);
	sAttackSideLeft[11].setPosition(0, 0);

	if (!tAttackSideLeft[12].loadFromFile("resources/image/attacksideleft12.png"))
	{
		printf("load attack side failed!\n");
	}
	sAttackSideLeft[12].setTexture(tAttackSideLeft[12]);
	sAttackSideLeft[12].setPosition(0, 0);


	//background pic
	if (!tBackGround.loadFromFile("resources/image/background.png"))
	{
		printf("load background pic failed!\n");
	}
	backGround1.setTexture(tBackGround);
	backGround1.setPosition(0, -1);
	backGround2.setTexture(tBackGround);
	backGround2.setPosition(0, 1-BoundaryHigh);

	//bomb
	if (!tBomb.loadFromFile("resources/image/bomb.png"))
	{
		printf("Load bomb pic failed!\n");
	}
	sBomb.setTexture(tBomb);
	sBomb.setPosition(BombPositionX, BombPositionY- offset);

	//bomb target
	/*if (!tBombTarget.loadFromFile("resources/image/bombtarget.png"))
	{
		printf("Load bomb_target failed!\n");
	}
	sBombTarget.setTexture(tBombTarget);
	sBombTarget.setPosition(BombPositionX,BombPositionY-30);*/

	//Bullet
	if (!tBullet.loadFromFile("resources/image/bulletselect.png"))
	{
		printf("load bullet pic failed!\n");
	}
	sBullet.setTexture(tBullet);
	sBullet.setPosition(BulletPositionX, BulletPositionY- offset);


	//frame
	if (!tFrame.loadFromFile("resources/image/frame.png"))
	{
		printf("load frame pic failed!\n");
	}
	sFrame.setTexture(tFrame);
	sFrame.setPosition(BulletPositionX -4, BulletPositionY -8- offset);


	//game over pic
	if (!tGameOver.loadFromFile("resources/image/gameover.png"))
	{
		printf("load game-over pic failed!\n");
	}
	gameOver.setTexture(tGameOver);
	gameOver.setPosition(0, 0);

	if (!tHealthPoint.loadFromFile("resources/image/HP100%.png"))
	{
		printf("load 100% pic failed!\n");
	}
	sHealthPoint.setTexture(tHealthPoint);
	sHealthPoint.setPosition(0, BoundaryHigh- offset);


	if (!tHelpButton.loadFromFile("resources/image/help.png"))
	{
		printf("load help pic failed!\n");
	}
	sHelpButton.setTexture(tHelpButton);
	sHelpButton.setPosition(HelpButtonX, HelpButtonY);

	if (!tHelp.loadFromFile("resources/image/intro.png"))
	{
		printf("load intro pic failed!\n");
	}
	sHelp.setTexture(tHelp);
	sHelp.setPosition(0, 0);

	//Jb Bullet
	if (!tJbBullet.loadFromFile("resources/image/jbbulletselect.png"))
	{
		printf("load Jb bullet pic failed!\n");
	}
	sJbBullet.setTexture(tJbBullet);
	sJbBullet.setPosition(JbBulletPositionX, JbBulletPositionY- offset);


	//Menu
	if (!tMenu.loadFromFile("resources/image/Menu.png"))
	{
		printf("Load Menu pic failed!\n");
	}
	sMenu.setTexture(tMenu);
	sMenu.setPosition(0, 0);

	//newgame button
	if (!tNewGameButton.loadFromFile("resources/image/newgame.png"))
	{
		printf("Load new game pic failed!\n");
	}
	sNewGameButton.setTexture(tNewGameButton);
	sNewGameButton.setPosition(NewGameButtonX, NewGameButtonY);

	//exit button
	if (!tExitButton.loadFromFile("resources/image/exit.png"))
	{
		printf("Load exit pic failed!\n");
	}
	sExitButton.setTexture(tExitButton);
	sExitButton.setPosition(ExitButtonX, ExitButtonY);

	//pause
	if (!tPause.loadFromFile("resources/image/pause.png"))
	{
		printf("Load pause pic failed!\n");
	}
	sPause.setTexture(tPause);
	sPause.setPosition(0, 0);

	//back to menu
	if (!tBackToMenu.loadFromFile("resources/image/backtomenu.png"))
	{
		printf("Load backtomenu pic failed!\n");
	}
	sBackToMenu.setTexture(tBackToMenu);
	sBackToMenu.setPosition(0, 0);

	mPlayer.eSprite.setPosition(240.f, 700.f);
	
	
	events.push_back(mPlayer);

	//srand
	srand((unsigned)time(NULL));
}


void Game::changeBullet(int flag)
{
	switch (flag)
	{
	case Bullet:
		bombType = Bullet;
		bullet=&commonBullet;
		sFrame.setPosition(BulletPositionX-4,BulletPositionY-8- offset);
		break;
	case JbBullet:
		bombType = JbBullet;
		bullet = &jbBullet;
		sFrame.setPosition(JbBulletPositionX - 4, JbBulletPositionY - 8- offset);
		break;
	case Bomb:
		bombType = Bomb;
		bullet = &bomb;
		sFrame.setPosition(BombPositionX - 4, BombPositionY - 8- offset);
		bomb.eSprite.setPosition(BombPositionX, BombPositionY - 40- offset);
		break;
	default:
		break;
	}
}


void Game::checkCollision(PFITERATOR iterator1, PFITERATOR iterator2)
{
	if ((*iterator1).type == (*iterator2).father || (*iterator2).type == (*iterator1).father || ((*iterator1).father!=Player && (*iterator2).father!=Player) || 
		((iterator1->father == Parachute) && iterator2->type!=Player)) return;
	int dam = (*iterator2).damage - (*iterator1).Armor*ArmorRate;
	if (dam < 0) dam = 0;
	(*iterator1).HealthPoint -= dam;

	if ((*iterator1).type == Player)
	{
		//printf("%d %d\n", (*iterator1).HealthPoint, dam);
		//mPlayer.HealthPoint = iterator1->HealthPoint;
		if (iterator2->father != Parachute)
			playerAttackSound.play();
		if (iterator2->damage != 0)
		{
			mPlayer.underAttack = UnderAttack;
			GetLocalTime(&AttackStartTime);
		}
		
	}
	if ((*iterator1).HealthPoint <= 0)
	{
		switch ((*iterator1).type)
		{
		case Bullet:
			(*iterator1).eTexture.loadFromFile("resources/image/smog.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			(*iterator1).flag = Destroy4;
			break;
		case JbBullet:
			(*iterator1).eTexture.loadFromFile("resources/image/smog.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			(*iterator1).flag = Destroy4;
			break;
		case Enemy:
			(*iterator1).eTexture.loadFromFile("resources/image/enemydestroy1.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			enemyDestroySound.play();
			(*iterator1).flag = Destroy1;
			sumScore += (*iterator1).Score;
			break;
		case Player:
			//CreateThread(NULL, 0, ThreadProc, (PVOID)&sumScore, 0, NULL);
			mPlayer.eTexture.loadFromFile("resources/image/playerdestroy1.png");
			mPlayer.eSprite.setTexture((*iterator1).eTexture);
			//gameOverSound.play();
			mPlayer.flag = Destroy1;
			iterator1->flag = Destroy1;
			gameOverFlag = Destroy1;
			break;
		case Boss:
			(*iterator1).eTexture.loadFromFile("resources/image/bossdestroy1.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			bossDestroy.play();
			(*iterator1).flag = Destroy1;
			sumScore += (*iterator1).Score;
			break;
		case ParachuteBomb:
			getBomb.play();
			(*iterator1).flag = Destroy4;
			bomb.ammo++;
			break;
		case ParachuteBullet:
			getJbBullet.play();
			(*iterator1).flag = Destroy4;
			jbBullet.ammo+=100;
			break;
		case ParachuteCure:
			cure.play();
			iterator1->flag = Destroy4;
			iterator2->HealthPoint += 300;
			if (iterator2->HealthPoint > 1000)
				iterator2->HealthPoint = 1000;
			break;
		default:
			break;
		}
	}
	
}


bool Game::checkOverstep(PFITERATOR iterator1)
{
	processDestroy(iterator1);	

	int aircraftY = (int)iterator1->eSprite.getPosition().y;
	int aircraftX = (int)iterator1->eSprite.getPosition().x;
	if (if_caiDan) return false;
	if (aircraftY > BoundaryHigh + 50 || aircraftY<-50 || aircraftX<-50 || aircraftX>BoundaryWidth+50)
		return true;
	return false;
}


void Game::enemyshoot()
{
	
	for (int i = 1; i <= bossPositionX[0]; i++)
	{
		laser.eSprite.setPosition(bossPositionX[i]+35, 150);
		laser.flag = Alive;
		events.push_back(laser);
		laserSound.play();
	}
}

//update enemies
void Game::enemyup()
{
	float xPostion = (rand() % (BoundaryWidth - 100)) + 50;
	GetLocalTime(&EnemyEndTime);
	BossEndTime = EnemyEndTime;
	if (abs(EnemyEndTime.wMilliseconds - EnemyStartTime.wMilliseconds) > Level3Time && enemyNum <limitEnemy)
	{
		enemyNum++;
		enemy.eSprite.setPosition(xPostion, -50);
		enemy.eVelocity = EnemySpeed + (abs(EnemyEndTime.wMilliseconds - EnemyStartTime.wMilliseconds) / 10);
		enemy.flag = Alive;
		events.push_back(enemy);
		GetLocalTime(&EnemyStartTime);
	}

	if (abs(BossEndTime.wSecond - BossStartTime.wSecond) > 20 && bossNum < limitBoss && enemyNum>100)
	{
		bossNum++;
		boss.eSprite.setPosition(xPostion, -50);
		boss.eVelocity = BossSpeed + (abs(EnemyEndTime.wMilliseconds - EnemyStartTime.wMilliseconds) / 20);
		boss.flag = Alive;
		events.push_back(boss);
		bossSound.play();
		GetLocalTime(&BossStartTime);
	}
	
	if (enemyNum == MaxNumOfenemy && bossNum == MaxNumOfBoss && if_caiDan==false)
	{
		caiDan.eSprite.setPosition(0, -BoundaryHigh-60);
		caiDan.flag = Alive;
		events.push_back(caiDan);
		music.pause();
		caidanSound.play();
		caidanSound.setLoop(true);
		if_caiDan = true;
		jbBullet.ammo += 1000;
		bomb.ammo += 15;
		mPlayer.HealthPoint = 1000;
	}
}


void Game::entityDestroy()
{
	for (auto i = destroy.begin(); i != destroy.end();i++)
	{
		events.erase(*i);
	}
	destroy.clear();
}


void Game::fire(Aircraft mPlayer)
{
	
	if (bullet->type == Bomb)
	{

		bullet->ammo--;
		int bombX = (int)bomb.eSprite.getPosition().x;
		int bombY = (int)bomb.eSprite.getPosition().y;
		bombExplode.play();
		for (int i = 0; i <= 360; i+=15)
		{
			explodeWave.eSprite.setPosition(bombX, bombY);
			explodeWave.eSprite.setRotation(i);
			explodeWave.flag = Alive;
			events.push_back(explodeWave);
		}
		mFire = false;
		bombstop = true;
		
		
		//changeBullet(Bullet);
	}
	else
	{
		int playX = (int)mPlayer.eSprite.getPosition().x;
		int playY = (int)mPlayer.eSprite.getPosition().y;
		bulletSound.play();
		
		bullet->eSprite.setPosition(playX, playY + 3);
		bullet->eSprite.setRotation(0.f);
		bullet->flag = Alive;
		events.push_back(*bullet);

		bullet->ammo--;

		bullet->eSprite.setPosition(playX + 35, playY + 3);
		bullet->eSprite.setRotation(0.f);
		bullet->flag = Alive;
		events.push_back(*bullet);
		bullet->ammo--;

		
	}
}


void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{

	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
	else if (key == sf::Keyboard::Space)
	{
		if (isPressed == true && bombstop == false)
			mFire = isPressed;
		if (isPressed == false)
		{
			mFire = isPressed;
			bombstop = false;
		}
	}
	else if (key == sf::Keyboard::Num1)
		mBullet = isPressed;
	else if (key == sf::Keyboard::Num2)
		mJbBullet = isPressed;
	else if (key == sf::Keyboard::Num3)
		mBomb = isPressed;
	else if (key == sf::Keyboard::Up)
	{
		if (isPressed == true && upstop == false)
			mUp = isPressed;
		if (isPressed == false)
		{
			mUp = isPressed;
			upstop = false;
		}
	}
	else if (key == sf::Keyboard::Down)
	{
		if (isPressed==true && downstop==false)
			mDown = isPressed;
		if (isPressed == false)
		{
			mDown = isPressed;
			downstop = false;
		}
	}
	else if (key == sf::Keyboard::Left)
		mLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mRight = isPressed;
	else if (key == sf::Keyboard::Return)
		mReturn = isPressed;
	else if (key == sf::Keyboard::Escape)
	{
		if (isPressed && if_pause) if_pause = false;
		else if (if_pause == false && isPressed) if_pause = true;
		
		mEsc = isPressed;
	}

}


void Game::layout()
{
	sf::Vector2f movement(0.f, BackGroundSpeed);
	auto timeDifference = globalTime.asSeconds();
	//printf("%f\n", timeDifference);
	//system("pause");
	backGround1.move(movement * timeDifference);
	backGround2.move(movement * timeDifference);
	mWindow.draw(backGround1);
	mWindow.draw(backGround2);
	
	if (backGround1.getPosition().y >= BoundaryHigh)
	{
		backGround1.setPosition(0, 10 - BoundaryHigh);
		//printf("1 : %f\n", backGround1.getPosition().y);
	}
	if (backGround2.getPosition().y >= BoundaryHigh)
	{
		backGround2.setPosition(0, 10 - BoundaryHigh);
		//printf("2 : %d\n", backGround2.getPosition().y);
	}
	
	
	showScore(2.f,2.f);
	mWindow.draw(text);
	mWindow.draw(sBullet);
	mWindow.draw(sJbBullet);
	mWindow.draw(sBomb);
	mWindow.draw(sFrame);
	mWindow.draw(sHealthPoint);

	if (mPlayer.HealthPoint == 1000)
	{
		tHealthPoint.loadFromFile("resources/image/HP100%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 900)
	{
		tHealthPoint.loadFromFile("resources/image/HP90%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 800)
	{
		tHealthPoint.loadFromFile("resources/image/HP80%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 700)
	{
		tHealthPoint.loadFromFile("resources/image/HP70%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 600)
	{
		tHealthPoint.loadFromFile("resources/image/HP60%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 500)
	{
		tHealthPoint.loadFromFile("resources/image/HP50%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 400)
	{
		tHealthPoint.loadFromFile("resources/image/HP40%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 300)
	{
		tHealthPoint.loadFromFile("resources/image/HP30%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else if (mPlayer.HealthPoint >= 200)
	{
		tHealthPoint.loadFromFile("resources/image/HP20%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
	else
	{
		tHealthPoint.loadFromFile("resources/image/HP10%.png");
		sHealthPoint.setTexture(tHealthPoint);
	}
		
	if (bullet->type == JbBullet)
	{
		std::string jbnum = std::to_string(bullet->ammo);
		jbBulletAmmo.setString(jbnum);
		mWindow.draw(jbBulletAmmo);
	}
	if (bullet->type == Bomb)
	{
		std::string bombnum = std::to_string(bullet->ammo);
		bombAmmo.setString(bombnum);
		mWindow.draw(bombAmmo);
		mWindow.draw(bomb.eSprite);
	}
	
	GetLocalTime(&AttackEndTime);
	if (mPlayer.underAttack==UnderAttack)
	{
		//printf("aa");
		for (int i = 1; i <= 12; i++)
		{
			int AttackTime = compareTime();
			if (AttackTime < i * 50 && AttackTime>(i-1)*50)
			{
				mWindow.draw(sAttackSideLeft[i]);
			}
		}
		if (AttackEndTime.wSecond - AttackStartTime.wSecond>=3 && lastHP == mPlayer.HealthPoint)
			mPlayer.underAttack = Safe;
		
	}
	//printf("%d %d %d\n", mPlayer.underAttack, lastHP, mPlayer.HealthPoint);
}


void Game::listup()
{
	entityDestroy();
	bossPositionX[0] = 0;
	for (auto i = events.begin(); i != events.end();i++)
	{
		
		if (i->type == Player && i->HealthPoint != mPlayer.HealthPoint)
			i->HealthPoint = mPlayer.HealthPoint;
		if (checkOverstep(i))  //check whether overstep(bullets and enemies)
			(*i).flag = Destroy4;
		if ((*i).flag<Destroy1  )
			{
				auto j = i;
				j++;
				for (; j != events.end(); j++)
				{
					if ((*i).eSprite.getGlobalBounds().intersects((*j).eSprite.getGlobalBounds()) && (*j).flag < Destroy1)
					{
						checkCollision(i, j);
						checkCollision(j, i);
					}

				}
				if ((*i).type != Player)
				{
					sf::Vector2f movment(0-i->eVelocity * sin(i->eSprite.getRotation() * M_PI / 180), i->eVelocity * cos(i->eSprite.getRotation() * M_PI / 180));
					//printf("%.f %.f %.f %.f\n", i->eVelocity * sin(i->eSprite.getRotation() * M_PI / 180), i->eVelocity * cos(i->eSprite.getRotation() * M_PI / 180), i->eVelocity, cos(i->eSprite.getRotation() * M_PI / 180));
					//sf::Vector2f movment(0.f, i->eVelocity);
					(*i).eSprite.move(movment * globalTime.asSeconds());

					if ((*i).type == Boss)
					{
						
						if ((*i).eSprite.getPosition().y >= 100 && (*i).eVelocity>0)
							(*i).eVelocity = 0 - (*i).eVelocity;
						if ((*i).eVelocity < 0 && (*i).eSprite.getPosition().y <=50)
							(*i).eVelocity = 0;
						if (i->eVelocity == 0)
						{
							bossPositionX[0]++;
							bossPositionX[bossPositionX[0]] = i->eSprite.getPosition().x;
							
						}
					}

					if (i->type == CaiDan && if_caiDan)
					{
						if (i->eSprite.getPosition().y >= 0)
							i->eVelocity = 0;
						if (mReturn)
						{
							i->eVelocity = CaiDanSpeed;
							if_caiDan = false;
							enemyNum++;
							bossNum++;
							limitBoss = 999999;
							limitEnemy = 999999999;
							caidanSound.pause();
							music.play();

						}
					}
				}
				//else
				//	(*i) = mPlayer;
			

			
			}
		if ((*i).flag == Destroy4)
			destroy.push_back(i);
		if (i->type == Player)
		{
			i->eSprite.setPosition(mPlayer.eSprite.getPosition().x, mPlayer.eSprite.getPosition().y);
			mPlayer.HealthPoint = i->HealthPoint;
			//*i = mPlayer;
			//i->underAttack = mPlayer.underAttack;
		}

		
	}
	
}


void Game::menu()
{
	
	mWindow.clear();
	mWindow.draw(sMenu);
	mWindow.draw(sNewGameButton);
	mWindow.draw(sExitButton);
	mWindow.draw(sHelpButton);
	mWindow.draw(sArrow);
	processEvents();
	if (mUp)
	{
		//printf("2");
		if (currentButton.currentNum == 2)
		{
			
			sArrow.setPosition(buttonList[1].positionX, buttonList[1].positionY);
			buttonSound.play();
			currentButton = buttonList[1];
			mUp = false;
		}
		else if (currentButton.currentNum == 3)
		{
			sArrow.setPosition(buttonList[2].positionX, buttonList[2].positionY);
			buttonSound.play();
			currentButton = buttonList[2];
			mUp = false;
		}
	}

	if (mDown)
	{
		if (currentButton.currentNum == 1)
		{
			sArrow.setPosition(buttonList[2].positionX, buttonList[2].positionY);
			buttonSound.play();
			currentButton = buttonList[2];
			mDown = false;
		}
		else if (currentButton.currentNum == 2)
		{
			sArrow.setPosition(buttonList[3].positionX, buttonList[3].positionY);
			buttonSound.play();
			currentButton = buttonList[3];
			mDown = false;
		}
	}
	
	if (mReturn)
		switch (currentButton.currentNum)
		{
		case 1:
			if_menu = false;
			menuSound.pause();
			music.play();
			GetLocalTime(&FireStartTime);
			EnemyStartTime = FireStartTime;
			BossStartTime = FireStartTime;
			EnemyFireStartTIme = FireStartTime;
			break;
		case 2:
			mWindow.draw(sHelp);
			break;
		case 3:
			exit(0);
			break;
		default:
			break;
		}
	mWindow.display();
}


void Game::parachuteup()
{
	float xPostion = (rand() % (BoundaryWidth - 100)) + 50;
	if (sumScore - parachuteBombScore >= 10000)
	{
		parachuteBomb.eSprite.setPosition(xPostion, -50);
		parachuteBomb.flag = Alive;
		events.push_back(parachuteBomb);
		parachuteBombScore = sumScore;
	}
	if (sumScore - parachuteBulletScore >= 3000)
	{
		parachuteBullet.eSprite.setPosition(xPostion, -50);
		parachuteBullet.flag = Alive;
		events.push_back(parachuteBullet);
		parachuteBulletScore = sumScore;
	}
	if (sumScore - parachuteCureScore >= 7000)
	{
		parachuteCure.eSprite.setPosition(xPostion, -50);
		parachuteCure.flag = Alive;
		events.push_back(parachuteCure);
		parachuteCureScore = sumScore;
	}
}


void Game::processDestroy(PFITERATOR iterator1)
{
	if ((*iterator1).flag >= Destroy1 && (*iterator1).flag<Destroy2)
		(*iterator1).flag++;

	if ((*iterator1).flag >= Destroy2 && (*iterator1).flag < Destroy3)
	{
		switch ((*iterator1).type)
		{
		case Enemy:
			(*iterator1).eTexture.loadFromFile("resources/image/enemydestroy2.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			//enemyDestroySound.play();
			(*iterator1).flag++;
			break;
		case Boss:
			(*iterator1).eTexture.loadFromFile("resources/image/bossdestroy2.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			bossDestroy.play();
			(*iterator1).flag++;
			break;
		case Player:
			playerAttackSound.play();
			(*iterator1).flag++;
			break;
		default:
			break;
		}
		
	}

	if ((*iterator1).flag >= Destroy3 && (*iterator1).flag < Destroy4)
	{
		switch ((*iterator1).type)
		{
		case Enemy:
			(*iterator1).eTexture.loadFromFile("resources/image/enemydestroy3.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			//enemyDestroySound.play();
			(*iterator1).flag++;
			break;
		case Boss:
			(*iterator1).eTexture.loadFromFile("resources/image/bossdestroy3.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			bossDestroy.play();
			(*iterator1).flag++;
			break;
		case Player:
			playerAttackSound.play();
			(*iterator1).flag++;
			break;
		default:
			break;
		}
		
	}

	if ((*iterator1).flag == Destroy4)
	{
		switch ((*iterator1).type)
		{
		case Enemy:
			(*iterator1).eTexture.loadFromFile("resources/image/enemydestroy4.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			//enemyDestroySound.play();
			break;
		case Boss:
			(*iterator1).eTexture.loadFromFile("resources/image/bossdestroy4.png");
			(*iterator1).eSprite.setTexture((*iterator1).eTexture);
			bossDestroy.play();
			break;
		case Player:
			playerAttackSound.play();
			(*iterator1).flag++;
			printf("%d\n", (*iterator1).flag);
			break;
		default:
			break;
		}
		
	}


	//Player
	if ((*iterator1).flag >Destroy4 && (*iterator1).flag<= Gameover1)
	{
		(*iterator1).eTexture.loadFromFile("resources/image/playerdestroy2.png");
		(*iterator1).eSprite.setTexture((*iterator1).eTexture);

		(*iterator1).flag++;
	}

	if ((*iterator1).flag >Gameover1 && (*iterator1).flag<= Gameover2)
	{
		(*iterator1).eTexture.loadFromFile("resources/image/playerdestroy3.png");
		(*iterator1).eSprite.setTexture((*iterator1).eTexture);

		(*iterator1).flag++;
	}

	if ((*iterator1).flag >Gameover2 && (*iterator1).flag<= Gameover3)
	{
		(*iterator1).eTexture.loadFromFile("resources/image/playerdestroy4.png");
		(*iterator1).eSprite.setTexture((*iterator1).eTexture);

		(*iterator1).flag++;
	}


	if ((*iterator1).flag >Gameover3 && (*iterator1).flag <= Gameover4)
	{
		gameOverSound.play();
		(*iterator1).flag++;
		gameOverFlag = Gameover4;
	}
}


void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		default:
			break;
		}
	}
}


void Game::render()
{
	mWindow.clear();
	if (gameOverFlag != Gameover4)
	{
		layout();
		
		for (auto i = events.begin(); i != events.end(); i++)
		{
			Aircraft pic = *i;
			mWindow.draw(pic.eSprite);
		}
		if (if_pause) mWindow.draw(sPause);
	}
	else
	{
		entityDestroy();
		mWindow.draw(gameOver);
		//mWindow.draw(sBackToMenu);
		//if (mReturn) return;
		showScore(50,400);
		text.setCharacterSize(80);
		mWindow.draw(text);
	}
	mWindow.display();
}


void Game::run()
{
	sf::Clock clock, globalClock;
	while (mWindow.isOpen())
	{
		//printf("%f %f\n", backGround1.getPosition().y, backGround2.getPosition().y);
		if (if_menu)
		{
			menu();
			if (if_menu == false)
			{
				globalTime = globalClock.restart();
				sf::Time deltaTime = clock.restart();
			}
		}
		else
		{
			globalTime = globalClock.restart();
			fireTime = fireClock.restart();
			sf::Time deltaTime = clock.restart();
			//printf("%f\n", globalTime.asSeconds());
				processEvents();
			if (if_pause == false)
			{
				if (gameOverFlag == Alive)
					update(deltaTime);
				enemyup();
				parachuteup();
				enemyshoot();
				listup();
			}
			render();
		}
	}
}


void Game::showScore(float x,float y)
{
	scoreString = L"分数: ";
	if (count < sumScore) count+=2;
	std::wstring score = std::to_wstring(count);
	scoreString.append(score);
	text.setPosition(x, y);
	text.setString(scoreString);
}


void Game::update(sf::Time deltaTime)
{
	sf::Vector2f movement(0.f, 0.f), bombMovement(0.f,0.f);

	GetLocalTime(&FireEndTime);
	if (mFire && abs(FireEndTime.wMilliseconds - FireStartTime.wMilliseconds)>100)
	{
		if (bullet->ammo != 0)
		{
			fire(mPlayer);
			GetLocalTime(&FireStartTime);
		}
		
	}
	if (mIsMovingUp && mPlayer.eSprite.getPosition().y >= 5.0)
	{
		movement.y -= PlayerSpeed;
	}
	if (mIsMovingDown && mPlayer.eSprite.getPosition().y <= BoundaryHigh - 50)
	{
		movement.y += PlayerSpeed;
	}
	if (mIsMovingLeft && mPlayer.eSprite.getPosition().x >= 5.0)
	{
		movement.x -= PlayerSpeed;
	}
	if (mIsMovingRight && mPlayer.eSprite.getPosition().x <= BoundaryWidth - 50)
	{
		movement.x += PlayerSpeed;
	}
	if (bullet->type==Bomb && mUp && bomb.eSprite.getPosition().y >= 5.0)
	{
		bombMovement.y -= BombSpeed;
	}
	if (bullet->type == Bomb && mDown && bomb.eSprite.getPosition().y <= BoundaryHigh - 50)
	{
		bombMovement.y += BombSpeed;
	}
	if (bullet->type == Bomb && mLeft && bomb.eSprite.getPosition().x >= 5.0)
	{
		bombMovement.x -= BombSpeed;
	}
	if (bullet->type == Bomb && mRight && bomb.eSprite.getPosition().x <= BoundaryWidth - 50)
	{
		bombMovement.x += BombSpeed;
	}
	if (mBullet)
	{
		changeBullet(Bullet);
	}
	if (mJbBullet)
	{
		changeBullet(JbBullet);
	}
	if (mBomb)
	{
		changeBullet(Bomb);
	}
	   
	mPlayer.eSprite.move(movement * deltaTime.asSeconds());
	bomb.eSprite.move(bombMovement * deltaTime.asSeconds());

}


Game::~Game()
{
}
