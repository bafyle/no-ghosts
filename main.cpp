#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <ctime>

#define PI 3.14159265
using namespace sf;

RenderWindow window(VideoMode(800, 600), "No Ghosts-beta-v0.3.1", Style::None);

std::vector<Sprite> enemies;
std::vector<Sprite> medkits;
std::vector<Sprite> bigGs;
std::vector<Sprite> bigGShots;
std::vector<Vector2f> enemiesDirs;
std::vector<Vector2f> dirs;
std::vector<Vector2f>bigGshotsDirs;
std::vector<int> hits;
std::vector<int> bigGsHits;
std::vector<CircleShape> bullets;

Vector2i mousePos;
unsigned long long score = 100;
double ghostSpeed = 7.00;
float musicVolume = 10;
float musicVolumeLimit = 10;
int magazine = 30, health = 100;
float mainCharSpeed = 2.5;
unsigned long sleepTime = 600;
int boost = 100;
short dmg = 1;
bool gameOver = false;
bool bigGExist = false;
bool mainMenu = true;
bool restartOne = true;
bool ghostSpeedUp = true;
bool reloading = false;
bool boostBool = false;
bool boostBullets = false;
bool pushmedkit = true;
bool pushBigG = true;
bool playPowerUp = true;
bool bigGClockRestart = true;
bool mouseClockRestart = true;
Sprite mainCharS;

void setupWindow(RenderWindow &window, unsigned FPS)
{
    window.setFramerateLimit(FPS);
    window.setKeyRepeatEnabled(false);
}
void enemiesSpawn()
{
    srand(time(0));
    Texture t;
    t.loadFromFile("textures/ghostE.png");

    unsigned ghostDinsty = 8;

    Sprite ghostEnemy;
    ghostEnemy.setTexture(t);
    ghostEnemy.setScale(0.2, -0.2);
    float dx, dy;
    while(window.isOpen())
    {
        int x = (rand() % 800) + 50;
        sleep(milliseconds(sleepTime));
        if(enemies.size() < ghostDinsty)
        {
            ghostEnemy.setPosition(x, -50);
            dx = (ghostEnemy.getPosition().x - mainCharS.getPosition().x - 10);
            dy = (ghostEnemy.getPosition().y - mainCharS.getPosition().y);
            float angle = std::atan2(dx, dy) * -180 / PI;
            ghostEnemy.setRotation(angle);

            if(mainMenu)
            {
                float dxx, dyy;
                dxx = ghostEnemy.getPosition().x - mousePos.x;
                dyy = ghostEnemy.getPosition().y - mousePos.y;
                float angle = std::atan2(dxx, dyy) * -180 / PI;
                ghostEnemy.setRotation(angle);
                enemiesDirs.push_back(Vector2f(ghostSpeed *(std::cos(atan2(dxx, dyy) + PI / 2)), ghostSpeed * (std::sin(atan2(dxx, dyy) - PI / 2))));
            }
            else
                enemiesDirs.push_back(Vector2f(ghostSpeed *(std::cos(atan2(dx, dy) + PI / 2)), ghostSpeed * (std::sin(atan2(dx, dy) - PI / 2))));
            enemies.push_back(ghostEnemy);
            hits.push_back(0);
        }
        if(bigGExist)
        {
            sleepTime = 1200;
            ghostDinsty  = 2;
        }
        else
        {
            sleepTime = 600;
            ghostDinsty = 5;
        }
    }
}

void restart(Texture &mainCharT)
{
    hits.clear();
    enemies.clear();
    hits.clear();
    dirs.clear();
    bullets.clear();
    enemiesDirs.clear();
    medkits.clear();
    bigGs.clear();
    bigGShots.clear();
    bigGshotsDirs.clear();
    bigGsHits.clear();
    score = 0;
    health = 100;
    boost = 100;
    magazine = 30;
    gameOver = false;
    ghostSpeed = 7.00;
    mainCharS.setPosition(100, 530);
    mainCharS.setTexture(mainCharT);
}

void clearAll(Texture &mainCharT)
{
    restart(mainCharT);
    mainMenu = false;
}

void shoot(Sound & shotS, CircleShape &bullet, float dx, float dy)
{
    bullets.push_back(bullet);
    dirs.push_back(Vector2f((13 * std::cos(atan2(dx, dy) + PI / 2)), (13 * std::sin(atan2(dx, dy) - PI / 2))));
    shotS.play();
    magazine--;
}

void eraseEnemy(unsigned g)
{
    enemies.erase(enemies.begin() + g);
    enemiesDirs.erase(enemiesDirs.begin() + g);
    hits.erase(hits.begin() + g);
}

int main()
{
    srand(time(0));
    setupWindow(window, 60);

    srand(time(0));
    Texture t;
    t.loadFromFile("textures/ghostE.png");

    unsigned ghostDinsty = 8;

    Sprite ghostEnemy;
    ghostEnemy.setTexture(t);
    ghostEnemy.setScale(0.2, -0.2);


    Texture mainCharT, closeT, mainCharD, medkitT, bigGT, fireBallT;
    medkitT.loadFromFile("textures/mdkt.png");
    mainCharT.loadFromFile("textures/ghostB.png");
    closeT.loadFromFile("textures/close.png");
    mainCharD.loadFromFile("textures/ghostBD.png");
    bigGT.loadFromFile("textures/bigG.png");
    fireBallT.loadFromFile("textures/fireball.png");

    Sprite closeS, medkitS, bigGS, fireBallS;
    closeS.setTexture(closeT);
    closeS.setPosition(770, 0);
    bigGS.setTexture(bigGT);
    bigGS.setOrigin(181/2, 146);
    bigGS.setPosition(400, -150);
    fireBallS.setTexture(fireBallT);
    fireBallS.setOrigin(20, 20);
    fireBallS.setPosition(400, 20);

    mainCharS.setTexture(mainCharT);
    mainCharS.setOrigin(228/2, 373/2);
    mainCharS.setPosition(100, 530);

    medkitS.setTexture(medkitT);
    medkitS.setScale(0.2, 0.2);

    RectangleShape pistol, ground;
    pistol.setSize(Vector2f(25, 10));
    pistol.setFillColor(Color(0, 0, 51));
    pistol.setOrigin(0, 5);

    ground.setFillColor(Color::Black);
    ground.setSize(Vector2f(800, 50));
    ground.setPosition(0, 558);

    CircleShape bullet;
    bullet.setRadius(3);
    bullet.setFillColor(Color(0, 25, 51));
    bullet.setScale(2, 1);

    SoundBuffer shotBuf, reloadingBuf, mainCharHitBuf, ghostDieBuf, pickupBuf, powerupBuf, bigGDBuf;
    shotBuf.loadFromFile("sounds/shotS.ogg");
    reloadingBuf.loadFromFile("sounds/reloadingS.ogg");
    mainCharHitBuf.loadFromFile("sounds/hit.ogg");
    ghostDieBuf.loadFromFile("sounds/ghostDieS.ogg");
    pickupBuf.loadFromFile("sounds/pickupS.ogg");
    powerupBuf.loadFromFile("sounds/powerupS.ogg");
    bigGDBuf.loadFromFile("sounds/bigGDiesS.wav");
    Sound shotS, reloadingS, mainCharHitS, ghostDieS, pickupS, powerupS, bigGDS;
    mainCharHitS.setVolume(50);
    mainCharHitS.setBuffer(mainCharHitBuf);
    shotS.setVolume(50);
    shotS.setBuffer(shotBuf);
    reloadingS.setBuffer(reloadingBuf);
    ghostDieS.setBuffer(ghostDieBuf);
    pickupS.setBuffer(pickupBuf);
    powerupS.setBuffer(powerupBuf);
    bigGDS.setBuffer(bigGDBuf);

    Music backGroundMusic;
    backGroundMusic.openFromFile("sounds/Lonely_Witch.ogg");
    backGroundMusic.setLoop(true);
    backGroundMusic.play();

    Font mainFont;
    mainFont.loadFromFile("fonts/Dimbo Regular.ttf");

    Text reloadText, healthText, boostText, scoreText, gameOverText, restartText, startText, exitText, madeBy, bulletsCount;
    reloadText.setCharacterSize(20);
    reloadText.setFont(mainFont);
    reloadText.setFillColor(Color::Black);
    reloadText.setPosition(365, 575);
    reloadText.setOutlineColor(Color::White);
    reloadText.setOutlineThickness(2);
    boostText = reloadText;
    healthText = reloadText;
    scoreText = reloadText;
    gameOverText.setFont(mainFont);
    gameOverText.setCharacterSize(32);
    gameOverText.setOutlineColor(Color(128, 0, 0));
    gameOverText.setOutlineThickness(5);
    gameOverText.setString("Game Over!");
    gameOverText.setPosition(330, 270);
    restartText = gameOverText;
    restartText.setOutlineColor(Color(0, 128, 0));
    restartText.setPosition(340, 320);
    restartText.setString("Restart?");
    scoreText.setPosition(215, 575);
    healthText.setPosition(15, 575);
    boostText.setPosition(115, 575);
    startText.setString("Play From Here");
    startText.setFont(mainFont);
    startText.setPosition(50, 350);
    startText.setCharacterSize(50);
    startText.setOutlineColor(Color(100, 0, 0));
    startText.setFillColor(Color::White);

    exitText = startText;
    exitText.setPosition(630, 5);
    exitText.setString("or exit from here =>");
    exitText.setOutlineColor(Color(100, 0, 0));
    exitText.setCharacterSize(18);
    exitText.setOutlineThickness(2);

    madeBy = exitText;
    madeBy.setString("Made By Andrew!\nbeta-v0.3.1");
    madeBy.setPosition(5, 557);

    //Thread enemiesThread(enemiesSpawn);
    //enemiesThread.launch();
    Clock clock1, boostClock, boostBulletsClock, BigGClock, musicFading, mouseClock, spawnEnemeyClock;
    while(window.isOpen())
    {
        Event event;
        mousePos = Mouse::getPosition(window);
        FloatRect mouseFloatRect(mousePos.x, mousePos.y, 1, 1);
        float dx = pistol.getPosition().x - mousePos.x;
        float dy = pistol.getPosition().y - mousePos.y;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
                window.close();
            if(event.type == Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    if(closeS.getGlobalBounds().intersects(mouseFloatRect))
                        window.close();
                    if(mouseFloatRect.intersects(restartText.getGlobalBounds()) && gameOver)
                    {
                        restart(mainCharT);
                    }
                    if(mouseFloatRect.intersects(startText.getGlobalBounds()) && mainMenu)
                    {
                        clearAll(mainCharT);
                        musicFading.restart();
                    }
                }
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Down && musicVolume > 0)
            {
                musicVolume -= 2;
                backGroundMusic.setVolume(musicVolume);
                musicVolumeLimit -= 2;
            }
            else if(event.type == Event::KeyPressed && event.key.code == Keyboard::Up && musicVolume < 100)
            {
                musicVolume += 2;
                backGroundMusic.setVolume(musicVolume);
                musicVolumeLimit += 2;
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::D) && mainCharS.getPosition().x < 780 && !gameOver)
            mainCharS.move(mainCharSpeed, 0);
        else if(Keyboard::isKeyPressed((Keyboard::A)) && mainCharS.getPosition().x > 20 && !gameOver)
            mainCharS.move(-mainCharSpeed, 0);

        if(Keyboard::isKeyPressed(Keyboard::R) && magazine != 30)
            magazine = 0;
        if(closeS.getGlobalBounds().intersects(mouseFloatRect))
            closeS.setColor(Color(255, 255, 255, 255));
        else
            closeS.setColor(Color(255, 255, 255, 100));
        if(Mouse::isButtonPressed(Mouse::Left) && mouseClockRestart && !gameOver && !mainMenu && !reloading)
        {
            mouseClockRestart = false;
            shoot(shotS, bullet, dx, dy);
            mouseClock.restart();
        }
        else if(Mouse::isButtonPressed(Mouse::Left) && !mouseClockRestart && !gameOver)
        {
            if(mouseClock.getElapsedTime().asSeconds() > 0.12f && magazine > 0)
            {
                shoot(shotS, bullet, dx, dy);
                mouseClock.restart();
            }
        }
        else
            mouseClockRestart = true;
        if(Keyboard::isKeyPressed(Keyboard::LShift) && !gameOver && !mainMenu)
        {
            if(!boostBool)
                boostClock.restart();
            boostBool = true;
            if(boostClock.getElapsedTime().asSeconds() > 0.015 && boost > 0)
            {
                boost -= 1;
                boostClock.restart();
            }
            if(boost > 0)
                mainCharSpeed = 6;
            else
                mainCharSpeed = 2.5;
        }
        else if(!mainMenu && !gameOver)
        {
            mainCharSpeed = 2.5;
            boostBool = false;
            if(boostClock.getElapsedTime().asSeconds() > 0.12 && boost < 100)
            {
                boost += 1;
                boostClock.restart();
            }
        }
        if(magazine == 0)
        {
            if(!reloading)
            {
                clock1.restart();
                reloadingS.play();
            }
            reloading = true;
            if(clock1.getElapsedTime().asSeconds() > 1)
            {
                magazine = 30;
                clock1.restart();
                reloading = false;
            }
        }
        if(score % 25 == 0 && medkits.size() < 1 && pushmedkit && score != 0)
        {
            pushmedkit = false;
            medkitS.setPosition((rand() % 600) +100, -50);
            medkits.push_back(medkitS);
        }
        if(score % 50 != 0)
        {
            ghostSpeedUp = true;
            playPowerUp = true;
            pushmedkit = true;
        }
        if(gameOver && restartText.getGlobalBounds().intersects(mouseFloatRect))
            restartText.setOutlineThickness(5);
        else if(gameOver)
            restartText.setOutlineThickness(0);
        if(mainMenu && mouseFloatRect.intersects(startText.getGlobalBounds()))
            startText.setOutlineThickness(4);
        else if(mainMenu)
            startText.setOutlineThickness(0);
        if(Mouse::getPosition(window).x <= mainCharS.getPosition().x && !gameOver && !mainMenu)
        {
            mainCharS.setScale(-0.15, 0.15);
            bullet.setScale(-2, 1);
            pistol.setPosition(mainCharS.getPosition().x - 12, mainCharS.getPosition().y);
            bullet.setPosition(mainCharS.getPosition().x - 15, mainCharS.getPosition().y + 3);
        }
        else
        {
            mainCharS.setScale(0.15, 0.15);
            bullet.setScale(2, 1);
            pistol.setPosition(mainCharS.getPosition().x + 12, mainCharS.getPosition().y);
            bullet.setPosition(mainCharS.getPosition().x + 15, mainCharS.getPosition().y - 3);
        }
        if(score % 50 == 0 && score != 0)
        {
            if(playPowerUp)
            {
                playPowerUp = false;
                powerupS.play();
            }
            boostBulletsClock.restart();
            boostBullets = true;
        }
        if(boostBullets)
        {
            bullet.setFillColor(Color::Yellow);
            dmg = 3;
            if(boostBulletsClock.getElapsedTime().asSeconds() > 10.f)
            {
                dmg = 1;
                bullet.setFillColor(Color(0, 25, 51));
                boostBullets = false;
            }
        }
        if(score % 125 == 0 && score != 0 && pushBigG && bigGs.size() <= 0)
        {
            pushBigG = false;
            bigGs.push_back(bigGS);
            bigGsHits.push_back(0);
        }
        if(score % 125 != 0)
            pushBigG = true;
        if(bigGExist && bigGs[0].getPosition() == Vector2f(400, 150) && bigGClockRestart)
        {
            bigGClockRestart = false;
            BigGClock.restart();
        }
        if(BigGClock.getElapsedTime().asSeconds() > 1.2f && bigGExist && bigGs[0].getPosition() == Vector2f(400, 150))
        {
            float dxx, dyy;
            dxx = fireBallS.getPosition().x - mainCharS.getPosition().x;
            dyy = fireBallS.getPosition().y - mainCharS.getPosition().y;
            bigGshotsDirs.push_back(Vector2f(7 * cos((std::atan2(dxx, dyy) + PI / 2)), 7 * sin((std::atan2(dxx, dyy) - PI / 2))));
            bigGShots.push_back(fireBallS);
            BigGClock.restart();
        }
        if(health <= 0)
        {
            mainCharS.setTexture(mainCharD);
            gameOver = true;
        }
        if(score % 50 == 0 && ghostSpeedUp && ghostSpeed <= 10)
        {
            ghostSpeed+=0.7;
            ghostSpeedUp = false;
        }
        if(musicFading.getElapsedTime().asSeconds() > 0.1 && musicVolume != 0 && !mainMenu && !gameOver)
        {
            musicVolume -= 2;
            backGroundMusic.setVolume(musicVolume);
            musicFading.restart();
        }
        else if(musicFading.getElapsedTime().asSeconds() > 0.1 && musicVolume < musicVolumeLimit && gameOver)
        {
            musicVolume += 2;
            backGroundMusic.setVolume(musicVolume);
            musicFading.restart();
        }
        mainCharS.setColor(Color(255, 55 + health * 2, 55 + health * 2, 255));
        healthText.setString("Health: " + std::to_string(health));
        boostText.setString("Boost: " + std::to_string(boost));
        scoreText.setString("Ghosts killed: " + std::to_string(score));
        reloadText.setString("Bullets: " + std::to_string(magazine));
        if(!gameOver)
        {
            pistol.setRotation(std::atan2(dx, dy) * -180 / PI -90);
            bullet.setRotation(pistol.getRotation());
        }
        if(spawnEnemeyClock.getElapsedTime().asMilliseconds() >= sleepTime)
        {
            int x = (rand() % 800) + 50;
            float dx, dy;
            if(enemies.size() < ghostDinsty)
            {
                ghostEnemy.setPosition(x, -50);
                dx = (ghostEnemy.getPosition().x - mainCharS.getPosition().x - 10);
                dy = (ghostEnemy.getPosition().y - mainCharS.getPosition().y);
                float angle = std::atan2(dx, dy) * -180 / PI;
                ghostEnemy.setRotation(angle);

                if(mainMenu)
                {
                    float dxx, dyy;
                    dxx = ghostEnemy.getPosition().x - mousePos.x;
                    dyy = ghostEnemy.getPosition().y - mousePos.y;
                    float angle = std::atan2(dxx, dyy) * -180 / PI;
                    ghostEnemy.setRotation(angle);
                    enemiesDirs.push_back(Vector2f(ghostSpeed *(std::cos(atan2(dxx, dyy) + PI / 2)), ghostSpeed * (std::sin(atan2(dxx, dyy) - PI / 2))));
                }
                else
                    enemiesDirs.push_back(Vector2f(ghostSpeed *(std::cos(atan2(dx, dy) + PI / 2)), ghostSpeed * (std::sin(atan2(dx, dy) - PI / 2))));
                enemies.push_back(ghostEnemy);
                hits.push_back(0);
            }
            if(bigGExist)
            {
                sleepTime = 1200;
                ghostDinsty  = 2;
            }
            else
            {
                sleepTime = 600;
                ghostDinsty = 5;
            }
            spawnEnemeyClock.restart();
        }
        window.clear(Color(80, 80, 80));

        for(unsigned i = 0; i < bigGShots.size(); i++)
        {
            bigGShots[i].move(bigGshotsDirs[i]);
            window.draw(bigGShots[i]);
            if(bigGShots[i].getGlobalBounds().intersects(mainCharS.getGlobalBounds()))
            {
                if(health >= 40)
                    health -= 40;
                else
                    health = 0;
                bigGShots.erase(bigGShots.begin() + i);
                bigGshotsDirs.erase(bigGshotsDirs.begin() + i);
                mainCharHitS.play();
            }
            if(bigGShots[i].getPosition().x > 850 || bigGShots[i].getPosition().y < -50 || bigGShots[i].getPosition().y > 650)
            {
                bigGShots.erase(bigGShots.begin() + i);
                bigGshotsDirs.erase(bigGshotsDirs.begin() + i);
            }
        }
        for(unsigned i = 0; i < bullets.size(); i++)
        {
            if(bullets[i].getPosition().x > 850 || bullets[i].getPosition().x < -50 || bullets[i].getPosition().y > 650 || bullets[i].getPosition().y < -50)
            {
                bullets.erase(bullets.begin()+i);
                dirs.erase(dirs.begin()+i);
            }
            for(unsigned k = 0; k < enemies.size(); k++)
            {
                if(bullets[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
                {
                    hits[k] += dmg;
                    dirs.erase(dirs.begin() + i);
                    bullets.erase(bullets.begin() + i);
                }
                if(hits[k] >= 3)
                {
                    eraseEnemy(k);
                    score++;
                    ghostDieS.play();
                }
            }
            bullets[i].move(dirs[i]);
            window.draw(bullets[i]);
        }
        for(unsigned i = 0; i < enemies.size(); i++)
        {
            if(enemies[i].getPosition().y > 650 || hits[i] >= 3)
                eraseEnemy(i);
            if(enemies[i].getGlobalBounds().intersects(mainCharS.getGlobalBounds()) && !gameOver && !mainMenu)
            {
                mainCharHitS.play();
                health -= 20;
                eraseEnemy(i);
            }
            enemies[i].move(enemiesDirs[i]);
            window.draw(enemies[i]);
        }
        window.draw(ground);
        for(unsigned i = 0; i < medkits.size(); i++)
        {
            window.draw(medkits[i]);
            if(!(medkits[i].getGlobalBounds().intersects(ground.getGlobalBounds())))
            {
                medkits[i].move(0, 2);
                medkits[i].rotate(1);
            }
            if(mainCharS.getGlobalBounds().intersects(medkits[i].getGlobalBounds()))
            {
                pickupS.play();
                if(health < 60)
                    health += 40;
                else
                    health += 100 - health;
                medkits.erase(medkits.begin() + i);
            }
        }
        if(bigGs.size() > 0)
        {
            bigGExist = true;
            for(unsigned i = 0; i < bigGs.size(); i++)
            {
                for(unsigned k = 0; k < bullets.size(); k++)
                {
                    if(bullets[k].getGlobalBounds().intersects(bigGs[i].getGlobalBounds()))
                    {
                        if(bigGs[i].getPosition().y > 5)
                            bigGsHits[i] += dmg;
                        bullets.erase(bullets.begin()+k);
                        dirs.erase(dirs.begin()+k);
                    }
                }
                if(bigGs[i].getPosition().y < 150)
                    bigGs[i].move(0, 1.5);
                if(bigGsHits[i] >= 60)
                {
                    bigGsHits.erase(bigGsHits.begin() + i);
                    bigGs.erase(bigGs.begin() + i);
                    score += 10;
                    bigGDS.play();
                }
                if(bigGs.size() > 0)
                    window.draw(bigGs[i]);
            }
        }
        else
        {
            bigGClockRestart = true;
            bigGExist = false;
        }
        if(reloading)
        {
            reloadText.setString("Reloading...");
        }
        if(!mainMenu)
        {
            window.draw(pistol);
            window.draw(mainCharS);
            window.draw(healthText);
            window.draw(boostText);
            window.draw(scoreText);
            window.draw(reloadText);

            if(gameOver)
            {
                if(restartOne)
                {
                    musicFading.restart();
                    restartOne = false;
                }
                window.draw(gameOverText);
                window.draw(restartText);
            }
            else
            {
                restartOne = true;
            }
        }
        else
        {
            window.draw(exitText);
            window.draw(startText);
            window.draw(madeBy);
        }
        window.draw(closeS);
        window.display();
    }
    return 0;
}
