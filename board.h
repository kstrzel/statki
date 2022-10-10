#ifndef board_h
#define board_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>		//zapisywanie i wczytywanie	

class Board
{
private:
	int number_of_lines;
	int number_of_columns;;

public:
	char player_board[15][15];
	char ai_board[15][15];
	char ai_hidden_board[15][15];

	Board(Menu menu) : number_of_columns(menu.number_of_columns), number_of_lines(menu.number_of_lines)
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				player_board[i][j] = ' ';
				ai_board[i][j] = ' ';
				ai_hidden_board[i][j] = ' ';
			}
		}
	}

	int get_number_of_lines()
	{
		return number_of_lines;
	}
	int get_number_of_columns()
	{
		return number_of_columns;
	}

	void colouring(int colour, std::string text)
	{
		HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hcon, colour); 	//	7 - normalny, 11 - jasnoniebieski, 12 - czerwony
		std::cout << std::endl << text << std::endl;
		SetConsoleTextAttribute(hcon, 7); 	//	7 - normalny, 11 - jasnoniebieski, 12 - czerwony	
	}

	void draw(Menu &menu, int line, int column, int previous_line, int previous_column)
	{
		std::cout << "   ";
		for (int i = 0; i < number_of_columns; i++)
		{
			printf("%c ", 65 + i); // numeracja kolumn pierwszej planszy
		}
		std::cout << "             ";
		for (int i = 0; i < number_of_columns; i++)
		{
			printf("%c ", 65 + i); // numeracja kolumn drugiej planszy
		}

		for (int i = 0; i < number_of_lines; i++)
		{
			std::cout << std::endl;
			if (i < 9)
			{
				std::cout << " ";		// wyr�wnanie liczb jednocyftrowych do dwucyfrowych
			}
			std::cout << i + 1 << " ";	//numeracja wierszy pierwszej planszy
			for (int j = 0; j < number_of_columns; j++) //wy�wietla wiersz pierwszej planszy
			{
				if (player_board[i][j] == menu.mark_ship)
				{
				menu.colouring(11); //jasnoniebieski
				std::cout << player_board[i][j] << " "; //zaznaczenie kolorem statk�w gracza
				menu.colouring(7); //normalny
				}
				else if (i == line && j == column)
				{
				menu.colouring(12); //czerwony
				std::cout << player_board[i][j] << " "; //zaznaczenie na czerwono ostatniego strza�u przeciwnika
				menu.colouring(7); //normalny

				}
				else
				std::cout << player_board[i][j] << " ";
			}
			std::cout << "          ";			// przerwa mi�dzy planszami
			if (i < 9)
			{
				std::cout << " ";
			}
			std::cout << i + 1 << " ";	//numeracja wierszy drugiej planszy
			for (int j = 0; j < number_of_columns; j++) //wy�wietla wiersz drugiej planszy obok pierwszej
			{
				if (i == previous_line && j == previous_column)
				{
				menu.colouring(11); //jasnoniebieski
				std::cout << ai_board[i][j] << " ";	// zaznaczenie na niebiesko ostatniego strza�u gracza
				menu.colouring(7); //normalny

				}
				else
				std::cout << ai_board[i][j] << " ";
			}
		}
	}

	xy coordinates_conversion(std::string input)
	{
		int line, column;
		if (input[0] >= 'A' && input[0] <= 'Z') // je�li du�a litera, to zamie� na ma��
			input[0] += 32;
		if (isalpha(input[0]) && isdigit(input[1]) && input.size() <= 3 && input.size() > 1)
		{
			column = input[0] - 97;
			if (input.size() == 3)	// wsp�rz�dna liczbowa jest dwucyfrowa
				line = (input[1] - 48) * 10 + input[2] - 49;
			if (input.size() == 2) // wsp�rz�dna liczbowa jest jednocyfrowa
				line = input[1] - 49;
		}
		else
			return xy(-5, -5);	//zwr�� nieprawid�owe wsp�rz�dne

		return xy(line, column);
	}

	bool collision(int line, int column, Menu &menu, bool players_ship) // sprawd�, czy na podanym polu lub kt�rymkolwiek z s�siednich jest ju� ustawiony statek
	{
		for (int i = line - 1; i <= line + 1; i++)
		{
			for (int j = column - 1; j <= column + 1; j++)
			{
				if (players_ship == true && player_board[i][j] == menu.mark_ship)
				{
					return true;
				}
				if (players_ship == false && ai_hidden_board[i][j] == menu.mark_ship)
				{
					return true;
				}
			}
		}

		return false;
	}
	void deploy_ship(std::vector < Ship* > &statki, Menu &menu) 
	{
		bool players_ship = statki[statki.size() - 1]->get_ownership();	//sprawd�, do kogo nale�y ustawiany statek: gracz true, ai false
		std::string user_input, horizontal_or_vertical;
		while (1)
		{
			
			xy coordinates;
			bool ship_horizontal = false;
			if (menu.ai_phase == false) // R�czne ustawianie statk�w przez gracza
			{
				while (menu.ship_type != 1)
				{
					std::cout << std::endl << "Ustawiasz pionowo czy poziomo?" << std::endl << std::endl << "1.Pionowo" << std::endl << "2.Poziomo" << std::endl << std::endl;
					std::cin >> horizontal_or_vertical;
					if (horizontal_or_vertical.size() == 1 && (horizontal_or_vertical == "1" || horizontal_or_vertical == "2"))
					{
						if (horizontal_or_vertical == "2")
							ship_horizontal = true;
						else
							ship_horizontal = false;
						break;
					}
					else
						colouring(12, "error!");

				}
				std::cout << std::endl << "Wpisz wsp�rz�dne pocz�tku statku i naci�nij ENTER\n(format wsp�rz�dnych: litera a po niej liczba, np. c1, A7, g10)" << std::endl;
				std::cin >> user_input;
				coordinates = coordinates_conversion(user_input);
			}
			else // losowe ustawianie statk�w
			{
				coordinates.column = (rand() % get_number_of_columns());
				coordinates.line = (rand() % get_number_of_lines());
				if (rand() % 100 < 50) // 50%
					ship_horizontal = true;
				else
					ship_horizontal = false;
			}
			
			xy ship_end_coordinates;
			if (ship_horizontal == true)
			{
				ship_end_coordinates.column = coordinates.column + menu.ship_type - 1;
				ship_end_coordinates.line = coordinates.line;
			}
			else
			{
				ship_end_coordinates.column = coordinates.column;
				ship_end_coordinates.line = coordinates.line + menu.ship_type - 1;
			}
			
			bool rules_compatible = false;
			if (coordinates.column < get_number_of_columns() && ship_end_coordinates.column < get_number_of_columns() && coordinates.line < get_number_of_lines() && ship_end_coordinates.line < get_number_of_lines())
			{
				if (coordinates.column >= 0 && ship_end_coordinates.column >= 0 && coordinates.line >= 0 && ship_end_coordinates.line >= 0)
					rules_compatible = true;
			}
			
			bool conflicts = false;
			if (rules_compatible == true)
			{
				for (int i = 0; i < menu.ship_type; i++)
				{
					if (ship_horizontal == true)
						conflicts = collision(coordinates.line, coordinates.column + i, menu, players_ship);	// sprawd�, czy nie ma kolizji z innym statkiem - dla statku w poziomie
					else
						conflicts = collision(coordinates.line + i, coordinates.column, menu, players_ship);	// sprawd�, czy nie ma kolizji z innym statkiem - dla statku w pionie
					if (conflicts == true)	// je�li dany maszt jest �le ustawiony, to nie sprawdzaj kolejnych
						break;
				}
			}
			if (rules_compatible == true && conflicts == false)
				if (ship_horizontal == true)
				{
					for (int i = 0; i < menu.ship_type; i++)	// rysuj statek	w poziomie	
					{
						statki[statki.size() - 1]->deploy_ship(coordinates.column + i, coordinates.line, i);  
						if (players_ship == true)
							player_board[coordinates.line][coordinates.column + i] = menu.mark_ship;		
						else
							ai_hidden_board[coordinates.line][coordinates.column + i] = menu.mark_ship;

					}
					break;
				}
				else
				{
					for (int i = 0; i < menu.ship_type; i++)	// rysuj statek w pionie
					{
						statki[statki.size() - 1]->deploy_ship(coordinates.column, coordinates.line + i, i); 
						if (players_ship == true)
							player_board[coordinates.line + i][coordinates.column] = menu.mark_ship;
						else
							ai_hidden_board[coordinates.line + i][coordinates.column] = menu.mark_ship;

					}
					break;
				}
			
			if (conflicts == true)
			{
				if (menu.ai_phase == false)
				{
					colouring(12, "\nUWAGA! Kolizja ze statkiem ustawionym wcze�niej!\nWybierz statek jeszcze raz i ustaw go w innym miejscu.\n"); //czerwony
				}
				statki.pop_back();
				switch (menu.ship_type) //	zwrot statku do puli nieustawionych
				{
				case 1:
					menu._1mast_ship++;
					break;
				case 2:
					menu._2mast_ship++;
					break;
				case 3:
					menu._3mast_ship++;
					break;
				case 4:
					menu._4mast_ship++;
					break;
				}
				break; //	powr�t do wybierania statku
			}

			
			if (menu.ai_phase == false)
				colouring(12, "ERROR! B��dne wsp�rz�dne!\nUstaw statek jeszcze raz.");		//Przy poprawnym ustawieniu statku wcze�niej musia� nast�pi� break.
		}
		
			
	}

	void check_destruction(std::vector < Ship* > &statki, Menu &menu, int line, int column, bool players_shot)
	{
		bool dummy = false;
		int temp_line = -1;
		int temp_column = -1;
		bool players_ship;
		for (int i = 0; i < statki.size(); i++)	// przejd� po statkach
		{
			
			for (int j = 0; j < 4; j++)	//przejd� po masztach
			{
				players_ship = (statki[i]->get_mast_coordinates(temp_line, temp_column, j));
				if (players_ship != players_shot && temp_line == line && temp_column == column) //Je�eli maszt tego statku zosta� trafiony...
				{
					statki[i]->hitpoint_lost();	//...to odejmij punkt wytrzyma�o�ci...
					if (statki[i]->get_hitpoits() < 1)//...a je�li po odj�ciu pozosta�o 0 punkt�w...
					{		
						for (int k = 0; k < 4; k++)	//...to przejd� po masztach jeszcze raz...
						{
							dummy = statki[i]->get_mast_coordinates(temp_line, temp_column, k);	//...wczytaj ich wsp�rz�dne... 
							for (int l = temp_line - 1; l <= temp_line + 1; l++)							
								for (int m = temp_column - 1; m <= temp_column + 1; m++)
									if (players_shot == true)
									{
										if (ai_board[l][m] != menu.mark_hit && l >= 0 && l < menu.number_of_lines && m >= 0 && m < menu.number_of_columns)
										{
											ai_board[l][m] = menu.mark_mishit;			// otocz pud�ami maszt zatopionego statku
											ai_hidden_board[l][m] = menu.mark_mishit;
										}
									}
									else
									{
										//std::cout << "linia :" << l << "kolumna: " << m << std::endl;
										if (player_board[l][m] != menu.mark_hit && l >=0 && l < menu.number_of_lines && m >= 0 && m < menu.number_of_columns)
											player_board[l][m] = menu.mark_mishit;
									}																					
						}
						if (players_shot == true)
							menu.colouring(11); //jasnoniebieski
						else
							menu.colouring(12); //czerwony
						std::cout << " I ZATOPIONY!";
						menu.colouring(7); //normalny
						return;						
					}
				}
				else
				{
					continue;
				}
			}
		}		
		return;
	}
			
				
	bool check_game_over(Menu &menu, bool players_turn)
	{
		for (int i = 0; i < menu.number_of_lines; i++)
			for (int j = 0; j < menu.number_of_columns; j++)
			{
				if (players_turn == false)
				{
					if (player_board[i][j] == menu.mark_ship)
						return false;
				}
				else if (ai_hidden_board[i][j] == menu.mark_ship)
					return false;
			}
		
		menu.game_over = true;
		if (players_turn == true)
			return true;
		else
			return false;
	}
	

	void save_conversion(char board[15][15], char copy_board[15][15], Menu menu)
	{

		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (board[i][j] == menu.mark_mishit)
					copy_board[i][j] = '1';
				if (board[i][j] == menu.mark_ship)
					copy_board[i][j] = '2';
				if (board[i][j] == menu.mark_hit)
					copy_board[i][j] = '3';
				if (board[i][j] == ' ')
					copy_board[i][j] = '0';
			}
		}
	}

	void save(Menu &menu)
	{

		char copy_player_board[15][15] = {};
		char copy_ai_board[15][15] = {};
		char copy_ai_hidden_board[15][15] = {};

		std::ofstream Plik;
		Plik.open("save.txt", std::ios_base::out);
		
		if (Plik.is_open())
		{
			save_conversion(player_board, copy_player_board, menu);
			save_conversion(ai_board, copy_ai_board, menu);
			save_conversion(ai_hidden_board, copy_ai_hidden_board, menu);
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					Plik << copy_player_board[i][j];
				}
			}
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					Plik << copy_ai_board[i][j];
				}
			}
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					Plik << copy_ai_hidden_board[i][j];
				}
			}
			if (number_of_columns < 10)
				Plik << "0";
			Plik << number_of_columns;
			if (number_of_lines < 10)
				Plik << "0";
			Plik << number_of_lines;


			Plik.close();

			std::cout << std::endl << "Gra zosta�a zapisana." << std::endl;
		}
	}

	void load(Menu &menu, std::vector < Ship* > &statki)
	{
		std::ifstream Plik;
		Plik.open("save.txt", std::ios_base::in);

		if (Plik.is_open())
		{
			std::string save_content;
			while (Plik.good())
			{
				getline(Plik, save_content);
				for (int i = 0; i < 675; i++)
				{
					if (save_content[i] == '1')
						save_content[i] = menu.mark_mishit;
					if (save_content[i] == '2')
						save_content[i] = menu.mark_ship;
					if (save_content[i] == '3')
						save_content[i] = menu.mark_hit;
					if (save_content[i] == '0')
						save_content[i] = ' ';
				}
				
				for (int i = 0; i < 15; i++)
				{
					for (int j = 0; j < 15; j++)
					{
						//wczytywanie plansz
						player_board[i][j] = save_content[i * 15 + j];	
						ai_board[i][j] = save_content[i * 15 + j + 225];
						ai_hidden_board[i][j] = save_content[i * 15 + j + 450];
					}
				}
				number_of_columns = (save_content[675] - '0') * 10 + save_content[676] - '0';
				number_of_lines = (save_content[677] - '0') * 10 + save_content[678] - '0';
				save_content.erase(0, 678);	//wykasowanie wczytanych "sta�ych" danych ze stringa
				statki.clear();	//opr�nienie pojemnika wspa�nik�w na statki (obiekty)
				
				for (int i = 0; i < save_content.size()-1; i += 89)	// p�tla przechodz�ca od statku do statku
				{
					// wczytanie danych statku do zmiennych, celem ich przekazania do metody tworz�cej statek
					int l_ship_type = save_content[i + 7] - '0';
					bool l_players_ship;
					if (save_content[i + 16] == '1')
						l_players_ship = true;
					else
						l_players_ship = false;
					int l_hitpoints = save_content[i + 29] - '0';
					xy l_mast[4]; 
					for (int j = 0; j < 4; j++)
					{
						l_mast[j].line = (save_content[i + 29 + 11 + j * 15] - '0') * 10 + save_content[i + 29 + 12 + j * 15] - '0';
						l_mast[j].column = (save_content[i + 29 + 14 + j * 15] - '0') * 10 + save_content[i + 29 + 15 + j * 15] - '0';
					}
					
					//std::cout << save_content.size() << "   i = " << i << "TYP: " << l_ship_type << "Flota: " << l_players_ship << "Hitpoity: " << l_hitpoints << " maszt 1: " << l_mast[2].line << " " << l_mast[2].column << std::endl;
					
					//tworzenie statku
					if (l_ship_type == 1)
					{
						One_Mast_Ship* pointer = new One_Mast_Ship;
						pointer->load_from_file(l_ship_type, l_players_ship, l_hitpoints, l_mast);
						statki.push_back(pointer);
					}
					else if (l_ship_type ==2)
					{
						Two_Mast_Ship* pointer = new Two_Mast_Ship;
						pointer->load_from_file(l_ship_type, l_players_ship, l_hitpoints, l_mast);
						statki.push_back(pointer);
					}
					else if (l_ship_type == 3)
					{
						Three_Mast_Ship* pointer = new Three_Mast_Ship;
						pointer->load_from_file(l_ship_type, l_players_ship, l_hitpoints, l_mast);
						statki.push_back(pointer);
					}
					else if (l_ship_type == 4)
					{
						Four_Mast_Ship* pointer = new Four_Mast_Ship;
						pointer->load_from_file(l_ship_type, l_players_ship, l_hitpoints, l_mast);
						statki.push_back(pointer);
					}
					else
					{
						std::cout << "ERROR! Nie mo�na wczyta� zapisanej gry!" << std::endl;
						std::cout << "Gra zostanie zamkni�ta." << std::endl;
						system("PAUSE");
						exit(0);
					}							
				}


				
			}
			Plik.close();
			std::cout << "Gra wczytana." << std::endl << std::endl;

			draw(menu, -1, -1, -1, -1);
			//std::cout << statki.size() << std::endl << std::endl;

		}
		else
		{
			std::cout << "B��d open(): sprawd�, czy plik save.txt znajduje si� w odpowiednim katalogu." << std::endl;
			std::cout << "Gra zostanie zamkni�ta." << std::endl;
			system("PAUSE");
			exit(0);
		}
		
	}
	
void sink_damaged(Menu &menu, int &line, int &column)
{

	for (int i = 0; i < menu.number_of_lines; i++)
	{
		for (int j = 0; j < menu.number_of_columns; j++)
		{
			if (player_board[i][j] == menu.mark_hit)
			{
				if (player_board[i][j + 1] != menu.mark_hit && player_board[i][j + 1] != menu.mark_mishit && j + 1 < menu.number_of_columns)	// je�li pole nie zosta�o jeszcze ostrzelane
					if (player_board[i + 1][j] != menu.mark_hit && player_board[i - 1][j] != menu.mark_hit) // je�li pole nie s�siaduje po skosie z trafionym wcze�niej masztem...
						if (player_board[i + 1][j + 2] != menu.mark_hit && player_board[i - 1][j + 2] != menu.mark_hit)// ci�g dalszy powy�szego
							{
								line = i;
								column = j + 1;
								return;
							}

				if (player_board[i + 1][j] != menu.mark_hit && player_board[i + 1][j] != menu.mark_mishit && i + 1 < menu.number_of_lines)
					if (player_board[i][j + 1] != menu.mark_hit && player_board[i][j - 1] != menu.mark_hit)
						if (player_board[i + 2][j + 1] != menu.mark_hit && player_board[i + 2][j - 1] != menu.mark_hit)
						{
							line = i + 1;
							column = j;
							return;
						}
				if (player_board[i - 1][j] != menu.mark_hit && player_board[i - 1][j] != menu.mark_mishit && i - 1 >= 0)
					if (player_board[i - 2][j + 1] != menu.mark_hit && player_board[i - 2][j - 1] != menu.mark_hit)
						if (player_board[i][j + 1] != menu.mark_hit && player_board[i][j - 1] != menu.mark_hit)
						{
							line = i - 1;
							column = j;
							return;
						}
				if (player_board[i][j - 1] != menu.mark_hit && player_board[i][j - 1] != menu.mark_mishit && j - 1 >= 0)
					if (player_board[i - 1][j] != menu.mark_hit && player_board[i + 1][j] != menu.mark_hit)
						if (player_board[i - 1][j - 2] != menu.mark_hit && player_board[i + 1][j - 2] != menu.mark_hit)
						{
							line = i;
							column = j - 1;
							return;
						}
			}
		}
	}
	column = (rand() % number_of_columns);
	line = (rand() % number_of_lines);

}

};
#endif
