/*
 * Allocation de m�moire s�curis�e :
 * - V�rifie que malloc (ou realloc) ne renvoie pas NULL
 * - Evite les fuites de m�moire (memory leak)
 * - V�rifie les erreurs d'�criture en dehors de la m�moire allou�e
 *   (underflow et overflow)
 *
 * Ce code est sous licence GPL et c'est le code simplifi� de la librairie
 * HAlloc issue du projet HaypoCALC (http://www.haypocalc.com/).
 *
 * Par Haypo - 7 Avril 2003 - http://haypo.developpez.com/
 */

#ifndef HAlloc_H
#define HAlloc_H
//-----------------------------------------------------------------------------
#include <stdlib.h>     // Pour malloc/free
#include <stdbool.h>    // C99 Power
//-----------------------------------------------------------------------------

// Allocation s�curis�e : v�rifie que la m�moire est effectivement
// allou�e lors des allocations
#define DEBUG_MALLOC

// Conteneur Malloc :
// - Liste les pointeurs allou�s (�vite les fuites de m�moire)
// - Avant de lib�rer un pointeur, v�rifie qu'il est effectivement allou�!
#define DEBUG_MALLOC_CONTENEUR

// Tampon anti-overflow :
// - V�rifie que la m�moire allou�e n'ai pas �t� modifi� en dehors
//   de l'espace allou�
#define DEBUG_MALLOC_OVERFLOW

//-----------------------------------------------------------------------------

// Inclut les principales librairies pour �viter le conflit de nom (strdup,
// malloc, new, ..)
#ifdef DEBUG_MALLOC
#  include <string.h>
#  ifdef __cplusplus
#    include <new>
#  endif
#endif

//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC
// Allocation s�curis�e : v�rifie que l'allocation s'est bien pass�e,
// affiche une erreur sinon
void* MallocSecurise (size_t size, const char* nom_fich, const unsigned long num_ligne);
void* ReallocSecurise (void* ptr, size_t size, const char* nom_fich, const unsigned long num_ligne);
void* CallocSecurise (size_t nitems, size_t size, const char* nom_fich, const unsigned long num_ligne);
char* StrdupSecurise (const char* src, const char* nom_fich, const unsigned long num_ligne);

// Macro qui rajoutent automatiquement le nom du fichier et la ligne
#  define malloc(size) MallocSecurise(size, __FILE__, __LINE__)
#  define calloc(nitems,size) CallocSecurise(nitems, size, __FILE__, __LINE__)
#  define realloc(ptr,size) ReallocSecurise(ptr, size, __FILE__, __LINE__)
#  define strdup(src) StrdupSecurise(src, __FILE__, __LINE__)

#  ifdef __cplusplus
// new/delete du C++
void* operator new (size_t size, const char* nom_fich, const unsigned long num_ligne)
throw (std::bad_alloc);
void* operator new[] (size_t size, const char* nom_fich, const unsigned long num_ligne)
throw (std::bad_alloc);
void operator delete (void *) throw ();
void operator delete[] (void *ptr) throw ();
extern const char *delete_FILE;
extern unsigned long delete_LINE;

#    define new new (__FILE__, __LINE__)
#    define delete delete_FILE=__FILE__; delete_LINE=__LINE__; delete
#  endif

#endif

//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC_CONTENEUR
// V�rifie qu'on d�bogue Malloc
# ifndef DEBUG_MALLOC
#  error "Le conteneur malloc n�cessite #define DEBUG_MALLOC:halloc.h"
# endif

// Informations �crites avant les donn�es du pointeur
typedef struct
{
    unsigned long nbr_magique;       // Nombre magique
    bool est_alloue;                 // Pointeur allou� ou lib�r� ?
    size_t taille;                   // Taille allou�e (sans ces informations)
    const char* nom_fich;            // Nom du fichier source
    unsigned long num_ligne;         // Ligne du fichier
    struct PtrChaineMalloc *ptr_liste;  // Pointeur dans la liste
    unsigned long somme_info;        // Somme des informations (checksum)
} InfoMalloc;

// Conteneur : liste des pointeurs allou�es
// Un pointeur cha�n�
struct PtrChaineMalloc
{
    struct PtrChaineMalloc *precedent;    // El�ment suivant
    struct PtrChaineMalloc *suivant;      // El�ment suivant
    unsigned long nbr_magique;            // Nombre magique
    InfoMalloc *ptr_info;                 // Pointeur � lib�rer
};

// Ajoute un pointeur dans le conteneur malloc
void ConteneurMalloc_Ajoute (InfoMalloc* info);

// Affiche le contenu du conteneur malloc (lib�re les pointeurs si
// libere_mem=true)
void ConteneurMalloc_Affiche (bool libere_mem);

// Sort un pointeur du conteneur malloc
void ConteneurMalloc_Remplace (InfoMalloc *Info, InfoMalloc *NvInfo);

// Sort un pointeur du conteneur malloc
void ConteneurMalloc_Efface (struct PtrChaineMalloc *ptr);

// Calcule la somme de v�rification (checksum) des donn�es d'un pointeur
unsigned long MallocCalculeSomme (const InfoMalloc *Info);

// Fonction qui �crit les informations d'un pointeur et modifie son adresse
// pour qu'il pointe sur les donn�es
void *MallocPreparePtr (InfoMalloc *Info, size_t size,
                        const char *nom_fich, unsigned long num_ligne);

// Lib�ration s�curis�e : v�rifie que le pointeur soit valide
void FreeSecurise (const void *ptr,
                   const char* NomFich, const unsigned long NumLigne);

// Macro qui rajoutent automatiquement le nom du fichier et la ligne
# define free(ptr) FreeSecurise(ptr, __FILE__, __LINE__)
#endif

//-----------------------------------------------------------------------------

// Tampon anti-overflow (�criture en dehors d'une zone m�moire allou�e)
#ifdef DEBUG_MALLOC_OVERFLOW
// V�rifie que le conteneur soit activ�
#  ifndef DEBUG_MALLOC_CONTENEUR
#    error "L'anti-overflow n�cessite '#define DEBUG_MALLOC_CONTENEUR'"
#  endif
#endif

//-----------------------------------------------------------------------------
#endif

