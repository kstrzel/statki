#ifndef menu_h
#define menu_h

#include <iostream>
#include <string>
#include <vector>


// OPCJE - zawarto�� poni�szych zmiennych mo�na zmieni� w menu gry
char znak_statku, znak_trafienia, znak_pudlo;
int ships_total_number = 0;
int czteromasztowce, trojmasztowce, dwumasztowce, jednomasztowce;


class Menu
{
private:
	friend class Board;
	int number_of_columns, number_of_lines;
;

public:
	bool ai_phase, load_game, game_over, difficulty_easy;
	int _4mast_ship, _3mast_ship, _2mast_ship, _1mast_ship;
	int ship_type; //4-czteromasztowiec, 3-tr�jmasztowiec, 2-dwumasztowiec, 1-jednomasztowiec
	char mark_ship, mark_hit, mark_mishit;

	Menu()
	{
		game_over = false;
		load_game = false;
		// OPCJE - zawarto�� poni�szych zmiennych mo�na zmieni� w menu gry
		ai_phase = true;
		difficulty_easy =false;
		mark_ship = 'O';
		mark_hit = 'X';
		mark_mishit = '*';
		number_of_columns = 10;
		number_of_lines = 10;
		_4mast_ship = 1;
		_3mast_ship = 2;
		_2mast_ship = 3;
		_1mast_ship = 4;
		ship_type = 0;
	}
	void colouring(int colour)
	{
		HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hcon, colour); 	//	7 - normalny, 11 - jasnoniebieski, 12 - czerwony
	}
	void colouring(int colour, std::string text)
	{
		HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hcon, colour); 	//	7 - normalny, 11 - jasnoniebieski, 12 - czerwony
		std::cout << std::endl << text << std::endl;
		SetConsoleTextAttribute(hcon, 7); 	//	7 - normalny, 11 - jasnoniebieski, 12 - czerwony	
	}


	void start()
	{

		std::string wybor_opcji;
		while (1)
		{
			colouring(11, "				GRA W STATKI\n\n\n				1.NOWA GRA\n				2.WCZYTAJ GR�\n				3.OPCJE\n				4.WYJ�CIE\n\n");
			std::cout << "Wskaz�wka: aby rozpocz�� now� gr�, wpisz: 1 i naci�nij ENTER.\nPost�puj analogicznie, by wybiera� inne opcje." << std::endl << std::endl;
			std::cin >> wybor_opcji;
			switch (wybor_opcji[0] - 48)
			{
			case 1:
				break;
			case 2:
				load_game = true;
				break;
			case 3:
				options();
				continue;
			case 4:
				system("PAUSE");
				exit(0);
			default:
				continue;
			}
			break;
		}
	}
	
	void options()
	{
		while (game_over == false)
		{


				std::cout << std::endl << "Kt�r� opcj� chcesz zmieni�?" << std::endl << std::endl;
				std::cout << "1.Symbol statku: " << mark_ship << std::endl;
				std::cout << "2.Symbol trafienia: " << mark_hit << std::endl;
				std::cout << "3.Symbol pud�a: " << mark_mishit << std::endl;
				std::cout << "4.Liczba czteromasztowc�w: " << _4mast_ship << std::endl;
				std::cout << "5.Liczba tr�jmasztowc�w: " << _3mast_ship << std::endl;
				std::cout << "6.Liczba dwumasztowc�w: " << _2mast_ship << std::endl;
				std::cout << "7.Liczba jednomasztowc�w: " << _1mast_ship << std::endl;
				std::cout << "8.Ustawianie statk�w gracza: ";
				if (ai_phase == true)
					colouring(12); //czerwony
				else
					colouring(11); //jasnoniebieski
				std::cout << ai_phase << "		 ";
				colouring(11); //jasnoniebieski
				std::cout << "0-r�czne, ";
				colouring(12); //czerwony
				std::cout << "1-losowe" << std::endl;
				colouring(7); //normalny
				std::cout << "9.Liczba wierszy planszy (min 5, max 15): " << number_of_lines << std::endl;
				std::cout << "10.Liczba kolumn planszy (min 5, max 15): " << number_of_columns << std::endl;
				colouring(11); //jasnoniebieski
				std::cout << "11.Poziom trudno�ci: ";
				if (difficulty_easy == false)
				{
					colouring(12); //czerwony
					std::cout << "normalny		 ";
				}
				else
				{
					colouring(11); //jasnoniebieski
					std::cout << "�atwy		 ";
				}
				colouring(12); //czerwony
				std::cout << "0-normalny, ";
				colouring(11); //jasnoniebieski
				std::cout << "1-�atwy" << std::endl;
				colouring(7); //normalny
				std::cout << std::endl << "12.Powr�t do menu gry" << std::endl;
				std::string choice;
				std::cin >> choice;

				try{
					if (stoi(choice) >= 1 && stoi(choice) < 12)
					{
						if (stoi(choice) > 3 && stoi(choice) < 8)
							colouring(12, "UWAGA! Wybranie zbyt du�ej ilo�ci statk�w, by zmie�ci�y si� na planszy\nuniemo�liwi gr�.");
						std::cout << std::endl << "Wpisz now� warto��:";
						std::string input;
						std::cin >> input;
						switch (stoi(choice))
						{
						case 1:
							mark_ship = input[0];
							continue;
						case 2:
							mark_hit = input[0];
							continue;
						case 3:
							mark_mishit = input[0];
							continue;
						case 4:
							_4mast_ship = stoi(input);
							if (_4mast_ship < 0)
							{
								_4mast_ship = 0;
								colouring(12, "Liczba statk�w nie mo�e by� ujemna, ustawiam na zero.");
							}
							continue;
						case 5:
							_3mast_ship = stoi(input);
							if (_3mast_ship < 0)
							{
								_3mast_ship = 0;
								colouring(12, "Liczba statk�w nie mo�e by� ujemna, ustawiam na zero.");
							}
							continue;							
						case 6:
							_2mast_ship = stoi(input);
							if (_2mast_ship < 0)
							{
								_2mast_ship = 0;
								colouring(12, "Liczba statk�w nie mo�e by� ujemna, ustawiam na zero.");
							}
							continue;
						case 7:
							_1mast_ship = stoi(input);
							if (_1mast_ship < 0)
							{
								_1mast_ship = 0;
								colouring(12, "Liczba statk�w nie mo�e by� ujemna, ustawiam na zero.");
							}
							continue;
						case 8:
							if (stoi(input) == 1)
								ai_phase = true;
							else if (stoi(input) == 0)
								ai_phase = false;
							else
								colouring(12, "B��d! Poprawne warto�ci to 0 i 1");
							continue;
						case 9:
							number_of_lines = stoi(input);
							continue;
						case 10:
							number_of_columns = stoi(input);
							continue;
						case 11:
							if (stoi(input) == 1)
								difficulty_easy = true;
							else if (stoi(input) == 0)
								difficulty_easy = false;
							else
								colouring(12, "B��d! Poprawne warto�ci to 0 i 1");
							continue;
						default:
							colouring(12, "error!");
							continue;
						}
					}
					else if (choice == "12")
					{
						if (number_of_columns >= 5 && number_of_columns <= 15 && number_of_lines >= 5 && number_of_lines <= 15)
							if ((_1mast_ship + _2mast_ship + _3mast_ship + _4mast_ship) > 0)
								break;
							else
								colouring(12, "Nie mo�esz gra� w statki bez statk�w!");
						else
							colouring(12, "Niepoprawna wielko�� planszy! Minimalna ilo�� wierszy i kolumn to 5, maksymalna to 15.");
					}
					else
						colouring(12, "error!");
				}
				catch (std::invalid_argument& e){
					colouring(12, "B��d! Liczba statk�w, wielko�� planszy, tryb ustawiania i poziom trudno�ci\n musz� by� okre�lone LICZBAMI.");
				}
				catch (std::out_of_range& e){
					colouring(12, "B��d! Za du�a liczba.");
				}
				catch (...){
					colouring(12, "Wyst�pi� nieprzewidziany b��d, tw�j komputer zaraz wybuchnie...");
				}		
		}
	}

	//wyb�r statku i odj�cie go od puli nieustawionych statk�w
	void ship_choice(std::vector < Ship* > &statki)
	{
		while (1)
		{
			std::cout << std::endl;
			std::cout << "1. Ustawiam jednomasztowiec (pozosta�o " << _1mast_ship << "):" << std::endl;
			std::cout << "2. Ustawiam dwumasztowiec (pozosta�o " << _2mast_ship << "):" << std::endl;
			std::cout << "3. Ustawiam tr�jmasztowiec (pozosta�o " << _3mast_ship << "):" << std::endl;
			std::cout << "4. Ustawiam czteromasztowiec (pozosta�o " << _4mast_ship << "):" << std::endl;
			std::string user_input;
			std::cin >> user_input;
			if (user_input == "close" || user_input == "CLOSE")
			{
				system("PAUSE");
				exit(0);
			}
			ship_type = user_input[0] - 48;
			if (ship_type == 1 && _1mast_ship > 0)
			{
				_1mast_ship--;
				ship_type = 1;
				One_Mast_Ship* pointer = new One_Mast_Ship;
				statki.push_back(pointer);
				break;
			}
			if (ship_type == 2 && _2mast_ship > 0)
			{
				_2mast_ship--;
				ship_type = 2;
				Two_Mast_Ship* pointer = new Two_Mast_Ship;
				statki.push_back(pointer);
				break;
			}
			if (ship_type == 3 && _3mast_ship > 0)
			{
				_3mast_ship--;
				ship_type = 3;
				Three_Mast_Ship* pointer = new Three_Mast_Ship;
				statki.push_back(pointer);
				break;
			}
			if (ship_type == 4 && _4mast_ship > 0)
			{
				_4mast_ship--;
				ship_type = 4;
				Four_Mast_Ship* pointer = new Four_Mast_Ship;
				statki.push_back(pointer);
				break;
			}
			
			colouring(12, "error!\n");
				continue;
		}
	}
	void ai_ship_choice(std::vector < Ship* > &statki)
	{
		if (_4mast_ship > 0)
		{
			_4mast_ship--;
			ship_type = 4;
			Four_Mast_Ship* pointer = new Four_Mast_Ship;
			statki.push_back(pointer);
			return;
		}
		if (_3mast_ship > 0)
		{
			_3mast_ship--;
			ship_type = 3;
			Three_Mast_Ship* pointer = new Three_Mast_Ship;
			statki.push_back(pointer);
			return;
		}
		if (_2mast_ship > 0)
		{
			_2mast_ship--;
			ship_type = 2;
			Two_Mast_Ship* pointer = new Two_Mast_Ship;
			statki.push_back(pointer);
			return;
		}
		if (_1mast_ship > 0)
		{
			_1mast_ship--;
			ship_type = 1;
			One_Mast_Ship* pointer = new One_Mast_Ship;
			statki.push_back(pointer);
			return;
		}
	}




};


#endif