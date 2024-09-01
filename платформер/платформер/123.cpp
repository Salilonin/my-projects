#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;

bool game = true;
bool win;

const int H = 46;
const int W = 84;

std::string TileMap[H] = {
    // ниже указано, что B это прямоугольник 16 на 16 пикселей 
       "                                                                                    ",
       "                                                                                    ",
       "                                                                                    ",
       "                                                    CC                              ",
       "                                                                                    ",
       "                                         pqo       C  C                             ",
       "                                CCC      PQO        CC                              ",
       "                        sSJJJJJGGGSGGSsGGGWGsGGSGGSGGsSGGss                         ",
       "    H                   BBIIIIIBBBBBBBBBBBBBBBBBBBBBBBBBBBB                         ",
       "   GGGJJsG               fffffffff                                 C        CC      ",
       "   BBBIIBB                                                        SGGsJJJJGSsGSs    ",
       "    fffff                                  CCC                    BBBBIIIIBBBBBB   C",
       "                                          GGGGGGG                  ffffffffffff     ",
       "                                          BBBBBBB                   ffffffffff     B",
       "                                                                                C   ",
       "        LVR                                                           pqo           ",
       "    pqo  T                   CCa                 H                    PQO       B   ",
       "   GPQO  T                  GsGAJJGGs      GGGGGGGGGGG            CC   w            ",
       "   BCwC  D   C              BBBfIIBBB     GBBBBBBBBBBBG        sGSGGsSSWSGs        C",
       "   fGWGS T GSGsG             fffffff      BfffffffffffB        BBBBBBBBBBBB         ",
       "    BBBB T BBBBB                                                                   B",
       "         T                                                                          ",
       "         T       C                                            C                 C   ",
       "         T    GSsGG                                          SGGG                   ",
       "         D    BBBBB                                          BBBB               B   ",
       "         T                                            LVR                           ",
       "         T                                             D                     a      ",
       "         T    C                                      GGTGGG                sgAJJJJJJ",
       "        sTsGSGsGGSs                                 GBBBBBB                BBfIIIIII",
       "        BBBBBBBBBBB                                GB                       ffffffff",
       "                              C                  GGB                                ",
       " CCC                    GSGGSsGsS                BB           C                     ",
       "sGsGG                   BBBBBBBBB              GB            sGS                    ",
       "BBBBB                                        GGB             BBB                    ",
       "                 C                          SBB                                     ",
       "            GsGGGsG                         B                                      S",
       "            BBBBBBB                 CC      f                                     GB",
       "                                    GG      f CCC         GS                     GBf",
       "                                    BB      f CCC         BB                 pqo fff",
       "                              C             f             ff                 PQO fff",
       "                              S              BBBBBBBBBBBBBffB              GSGWGSfff",
       "                      LVR     B                                   G        BBBBBBfff",
       "                       D                               H          B                 ",
       "              LVR     HT                      ssSgGGsGGGGG      a             CCC   ",
       "GsSGGsSJJJGGsGGDGGGsGGsTGGssJJJJJJJJJJJJJJGGGGBBBBBBBBBBBBGSGGGGAJJJJJJJsSGSGGsGsSSG",
       "BBBBBBBIIIBBBBBBBBBBBBBBBBBBIIIIIIIIIIIIIIBBBBffffffffffffBBBBBBfIIIIIIIBBBBBBBBBBBB"
};
//B - земля)
//G - трава)
//H - сердце)
//C - монета
//f - грязь)
//T - дерево без дупла)
//D - дерево с дуплом)
//L - левая листва)
//V - верхушка дерева)
//R - правая листва)
//I - яма)
//J - щупальца
//E - глаза
//A - рука
//a - рука1

class PLAYER {

public:
    float dx, dy; //скорость
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;
    Texture textureIdle, textureRun, textureJump, textureDeath;

    bool life = true;
    int hp = 1;
    int score = 0;
    bool notHit = true;
    float hitDelay = 0;
    bool jumpState = false;

    bool soundPlayedPl;
    bool soundPlayedHeal;
    SoundBuffer scoreSoundBuffer;
    Sound scoreSound;

    SoundBuffer deathSoundBufferPl;
    Sound deathSoundPl;
    SoundBuffer healSoundBuffer;
    Sound healSound;


    void Collision(int direction) {
        for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) { // от номера плитки до плитки
            for (int j = (rect.left) / 16; j < (rect.left + rect.width) / 16; j++) {
                if (TileMap[i][j] == 'B' or TileMap[i][j] == 'f' or TileMap[i][j] == 'T' or TileMap[i][j] == 'D' or TileMap[i][j] == 'L' or TileMap[i][j] == 'V' or TileMap[i][j] == 'R') {
                    if (dx > 0 and direction == 0)
                        rect.left = j * 16 - rect.width;
                    if (dx < 0 and direction == 0)
                        rect.left = j * 16 + 16;
                    if (dy > 0 and direction == 1) {
                        rect.top = i * 16 - rect.height;
                        dy = 0;
                        onGround = true;
                        jumpState = false;
                    }
                    if (dy < 0 and direction == 1) {
                        rect.top = i * 16 + 16;
                        dy = 0;
                        jumpState = false;
                    }
                }
                if (TileMap[i][j] == 'H') {
                    TileMap[i][j] = ' ';
                    hp += 1;
                    healSound.play();
                    soundPlayedHeal = true;
                }
                if (TileMap[i][j] == 'C') {
                    TileMap[i][j] = ' ';
                    score += 5;
                    scoreSound.play();
                }
                if (TileMap[i][j] == 'I') {
                    if (notHit and life) {
                        notHit = false;
                        hp -= 1;
                        if (hp == 0) currentFrame = 0;
                        hitDelay = 0;
                    }
                }
            }
        }
    }

    void update(float time) {
        rect.left += dx * time; // меняем координату x
        if (rect.left < 0) rect.left = 0; // if (rect.left < -16) rect.left = -16;
        if (rect.left > W * 16 - rect.width) rect.left = W * 16 - rect.width; // if (rect.left > W * 16 - rect.width + 16) rect.left = W * 16 - rect.width + 16;
        Collision(0);
        if (!onGround) {
            dy = dy + 0.0005 * time; // падаем с ускорением
        }
        rect.top += dy * time; // меняем координату y
        onGround = false;
        if (rect.top < 0) {
            rect.top = 0;
            dy = 0.005;
        }
        if (rect.top + rect.height > H * 16) {
            rect.top = H * 16 - rect.height;
            onGround = true;
            dy = 0;
            jumpState = false;
        }
        Collision(1);

        if (!notHit and life) {
            hitDelay += time;
            if (hitDelay > 2200) { // 2,2 секунды неуязвимости
                notHit = true;
                sprite.setColor(Color::White);
            }
            else {
                sprite.setColor(Color::Yellow);
            }
        }



        if (hp == 0) {
            currentFrame += 0.002 * time;
            sprite.setColor(Color::Red);
            life = false;
            if (!soundPlayedPl) {
                deathSoundPl.play();
                soundPlayedPl = true;
            }
            if (currentFrame > 10) {
                game = false;
                win = false;
            }

            sprite.setTexture(textureDeath);
            sprite.setTextureRect(IntRect(69 * int(currentFrame) + 20, 8, 32, 48));
        }


        if (life) {
            currentFrame += 0.01 * time;
            if (!jumpState) {
                if (dx == 0) {
                    if (currentFrame > 13)
                        currentFrame -= 13;
                    sprite.setTexture(textureIdle);
                    sprite.setTextureRect(IntRect(69 * int(currentFrame) + 20, 8, 32, 48)); // 69 * int(currentFrame), 0, 69, 56
                }
                else {
                    if (currentFrame > 16)
                        currentFrame -= 16;
                    sprite.setTexture(textureRun);
                    if (dx > 0) // при движении вправо
                        sprite.setTextureRect(IntRect(69 * int(currentFrame) + 20, 8, 32, 48)); // 69 * int(currentFrame), 0, 69, 56
                    if (dx < 0) // при движении влево
                        sprite.setTextureRect(IntRect(69 * int(currentFrame) + 69 - 16, 8, -32, 48)); // 69 * int(currentFrame) + 69, 0, -69, 56
                }
            }
            else { // анимация прыжка
                if (currentFrame > 12)
                    currentFrame -= 12;
                sprite.setTexture(textureJump);
                if (dx >= 0)
                    sprite.setTextureRect(IntRect(69 * int(currentFrame) + 20 + 207, 0, 32, 56));
                else
                    sprite.setTextureRect(IntRect(69 * int(currentFrame) + 69 - 20 + 207, 0, -32, 56));
            }
        }

        sprite.setPosition(rect.left, rect.top);
        dx = 0;
    }

    PLAYER(Texture& idle, Texture& run, Texture& jump, Texture& death, SoundBuffer& scoreBuffer, SoundBuffer& soundBufferPl, SoundBuffer& healBuffer) {
        sprite.setTexture(idle);
        textureIdle = idle;
        textureRun = run;
        textureJump = jump;
        textureDeath = death;
        sprite.setTextureRect(IntRect(0 + 20, 8, 32, 48)); // sprite.setTextureRect(IntRect(0, 0, 69, 56));
        rect = FloatRect(0, 0, 32, 48);
        dx = dy = 0;
        currentFrame = 0;

        scoreSoundBuffer = scoreBuffer;
        scoreSound.setBuffer(scoreSoundBuffer);
        soundPlayedPl = false;

        healSoundBuffer = healBuffer;
        healSound.setBuffer(healSoundBuffer);
        soundPlayedHeal = false;

        // Загрузка звука смерти	
        deathSoundBufferPl = soundBufferPl;
        deathSoundPl.setBuffer(deathSoundBufferPl);
    }
};

class Ghost {
public:
    Sprite sprite;
    float currentFramex;
    float dx;
    FloatRect rect;
    int xSpawn, ySpawn;
    bool life = true;
    bool animDeath = false;
    int R;

    bool soundPlayed;
    Texture textureghostDeath;
    Texture textureghost;
    SoundBuffer deathSoundBuffer;
    Sound deathSound;

    Ghost(Texture& texture, Texture& ghostDeath, float x, float y, SoundBuffer& soundBuffer, float speed, int radius) {
        sprite.setTexture(texture);
        textureghost = texture;
        textureghostDeath = ghostDeath;
        rect = FloatRect(x, y, 32, 64);
        xSpawn = x;
        ySpawn = y;
        R = radius;
        currentFramex = 0;
        life = true;
        soundPlayed = false;
        dx = speed; // 0.05
        // Загрузка звука смерти
        deathSoundBuffer = soundBuffer;
        deathSound.setBuffer(deathSoundBuffer);
    }

    void Collision() {
        for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
            for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
                if (TileMap[i][j] == 'B' or TileMap[i][j] == 'f' or TileMap[i][j] == 'T' or TileMap[i][j] == 'D' or TileMap[i][j] == 'L' or TileMap[i][j] == 'V' or TileMap[i][j] == 'R') {
                    if (dx > 0) {
                        rect.left = (j * 16) - rect.width; //j * 16 + 16;
                        dx = -dx;
                    }
                    else if (dx < 0) {
                        rect.left = (j * 16) + 16; //j * 16 - rect.width
                        dx = -dx;
                    }
                }
            }
        }
    }

    void update(float time) {
        rect.left += dx * time;
        if (abs(xSpawn - rect.left) > 16 * R) dx = -dx;
        if (rect.left < 0) {
            rect.left = 0;
            dx = -dx;
        }
        if (rect.left > W * 16 - rect.width + 16) {
            rect.left = W * 16 - rect.width + 16;
            dx = -dx;
        }

        Collision();
        if (life == false) {
            currentFramex += 0.001 * time;
            // Воспроизведение звука смерти
            //if (deathSound.getStatus()!= Sound::Playing) {
            //    deathSound.play();
            //    /*soundPlayed = true;*/
            //}
            if (!soundPlayed) {
                deathSound.play();
                soundPlayed = true;
            }
            if (currentFramex > 3)
                animDeath = true;
            sprite.setTexture(textureghostDeath);
            sprite.setTextureRect(IntRect(32 * int(currentFramex), 0, 32, 64));
        }

        if (life) {
            currentFramex += 0.002 * time;
            if (currentFramex > 3) {
                currentFramex -= 3;
            }
            sprite.setTextureRect(IntRect(32 * int(currentFramex), 0, 32, 64));
        }

        sprite.setPosition(rect.left, rect.top);
    }
};

void CollisionWithEnemy(PLAYER& Pl, Ghost& Gh, bool& notHit, float& hitDelay, float& time) {
    if (Pl.rect.intersects(Gh.rect) and Pl.life) { // пересечение фигур
        if (Gh.life) {
            if (Pl.dy > 0 and Pl.rect.top < Gh.rect.top) {
                Gh.dx = 0;
                Pl.dy = -0.2;
                Gh.life = false;
                Gh.currentFramex = 0;

                notHit = false;
                hitDelay = 2199;
                Pl.score += 10;
            }
            if (notHit) {
                notHit = false;
                Pl.hp -= 1;
                hitDelay = 0;
                Pl.sprite.setColor(Color::Yellow);
                if (Pl.hp == 0) Pl.currentFrame = 0;
            }
        }
    }
}

void coinAnim(float time, float& coinCurrentFrame, RectangleShape& rectangle) {
    coinCurrentFrame += 0.0003 * time;
    if (coinCurrentFrame > 8)
        coinCurrentFrame -= 8;
    rectangle.setTextureRect(IntRect(15.875 * int(coinCurrentFrame), 0, 15.875, 16));

}

void resetMap() {
    std::string newMap[] = {
        "                                                                                    ",
        "                                                                                    ",
        "                                                                                    ",
        "                                                    CC                              ",
        "                                                                                    ",
        "                                         pqo       C  C                             ",
        "                                CCC      PQO        CC                              ",
        "                        sSJJJJJGGGSGGSsGGGWGsGGSGGSGGsSGGss                         ",
        "    H                   BBIIIIIBBBBBBBBBBBBBBBBBBBBBBBBBBBB                         ",
        "   GGGJJsG               fffffffff                                 C        CC      ",
        "   BBBIIBB                                                        SGGsJJJJGSsGSs    ",
        "    fffff                                  CCC                    BBBBIIIIBBBBBB   C",
        "                                          GGGGGGG                  ffffffffffff     ",
        "                                          BBBBBBB                   ffffffffff     B",
        "                                                                                C   ",
        "        LVR                                                           pqo           ",
        "    pqo  T                   CCa                 H                    PQO       B   ",
        "   GPQO  T                  GsGAJJGGs      GGGGGGGGGGG            CC   w            ",
        "   BCwC  D   C              BBBfIIBBB     GBBBBBBBBBBBG        sGSGGsSSWSGs        C",
        "   fGWGS T GSGsG             fffffff      BfffffffffffB        BBBBBBBBBBBB         ",
        "    BBBB T BBBBB                                                                   B",
        "         T                                                                          ",
        "         T       C                                            C                 C   ",
        "         T    GSsGG                                          SGGG                   ",
        "         D    BBBBB                                          BBBB               B   ",
        "         T                                            LVR                           ",
        "         T                                             D                     a      ",
        "         T    C                                      GGTGGG                sgAJJJJJJ",
        "        sTsGSGsGGSs                                 GBBBBBB                BBfIIIIII",
        "        BBBBBBBBBBB                                GB                       ffffffff",
        "                              C                  GGB                                ",
        " CCC                    GSGGSsGsS                BB           C                     ",
        "sGsGG                   BBBBBBBBB              GB            sGS                    ",
        "BBBBB                                        GGB             BBB                    ",
        "                 C                          SBB                                     ",
        "            GsGGGsG                         B                                      S",
        "            BBBBBBB                 CC      f                                     GB",
        "                                    GG      f CCC         GS                     GBf",
        "                                    BB      f CCC         BB                 pqo fff",
        "                              C             f             ff                 PQO fff",
        "                              S              BBBBBBBBBBBBBffB              GSGWGSfff",
        "                      LVR     B                                   G        BBBBBBfff",
        "                       D                               H          B                 ",
        "              LVR     HT                      ssSgGGsGGGGG      a             CCC   ",
        "GsSGGsSJJJGGsGGDGGGsGGsTGGssJJJJJJJJJJJJJJGGGGBBBBBBBBBBBBGSGGGGAJJJJJJJsSGSGGsGsSSG",
        "BBBBBBBIIIBBBBBBBBBBBBBBBBBBIIIIIIIIIIIIIIBBBBffffffffffffBBBBBBfIIIIIIIBBBBBBBBBBBB"
    };

    std::copy(std::begin(newMap), std::end(newMap), std::begin(TileMap));
}

void resetPl(PLAYER& Pl) {
    Pl.hp = 1;
    Pl.life = true;
    Pl.soundPlayedPl = false;
    Pl.notHit = true;
    Pl.sprite.setColor(Color::White);
    Pl.score = 0;
    Pl.rect.left = 16;
    Pl.rect.top = H * 16 - 2 * 16 - Pl.rect.height;
}

void resetGh(Ghost& Gh) {
    Gh.life = true;
    Gh.animDeath = false;
    Gh.rect.left = Gh.xSpawn;
    Gh.rect.top = Gh.ySpawn;
    Gh.dx = 0.05;
    Gh.sprite.setTexture(Gh.textureghost);
    Gh.soundPlayed = false;
}

int main() {

    RenderWindow window(VideoMode(16 * W, 16 * H), "Witchy Hop");

    //Witch
    Texture tRunPl, tIdlePl, tJumpPl, tDeathPl;
    tRunPl.loadFromFile("textures/WitchRun.png");
    tIdlePl.loadFromFile("textures/WitchIdle.png");
    tJumpPl.loadFromFile("textures/WitchJump.png");
    tDeathPl.loadFromFile("textures/WitchDeath.png");
    SoundBuffer scoreSoundBuffer;
    if (!scoreSoundBuffer.loadFromFile("sounds/score_sound.mp3")) {
        return -1;
    }
    SoundBuffer soundBufferPl;
    if (!soundBufferPl.loadFromFile("sounds/witch_death.mp3")) {
        return -1;
    }
    SoundBuffer soundBufferHeal;
    if (!soundBufferHeal.loadFromFile("sounds/heal_sound.mp3")) {
        return -1;
    }
    PLAYER Pl(tIdlePl, tRunPl, tJumpPl, tDeathPl, scoreSoundBuffer, soundBufferPl, soundBufferHeal);
    Pl.rect.left = 16; // расстояние от левой стенки
    Pl.rect.top = H * 16 - 2 * 16 - Pl.rect.height;

    // Ghost
    Texture ghostTextureB;
    ghostTextureB.loadFromFile("textures/ghost1.png");
    Texture ghostTextureG;
    ghostTextureG.loadFromFile("textures/ghost2.png");
    Texture ghostTextureF;
    ghostTextureF.loadFromFile("textures/ghost3.png");

    Texture textureghostDeath;
    textureghostDeath.loadFromFile("textures/ghostdeath.png");
    // Загрузка звука смерти
    SoundBuffer ghostDeathSoundBuffer;
    if (!ghostDeathSoundBuffer.loadFromFile("sounds/ghost_death.mp3")) {
        return -1;
    }

    Ghost Gh(ghostTextureB, textureghostDeath, 41 * 16, 39 * 16 - 64, ghostDeathSoundBuffer, 0.05, 6);
    Ghost Gh2(ghostTextureB, textureghostDeath, 20 * 16, 45 * 16 - 64, ghostDeathSoundBuffer, 0.05, 6);
    Ghost Gh3(ghostTextureG, textureghostDeath, 4 * 16, 31 * 16 - 64, ghostDeathSoundBuffer, 0.1, 6);
    Ghost Gh4(ghostTextureB, textureghostDeath, 7 * 16, 20 * 16 - 64, ghostDeathSoundBuffer, 0.05, 6); // слева у дерева наверху
    Ghost Gh5(ghostTextureF, textureghostDeath, 43 * 16, 8 * 16 - 64, ghostDeathSoundBuffer, 0.07, 12); // в самом верху
    Ghost Gh6(ghostTextureF, textureghostDeath, 61 * 16, 13 * 16 - 64, ghostDeathSoundBuffer, 0.07, 18); // под самым верхним
    Ghost Gh7(ghostTextureG, textureghostDeath, 51 * 16, 40 * 16 - 64, ghostDeathSoundBuffer, 0.1, 9); // пещерка с 6 монетами
    Ghost Gh8(ghostTextureB, textureghostDeath, 70 * 16, 45 * 16 - 64, ghostDeathSoundBuffer, 0.05, 6); // на правооом конце карты

    //Skeleton
    Texture skeletonTexture;
    skeletonTexture.loadFromFile("textures/Boss.png");
    RectangleShape skeleton(Vector2f(28, 36));
    skeleton.setTexture(&skeletonTexture);
    skeleton.setTextureRect(IntRect(0, 0, 28, 36));
    skeleton.setPosition(53 * 16, 42 * 16 - 4);
    FloatRect skeletonRect(53 * 16, 42 * 16 - 4, 28, 36);

    Clock clock;
    float time;

    Clock gameClock;
    int gameTime = 0;
    std::string String;

    // ТЕКСТ
    Font font;
    if (!font.loadFromFile("textures/Minecraft Seven_2.ttf"));
    Text text;
    text.setCharacterSize(12);
    text.setFillColor(Color::White);
    text.setFont(font);

    RectangleShape rectangle(Vector2f(16.f, 16.f));

    // ЛУНА
    RectangleShape Moon(Vector2f(75, 76));
    Texture tMoon;
    tMoon.loadFromFile("textures/Moon.png");
    Moon.setTexture(&tMoon);
    Moon.setPosition(35 * 16 * 2 + 100, 2 * 2 * 16 - 30);

    Texture ground;
    ground.loadFromFile("textures/Forest.png");

    Texture pit;
    pit.loadFromFile("textures/Pit.png");

    Texture arm;
    arm.loadFromFile("textures/хз.png");

    // СЕРДЕЧКО
    Texture heart;
    heart.loadFromFile("textures/HealthUI.png");
    RectangleShape hpRect(Vector2f(24, 24));
    hpRect.setTexture(&heart);
    hpRect.setTextureRect(IntRect(0, 0, 11, 11));

    // МОНЕТКА
    Texture coin;
    coin.loadFromFile("textures/Coin.png");
    float coinCurrentFrame = 0;

    // ТАБЛИЧКИ
    Texture keepawayTexture;
    Texture noswimTextureR;
    Texture cautionTexture;
    keepawayTexture.loadFromFile("textures/keepaway_c.png");
    noswimTextureR.loadFromFile("textures/noswim_c_r.png");
    cautionTexture.loadFromFile("textures/caution_c.png");
    RectangleShape keepaway(Vector2f(32, 38));
    RectangleShape noswimR(Vector2f(32, 38));
    RectangleShape caution(Vector2f(32, 38));
    keepaway.setTexture(&keepawayTexture);
    noswimR.setTexture(&noswimTextureR);
    caution.setTexture(&cautionTexture);
    keepaway.setPosition(16 * 50, 16 * 42 - 6);
    noswimR.setPosition(16 * 5, 16 * 43 - 6);
    caution.setPosition(16 * 43, 16 * 43 - 6);


    // МЕНЮ
    Texture MenuTexture, MenuTexturel;
    MenuTexture.loadFromFile("textures/меню.png");
    RectangleShape menu(Vector2f(W * 16, H * 16));
    menu.setTexture(&MenuTexture);
    bool isMenu = 1;
    int menuNum = 0;
    menu.setPosition(0, 0);

    // ВИД
    int vW = 800;
    int vH = 440;
    View view(FloatRect(0, 0, vW, vH));
    int xCenter, yCenter;

    //Звук победы
    Sound winApplause;
    SoundBuffer applauseBuffer;
    applauseBuffer.loadFromFile("sounds/applause10.mp3");
    winApplause.setBuffer(applauseBuffer);
    bool winSoundPlayed = false;


    //____________________________________________________________________________________________________ isOpen()
    while (window.isOpen()) {

        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                isMenu = false; // если нажали кнопку "Start"
            }
        }

        if (isMenu) {
            gameClock.restart();
            window.clear();

            window.draw(menu);

            window.display();

            continue;
        }

        if (game) {

            if (Pl.life)
                gameTime = gameClock.getElapsedTime().asSeconds();

            time = time / 600; //регулировка скорости изменения координаты


            if (Pl.life) {
                if (Keyboard::isKeyPressed(Keyboard::Left) or Keyboard::isKeyPressed(Keyboard::A)) {
                    Pl.dx = -0.1;
                }

                if (Keyboard::isKeyPressed(Keyboard::Right) or Keyboard::isKeyPressed(Keyboard::D)) {
                    Pl.dx = 0.1;
                }

                if (Keyboard::isKeyPressed(Keyboard::Up) or Keyboard::isKeyPressed(Keyboard::W) or Keyboard::isKeyPressed(Keyboard::Space)) {
                    if (Pl.onGround) {
                        Pl.jumpState = true;
                        Pl.currentFrame = 0;
                        Pl.dy = -0.3;
                        Pl.onGround = false;
                    }
                }
            }

            Pl.update(time);

            Gh.update(time);
            Gh2.update(time);
            Gh3.update(time);
            Gh4.update(time);
            Gh5.update(time);
            Gh6.update(time);
            Gh7.update(time);
            Gh8.update(time);

            CollisionWithEnemy(Pl, Gh, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh2, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh3, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh4, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh5, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh6, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh7, Pl.notHit, Pl.hitDelay, time);
            CollisionWithEnemy(Pl, Gh8, Pl.notHit, Pl.hitDelay, time);

            if (Pl.rect.intersects(skeletonRect) and Pl.life) {
                Pl.hp = 0;
                Pl.currentFrame = 0;
                skeleton.setTextureRect(IntRect(2 * 28, 2 * 36, 28, 36));
            }


            xCenter = std::max(vW / 2, std::min(int(Pl.rect.left + Pl.rect.width / 2), W * 16 - vW / 2));
            yCenter = std::max(vH / 2, std::min(int(Pl.rect.top + Pl.rect.height / 2), H * 16 - vH / 2));
            view.setCenter(xCenter, yCenter);

            window.setView(view);



            window.clear(Color::Black);

            for (int i = 0; i < H; i++) {
                for (int j = 0; j < W; j++) {
                    if (TileMap[i][j] == 'G') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(0, 0, 16, 16));
                    }
                    if (TileMap[i][j] == 'B') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(16, 16, 16, 16));
                        //rectangle.setFillColor(Color{ 0, 130, 0, 255 }); // rgba
                    }
                    if (TileMap[i][j] == 'H') {
                        rectangle.setTexture(&heart);
                        rectangle.setTextureRect(IntRect(0, 0, 11, 11));
                        //rectangle.setFillColor(Color{ 255, 0, 0, 255 });
                    }
                    if (TileMap[i][j] == 'C') {
                        rectangle.setTexture(&coin);
                        coinAnim(time, coinCurrentFrame, rectangle);
                    }
                    if (TileMap[i][j] == 'T') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(4 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'D') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(5 * 16, 1 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'V') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(6 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'L') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(5 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'R') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(7 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'I') {
                        rectangle.setTexture(&pit);
                        rectangle.setTextureRect(IntRect(0, 5 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'J') {
                        rectangle.setTexture(&pit);
                        rectangle.setTextureRect(IntRect(90, 48, 16, 16));
                    }
                    if (TileMap[i][j] == 'A') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(7 * 16, 64, 16, 16));
                    }
                    if (TileMap[i][j] == 'a') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(7 * 16, 3 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'f') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(1 * 16, 4 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 's') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(3 * 16, 0 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'W') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(5 * 16, 4 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'w') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(5 * 16, 3 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'Q') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(5 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'q') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(5 * 16, 1 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'P') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(4 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'p') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(4 * 16, 1 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'O') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(6 * 16, 2 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'o') {
                        rectangle.setTexture(&arm);
                        rectangle.setTextureRect(IntRect(6 * 16, 1 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == 'S') {
                        rectangle.setTexture(&ground);
                        rectangle.setTextureRect(IntRect(2 * 16, 0 * 16, 16, 16));
                    }
                    if (TileMap[i][j] == ' ')
                        continue;

                    rectangle.setPosition(j * 16, i * 16);
                    window.draw(rectangle);
                }
            }


            window.draw(Moon);
            window.draw(keepaway);
            window.draw(noswimR);
            window.draw(caution);
            window.draw(skeleton);
            if (Gh.animDeath == false)
                window.draw(Gh.sprite);
            if (Gh2.animDeath == false)
                window.draw(Gh2.sprite);
            if (Gh3.animDeath == false)
                window.draw(Gh3.sprite);
            if (Gh4.animDeath == false)
                window.draw(Gh4.sprite);
            if (Gh5.animDeath == false)
                window.draw(Gh5.sprite);
            if (Gh6.animDeath == false)
                window.draw(Gh6.sprite);
            if (Gh7.animDeath == false)
                window.draw(Gh7.sprite);
            if (Gh8.animDeath == false)
                window.draw(Gh8.sprite);

            window.draw(Pl.sprite);

            for (int i = 1; i < Pl.hp + 1; i++) {
                hpRect.setPosition(xCenter - vW / 2 + 24 * (i)-20, yCenter - vH / 2 + 8);
                window.draw(hpRect);
            }

            String = std::to_string(gameTime);
            String += " s";
            text.setCharacterSize(12);
            text.setFillColor(Color::White);
            text.setString(String);
            text.setPosition(xCenter - vW / 2 + 8, yCenter - vH / 2 + 52);
            window.draw(text);

            String = std::to_string(Pl.score);
            String = "Score: " + String + "/315";
            text.setString(String);
            text.setPosition(xCenter - vW / 2 + 8, yCenter - vH / 2 + 32);
            window.draw(text);

            if (Pl.score == 315) { // 315
                win = true;
                game = false;
            }

            window.display();
        }
        else if (!win) {

            if (Keyboard::isKeyPressed(Keyboard::R)) {
                resetMap();
                win = false;
                game = true;
                view.setCenter(vW / 2, vH / 2);

                resetPl(Pl);
                gameClock.restart();

                if (!Gh.life) {
                    resetGh(Gh);
                }
                if (!Gh2.life) {
                    resetGh(Gh2);
                }
                if (!Gh3.life) {
                    resetGh(Gh3);
                    Gh3.dx = 0.1;
                }
                if (!Gh4.life) {
                    resetGh(Gh4);
                }
                if (!Gh5.life) {
                    resetGh(Gh5);
                    Gh5.dx = 0.07;
                }
                if (!Gh6.life) {
                    resetGh(Gh6);
                    Gh6.dx = 0.07;
                }
                if (!Gh7.life) {
                    resetGh(Gh7);
                    Gh7.dx = 0.1;
                }
                if (!Gh8.life) {
                    resetGh(Gh8);
                }

                skeleton.setTextureRect(IntRect(0, 0, 28, 36));
            }

            window.clear(Color::Black);

            view.setCenter(vW / 2, vH / 2);
            window.setView(view);

            String = "Don't be upset";
            text.setString(String);
            text.setPosition(vW / 2 - String.size() / 2 * 16 + 16 * 2, vH / 2 + 16 * 1.5 * 3);
            text.setCharacterSize(16);
            text.setFillColor(Color::White);
            window.draw(text);

            String = "Press R to retry";
            text.setString(String);
            text.setPosition(vW / 2 - String.size() / 2 * 16 * 1.5 + 16 * 4, vH / 2 + 16 * 2);
            text.setCharacterSize(16 * 1.5);
            text.setFillColor(Color::White);
            window.draw(text);

            String = "GAME OVER";
            text.setString(String);
            text.setPosition(vW / 2 - String.size() / 2 * 16 * 4, vH / 2 - 16 * 4);
            text.setCharacterSize(16 * 5);
            text.setFillColor(Color::Red);
            window.draw(text);

            window.display();
        }
        else {

            if (!winSoundPlayed) {
                winApplause.play();
                winSoundPlayed = true;
            }

            if (Keyboard::isKeyPressed(Keyboard::R)) {
                resetMap();
                win = false;
                game = true;
                view.setCenter(vW / 2, vH / 2);

                resetPl(Pl);
                gameClock.restart();

                if (!Gh.life) {
                    resetGh(Gh);
                }
                if (!Gh2.life) {
                    resetGh(Gh2);
                }
                if (!Gh3.life) {
                    resetGh(Gh3);
                    Gh3.dx = 0.1;
                }
                if (!Gh4.life) {
                    resetGh(Gh4);
                }
                if (!Gh5.life) {
                    resetGh(Gh5);
                    Gh5.dx = 0.07;
                }
                if (!Gh6.life) {
                    resetGh(Gh6);
                    Gh6.dx = 0.07;
                }
                if (!Gh7.life) {
                    resetGh(Gh7);
                    Gh7.dx = 0.1;
                }
                if (!Gh8.life) {
                    resetGh(Gh8);
                }

                skeleton.setTextureRect(IntRect(0, 0, 28, 36));
                winSoundPlayed = false;
            }


            window.clear(Color::Black);

            view.setCenter(vW / 2, vH / 2);
            window.setView(view);

            String = "Result: ";
            String += std::to_string(gameTime) + " s";
            text.setString(String);
            text.setPosition(vW / 2 - String.size() / 2 * 16 + 16 * 2, vH / 2 + 16 * 1.5 * 3);
            text.setCharacterSize(16);
            text.setFillColor(Color::White);
            window.draw(text);

            String = "Press R to retry";
            text.setString(String);
            text.setPosition(vW / 2 - String.size() / 2 * 16 * 1.5 + 16 * 4, vH / 2 + 16 * 2);
            text.setCharacterSize(16 * 1.5);
            text.setFillColor(Color::White);
            window.draw(text);

            String = "GREAT JOB";
            text.setString(String);
            text.setPosition(vW / 2 - String.size() / 2 * 16 * 4, vH / 2 - 16 * 4);
            text.setCharacterSize(16 * 5);
            text.setFillColor(Color::Yellow);
            window.draw(text);

            window.display();
        }
    }

    return 0;
}