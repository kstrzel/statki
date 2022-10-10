#include <iostream>
#include <string>
#include <cstdlib>		//do liczb pseudolosowych
#include <ctime>		//do liczb pseudolosowych	
#include <windows.h>	//kolory w windowsowej konsoli 
#include <exception>
#include "ships.h"
#include "menu.h"
#include "board.h"


int main()
{
	setlocale(LC_ALL, ""); //polskie znaki
	srand(time(NULL));	//liczby pseudolosowe
	
	Menu menu;
	menu.start();
	int ai_4 = menu._4mast_ship, ai_3 = menu._3mast_ship, ai_2 = menu._2mast_ship, ai_1 = menu._1mast_ship; // AI otrzymuje tyle samo statków, ile gracz sobie przyzna³ w menu
	Board board(menu);

	std::vector < Ship* > statki;

	// GRACZ - RÊCZNE USTAWIANIE STATKÓW
	if (menu.ai_phase == false && menu.load_game == false)
	{
		board.draw(menu, -1, -1, -1, -1);
		std::cout << std::endl << std::endl << "		FAZA USTAWIANIA.";
		while (menu._4mast_ship > 0 || menu._3mast_ship > 0 || menu._2mast_ship > 0 || menu._1mast_ship > 0)
		{
			std::cout << std::endl << std::endl << "Wybierz, jaki statek chcesz ustawiæ:" << std::endl;
			std::cout << "(aby zakoñczyæ grê wpisz: close)" << std::endl;
			menu.ship_choice(statki);
			statki[statki.size() - 1]->change_ownership(true);
			board.deploy_ship(statki, menu);
			board.draw(menu, -1, -1, -1, -1);
			//statki[statki.size()-1]->show();
		}
	}
	
	if (menu.ai_phase == true && menu.load_game == false)
	{
		while (menu._4mast_ship > 0 || menu._3mast_ship > 0 || menu._2mast_ship > 0 || menu._1mast_ship > 0)
		{
			menu.ai_ship_choice(statki);
			statki[statki.size() - 1]->change_ownership(true);
			board.deploy_ship(statki, menu);
		}
	}
	std::cout << std::endl;
	if (menu.load_game == false)
	{
		menu.ai_phase = true;
		menu._4mast_ship = ai_4, menu._3mast_ship = ai_3, menu._2mast_ship = ai_2, menu._1mast_ship = ai_1; // regeneracja puli statków dla przeciwnika

		while (menu._4mast_ship > 0 || menu._3mast_ship > 0 || menu._2mast_ship > 0 || menu._1mast_ship > 0)
		{
			menu.ai_ship_choice(statki);
			statki[statki.size() - 1]->change_ownership(false);
			board.deploy_ship(statki, menu);
		}
		board.draw(menu, -1, -1, -1, -1);
	}

	// ROZPOCZÊCIE GRY
	if (menu.load_game == false)
	{
		menu.colouring(11, "\n\n\n			Bitwa siê rozpoczyna!");
		std::cout << std::endl << std::endl << "Wskazówki:\nAby strzeliæ, wpisz wspó³rzêdne pola, i naciœnij ENTER. \n(format wspó³rzêdnych: litera i liczba, np c7 albo B10)." << std::endl;
		std::cout << "Aby zapisaæ grê, wpisz: save i naciœnij ENTER." << std::endl;
		std::cout << "Aby wczytaæ grê, wpisz: load i naciœnij ENTER." << std::endl;
		std::cout << "Aby zakoñczyæ grê, wpisz: close i naciœnij ENTER." << std::endl;
		std::cout << "Pola wokó³ zatopionego statku zostan¹ automatycznie zaznaczone jako ostrzelane." << std::endl << std::endl;
	}
	else
		board.load(menu, statki);

	//int a = statki.size();	//iloœæ statków w vectorze
	//std::cout << a << std::endl;//

	std::string input;
	xy coordinates;
	int line, column;
	int previous_line, previous_column;
	bool skip_players_turn = false;
	bool players_victory = false;
	while (menu.game_over == false)
	{
			if (skip_players_turn == false)
			{
				// Tura gracza
				std::cout << std::endl << std::endl << "Twoja tura. Gdzie strzelasz? " << std::endl;
				std::cout << "(aby zapisaæ grê, wpisz: save, aby wczytaæ: load, aby zamkn¹æ grê: close)" << std::endl;
				std::cin >> input;
				if (input == "save" || input == "SAVE")
				{
					board.save(menu);
					for (int i = 0; i < statki.size(); i++)
					{
						statki[i]->save();
					}
					continue;
				}
				if (input == "load" || input == "LOAD")
				{
					board.load(menu, statki);
					continue;
				}
				if (input == "close" || input == "CLOSE")
				{
					system("PAUSE");
					exit(0);
				}
				coordinates = board.coordinates_conversion(input);
				line = coordinates.line;
				column = coordinates.column;
				if (column < 0 || column >= board.get_number_of_columns() || line < 0 || line >= board.get_number_of_lines())
				{
					menu.colouring(12, "Niepoprawne wspó³rzêdne!\n");
					continue;
				}
				if (board.ai_board[line][column] == menu.mark_hit || board.ai_board[line][column] == menu.mark_mishit)
				{
					menu.colouring(12, "To pole by³o ju¿ ostrzelane, wybierz inne.");
					continue;
				}
				if (board.ai_hidden_board[line][column] == menu.mark_ship)
				{
					std::cout << std::endl << "Okrêt przeciwnika zosta³ ";
					menu.colouring(11); //jasnoniebieski
					std::cout << "TRAFIONY";
					menu.colouring(7); //normalny
					board.ai_board[line][column] = menu.mark_hit;
					board.ai_hidden_board[line][column] = menu.mark_hit;
					board.check_destruction(statki, menu, line, column, 1);	// SprawdŸ, czy okrêt zosta³ zatopiony; jeœli tak, otocz pud³ami. Jedynka oznacza, ¿e strzela³ gracz.
				}
				else
				{
					std::cout << std::endl << "PUD£O!" << std::endl;
					board.ai_board[line][column] = menu.mark_mishit;
					board.ai_hidden_board[line][column] = menu.mark_mishit;
				}
				players_victory = board.check_game_over(menu, 1);	// sprawdŸ, czy to koniec gry, a jeœli tak, to kto zwyciê¿y³
			}
		

		//Tura AI
		if (menu.game_over == false)
		{
			if (skip_players_turn == false)
			{
				previous_line = line;
				previous_column = column;
			}

			skip_players_turn = false;
			if (menu.difficulty_easy == true)	// wybór celu na ³atwym stopniu trudnoœci (strzelaj losowo)
			{
				column = (rand() % board.get_number_of_columns());
				line = (rand() % board.get_number_of_lines());
			}
			else	// wybór celu na normalnym stopniu trudnoœci (zatop uszkodzony statek, a jeœli brak uszkodzonych, strzelaj losowo)				
				board.sink_damaged(menu, line, column);

			if (board.player_board[line][column] == menu.mark_hit || board.player_board[line][column] == menu.mark_mishit)
			{
				skip_players_turn = true;
				continue;
			}
			char number_to_letter = column + 97;
			std::cout << std::endl << "Przeciwnik strzela: " << number_to_letter << line << ". ";
			if (board.player_board[line][column] == menu.mark_ship)
			{
				std::cout << "Twój okrêt zosta³ ";
				menu.colouring(12); //czerwony
				std::cout << "TRAFIONY";
				menu.colouring(7); //normalny
				board.player_board[line][column] = menu.mark_hit;
				board.check_destruction(statki, menu, line, column, 0);	// SprawdŸ, czy okrêt zosta³ zatopiony; jeœli tak, otocz pud³ami. Zero oznacza, ¿e strzela³ komputer.
			}
			else
			{
				std::cout << "Pud³o." << std::endl;
				board.player_board[line][column] = menu.mark_mishit;
			}
			std::cout << std::endl << std::endl;
			board.draw(menu, line, column, previous_line, previous_column);
			players_victory = board.check_game_over(menu, 0);	// sprawdŸ, czy to koniec gry, a jeœli tak, to kto zwyciê¿y³
		}
	}
		
	if (players_victory == true)
		{
			previous_line = line;
			previous_column = column;
			line = -30; // niepotrzebne, bo AI nie oddaje strza³u w ostatniej turze, wiêc wyrzuciæ poza planszê
			column = -30; 
			std::cout << std::endl << std::endl;
			board.draw(menu, line, column, previous_line, previous_column);
			menu.colouring(12, "\n\n\n	FLOTA PRZECIWNIKA ZOSTA£A UNICESTWIONA!ODNOSISZ ZWYCIÊSTWO!\n\n");
		}
	else
	{
		menu.colouring(12, "\n\n\n			KLÊSKA! TWOJA FLOTA ZOSTA£A UNICESTWIONA.\n\n");
	}
	
	system("PAUSE");
	exit(0);

	return 0;
}





