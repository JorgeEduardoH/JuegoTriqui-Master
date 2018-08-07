#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

using std::cout;
using std::cin;
using std::endl;

#define WIN 1000
#define	DRAW 0
#define LOSS -1000

#define SIMBOLO_MAQUINA 'O'
#define SIMBOLO_JUGADOR 'X'
#define SIMBOLO_ESPACIO_VACIO '-'

#define COMIENZO_PROFUNDIDAD 0

// Print game state
void print_game_state(int state)
{
	if (WIN == state) { cout << "GANO" << endl; }
	else if (DRAW == state) { cout << "EMPATO" << endl; }
	else if (LOSS == state) { cout << "PERDIO" << endl; }
}

//  estados ganadores (posiciones)
std::vector<std::vector<std::pair<int, int>>> estadosParaGanar  
{
	// cada fila
	{ std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2) },
	{ std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, 2) },
	{ std::make_pair(2, 0), std::make_pair(2, 1), std::make_pair(2, 2) },

	// cada columna
	{ std::make_pair(0, 0), std::make_pair(1, 0), std::make_pair(2, 0) },
	{ std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1) },
	{ std::make_pair(0, 2), std::make_pair(1, 2), std::make_pair(2, 2) },

	// diagonales
	{ std::make_pair(0, 0), std::make_pair(1, 1), std::make_pair(2, 2) },
	{ std::make_pair(2, 0), std::make_pair(1, 1), std::make_pair(0, 2) }

};

// pintar tablero
void pintar_tablero(char tablero[3][3])
{
	cout <<"\t"<<"   JUEGO"<<"\t"<<"CORDENADAS"<< endl;
	cout << endl;
	cout <<"\t"<< tablero[0][0] << " | " << tablero[0][1] << " | " << tablero[0][2] <<"\t"<< "0,0"<<" | "<<"0,1"<<" | "<<"0,2"<<endl;
	cout <<"\t"<< "----------" <<"\t"<< "----------------"<< endl;
	cout <<"\t"<< tablero[1][0] << " | " << tablero[1][1] << " | " << tablero[1][2] <<"\t"<< "1,0"<<" | "<<"1,1"<<" | "<<"1,2"<<endl;
	cout <<"\t"<< "----------" <<"\t"<<"----------------"<< endl;
	cout <<"\t"<< tablero[2][0] << " | " << tablero[2][1] << " | " << tablero[2][2] <<"\t"<< "2,0"<<" | "<<"2,1"<<" | "<<"2,2"<<endl<<endl;
}


std::vector<std::pair<int, int>> get_movimientos_posibles(char tablero[3][3]) //( validar) espacios que no estan ocupados
{
	std::vector<std::pair<int, int>> movimientos;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (tablero[i][j] != SIMBOLO_MAQUINA && tablero[i][j] != SIMBOLO_JUGADOR) //si el jugador y la maquina no han llenado el espacio
			{
				//llene espacios vacios
				movimientos.push_back(std::make_pair(i, j));
			}
		}
	}

	return movimientos;
}


bool Validar_posicion_ocupada(char tablero[3][3], std::pair<int, int> pos) // se mira si la posicion esta ocupada
{
	std::vector<std::pair<int, int>> movimientos_permitidos = get_movimientos_posibles(tablero);

	for (int i = 0; i < movimientos_permitidos.size(); i++)
	{
		if (pos.first == movimientos_permitidos[i].first && pos.second == movimientos_permitidos[i].second)
		{
			return false;
		}
	}

	return true;
}

// se obtiene las posiciones que esta ocupada por algun simbolos
std::vector<std::pair<int, int>> get_posiciones_ocupadas(char tablero[3][3], char simbolo)
{
	std::vector<std::pair<int, int>> posiciones_llenas;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (simbolo == tablero[i][j])
			{
				posiciones_llenas.push_back(std::make_pair(i, j));
			}
		}
	}

	return posiciones_llenas;
}


bool tablero_esta_lleno(char tablero[3][3]) //funcion que mira si el tablero esta lleno
{
	std::vector<std::pair<int, int>> movimientos = get_movimientos_posibles(tablero);
  
     return  (0 == movimientos.size()) ? true  :  false ;
}

// Check if the game has been won
bool juegoEstaGanado(std::vector<std::pair<int, int>> occupied_positions)
{
	bool juegoGanado;

	for (int i = 0; i < estadosParaGanar.size(); i++)
	{
		juegoGanado = true;
		std::vector<std::pair<int, int>> curr_win_state = estadosParaGanar[i];
		for (int j = 0; j < 3; j++)
		{
			if (!(std::find(std::begin(occupied_positions), std::end(occupied_positions), curr_win_state[j]) != std::end(occupied_positions)))
			{
				juegoGanado = false;
				break;
			}
		}

		if (juegoGanado)
		{
			break;
		}
	}
	return juegoGanado;
}

char get_simbolo_oponente(char simbolo)
{
	char simbolo_oponente;
	if (simbolo == SIMBOLO_JUGADOR)
	{
		simbolo_oponente = SIMBOLO_MAQUINA;
	}
	else
	{
		simbolo_oponente = SIMBOLO_JUGADOR;
	}

	return simbolo_oponente;
}


int get_estado_tablero(char tablero[3][3], char simbolo) // se verifica si alguien ha ganado  o perdido 
{

	char oponente_simbolo = get_simbolo_oponente(simbolo);  // se obtiene el simbolo del oponente

	std::vector<std::pair<int, int>> posiciones_ocupadas = get_posiciones_ocupadas(tablero, simbolo); // se retorna las pos ocupadas
	bool es_ganador = juegoEstaGanado(posiciones_ocupadas);

	if (es_ganador)
		return WIN;
	
	posiciones_ocupadas = get_posiciones_ocupadas(tablero, oponente_simbolo);
	bool es_perdedor = juegoEstaGanado(posiciones_ocupadas);

	if (es_perdedor)
		return LOSS;
	

	bool esta_lleno = tablero_esta_lleno(tablero);
	
	if (esta_lleno)
		return DRAW;
	

	return DRAW;

}

// aplicar algoritmo
std::pair<int, std::pair<int, int>> Scout(char tablero[3][3], char simbolo, int profundidad, int alpha, int beta)
{
	// se declara el mejor movimiento
	std::pair<int, int> mejorMovimiento = std::make_pair(-1, -1);
	
	int best_score = (simbolo == SIMBOLO_MAQUINA) ? LOSS : WIN;

	// si se alcanza un estado final (nodo hoja) se regresa el mejor puntaje y retorna
	if (tablero_esta_lleno(tablero) || DRAW != get_estado_tablero(tablero, SIMBOLO_MAQUINA))
	{
		best_score = get_estado_tablero(tablero, SIMBOLO_MAQUINA);
		return std::make_pair(best_score, mejorMovimiento);
	}
   
   //se obtiene los espacios en blanco
	std::vector<std::pair<int, int>> movimientos_permitidos = get_movimientos_posibles(tablero);
   //recorrido
	for (int i = 0; i < movimientos_permitidos.size(); i++)
	{
		// se obtienen el movimiento en la posicion (x , y)
		std::pair<int, int> movimiento = movimientos_permitidos[i];	
		tablero[movimiento.first][movimiento.second] = simbolo;

		// Maximizing player's turn
		if (simbolo == SIMBOLO_MAQUINA)
		{   

			int score = Scout(tablero, SIMBOLO_JUGADOR, profundidad + 1, alpha, beta).first;

			// Get the best scoring move
			if (best_score < score)
			{
				best_score = score - profundidad * 10;
				mejorMovimiento = movimiento;

				// Check if this branch's best move is worse than the best
				// option of a previously search branch. If it is, skip it
				alpha = std::max(alpha, best_score);
				tablero[movimiento.first][movimiento.second] = SIMBOLO_ESPACIO_VACIO;
				if (beta <= alpha) 
				{ 
					break; 
				}
			}

		} // Minimizing opponent's turn
		else
		{
			int score = Scout(tablero, SIMBOLO_MAQUINA, profundidad + 1, alpha, beta).first;

			if (best_score > score)
			{
				best_score = score + profundidad * 10;
				mejorMovimiento = movimiento;

				// Check if this branch's best move is worse than the best
				// option of a previously search branch. If it is, skip it
				beta = std::min(beta, best_score);
				tablero[movimiento.first][movimiento.second] = SIMBOLO_ESPACIO_VACIO;
				if (beta <= alpha) 
				{ 
					break; 
				}
			}

		}

		tablero[movimiento.first][movimiento.second] = SIMBOLO_ESPACIO_VACIO; // Undo move

	}

	return std::make_pair(best_score, mejorMovimiento);
}


bool juego_acabo(char tablero[3][3]) //  juego terminado ? 
{
	if (tablero_esta_lleno(tablero))
	{
		return true;
	}

	if (DRAW != get_estado_tablero(tablero, SIMBOLO_MAQUINA))
	{
		return true;
	}

	return false;
}


int main()
{
	char tablero[3][3] = { SIMBOLO_ESPACIO_VACIO }; // definicion del tablero

	cout << "********************************\n\n\t TRIQUI \n\n********************************" << endl << endl;
	cout << "JUGADOR = X\t MAQUINA = O" << endl << endl;

	pintar_tablero(tablero);

	while (!juego_acabo(tablero))
	{
		int row, col;
		cout << "Fila Juego: ";
		cin >> row;
		cout << "column Juego: ";
		cin >> col;
		cout << endl << endl;

		if (Validar_posicion_ocupada(tablero, std::make_pair(row, col)))
		{
			cout << "esta Posicion  (" << row << ", " << col << ") esta ocupada. intenta con otra..." << endl;
			continue;
		}
		else
		{
			tablero[row][col] = SIMBOLO_JUGADOR;
		}
        
        //retornar jugada de la maquina
		std::pair<int, std::pair<int, int>> ai_move = Scout(tablero, SIMBOLO_MAQUINA, COMIENZO_PROFUNDIDAD, LOSS, WIN);
        
        //llenar en el tablero la jugada 
		tablero[ai_move.second.first][ai_move.second.second] = SIMBOLO_MAQUINA;

		pintar_tablero(tablero);
	}

	cout << "********** FIN DEL JUEGO **********" << endl << endl;

	int player_state = get_estado_tablero(tablero, SIMBOLO_JUGADOR);

	cout << "PLAYER "; print_game_state(player_state);

	return 0;

}
