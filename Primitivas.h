//ESTRUCTURA DE LISTA
typedef struct
{
    char pos1[30];
    char pos2[30];
    char pos3[30];
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
    struct tagSExpression *estructura;
} StackItem;

typedef struct s_nodo_pila
{
    StackItem dato;
    struct s_nodo_pila *psig;
} t_nodo_pila;

typedef t_nodo_pila *t_pila;

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

void formatearPosicion(char *);

extern int posicionTerceto;