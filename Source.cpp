//Pablo Gonzalez Corredor y Alejandro Magarzo Gonzalo G15 VS017
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

//Version 1
int menu();
int robo();
float modoA(ifstream& file, int numCartas);
float modoBhumano(ifstream& file, int numCartas);
float modoBmaquina(ifstream& file, int numCartas, float puntosHumano);
int determinaGanador(float puntosJugador, float puntosMaquina);

//Version 2
typedef double tCartasPorAparecer[8];
void modoChumano(ifstream& file, tCartasPorAparecer cartas, float& puntos);
void modoCmaquina(ifstream& file, tCartasPorAparecer cartas, float puntosHumano, float& puntos);
bool esProbablePasarse(float puntosMaquina, const tCartasPorAparecer cartas);

//Version 3
const int CARTAS = 40;
typedef double tArray[CARTAS];
typedef struct {
	tArray elementos;
	int contador;
}tConjuntoCartas;
void crearMazo(tConjuntoCartas& mazo);
void mostrarMazo(tConjuntoCartas& mazo);
void inicializa(tConjuntoCartas & cartas);
void sacarCarta(tConjuntoCartas & cartas, int & carta, tConjuntoCartas& mazo);
void annadirCarta(tConjuntoCartas & cartas, int carta);
void modoDhumano(tConjuntoCartas & mazo, tCartasPorAparecer cartas, tConjuntoCartas & cartasHumano, float & puntos);
void modoDmaquina(tConjuntoCartas & mazo, tCartasPorAparecer cartas, float puntosHumano, tConjuntoCartas & cartasMaquina, float & puntos);

int main() {
	int opcion, ganador;
	float puntosH, puntosM;
	ifstream file;
	string nf;
	int numPartida = 1;
	tCartasPorAparecer cartas;

	do {
		opcion = menu();
		cartas[0] = 12;
		for (int i = 1; i < 8; i++) {
			cartas[i] = 4;
		}
		if (opcion == 0) return 0;
		// MODO D
		if (opcion == 4) {
			tConjuntoCartas mazo, cartasHumano, cartasMaquina;
			crearMazo(mazo);
			cout << "TURNO DEL JUGADOR....." << endl;
			modoDhumano(mazo, cartas, cartasHumano, puntosH);
			if (puntosH <= 7.5) {
				cout << "TURNO DE LA MAQUINA....." << endl;
				modoDmaquina(mazo, cartas, puntosH, cartasMaquina, puntosM);
				if (puntosM > 7.5) {
					cout << "La maquina se pasa. El jugador gana automaticamente." << endl << endl;
					ganador = 1;
				}
				else {
					if (puntosM != puntosH) {
						ganador = determinaGanador(puntosH, puntosM);
						switch (ganador) {
						case 1: cout << "Gana el jugador." << endl << endl; break;
						case 2: cout << "Gana la maquina." << endl << endl; break;
						}
					}
					else {
						if (cartasHumano.contador < cartasMaquina.contador) {
							cout << "El jugador ha robado menos cartas por lo que gana." << endl;
							ganador = 1;
						}
						if (cartasHumano.contador > cartasMaquina.contador) {
							cout << "La maquina ha robado menos cartas por lo que gana." << endl;
							ganador = 2;
						}
						if (cartasHumano.contador == cartasMaquina.contador) ganador = determinaGanador(puntosH, puntosM);
					}
				}
			}
			else {
				cout << "El jugador se pasa. La maquina gana automaticamente." << endl << endl;
				puntosM = 0;
				ganador = 2;
			}
			ofstream outFile;
			outFile.open(to_string(numPartida) + ".txt");
			outFile << "Partida #" << numPartida << endl << "Ganador: ";
			if (ganador == 1) outFile << "Humano." << endl;
			else outFile << "Maquina." << endl;
			outFile << "Resultados Humano: - Puntuacion: " << puntosH << " - Cartas: ";
			for (int i = 0; i < 41; i++) {
				if (cartasHumano.elementos[i] >= 1)outFile << cartasHumano.elementos[i] << " ";
			}
			outFile << endl << "Resultados Maquina: - Puntuacion: " << puntosM << " - Cartas: ";
			for (int i = 0; i < 41; i++) {
				if (cartasMaquina.elementos[i] >= 1 && puntosH <= 7.5) outFile << cartasMaquina.elementos[i] << " ";
			}
			outFile.close();
			numPartida++;
		}
		// MODO A B C
		else if (1 <= opcion <= 3) {
			do {
				cout << "Introduzca el nombre del archivo: ";
				cin >> nf;
				file.open(nf);
				if (!file.is_open()) cout << "Mazo incorrecto. ";
			} while (!file.is_open());
			cout << "Mazo " << nf << " abierto correctamente." << endl << endl;
		}
		if (opcion == 1) {
			int n = robo();
			cout << "Los dos jugadores, humano y maquina, roban " << n << " cartas cada uno obligatoriamente."
				<< endl << "TURNO DEL JUGADOR....." << endl;
			puntosH = modoA(file, n);
			if (puntosH <= 7.5) {
				cout << "TURNO DE LA MAQUINA....." << endl;
				puntosM = modoA(file, n);
				if (puntosM > 7.5) cout << "La maquina se pasa. El jugador gana automaticamente." << endl << endl;
				else {
					ganador = determinaGanador(puntosH, puntosM);
					switch (ganador) {
					case 1: cout << "Gana el jugador." << endl << endl; break;
					case 2: cout << "Gana la maquina." << endl << endl; break;
					}
				}
			}
			else cout << "El jugador se pasa. La maquina gana automaticamente." << endl << endl;
			file.close();
		}
		if (opcion == 2) {
			int maxn;
			maxn = robo();
			cout << "En este modo se pueden robar un maximo numero de cartas, entre 3 y 5 y es aleatorio. Se podran robar " <<
				maxn << " cartas." << endl << "TURNO DEL JUGADOR....." << endl;
			puntosH = modoBhumano(file, maxn);
			if (puntosH <= 7.5) {
				cout << "TURNO DE LA MAQUINA....." << endl;
				puntosM = modoBmaquina(file, maxn, puntosH);
				if (puntosM > 7.5) cout << "La maquina se pasa. El jugador gana automaticamente." << endl << endl;
				else {
					ganador = determinaGanador(puntosH, puntosM);
					switch (ganador) {
					case 1: cout << "Gana el jugador." << endl << endl; break;
					case 2: cout << "Gana la maquina." << endl << endl; break;
					}
				}
			}
			else cout << "El jugador se pasa. La maquina gana automaticamente." << endl << endl;
			file.close();
		}
		if (opcion == 3) {
			cout << "En este modo no hay un maximo de numero de cartas que se puedan robar." << endl <<
				"Se mostraran el numero de cartas de cada tipo cuando roba la maquina o el jugador." << endl << "TURNO DEL JUGADOR....." << endl;
			modoChumano(file, cartas, puntosH);
			if (puntosH <= 7.5) {
				cout << "TURNO DE LA MAQUINA....." << endl;
				modoCmaquina(file, cartas, puntosH, puntosM);
				if (puntosM > 7.5) cout << "La maquina se pasa. El jugador gana automaticamente." << endl << endl;
				else {
					ganador = determinaGanador(puntosH, puntosM);
					switch (ganador) {
					case 1: cout << "Gana el jugador." << endl << endl; break;
					case 2: cout << "Gana la maquina." << endl << endl; break;
					}
				}
			}
			else {
				cout << "El jugador se pasa. La maquina gana automaticamente." << endl << endl;
				puntosM = 0;
			}
			file.close();
		}

	} while (0 < opcion < 5);
	system("pause");
	return 0;
}

int menu() {
	int op;
	do {
		cout << "--------------JUEGO DE LAS SIETE Y MEDIA--------------" << endl << "0 - Salir" << endl << "1 - Modo A" << endl << "2 - Modo B"
			<< endl << "3 - Modo C" << endl << "4 - Modo D" << endl << "------------------------------------------------------" << endl;
		cin >> op;
		if (op > 4) cout << "Opcion incorrecta. " << endl;
	} while (op > 4);
	if (op != 0) cout << endl << "Iniciando el Modo " << op << ". ";
	return op;
}

int robo() {
	int numcartas;
	const int min = 3;
	const int max = 5;
	srand(time(NULL));
	numcartas = min + rand() % (max - min + 1);
	return numcartas;
}

float modoA(ifstream & file, int numCartas) {
	float punto, puntuacion = 0;
	int c;
	for (int i = 0; i < numCartas; i++) {
		file >> c;
		if (c <= 7) punto = c;
		else if (10 <= c <= 12) punto = 0.5;
		puntuacion = puntuacion + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntuacion << "." << endl;
	}
	cout << "La puntuacion total es de " << puntuacion << " puntos." << endl;
	return puntuacion;
}

float modoBhumano(ifstream & file, int numCartas) {
	float punto, puntuacion = 0;
	char a;
	int c;
	bool continuar = true;
	for (int i = 0; i < numCartas && continuar && puntuacion <= 7.5; i++) {
		file >> c;
		if (c <= 7) punto = c;
		if (c >= 10 && c <= 12) punto = 0.5;
		puntuacion = puntuacion + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntuacion << "." << endl;
		if (i < numCartas - 1 && puntuacion <= 7.5) {
			cout << "Quiere robar mas cartas (S/N)?";
			cin >> a;
			if (toupper(a) == 'S') continuar = true;
			else continuar = false;
		}
	}
	cout << "La puntuacion total es de " << puntuacion << " puntos." << endl;
	return puntuacion;
}

float modoBmaquina(ifstream& file, int numCartas, float puntosHumano) {
	float punto, puntuacion = 0;
	int c;
	bool superar = false;
	for (int i = 0; i < numCartas && superar == false && puntuacion < 7.5; i++) {
		file >> c;
		if (c <= 7) punto = c;
		if (c >= 10 && c <= 12) punto = 0.5;
		puntuacion = puntuacion + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntuacion << "." << endl;
		if (puntuacion > puntosHumano && puntuacion <= 7.5) {
			cout << "La puntuacion de la maquina es superior que la del jugador por lo que se planta." << endl;
			superar = true;
		}
	}
	cout << "La puntuacion total es de " << puntuacion << " puntos." << endl;
	return puntuacion;
}

int determinaGanador(float puntosJugador, float puntosMaquina) {
	int ganador;
	if (puntosJugador > puntosMaquina) ganador = 1;
	if (puntosJugador < puntosMaquina) ganador = 2;
	if (puntosJugador == puntosMaquina) {
		ganador = 1 + rand() % (2 + 1 - 1);
		cout << "Hay empate y se elige al ganador aleatoriamente. ";
	}
	return ganador;
}

void modoChumano(ifstream & file, tCartasPorAparecer cartas, float & puntos) {
	float punto;
	puntos = 0;
	int tipo, c;
	char a;
	bool continuar = true;
	cout << "Al comenzar la partida hay: " << cartas[0] << " figuras. ";
	for (int j = 1; j < 8; j++) {
		cout << cartas[j] << " " << j << ". ";
	}
	cout << endl;
	while (continuar == true) {
		file >> c;
		if (c <= 7) punto = tipo = c;
		if (c >= 10 && c <= 12) {
			punto = 0.5;
			tipo = 0;
		}
		cartas[tipo] = cartas[tipo] - 1;
		puntos = puntos + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntos << "." << endl;
		cout << "Tras este robo quedan " << cartas[0] << " figuras." << endl;
		for (int j = 1; j < 8; j++) {
			cout << "Tras este robo quedan " << cartas[j] << " " << j << "." << endl;
		}
		cout << "Quiere robar mas cartas (S/N)?";
		cin >> a;
		if (toupper(a) == 'S') continuar = true;
		else continuar = false;
	}
	cout << "La puntuacion total es de " << puntos << " puntos." << endl;
}

void modoCmaquina(ifstream & file, tCartasPorAparecer cartas, float puntosHumano, float & puntos) {
	float punto;
	puntos = 0;
	bool superar = false, pasarse = false;
	int tipo, c;
	while (!superar && !pasarse && puntos <= puntosHumano) {
		file >> c;
		if (c <= 7) punto = tipo = c;
		if (c >= 10 && c <= 12) {
			punto = 0.5;
			tipo = 0;
		}
		cartas[tipo] = cartas[tipo] - 1;
		puntos = puntos + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntos << "." << endl;
		cout << "Tras este robo quedan " << cartas[0] << " figuras." << endl;
		for (int j = 1; j < 8; j++) {
			cout << "Tras este robo quedan " << cartas[j] << " " << j << "." << endl;
		}
		if (puntos > puntosHumano && puntos <= 7.5) {
			cout << "La puntuacion de la maquina es superior que la del jugador por lo que se planta." << endl;
			superar = true;
		}
		else if (puntos == puntosHumano && puntos < 7.5) pasarse = esProbablePasarse(puntos, cartas);

	}
	cout << "La puntuacion total es de " << puntos << " puntos." << endl;
}

bool esProbablePasarse(float puntosMaquina, const tCartasPorAparecer cartas) {
	bool pasarse;
	float probabilidad, cartasup, totalcartas = 0;
	int cartasupera, suma = 0;
	cartasup = 7.5 - puntosMaquina;
	cartasupera = cartasup + 1;
	for (int i = 0; i < 8; i++) {
		totalcartas = totalcartas + cartas[i];
	}
	for (int i = cartasupera; i < 8; i++) {
		suma = suma + cartas[i];
	}
	cout << "El numero total de cartas que quedan es de " << totalcartas << " y el numero de cartas con las que se pasa de 7.5 es de "
		<< suma << "." << endl;
	probabilidad = suma / totalcartas;
	if (probabilidad > 0.5) {
		pasarse = true;
		cout << "La probabilidad de pasarse es de " << probabilidad << " por lo que no roba." << endl;
	}
	else {
		pasarse = false;
		cout << "La probabilidad de pasarse es de " << probabilidad << " por lo que roba." << endl;
	}
	return pasarse;
}

void crearMazo(tConjuntoCartas& mazo) {
	int valor = 1, valorf = 10;
	for (int i = 0; i < 40; i = i + 4) {
		if (i <= 27) {
			mazo.elementos[i] = mazo.elementos[i + 1] = mazo.elementos[i + 2] = mazo.elementos[i + 3] = valor;
			valor++;
		}
		else {
			mazo.elementos[i] = mazo.elementos[i + 1] = mazo.elementos[i + 2] = mazo.elementos[i + 3] = valorf;
			valorf++;
		}
	}
	cout << "Asi queda el mazo creado: " << endl;
	mostrarMazo(mazo);
	random_shuffle(&(mazo.elementos[0]), &(mazo.elementos[40]));
	cout << endl << "Se barajea el mazo: " << endl;
	mostrarMazo(mazo);
	cout << endl << endl;
}

void mostrarMazo(tConjuntoCartas& mazo) {
	for (int i = 0; i < 40; i++) {
		if (mazo.elementos[i] != 0) cout << mazo.elementos[i] << " ";
		if (i == 19) cout << endl;
	}
}

void inicializa(tConjuntoCartas & cartas) {
	cartas.contador = 0;
	for (int i = 0; i < 41; i++) {
		cartas.elementos[i] = 0;
	}
}

void sacarCarta(tConjuntoCartas & cartas, int &carta, tConjuntoCartas& mazo) {
	carta = mazo.elementos[mazo.contador - 1];
	mazo.elementos[mazo.contador - 1] = 0;
	cartas.contador++;
	mazo.contador--;
}

void annadirCarta(tConjuntoCartas & cartas, int carta) {
	cartas.elementos[cartas.contador - 1] = carta;
}

void modoDhumano(tConjuntoCartas & mazo, tCartasPorAparecer cartas, tConjuntoCartas & cartasHumano, float & puntos) {
	float punto;
	puntos = 0;
	int tipo, c;
	char a;
	bool continuar = true;
	mazo.contador = 40;
	inicializa(cartasHumano);
	while (continuar) {
		sacarCarta(cartasHumano, c, mazo);
		annadirCarta(cartasHumano, c);
		if (c <= 7) punto = tipo = c;
		if (c >= 10 && c <= 12) {
			punto = 0.5;
			tipo = 0;
		}
		cartas[tipo] = cartas[tipo] - 1;
		puntos = puntos + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntos << "." << endl;
		cout << "Tras este robo quedan " << cartas[0] << " figuras." << endl;
		for (int j = 1; j < 8; j++) {
			cout << "Tras este robo quedan " << cartas[j] << " " << j << "." << endl;
		}
		cout << "El mazo queda asi: " << endl;
		mostrarMazo(mazo);
		cout << endl << "Respecto a las cartas que tiene el jugador son " << cartasHumano.contador << ": ";
		for (int d = 0; d < cartasHumano.contador; d++) {
			cout << cartasHumano.elementos[d] << " ";
		}
		cout << ".Quedan " << mazo.contador << " cartas en el mazo." << endl;
		if (mazo.contador >= 1) {
			cout << "Quiere robar mas cartas (S/N)?";
			cin >> a;
			if (toupper(a) == 'S') continuar = true;
			else continuar = false;
		}
	}
	cout << "La puntuacion total es de " << puntos << " puntos." << endl;
}

void modoDmaquina(tConjuntoCartas & mazo, tCartasPorAparecer cartas, float puntosHumano, tConjuntoCartas & cartasMaquina, float & puntos) {
	float punto;
	puntos = 0;
	bool superar = false, pasarse = false;
	int tipo, c;
	inicializa(cartasMaquina);
	while (superar == false && mazo.contador >= 1 && pasarse == false && puntos <= puntosHumano) {
		sacarCarta(cartasMaquina, c, mazo);
		annadirCarta(cartasMaquina, c);
		if (c <= 7) punto = tipo = c;
		if (c >= 10 && c <= 12) {
			punto = 0.5;
			tipo = 0;
		}
		cartas[tipo] = cartas[tipo] - 1;
		puntos = puntos + punto;
		cout << "La carta robada es: " << c << ". La puntuacion total es: " << puntos << "." << endl;
		cout << "Tras este robo quedan " << cartas[0] << " figuras." << endl;
		for (int j = 1; j < 8; j++) {
			cout << "Tras este robo quedan " << cartas[j] << " " << j << "." << endl;
		}
		cout << "El mazo queda asi: " << endl;
		mostrarMazo(mazo);
		cout << endl << "Respecto a las cartas que tiene la maquina son " << cartasMaquina.contador << ": ";
		for (int d = 0; d < cartasMaquina.contador; d++) {
			cout << cartasMaquina.elementos[d] << " ";
		}
		cout << ".Quedan " << mazo.contador << " cartas en el mazo." << endl;
		if (puntos > puntosHumano && puntos <= 7.5) {
			cout << "La puntuacion de la maquina es superior que la del jugador por lo que se planta." << endl;
			superar = true;
		}
		if (puntos == puntosHumano && puntos < 7.5) pasarse = esProbablePasarse(puntos, cartas);
	}
	cout << "La puntuacion total es de " << puntos << " puntos." << endl;
}