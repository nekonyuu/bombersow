/*
 * Allocation de mémoire sécurisée :
 * - Vérifie que malloc (ou realloc) ne renvoie pas NULL
 * - Evite les fuites de mémoire (memory leak)
 * - Vérifie les erreurs d'écriture en dehors de la mémoire allouée
 *   (underflow et overflow)
 *
 * Ce code est sous licence GPL et c'est le code simplifié de la librairie
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

// Allocation sécurisée : vérifie que la mémoire est effectivement
// allouée lors des allocations
#define DEBUG_MALLOC

// Conteneur Malloc :
// - Liste les pointeurs alloués (évite les fuites de mémoire)
// - Avant de libérer un pointeur, vérifie qu'il est effectivement alloué!
#define DEBUG_MALLOC_CONTENEUR

// Tampon anti-overflow :
// - Vérifie que la mémoire allouée n'ai pas été modifié en dehors
//   de l'espace alloué
#define DEBUG_MALLOC_OVERFLOW

//-----------------------------------------------------------------------------

// Inclut les principales librairies pour éviter le conflit de nom (strdup,
// malloc, new, ..)
#ifdef DEBUG_MALLOC
#  include <string.h>
#  ifdef __cplusplus
#    include <new>
#  endif
#endif

//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC
// Allocation sécurisée : vérifie que l'allocation s'est bien passée,
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
// Vérifie qu'on débogue Malloc
# ifndef DEBUG_MALLOC
#  error "Le conteneur malloc nécessite #define DEBUG_MALLOC:halloc.h"
# endif

// Informations écrites avant les données du pointeur
typedef struct
{
    unsigned long nbr_magique;       // Nombre magique
    bool est_alloue;                 // Pointeur alloué ou libéré ?
    size_t taille;                   // Taille allouée (sans ces informations)
    const char* nom_fich;            // Nom du fichier source
    unsigned long num_ligne;         // Ligne du fichier
    struct PtrChaineMalloc *ptr_liste;  // Pointeur dans la liste
    unsigned long somme_info;        // Somme des informations (checksum)
} InfoMalloc;

// Conteneur : liste des pointeurs allouées
// Un pointeur chaîné
struct PtrChaineMalloc
{
    struct PtrChaineMalloc *precedent;    // Elément suivant
    struct PtrChaineMalloc *suivant;      // Elément suivant
    unsigned long nbr_magique;            // Nombre magique
    InfoMalloc *ptr_info;                 // Pointeur à libérer
};

// Ajoute un pointeur dans le conteneur malloc
void ConteneurMalloc_Ajoute (InfoMalloc* info);

// Affiche le contenu du conteneur malloc (libère les pointeurs si
// libere_mem=true)
void ConteneurMalloc_Affiche (bool libere_mem);

// Sort un pointeur du conteneur malloc
void ConteneurMalloc_Remplace (InfoMalloc *Info, InfoMalloc *NvInfo);

// Sort un pointeur du conteneur malloc
void ConteneurMalloc_Efface (struct PtrChaineMalloc *ptr);

// Calcule la somme de vérification (checksum) des données d'un pointeur
unsigned long MallocCalculeSomme (const InfoMalloc *Info);

// Fonction qui écrit les informations d'un pointeur et modifie son adresse
// pour qu'il pointe sur les données
void *MallocPreparePtr (InfoMalloc *Info, size_t size,
                        const char *nom_fich, unsigned long num_ligne);

// Libération sécurisée : vérifie que le pointeur soit valide
void FreeSecurise (const void *ptr,
                   const char* NomFich, const unsigned long NumLigne);

// Macro qui rajoutent automatiquement le nom du fichier et la ligne
# define free(ptr) FreeSecurise(ptr, __FILE__, __LINE__)
#endif

//-----------------------------------------------------------------------------

// Tampon anti-overflow (écriture en dehors d'une zone mémoire allouée)
#ifdef DEBUG_MALLOC_OVERFLOW
// Vérifie que le conteneur soit activé
#  ifndef DEBUG_MALLOC_CONTENEUR
#    error "L'anti-overflow nécessite '#define DEBUG_MALLOC_CONTENEUR'"
#  endif
#endif

//-----------------------------------------------------------------------------
#endif

