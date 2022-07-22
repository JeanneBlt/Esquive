#pragma warning( disable : 4996 ) 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
using namespace std;


struct PlayerCar
{
	
	V2 PlayerPos;
	V2 PlayerSize;
	int IdSpritePlayer;

	PlayerCar()
	{
		PlayerSize = V2(65, 140);
		PlayerPos = V2(300 - (PlayerSize.x) / 2, 150 - (PlayerSize.y) / 2);
	}

	void initTextures()
	{
		IdSpritePlayer = G2D::ExtractTextureFromPPM(".//assets//playerCar.ppm", false);
		int zoom = 1;
		PlayerSize = PlayerSize * zoom;
	}

};
 PlayerCar Pc;

// information de partie
struct GameData
{
	int idFrame;
	int HeighPix;
	int WidthPix;

	string Screen;


	int difficulte;


	float timing;
	float eqtiming;

	float difftemps;
	float apptps;

	V2 ContLigne[4] = { V2(50, 0), V2(50, 735), V2(550, 0), V2(550, 735) };
	V2 ContLigneP[6] = { V2(175, 0), V2(175, 735), V2(300, 0), V2(300, 735), V2(425, 0), V2(425, 735) };

	int score;
	GameData() 
	{
		int idFrame = 0;
		HeighPix = 735;
		WidthPix = 600;

		Screen = "Home";

		difficulte = 1;


		timing = 0;
		eqtiming = 0;

		difftemps = 0;
		apptps = 2.5;
		
		score = 0;
	}
};

GameData G;

// information du jouer
struct PlayerData
{
	V2 PlayerSpeed;
	bool PlayerHit[8] = { false, false, false, false, false, false, false, false };
	int PlayerLife;

	bool invul;
	float tpvul;
	bool invanim;
	float tpanim;
	
	PlayerData()
	{
		PlayerSpeed = V2(0, 0);
		PlayerLife = 3;
		invul = false;
		invanim = true;

	}
};

PlayerData P;

// information des véhicules
struct VehiculData
{
	V2 CarSize;
	int nbCar;
	V2 CarPos[8] = { V2(700, 700), V2(700,700), V2(700, 700), V2(700,700), V2(700, 700), V2(700,700), V2(700, 700), V2(700,700) };
	V2 CarSpeed[8] = { V2(0, 0), V2(0,0), V2(0, 0), V2(0,0), V2(0, 0), V2(0,0), V2(0, 0), V2(0,0) };

	bool spawn[8] = { false, false, false, false, false, false, false, false };

	int IdCars;
	int nbSpawn;
	float lastSpawn;
	
	VehiculData()
	{
		CarSize = V2(65, 140);
		nbCar = 8;
		nbSpawn = 0;
		lastSpawn = G.eqtiming;
	}

	void initTextures()
	{
		IdCars = G2D::ExtractTextureFromPPM(".//assets//cars.ppm", false);
		int zoom = 1;
		IdCars = IdCars * zoom;
	}
};

VehiculData V;


struct BonusData
{
	V2 BonusSize;
	V2 BonusPos;
	V2 BonusSpeed;

	bool apparu;
	float tpApparition;
	int IdBonusCoeur;
	int IdBonusIronPlate;
	int Bonus;
	
	BonusData()
	{
		BonusSize = V2(30, 30);
		BonusPos = { V2(700, 700) };
		BonusSpeed = { V2(0, 0) };
		apparu = false;		
	}
	void initTextures()
	{
		IdBonusCoeur = G2D::ExtractTextureFromPPM(".//assets//coeur.ppm", false);
		IdBonusIronPlate = G2D::ExtractTextureFromPPM(".//assets//IronPlate.ppm", false);
		int zoom = 1;
		int zoom2 = 1;
		IdBonusCoeur = IdBonusCoeur * zoom;
		IdBonusIronPlate = IdBonusIronPlate * zoom2;
	}
};

BonusData B;



// Dessiner arrière plan
void DrawArriere()
{
	G2D::DrawRectangle(V2(0, 0), V2(G.WidthPix, G.HeighPix), Color::Black);

	//Draw Road
	G2D::DrawRectangle(V2(50, 0), V2(G.WidthPix - 100, G.HeighPix), Color::Gray, true);

	// Draw Grass
	G2D::DrawRectangle(V2(0, 0), V2(50, G.HeighPix), Color::Green, true);
	G2D::DrawRectangle(V2(550, 0), V2(50, G.HeighPix), Color::Green, true);

	//Draw Continuous Ligne

	for (int i = 0; i < 2; i++)
		G2D::DrawLine(G.ContLigne[2 * i], G.ContLigne[2 * i + 1], Color::Black);

	//Draw Broken Ligne

	for (int i = 0; i < 3; i++)
		G2D::DrawLine(G.ContLigneP[2 * i], G.ContLigneP[2 * i + 1], Color::White);
}

void tpAnim(float realtime, float lanctime)
{
	if (realtime - lanctime > 0.2)
	{
		if (P.invanim)
		{
			P.invanim = false;
		}
		else
		{
			P.invanim = true;
		}
		P.tpanim = realtime;
	}
}

// affichage du jeu
void render()
{

	G2D::ClearScreen(Color::Black);

	if (G.Screen == "Home")
	{
		DrawArriere();

		G2D::DrawStringFontMono(V2(115, G.HeighPix - 150), string("Dodge all the cars"), 30, 3, Color::Blue);
		G2D::DrawStringFontMono(V2(105, G.HeighPix - 200), string("Move with the arrows"), 30, 3, Color::Blue);
		G2D::DrawStringFontMono(V2(95, G.HeighPix - 350), string("PRESS ENTER TO START"), 30, 3, Color::Blue);
	}

	if (G.Screen == "Menu")
	{
		DrawArriere();

		G2D::DrawStringFontMono(V2(190, G.HeighPix - 200), string("SELECT MODE"), 30, 3, Color::Blue);
		G2D::DrawStringFontMono(V2(100, G.HeighPix - 350), string("PRESS N TO START NORMAL MODE"), 20, 3, Color::Blue);
		G2D::DrawStringFontMono(V2(110, G.HeighPix - 450), string("PRESS G TO START GLIDE MODE"), 20, 3, Color::Blue);
	}



	if (G.Screen == "Game")
	{

		DrawArriere();

		//Draw Car
		for (int i = 0; i < V.nbCar; i++)
		{
			if (V.CarPos[i].x != 700)
			{
				int idspriteB = V.IdCars;
				G2D::DrawRectWithTexture(idspriteB, V.CarPos[i], V.CarSize);
			}
		}

		
		//Draw Bonus
		if (B.BonusPos.x != 700)
		{
			if (B.Bonus == 0)
			{
				int idspriteC = B.IdBonusCoeur;
				G2D::DrawRectWithTexture(idspriteC, B.BonusPos, B.BonusSize);
			}
			else
			{
				int idspriteI = B.IdBonusIronPlate;
				G2D::DrawRectWithTexture(idspriteI, B.BonusPos, B.BonusSize);
			}
			
		}
		

		//Draw Player
		int idsprite = Pc.IdSpritePlayer;
		if (P.invul == false)
		{
			G2D::DrawRectWithTexture(idsprite, Pc.PlayerPos, Pc.PlayerSize);
		}
		else
		{
			tpAnim(G.timing - G.eqtiming, P.tpanim);
			if (P.invanim)
			{
				G2D::DrawRectWithTexture(idsprite, Pc.PlayerPos, Pc.PlayerSize);
			}
		}
		
		

		//affichage rectangle vie et score
		G2D::DrawRectangle(V2(0,700), V2(600, 35), Color::Black, true);
		G2D::DrawRectangle(V2(0,700), V2(G.WidthPix, 35), Color::Red, false);
		

		//Affichage Vie
		G2D::DrawStringFontMono(V2(10, G.HeighPix - 25), string("Life : ") + to_string(P.PlayerLife), 20, 2, Color::Red);

		//Affichage Score
		G2D::DrawStringFontMono(V2(200, G.HeighPix - 25), string("Score : ") + to_string(G.score), 20, 2, Color::Red);
	}

	if (G.Screen == "Game_Over")
	{
		G2D::DrawStringFontMono(V2(210, G.HeighPix - 150), string("Game Over"), 30, 3, Color::Red);
		G2D::DrawStringFontMono(V2(200, G.HeighPix - 250), string("YOUR SCORE : ") + to_string(G.score), 20, 2, Color::Red);
		G2D::DrawStringFontMono(V2(160, G.HeighPix - 350), string("PRESS ENTER TO RESTART"), 20, 2, Color::Red);
		G2D::DrawStringFontMono(V2(165, G.HeighPix - 450), string("PRESS H TO GO TO HOME"), 20, 2, Color::Red);
	}

	G2D::Show();
}



// fonction de déplacment du joueur
int Deplacement(int Vit, int M)
{
	if (M == -1)
	{
		if (Vit > -201)
		{
			return Vit - 20 / G.difficulte;
		}
	}

	if (M == 0)
	{
		if (Vit > 1)
		{
			return Vit - 20 / G.difficulte;
		}

		if (Vit < -1)
		{
			return Vit + 20 / G.difficulte;
		}
	}

	if (M == 1)
	{
		if (Vit < 201)
		{
			return Vit + 20 / G.difficulte;
		}
	}

	return Vit;
}

// fonction pour perdre 1 vie
void lostLife()
{
	if (P.PlayerLife > 0)
	{
		P.PlayerLife = P.PlayerLife - 1;
	}
}

// fonction pour toucher un véhicule
bool Hit(V2 Pos1, V2 Pos2, V2 Size1, V2 Size2)
{
	V2 diffPos = Pos2 - Pos1;
	
	if (-Size2.x < diffPos.x && diffPos.x < Size1.x && -Size2.y < diffPos.y && diffPos.y < Size1.y)
	{
		return true;
	}
	return false;
}

// fonction pour réinitialiser le chronomètre
void ResetCrono()
{
	G.eqtiming = G.timing;
	V.lastSpawn = G.eqtiming;
	G.difftemps = G.eqtiming;
	P.tpvul = G.eqtiming - 3;
}

//envoie voiture
void launchCar(int i, int nbSp)
{
	V.CarPos[i] = V2( 80 + 125 * (i%4), 700);
	V.CarSpeed[i] = V2(0, -300);
	V.spawn[i] = true;
	if (nbSp == 1)
	{
		V.lastSpawn = G.timing;
	}
}

// recupere les voitures
void recupCar(int i)
{
	V.CarPos[i] = V2(700, 700);
	V.CarSpeed[i] = V2(0, 0);
	V.spawn[i] = false;
}

//ranger cadeau
void recupCadeau()
{
	B.BonusPos = V2(700, 700);
	B.apparu = false;
}

// fonction pour réinitialiser le joueur et le jeu
void Restart()
{
	Pc.PlayerPos = V2(300 - (Pc.PlayerSize.x) / 2, 150 - (Pc.PlayerSize.y) / 2);
	P.PlayerSpeed = V2(0, 0);
	for (int i= 0; i < V.nbCar; i++)
	{
		P.PlayerHit[i] = false;
		recupCar(i);
	}

	recupCadeau();

	P.PlayerLife = 3;
	P.invul = false;
}

// lance invulnerabilite
void LInvu(float realtime)
{
	P.invul = true;
	P.tpvul = realtime;
	P.tpanim = realtime;
}

// fonctionnalité bonus
void ActBonus(int i)
{
	if (i == 0)
	{
		P.PlayerLife = P.PlayerLife + 1;
	}

	if (i == 1)
	{
		LInvu(G.timing - G.eqtiming);
	}

}

// enleve inlvunelabilite
void envinv(float realtime, float lanctime)
{
	if (realtime - lanctime > 3)
	{
		P.invul = false;
		P.invanim = true;
	}
}

// fonction pour faire fonction le jeu
void Logic()

{
	
	

	if (G.Screen == "Home")
	{
		if (G2D::IsKeyPressed(Key::ENTER))
		{

			G.Screen = "Menu";
			G.difficulte = 1;
			ResetCrono();
		}
	}

	if (G.Screen == "Menu")
	{
		if (G2D::IsKeyPressed(Key::N))
		{

			G.Screen = "Game";
			G.difficulte = 1;
			ResetCrono();
		}

		if (G2D::IsKeyPressed(Key::G))
		{

			G.Screen = "Game";
			G.difficulte = 10;
			ResetCrono();
		}
	}


	//initialisation chrono
	G.timing = (G2D::ElapsedTimeFromStartSeconds());

	if (G.Screen == "Game")
	{

		G.idFrame += 1;
		double dt = G2D::ElapsedTimeFromLastCallbackSeconds();

		float realtime = G.timing - G.eqtiming;

		// calcule le score
		int intrealtiming = realtime;
		G.score = intrealtiming * 100;

		// difficulte augmenter avec le temps
		if (realtime > 20 * G.difficulte)
			G.difftemps = 20;
		else
			G.difftemps = realtime;
		G.apptps = 2.5 - (1.2 / 20) * G.difftemps;

		
		// Spawn bonus
		if (int (G.timing - G.eqtiming) % 11 == 10 && B.apparu == false)
		{
			int xapp = rand() % 470;
			int yapp = rand() % 600;
			B.BonusPos = V2( xapp, yapp);
			B.tpApparition = realtime;
			B.apparu = true;
			B.Bonus = rand() % 2;
		}
		

		//trouver le nombre aléatoire de voiture qui va spawn
		if (V.nbSpawn == 0)
		{
			V.nbSpawn = rand() % 4;
		}

		//envoyer et recuperer et bouger voitures
		for (int i = 0; i < V.nbCar; i++)
		{

			// envoyer les voitures
			if (G.timing - V.lastSpawn > G.apptps && V.spawn[i] == false && V.nbSpawn != 0)
			{
				launchCar(i, V.nbSpawn);
				V.nbSpawn = V.nbSpawn - 1;

			}

			//recuperer les voitures
			if (V.CarPos[i].y < (-V.CarSize.y))
			{
				recupCar(i);
			}

			//deplacmeent voiture
			V.CarPos[i] = V.CarPos[i] + V.CarSpeed[i] * dt;

		}

		

		//prevision
		V2 PlayerNewPos = Pc.PlayerPos + P.PlayerSpeed * dt;
		V2 PlayerNewSpeed = P.PlayerSpeed;

		
		


		// test des diagonales

		if (G2D::IsKeyPressed(Key::UP) && G2D::IsKeyPressed(Key::LEFT))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, -1);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, 1);
		}

		else if (G2D::IsKeyPressed(Key::UP) && G2D::IsKeyPressed(Key::RIGHT))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, 1);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, 1);
		}

		else if (G2D::IsKeyPressed(Key::DOWN) && G2D::IsKeyPressed(Key::LEFT))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, -1);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, -1);
		}

		else if (G2D::IsKeyPressed(Key::DOWN) && G2D::IsKeyPressed(Key::RIGHT))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, 1);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, -1);
		}

		//test des directions unique

		else if (G2D::IsKeyPressed(Key::LEFT))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, -1);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, 0);
		}

		else if (G2D::IsKeyPressed(Key::RIGHT))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, 1);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, 0);
		}

		else if (G2D::IsKeyPressed(Key::UP))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, 0);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, 1);
		}

		else if (G2D::IsKeyPressed(Key::DOWN))
		{
			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, 0);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, -1);
		}


		// si rien est appuyer la vitesse diminue
		else
		{

			PlayerNewSpeed.x = Deplacement(PlayerNewSpeed.x, 0);
			PlayerNewSpeed.y = Deplacement(PlayerNewSpeed.y, 0);
		}

		//test limite
		if (PlayerNewPos.y > 700 - Pc.PlayerSize.y)
		{
			PlayerNewPos = Pc.PlayerPos;
		}

		if (PlayerNewPos.y < 0)
		{
			PlayerNewPos = Pc.PlayerPos;
		}

		if (PlayerNewPos.x > 550 - Pc.PlayerSize.x)
		{
			PlayerNewPos = Pc.PlayerPos;
		}

		if (PlayerNewPos.x < 50)
		{
			PlayerNewPos = Pc.PlayerPos;
		}

		//Bouger le joueur
		Pc.PlayerPos = PlayerNewPos;
		P.PlayerSpeed = PlayerNewSpeed;

		// toucher ou ne touche plus une voiture
		
		for (int i = 0; i < 8; i++)
		{
			// toucher
			if (P.PlayerHit[i] == false && P.invul == false)
			{
				if (Hit(Pc.PlayerPos, V.CarPos[i], Pc.PlayerSize, V.CarSize) )
				{
					P.PlayerHit[i] = true;
					lostLife();

					LInvu(realtime);
				}
			}
			// ne touche plus
			else
			{
				if (Hit(Pc.PlayerPos, V.CarPos[i], Pc.PlayerSize, V.CarSize) == false)
				{
					P.PlayerHit[i] = false;
				}
			}
		}

		// enleve invulnerabilite
		envinv(realtime, P.tpvul);

		// toucher le bonus
		if (Hit(Pc.PlayerPos, B.BonusPos, Pc.PlayerSize, B.BonusSize))
		{
			recupCadeau();
			ActBonus(B.Bonus);
		}

		// recuperer le bonus au bout de 5 sec
		if (realtime - B.tpApparition > 5)
		{
			recupCadeau();
		}
		

		//player dead
		if (P.PlayerLife == 0)
		{
			G.Screen = "Game_Over";
		}
	}

	if (G.Screen == "Game_Over")
	{
		Restart();
		if (G2D::IsKeyPressed(Key::ENTER))
		{
			G.Screen = "Game";
			ResetCrono();
		}
		if (G2D::IsKeyPressed(Key::H))
		{
			G.Screen = "Home";
		}
		
		
	}
}

void AssetsInit()
{
	Pc.initTextures();
	B.initTextures();
	V.initTextures();

}


// Pour lancer le jeu
int main(int argc, char* argv[])
{
	G2D::InitWindow(argc, argv, V2(G.WidthPix, G.HeighPix), V2(200, 200), string("Super Esquive UwU !!"));

	AssetsInit();

	G2D::Run(Logic, render);
}