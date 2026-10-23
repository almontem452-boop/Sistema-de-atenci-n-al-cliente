#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct NodoPila {
    char accion[100];
    struct NodoPila *siguiente;
} NodoPila;
typedef struct {
    NodoPila *tope;
} Pila;
void inicializarPila(Pila *pila) {
    pila->tope = NULL;
}
void registrarAccion(Pila *pila, const char *descripcion) {
    NodoPila *nuevo = (NodoPila *)malloc(sizeof(NodoPila));
    if (nuevo == NULL) {
        printf("Error: No se pudo asignar memoria para el historial.\n");
        return;
    }
    strncpy(nuevo->accion, descripcion, sizeof(nuevo->accion) - 1);
    nuevo->accion[sizeof(nuevo->accion) - 1] = '\0';
    nuevo->siguiente = pila->tope;
    pila->tope = nuevo;
}
void mostrarHistorial(Pila *pila) {
    printf("\n--- HISTORIAL DE ACCIONES (PILA) ---\n");
    if (pila->tope == NULL) {
        printf("El historial esta vacio.\n");
        return;
    }
    NodoPila *actual = pila->tope;
    int contador = 1;
    while (actual != NULL) {
        printf("%d. %s\n", contador++, actual->accion);
        actual = actual->siguiente;
    }
}
void liberarpila (Pila *pila) {
    NodoPila *actual = pila->tope;
    while (actual != NULL) {
        NodoPila *temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    pila->tope = NULL;
}
typedef struct NodoCola {
    int numeroTurno;
    char nombreCliente[50];
    struct NodoCola *siguiente;
} NodoCola;
typedef struct {
    NodoCola *frente;
    NodoCola *final;
    int cantidad;
} Cola;
void inicializarCola(Cola *cola) {
    cola->frente = NULL;
    cola->final = NULL;
    cola->cantidad = 0;
}
void generarTurno(Cola *cola, Pila *historial, int *contadorTurno) {
    char nombre[50];
    printf("\nIngrese el nombre del cliente: ");
    scanf(" %[^\n]", nombre);
    NodoCola *nuevo = (NodoCola *)malloc(sizeof(NodoCola));
    if (nuevo == NULL) {
        printf("Error de memoria.\n");
        return;
    }
    (*contadorTurno)++;
    nuevo->numeroTurno = *contadorTurno;
    strncpy(nuevo->nombreCliente, nombre, sizeof(nuevo->nombreCliente) - 1);
    nuevo->nombreCliente[sizeof(nuevo->nombreCliente) - 1] = '\0';
    nuevo->siguiente = NULL;
    if (cola->final == NULL) {
        cola->frente = nuevo;
    } else {
        cola->final->siguiente = nuevo;
    }
    cola->final = nuevo;
    cola->cantidad++;
    printf("-> Turno asignado con exito: [#%03d] - %s\n", nuevo->numeroTurno, nuevo->nombreCliente);
    snprintf(log, sizeof(log), "Generado Turno #%03d (%s)", nuevo->numeroTurno, nuevo->nombreCliente);
    registrarAccion(historial, log);
}
void atenderCliente(Cola *cola, Pila *historial, int *turnoActual, char *nombreActual) {
    if (cola->frente == NULL) {
        printf("\nNo hay clientes en espera.\n");
    NodoCola *temp = cola->frente;
    *turnoActual = temp->numeroTurno;
    strncpy(nombreActual, temp->nombreCliente, 50);
    cola->frente = cola->frente->siguiente;
    if (cola->frente == NULL) {
        cola->final = NULL;
    }
    cola->cantidad--;
    printf("\n[ATENDIENDO] -> Turno #%03d: %s\n", *turnoActual, nombreActual);
    char log[100];
    snprintf(log, sizeof(log), "Atendido Turno #%03d (%s)", *turnoActual, nombreActual);
    registrarAccion(historial, log);
    free(temp);
}
void mostrarClientesEnEspera(Cola *cola) {
    printf("\n--- CLIENTES EN ESPERA (COLA) ---\n");
    if (cola->frente == NULL) {
        printf("La cola esta vacia.\n");
        return;
    }
    NodoCola *actual = cola->frente;
    int posicion = 1;
    while (actual != NULL) {
        printf("%d. Turno #%03d - Cliente: %s\n", posicion++, actual->numeroTurno, actual->nombreCliente);
        actual = actual->siguiente;
    }
}
void consultarCantidad(Cola *cola) {
    printf("\nPersonas actualmente en cola: %d\n", cola->cantidad);
}
void mostrarTurnoActual(int turnoActual, const char *nombreActual) {
    printf("\n--- TURNO ACTUAL EN VENTANILLA ---\n");
    if (turnoActual == 0) {
        printf("Ningun cliente ha sido atendido todavia o la ventanilla esta libre.\n");
    } else {
        printf("Turno: #%03d | Cliente: %s\n", turnoActual, nombreActual);
    }
}
void liberarCola(Cola *cola) {
    NodoCola *actual = cola->frente;
    while (actual != NULL) {
        NodoCola *temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    cola->frente = NULL;
    cola->final = NULL;
    cola->cantidad = 0;
}
int main() {
    Cola colaAtencion;
    Pila historialSistema;
    inicializarCola(&colaAtencion);
    inicializarPila(&historialSistema);
    int contadorTurnos = 0;
    int turnoEnVentanilla = 0;
    char clienteEnVentanilla[50] = "";
    int opcion;
    do {
        printf("\n========================================\n");
        printf("  SISTEMA DE ATENCION AL CLIENTE\n");
        printf("========================================\n");
        printf("1. Generar turno\n");
        printf("2. Atender siguiente cliente\n");
        printf("3. Mostrar clientes en espera\n");
        printf("4. Consultar cantidad de personas en cola\n");
        printf("5. Mostrar turno actual en ventanilla\n");
        printf("6. Ver historial de acciones (Pila)\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            // Limpieza de buffer en caso de entrada invalida
            while (getchar() != '\n');
            opcion = 0;
        }
        switch (opcion) {
            case 1:
                generarTurno(&colaAtencion, &historialSistema, &contadorTurnos);
                break;
            case 2:
                atenderCliente(&colaAtencion, &historialSistema, &turnoEnVentanilla, clienteEnVentanilla);
                break;
            case 3:
                mostrarClientesEnEspera(&colaAtencion);
                break;
            case 4:
                consultarCantidad(&colaAtencion);
                break;
            case 5:
                mostrarTurnoActual(turnoEnVentanilla, clienteEnVentanilla);
                break;
            case 6:
                mostrarHistorial(&historialSistema);
                break;
            case 7:
                printf("\nCerrando el sistema...\n");
                break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 7);
    liberarCola(&colaAtencion);
    liberarpila(&historialSistema);
    return 0;
}