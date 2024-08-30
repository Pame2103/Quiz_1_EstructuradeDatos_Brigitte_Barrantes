#include <iostream>
#include <string>

using namespace std;

// Estructura para almacenar las cantidades vendidas por los empleados
struct NodoCantidad {
	int cantidad;
	NodoCantidad* siguiente;

	NodoCantidad(int c) : cantidad(c), siguiente(nullptr) {}
};

// Estructura para representar a un empleado
struct NodoEmpleado {
	string nombre;
	int totalVendido;
	NodoEmpleado* siguiente;
	NodoCantidad* listaCantidades;

	NodoEmpleado(string n) : nombre(n), totalVendido(0), siguiente(nullptr), listaCantidades(nullptr) {}
};

// Estructura para representar un piso del edificio
struct NodoPiso {
	string nombre;
	int totalVendido;
	NodoPiso* siguiente;
	NodoPiso* anterior;
	NodoEmpleado* listaEmpleados;

	NodoPiso(string n) : nombre(n), totalVendido(0), siguiente(nullptr), anterior(nullptr), listaEmpleados(nullptr) {}
};

// Clase que representa el edificio
class Edificio {
private:
	NodoPiso* inicio;

public:
	Edificio() : inicio(nullptr) {}

	// Función para insertar un nuevo piso
	void insertarPiso(string nombrePiso) {
		NodoPiso* nuevo = new NodoPiso(nombrePiso);

		if (!inicio) {
			inicio = nuevo;
		}
		else {
			NodoPiso* actual = inicio;
			NodoPiso* anterior = nullptr;

			while (actual && nombrePiso > actual->nombre) {
				anterior = actual;
				actual = actual->siguiente;
			}

			if (!anterior) {
				nuevo->siguiente = inicio;
				inicio->anterior = nuevo;
				inicio = nuevo;
			}
			else {
				nuevo->siguiente = actual;
				nuevo->anterior = anterior;
				anterior->siguiente = nuevo;
				if (actual) {
					actual->anterior = nuevo;
				}
			}
		}
		cout << "Piso " << nombrePiso << " insertado." << endl;
	}

	// Función para insertar un nuevo empleado en un piso
	void insertarEmpleado(string nombrePiso, string nombreEmpleado) {
		NodoPiso* piso = buscarPiso(nombrePiso);
		if (piso) {
			NodoEmpleado* nuevo = new NodoEmpleado(nombreEmpleado);

			if (!piso->listaEmpleados) {
				piso->listaEmpleados = nuevo;
			}
			else {
				NodoEmpleado* actual = piso->listaEmpleados;
				NodoEmpleado* anterior = nullptr;

				while (actual && nombreEmpleado > actual->nombre) {
					anterior = actual;
					actual = actual->siguiente;
				}

				if (!anterior) {
					nuevo->siguiente = piso->listaEmpleados;
					piso->listaEmpleados = nuevo;
				}
				else {
					nuevo->siguiente = actual;
					anterior->siguiente = nuevo;
				}
			}
			cout << "Empleado " << nombreEmpleado << " insertado en el piso " << nombrePiso << "." << endl;
		}
		else {
			cout << "Piso no encontrado." << endl;
		}
	}

	// Función para insertar una cantidad de venta para un empleado
	void insertarCantidad(string nombreEmpleado, int cantidad) {
		NodoEmpleado* empleado = buscarEmpleado(nombreEmpleado);
		if (empleado) {
			NodoCantidad* nuevo = new NodoCantidad(cantidad);

			if (!empleado->listaCantidades) {
				empleado->listaCantidades = nuevo;
			}
			else {
				NodoCantidad* actual = empleado->listaCantidades;
				NodoCantidad* anterior = nullptr;

				while (actual && cantidad < actual->cantidad) {
					anterior = actual;
					actual = actual->siguiente;
				}

				if (!anterior) {
					nuevo->siguiente = empleado->listaCantidades;
					empleado->listaCantidades = nuevo;
				}
				else {
					nuevo->siguiente = actual;
					anterior->siguiente = nuevo;
				}
			}
			empleado->totalVendido += cantidad;
			actualizarTotalPiso(empleado);
			cout << "Cantidad " << cantidad << " insertada para el empleado " << nombreEmpleado << "." << endl;
		}
		else {
			cout << "Empleado no encontrado." << endl;
		}
	}

	// Función para mostrar los datos del edificio
	void mostrarDatos() {
		NodoPiso* actualPiso = inicio;
		while (actualPiso) {
			cout << "Piso: " << actualPiso->nombre << " - Total Vendido: " << actualPiso->totalVendido << endl;
			NodoEmpleado* actualEmpleado = actualPiso->listaEmpleados;
			while (actualEmpleado) {
				cout << "\tEmpleado: " << actualEmpleado->nombre << " - Total Vendido: " << actualEmpleado->totalVendido << endl;
				NodoCantidad* actualCantidad = actualEmpleado->listaCantidades;
				while (actualCantidad) {
					cout << "\t\tCantidad: " << actualCantidad->cantidad << endl;
					actualCantidad = actualCantidad->siguiente;
				}
				actualEmpleado = actualEmpleado->siguiente;
			}
			actualPiso = actualPiso->siguiente;
		}
	}

	// Función para verificar que las sumatorias en los nodos sean correctas
	void verificarSumatorias() {
		NodoPiso* actualPiso = inicio;
		while (actualPiso) {
			int sumaTotalPiso = 0;
			NodoEmpleado* actualEmpleado = actualPiso->listaEmpleados;
			while (actualEmpleado) {
				int sumaTotalEmpleado = 0;
				NodoCantidad* actualCantidad = actualEmpleado->listaCantidades;
				while (actualCantidad) {
					sumaTotalEmpleado += actualCantidad->cantidad;
					actualCantidad = actualCantidad->siguiente;
				}

				if (sumaTotalEmpleado != actualEmpleado->totalVendido) {
					cout << "Error en la sumatoria del empleado " << actualEmpleado->nombre << endl;
				}

				sumaTotalPiso += sumaTotalEmpleado;
				actualEmpleado = actualEmpleado->siguiente;
			}

			if (sumaTotalPiso != actualPiso->totalVendido) {
				cout << "Error en la sumatoria del piso " << actualPiso->nombre << endl;
			}

			actualPiso = actualPiso->siguiente;
		}
	}

	// Función para eliminar el piso 3 y mover los empleados al piso anterior
	void eliminarPiso3() {
		NodoPiso* piso3 = buscarPiso("Piso 3");
		if (piso3 && piso3->anterior) {
			NodoPiso* pisoAnterior = piso3->anterior;
			NodoEmpleado* ultimoEmpleado = pisoAnterior->listaEmpleados;

			// Encontrar el último empleado del piso anterior
			if (ultimoEmpleado) {
				while (ultimoEmpleado->siguiente) {
					ultimoEmpleado = ultimoEmpleado->siguiente;
				}
			}

			// Mover los empleados del piso 3 al piso anterior
			NodoEmpleado* actualEmpleado = piso3->listaEmpleados;
			while (actualEmpleado) {
				NodoEmpleado* siguienteEmpleado = actualEmpleado->siguiente;

				if (!ultimoEmpleado) {
					pisoAnterior->listaEmpleados = actualEmpleado;
				}
				else {
					ultimoEmpleado->siguiente = actualEmpleado;
				}
				ultimoEmpleado = actualEmpleado;
				actualEmpleado = siguienteEmpleado;
			}

			// Actualizar la sumatoria del piso anterior
			actualizarTotalPiso(pisoAnterior);

			// Eliminar el piso 3
			if (piso3->siguiente) {
				piso3->siguiente->anterior = pisoAnterior;
			}
			pisoAnterior->siguiente = piso3->siguiente;
			delete piso3;

			cout << "Piso 3 eliminado y empleados movidos al piso anterior." << endl;
		}
		else {
			cout << "No se puede eliminar el piso 3." << endl;
		}
	}

	// Función para mostrar el menú de la aplicación
	void menu() {
		int opcion;
		string nombrePiso, nombreEmpleado;
		int cantidad;

		do {
			cout << "\n--- Menú ---\n";
			cout << "1. Insertar Piso\n";
			cout << "2. Insertar Empleado\n";
			cout << "3. Insertar Cantidad\n";
			cout << "4. Mostrar Datos\n";
			cout << "5. Verificar Sumatorias\n";
			cout << "6. Eliminar Piso 3\n";
			cout << "7. Salir\n";
			cout << "Seleccione una opción: ";
			cin >> opcion;

			switch (opcion) {
			case 1:
				cout << "Ingrese el nombre del piso: ";
				cin >> nombrePiso;
				insertarPiso(nombrePiso);
				break;

			case 2:
				cout << "Ingrese el nombre del piso: ";
				cin >> nombrePiso;
				cout << "Ingrese el nombre del empleado: ";
				cin >> nombreEmpleado;
				insertarEmpleado(nombrePiso, nombreEmpleado);
				break;

			case 3:
				cout << "Ingrese el nombre del empleado: ";
				cin >> nombreEmpleado;
				cout << "Ingrese la cantidad: ";
				cin >> cantidad;
				insertarCantidad(nombreEmpleado, cantidad);
				break;

			case 4:
				mostrarDatos();
				break;

			case 5:
				verificarSumatorias();
				break;

			case 6:
				eliminarPiso3();
				break;

			case 7:
				cout << "Saliendo del programa..." << endl;
				break;

			default:
				cout << "Opción no válida. Intente de nuevo." << endl;
			}
		} while (opcion != 7);
	}

private:
	// Función para buscar un piso por nombre
	NodoPiso* buscarPiso(string nombrePiso) {
		NodoPiso* actual = inicio;
		while (actual) {
			if (actual->nombre == nombrePiso) {
				return actual;
			}
			actual = actual->siguiente;
		}
		return nullptr;
	}

	// Función para buscar un empleado por nombre
	NodoEmpleado* buscarEmpleado(string nombreEmpleado) {
		NodoPiso* actualPiso = inicio;
		while (actualPiso) {
			NodoEmpleado* actualEmpleado = actualPiso->listaEmpleados;
			while (actualEmpleado) {
				if (actualEmpleado->nombre == nombreEmpleado) {
					return actualEmpleado;
				}
				actualEmpleado = actualEmpleado->siguiente;
			}
			actualPiso = actualPiso->siguiente;
		}
		return nullptr;
	}

	// Función para actualizar el total vendido de un piso
	void actualizarTotalPiso(NodoEmpleado* empleado) {
		NodoPiso* piso = inicio;
		while (piso) {
			NodoEmpleado* actualEmpleado = piso->listaEmpleados;
			int sumaTotal = 0;
			while (actualEmpleado) {
				sumaTotal += actualEmpleado->totalVendido;
				actualEmpleado = actualEmpleado->siguiente;
			}
			piso->totalVendido = sumaTotal;
			piso = piso->siguiente;
		}
	}

	// Sobrecarga de actualizarTotalPiso para actualizar piso en función de nodoPiso
	void actualizarTotalPiso(NodoPiso* piso) {
		NodoEmpleado* actualEmpleado = piso->listaEmpleados;
		int sumaTotal = 0;
		while (actualEmpleado) {
			sumaTotal += actualEmpleado->totalVendido;
			actualEmpleado = actualEmpleado->siguiente;
		}
		piso->totalVendido = sumaTotal;
	}
};

// Función principal
int main() {
	Edificio edificio;
	edificio.menu();  // Llamada al menú para interactuar con el sistema.
	return 0;
}
