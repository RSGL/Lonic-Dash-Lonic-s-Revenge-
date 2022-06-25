#include <RSGL/RSGL.hpp>
#include <iostream>

RSGL::window win("Lonic Dash",{500,500,500,500},{125,125,125});
bool running=true,dead=false, shooting=false, intro=true, info=false;

int speed=4, jumpRes=20, pipe, pipex=600, reward=0, lastmove, bidenHits;

RSGL::rect bird = {60,20,30,25};
std::vector<std::vector<RSGL::rect>> pipes{
      {{200,0,50,win.r.length-100}}, {{200,100,50,win.r.length}},
      {{200,0,50,win.r.length/4},{200,win.r.length/2,50,win.r.length}},
};


std::string lonicIMG="res/images/run1.png"; int img=0; int shootTick=0, pacTick=0, addTick=0;
RSGL::rect BidenRect; int BidenHealth=200; bool p1=true;
std::vector<RSGL::rect> bidens{{0,win.r.length-100,100,100}, {20,0,100,100},{20,130,100,120}};

RSGL::point pacBullet={-20,-20}; 
std::string outputText="";
std::string text = "YAY! Lonic defated Biden,\ngot all his monies back,\nand saved president Tux!!!\nThanks for Playing";

void splashScreen(std::string file="res/images/logo.png",std::string font="res/fonts/SansPosterBold.ttf",int t=100,RSGL::window d=RSGL::root){
    static bool running=true;
    RSGL::color c = d.color; if (t < 100) t=100;
    int cl=0;
    while (running){
        win.checkEvents();  win.setColor({0,0,0}); cl++;
        if (cl==t) running=false;
        switch(win.event.type){
            case RSGL::quit: running=false; win.close(); break;
            default: break;
        }
        RSGL::drawImage(file,{win.r.width/6,win.r.length/6,win.r.width/1.5,win.r.length/1.5});
        RSGL::drawText("Powered With",{win.r.width/6,win.r.length/9,win.r.width/20},font.data(),{255,0,0});
        win.clear();
    } win.setColor(c); running=false;
}

int main(){

      bool biden=false; bool init=false;
      srand(time(NULL)); pipe=rand() % 2;
      RSGL::Button b("play","res/fonts/SansPosterBold.ttf",{win.r.width/2-80+15,400+15,15}, {win.r.width/2-80+1,400+1,90-2,50-2},{255,255,255},{255,146,28});
      RSGL::Button b2("info","res/fonts/SansPosterBold.ttf",{win.r.width/2+20+15,400+15,15},{win.r.width/2+20,400,90,50},{255,255,255},{255,146,28} );
      RSGL::Button b3("play","res/fonts/SansPosterBold.ttf",{win.r.width/2+20+120,400+15,15},{win.r.width/2+120,400,90,50},{255,255,255},{255,146,28} );
      while(running){
            win.checkEvents();
            switch(win.event.type){
                 case RSGL::quit: running=false; break;
                 default: break;
            }
            if (intro){
                  if(!info){
                        splashScreen();
                        win.setColor({0,0,0});
                        b.checkAndDraw(); b2.checkAndDraw();
                        if (b2.event && b2.event < 3) info=true;
                        if (b.event && b.event < 3){ intro=false; win.setColor({125,125,125}); }
                        RSGL::drawImage("res/images/lonic.png",{win.r.width/2-150,20,300,100});
                        RSGL::drawImage("res/images/Dash.png",{win.r.width/2-80,120,100,100});

                        RSGL::drawCircle({win.r.width/2-80,220,150},{155,0,255});
                        RSGL::drawImage("res/images/lonicStart.png",{win.r.width/2-67,250,120,120});
                        RSGL::drawImage("res/images/sega.png",{win.r.width/2-110,200,190,190});
                        RSGL::drawText("N",{win.r.width/2-110,285,40},"res/fonts/SansPosterBold.ttf",{255,0,0});

                        RSGL::drawRoundRect({win.r.width/2-80,400,90,50},{254,223,119},false);
                        RSGL::drawRoundRect({win.r.width/2+20,400,90,50},{254,223,119},false);
                  } else{ 
                        b3.checkAndDraw(); if (b3.event && b2.event < 3){ intro=false; win.setColor({125,125,125}); }
                        RSGL::drawText("Lonic Dash is a political parody of Sonic\nmixed with gameplay comparable to flappy bird.\n\nIn the game you play as Lonic, RSGL's mascot.\nLonic is going after Joe Biden to get his monies\nback after Joe Biden stole it\nas seen in RSGLLONIC POPCORNSHOP*\n\nThe controls are:\n* space to shoot\n* W/Up to go up\n* W/Down to go down",{5,20,10},"res/fonts/PublicPixel.ttf",{255,255,255});
                        RSGL::drawRoundRect({win.r.width/2+120,400,90,50},{254,223,119},false);
                  }
            }
            else if (BidenHealth >= 0){
                  if (dead && img >= 20){ BidenHealth=200; BidenRect={}; pipex=600; img=0; intro=true;  biden=false; init=false; dead=false; }
                  if (win.event.type == RSGL::KeyPressed) init=true;
                  if (img >= 5 && init && !dead){ img=0; lonicIMG = (lonicIMG=="res/images/run1.png"?  "res/images/run2.png" : "res/images/run1.png"); }

                  if (pacTick == 12){ pacTick=0; p1=!p1;}
                  if (!p1) RSGL::drawImage("res/images/pac.png",{pacBullet.x,pacBullet.y,10,10});
                  else {RSGL::drawCircle({pacBullet.x,pacBullet.y,8},{241,243,10}); }

                  RSGL::drawImage(lonicIMG,bird);
                  for (int i=0; i < pipes.at(pipe).size() && init; i++){ 
                        pipes.at(pipe).at(i).x=pipex;
                        RSGL::drawRect(pipes.at(pipe).at(i),{240,0,0});
                        if (RSGL::RectCollideRect(bird,pipes.at(pipe).at(i))) dead=true;
                        if (RSGL::RectCollideRect({pacBullet.x+12,pacBullet.y,10,10},pipes.at(pipe).at(i))){pacBullet={-20,-20}; shooting=false;}
                  
                        if (RSGL::RectCollideRect({pacBullet.x+12,pacBullet.y,10,10},{pipex,BidenRect.y,BidenRect.width,BidenRect.length})){ bidenHits++; BidenHealth--; pacBullet={-20,-20}; shooting=false;}

                        if (biden && RSGL::RectCollideRect(bird,{pipex,BidenRect.y,BidenRect.width,BidenRect.length})) dead=true;
                        if (!dead && pipe != 2) pipex-=3;
                        if (!dead && pipe == 2) pipex-=2;
                        if (!dead && pipex < 0){
                              pipex=600;  pipe=2; pipe=rand() % 3;
                              biden=true; BidenRect=bidens.at(pipe);
                        }
                  }
                  if (bidenHits == 2){bidenHits=0; biden=false;}
                  if (bird.y-speed < win.r.length-bird.length && (win.isPressed("Down")||win.isPressed("S")) && !dead) bird.y+=speed;
                  if (bird.y+speed > 0 && (win.isPressed("Up")||win.isPressed("W"))  && !dead) bird.y-=speed;
                  if (shooting) pacBullet.x+=2;
                  if (shootTick >= 125 && shooting){ shootTick=0; shooting=false; pacBullet={-20,-20}; }
                  if (!shooting && win.isPressed("space") && !dead){
                        pacBullet={bird.x+20,bird.y+10}; shooting=true; 
                  }
                  if (speed < 0) speed=abs(speed)-jumpRes;
                  if (biden){ 
                        RSGL::drawImage("res/images/Biden.png",{pipex,BidenRect.y,BidenRect.width,BidenRect.length});
                        RSGL::drawText("Biden HP : " + std::to_string(BidenHealth),{win.r.width-150,20,8},"res/fonts/PublicPixel.ttf",{255,0,0});
                  }
                  img++; pacTick++; if (shooting) shootTick++; 
            }
            else if (!intro){
                  if (addTick >= 3 && text.size() != outputText.size()){ addTick=0; outputText += text.at(outputText.size());}
                  RSGL::drawText(outputText,{20,20,15},"res/fonts/PublicPixel.ttf",{255,0,0});
                  RSGL::drawImage("res/images/lonicEnd.png",{win.r.width/2-200,win.r.width/2-50,200,200});
                  RSGL::drawImage("res/images/tux.png",{win.r.width-200,100,200,200});
                  RSGL::drawImage("res/images/bidenEnd.png",{win.r.width-200,win.r.length-200,250,200});
                  RSGL::drawImage("res/images/RSGL.png",{20,win.r.length-80,100,80}); 
                  addTick++;
            }
            win.clear();
      } win.close();
}