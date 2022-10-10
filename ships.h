#ifndef ships_h
#define ships_h

#include <iostream>
#include <fstream>		//zapisywanie i wczytywanie	

class xy
{
public:
	int line;
	int column;

	xy(){};
	xy(int l, int c)
	{
		line = l;
		column = c;
	}
};

class Ship
{
protected:
	bool players_ship;
	int hitpoints;
	int ship_type;
	friend class Board;
	xy mast[4];

public:
	bool get_ownership(){ return players_ship; }
	void change_ownership(bool ownership)
	{ 
		players_ship = ownership;
	}
	void hitpoint_lost()
	{
		hitpoints--;
	}
	int get_hitpoits()
	{
		return hitpoints;
	}
	void save()
	{
		xy copy_mast[4];
	

		std::ofstream Plik;
		Plik.open("save.txt", std::ios_base::app);

		if (Plik.is_open())
		{
			Plik.fill('0');
			Plik << " Typ: " << ship_type << " flota: " << players_ship << " hitpointy: " << hitpoints;
			for (int i = 0; i < 4; i++)
			{
				Plik << " maszt " << i << ": ";
				Plik.width(2);
				Plik << mast[i].line << " ";
				Plik.width(2);
				Plik << mast[i].column;
			}
			Plik.close();
		}
	}
	void load_from_file(int l_ship_type, bool l_players_ship, int l_hitpoints, xy l_mast[])
	{
		ship_type = l_ship_type;
		players_ship =l_players_ship;
		hitpoints = l_hitpoints;
		mast[0] = l_mast[0];
		mast[1] = l_mast[1];
		mast[2] = l_mast[2];
		mast[3] = l_mast[3];
	}
	virtual void deploy_ship(int column, int line, int current_mast) = 0;
	virtual void show() = 0;	// s³u¿y tylko do debugowania
	virtual bool get_mast_coordinates(int &line, int &column, int counter) = 0;
};



class One_Mast_Ship : public Ship
{

public:
	
	void deploy_ship(int column, int line, int current_mast)
	{ 
		mast[current_mast].column = column;
		mast[current_mast].line = line;
		for (int i = 1; i < 4; i++)
		{
			mast[i].column = 55;	// gdzieœ poza plansz¹
			mast[i].line = 55;
		}
		hitpoints = 1;
		ship_type = 1;
	}
	void show()
	{
		std::cout << "maszt1: " << mast[0].column << mast[0].line << std::endl;
	}
	bool get_mast_coordinates(int &line, int &column, int counter)
	{
		column = mast[counter].column;
		line = mast[counter].line;
		return players_ship;
	}
};

class Two_Mast_Ship : public Ship
{

public:
	void deploy_ship(int column, int line, int current_mast)
	{
		mast[current_mast].column = column;
		mast[current_mast].line = line;
		for (int i = 2; i < 4; i++)
		{
			mast[i].column = 55;	// gdzieœ poza plansz¹
			mast[i].line = 55;
		}
		hitpoints = 2;
		ship_type = 2;
	}
	void show()
	{
		std::cout << "maszt1: " << mast[0].column << mast[0].line << std::endl;
		std::cout << "maszt2: " << mast[1].column << mast[1].line << std::endl;
	}
	bool get_mast_coordinates(int &line, int &column, int counter)
	{
		column = mast[counter].column;
		line = mast[counter].line;
		return players_ship;
	}

};

class Three_Mast_Ship : public Ship
{

public:
	void deploy_ship(int column, int line, int current_mast)
	{
		mast[current_mast].column = column;
		mast[current_mast].line = line;
		mast[3].column = 55;	// gdzieœ poza plansz¹
		mast[3].line = 55;
		hitpoints = 3;
		ship_type = 3;
	}
	void show()
	{
		std::cout << "maszt1: " << mast[0].column << mast[0].line << std::endl;
		std::cout << "maszt2: " << mast[1].column << mast[1].line << std::endl;
		std::cout << "maszt3: " << mast[2].column << mast[2].line << std::endl;
	}
	bool get_mast_coordinates(int &line, int &column, int counter)
	{
		column = mast[counter].column;
		line = mast[counter].line;
		return players_ship;
	}
};

class Four_Mast_Ship : public Ship
{

public:
	void deploy_ship(int column, int line, int current_mast)
	{
		mast[current_mast].column = column;
		mast[current_mast].line = line;
		hitpoints = 4; 
		ship_type = 4;
	}
	void show()
	{
		std::cout << "maszt1: " << mast[0].column << mast[0].line << std::endl;
		std::cout << "maszt2: " << mast[1].column << mast[1].line << std::endl;
		std::cout << "maszt3: " << mast[2].column << mast[2].line << std::endl;
		std::cout << "maszt4: " << mast[3].column << mast[3].line << std::endl;
	}
	bool get_mast_coordinates(int &line, int &column, int counter)
	{
		column = mast[counter].column;
		line = mast[counter].line;
		return players_ship;
	}
};



#endif