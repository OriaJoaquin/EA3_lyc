//ESTRUCTURA DE LISTA
typedef struct
{
    char pos1[60];
    char pos2[60];
    char pos3[60];
    int posicion;
} t_info;

typedef struct s_nodoTerceto
{
    t_info info;
    struct s_nodoTerceto *psig;
} t_nodoTerceto;

typedef t_nodoTerceto *t_listaTercetos;

// ESTRUCTURA DE PILA
typedef struct tagStackItem
{
    char value[30];
} StackItem;

typedef struct s_nodo_pila
{
    StackItem dato;
    struct s_nodo_pila *psig;
} t_nodo_pila;

typedef t_nodo_pila *t_pila;
// ESTRUCTURA DE COLA
typedef struct
{
    char nombre[30];
    char tipo[30];
    int longitud;
    char valor[50];
} QueueItem;

typedef struct s_nodo
{
    QueueItem dato;
    struct s_nodo *sig;
} t_nodo;

typedef struct
{
    t_nodo *pri;
    t_nodo *ult;
} t_cola;

//DEFINICIONES FUNCIONES PILA
void crearPila(t_pila *);
void vaciar_pila(t_pila *);
int meterEnPila(t_pila *, StackItem *);
int sacarDePila(t_pila *, StackItem *);
int esPilaVacia(const t_pila *);
int esPilaLlena(const t_pila *);
void vaciarPila(t_pila *);
int tope_de_pila(const t_pila *, StackItem *);

//DEFINICIONES FUNCIONES LISTA
void crearListaTercetos(t_listaTercetos *);
int insertarTerceto(t_listaTercetos *, char *, char *, char *);
void imprimirListaTercetos(t_listaTercetos *);

//DEFINICIONES FUNCIONES COLA
void crearCola(t_cola *);
int acolar(t_cola *, const QueueItem *);
int desacolar(t_cola *, QueueItem *);
int colaLlena(const t_cola *);
int colaVacia(const t_cola *);
int verPrimero(const t_cola *, QueueItem *);
void vaciarCola(t_cola *);

void cargarItemSimboloEntero(QueueItem *, char *);
void cargarItemSimboloCadena(QueueItem *, char *);
void cargarItemSimboloVariable(QueueItem *, char *, char *);
void cargarItemSimboloVariableConValor(QueueItem *, char *, char *, char *);

void formatearPosicion(int, char *);
char *obtenerPosicion(char *);
char *transformarPosicionEnAux(char *);
char *crearNombreVariableAux(char *, int);

extern int posicionTerceto;
extern int contadorCadena;