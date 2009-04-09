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

#include "halloc.h"
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC_CONTENEUR
  const char *delete_FILE = NULL;
  unsigned long delete_LINE = 0;
#endif

#ifdef DEBUG_MALLOC_CONTENEUR
  // Le conteneur Malloc
  struct
  {
    struct PtrChaineMalloc *debut;   // Pointeur vers le premier élément
    struct PtrChaineMalloc *fin;     // Pointeur vers le dernier élément
  } ConteneurMalloc = {NULL,NULL};
  const unsigned long TAILLE_INFO_MALLOC=sizeof(InfoMalloc);
#else
  const unsigned long TAILLE_INFO_MALLOC=0;
#endif

#ifdef DEBUG_MALLOC_OVERFLOW
  const unsigned long TAILLE_TAMPON_OVERFLOW=16;
#else
  const unsigned long TAILLE_TAMPON_OVERFLOW=0;
#endif

//-----------------------------------------------------------------------------

// Affiche un message d'erreur, puis quitte le programme
void ERREUR (char *message, ...)
{
  va_list arg;
  va_start (arg, message);
  printf ("ERREUR : ");
  vprintf (message, arg);
  printf ("\n");
  printf ("Pressez <ENTREE> pour quitter : ");
  getchar();
  va_end (arg);
  exit (1);
}

//-----------------------------------------------------------------------------

// Désactive les macros pour sécuriser l'allocation de mémoire
#undef malloc
#undef calloc
#undef realloc
#undef strdup
#undef free
#undef new
#undef delete

// Nombres magiques pour valider un pointeur
#define MALLOC_NBR_MAGIQUE 0x1A71A25C

//-----------------------------------------------------------------------------
//--- Tests anti-overflow (écriture en dehors de la zone allouée --------------
//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC_OVERFLOW
// Prépare la zone anti-overflow d'un pointeur
// (ptr pointe sur les infos de débogage InfoMalloc)
void Malloc_PrepareOverflow (InfoMalloc *info)
{
  char *ptr;
  unsigned long i;
  unsigned char rand = 0xA0;

  // Calcule la position du tampon anti-overflow
  ptr = (char *)info +sizeof(InfoMalloc) +info -> taille;

  // Remplit le tampon avec des valeurs fixées
  for (i=0; i<TAILLE_TAMPON_OVERFLOW; i++)
  {
    *ptr = rand;
    ptr++;
    rand++;
  }
}

//-----------------------------------------------------------------------------

// Vérifie la tampon anti-overflow : renvoie true s'il n'a pas été touché,
// false sinon
// (ptr pointe sur les informations de débogage InfoMalloc)
bool Malloc_TamponOverflowIntact (const InfoMalloc *info)
{
  char *ptr;
  unsigned long i;
  unsigned char rand = 0xA0;

  // Calcule la position du tampon anti-overflow
  ptr = (char *)info +sizeof(InfoMalloc) +info -> taille;

  // Remplit le tampon avec des valeurs fixées
  for (i=0; i<TAILLE_TAMPON_OVERFLOW; i++)
  {
    // Valeur différente de celle définie ?
    if ((unsigned char)*ptr != rand)
    {
      // Erreur !
      return false;
    }
    ptr++;
    rand++;
  }

  // Tampon intact
  return true;
}
#endif

//-----------------------------------------------------------------------------
//--- Conteneur Malloc : liste des pointeurs alloués --------------------------
//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC_CONTENEUR
// Calcule la somme de vérification (checksum) des données d'un pointeur
unsigned long MallocCalculeSomme (const InfoMalloc *info)
{
  // Calcule la somme des données d'un pointeur
  return
   ((unsigned long)(info -> nbr_magique)
   +(unsigned long)(info -> est_alloue)
   +(unsigned long)(info -> taille)
   +(unsigned long)(info -> nom_fich)
   +(unsigned long)(info -> num_ligne));
}

//-----------------------------------------------------------------------------

// Fonction qui écrit les informations d'un pointeur et modifie son adresse
// pour qu'il pointe sur les données
void *MallocPreparePtr (InfoMalloc *info, size_t size,
                        const char *nom_fich, unsigned long num_ligne)
{
  // Ecrit l'entête //
  info -> nbr_magique = MALLOC_NBR_MAGIQUE;
  info -> est_alloue = true;
  info -> taille = size;
  info -> nom_fich = nom_fich;
  info -> num_ligne = num_ligne;
  info -> somme_info = MallocCalculeSomme (info);

#ifdef DEBUG_MALLOC_OVERFLOW
  /* Prépare le tampon anti-overflow */
  Malloc_PrepareOverflow (info);
#endif

  /* Passe les informations pour obtenir un pointeur sur les données */
  return (info+1);
}

//-----------------------------------------------------------------------------

// Ajoute un pointeur au conteneur malloc
void ConteneurMalloc_Ajoute (InfoMalloc* info)
{
  // Tente d'allouer un nouvel élément à la liste
  struct PtrChaineMalloc *elem;
  elem = (struct PtrChaineMalloc *)malloc(sizeof(struct PtrChaineMalloc));

  // Plus de mémoire ):-(
  if (elem == NULL)
  {
    // Erreur
    ERREUR (
      "Impossible d'agrandir le conteneur des malloc : plus assez "
      "de mémoire libre disponnible !(tampon %s:%u)",
            info -> nom_fich, info -> num_ligne);
  }

  // Sauve les informations
  elem -> nbr_magique = MALLOC_NBR_MAGIQUE;
  elem -> ptr_info = info;
  info -> ptr_liste = elem;

  //--- Ajout en queue ---

  // Modifie le début de la liste (si nécessaire)
  if (ConteneurMalloc.debut == NULL) ConteneurMalloc.debut = elem;

  // Ajoute l'élément en queue
  elem -> suivant = NULL;
  if (ConteneurMalloc.fin != NULL) ConteneurMalloc.fin -> suivant = elem;
  elem -> precedent = ConteneurMalloc.fin;
  ConteneurMalloc.fin = elem;
}

//-----------------------------------------------------------------------------

// Vérifie les informations sur un pointeur
void VerifieInfoMalloc (InfoMalloc* info)
{
  unsigned long somme;

  // Vérifie les nombres magiques
  if (info -> nbr_magique != MALLOC_NBR_MAGIQUE)
    ERREUR ("Pointeur invalide(tampon %s:%u)",
            info -> nom_fich, info -> num_ligne);

  // Vérifie la somme
  if (info -> somme_info != MallocCalculeSomme (info))
    ERREUR ("Somme incorrecte(tampon %s:%u)",
            info -> nom_fich, info -> num_ligne);

#ifdef DEBUG_MALLOC_OVERFLOW
  // Vérifie qu'on est pas écrit en dehors du tampon
  if (!Malloc_TamponOverflowIntact(info))
  {
    ERREUR ("Erreur d'overflow (tampon %s:%u)",
            info -> nom_fich, info -> num_ligne);
  }
#endif
}

//-----------------------------------------------------------------------------

// Affiche le contenu du conteneur malloc (libère les pointeurs si
// libere_mem=true)
void ConteneurMalloc_Affiche (bool libere_mem)
{
  struct PtrChaineMalloc *elem, *suivant;
  bool titre_affiche = false;  // Le titre a déjà été affiché ?
  unsigned long nbr_pointeur=0;       // Nombre de pointeurs non libérés
  unsigned long taille_totale=0;      // Taille totale en octet non libérée
  InfoMalloc* info;
  char* vrai_ptr;

  // Parcoure la liste du début à la fin
  elem = ConteneurMalloc.debut;
  while (elem != NULL)
  {
    // Sauve le pointeur suivant
    suivant = elem -> suivant;

    // Calcule l'adresse du vrai pointeur
    info = elem -> ptr_info;
    vrai_ptr = (char *)(info+1);

    // Vérifie les informations
    VerifieInfoMalloc (info);

    // Affiche le titre si nécessaire
    if (titre_affiche == false)
    {
      titre_affiche = true;
      printf ("[ Memoire non liberee ]\n");
    }

    // Affiche le nom du fichier et la ligne
    printf ("%s:%lu - ",
            info -> nom_fich,
            (unsigned long)info -> num_ligne);

    // Affiche le pointeur si demandé
    printf ("pointeur %p de taille %lu\n",
            vrai_ptr,
            (unsigned long)info -> taille);

    // Ajoute tout ca au total
    nbr_pointeur++;
    taille_totale += info -> taille;

    // Sort le pointeur de la liste
    if (libere_mem)
    {
      ConteneurMalloc_Efface (elem);
    }

    // Passe au pointeur suivant
    elem = suivant;
  }

  // Affiche le total
  if (titre_affiche == true)
  {
    printf ("TOTAL = %lu pointeur(s) pour %lu octets.\n",
            nbr_pointeur,
            taille_totale);
    printf ("Pressez <ENTREE> pour continuer : ");
    getchar ();
  }
}

//-----------------------------------------------------------------------------

// Sort un pointeur du conteneur malloc
void ConteneurMalloc_Remplace (InfoMalloc *info, InfoMalloc *nv_info)
{
  struct PtrChaineMalloc *elem;

  // Cherche les informations
  elem = info -> ptr_liste;
  if (elem -> nbr_magique != MALLOC_NBR_MAGIQUE)
    ERREUR ("Erreur de réallocation de mémoire(tampon %s:%u)",
            info -> nom_fich, info -> num_ligne);

  // Enfin, remplace le pointeur
  elem -> ptr_info = nv_info;
}

//-----------------------------------------------------------------------------

// Sort un pointeur du conteneur malloc
void ConteneurMalloc_Efface (struct PtrChaineMalloc* elem)
{
  // Pointeur déjà désalloué ?
  if (elem -> ptr_info -> est_alloue == false)
  {
    ERREUR ("Pointeur déjà desalloué !");
  }

  // Sort l'élément de la liste
  if (elem -> precedent != NULL)
    (elem -> precedent) -> suivant = elem -> suivant;
  if (elem -> suivant != NULL)
    (elem -> suivant) -> precedent = elem -> precedent;

  // Change le début (si l'élément était au début de la liste)
  if (ConteneurMalloc.debut == elem) ConteneurMalloc.debut = elem -> suivant;

  // Change la fin (si l'élément était à la fin de la liste)
  if (ConteneurMalloc.fin == elem) ConteneurMalloc.fin = elem -> precedent;

  // Détruit l'élément (ne libère pas la mémoire)
  free (elem);
}

//-----------------------------------------------------------------------------

// free sécurisé : utilisé pour le conteneur
void FreeSecurise (const void *ptr,
	  	   const char* nom_fich, const unsigned long num_ligne)
{
  InfoMalloc *Info;

  // Pointeur NULL : La norme demande de ne rien faire, on va pas se gêner!
  if (ptr == NULL) return;

  // Rêgle le pointeur sur les données
  Info = (InfoMalloc *)ptr-1;

  // Vérifie les informations
  VerifieInfoMalloc (Info);

  // Sort le pointeur du conteneur en vérifiant les
  // informations sur le pointeur
  ConteneurMalloc_Efface (Info -> ptr_liste);

  // Le pointeur n'est plus alloué !
  Info -> est_alloue = false;

  // Enfin, libère la mémoire
  free (Info);
}
#endif // de "#ifdef DEBUG_MALLOC_CONTENEUR"

//-----------------------------------------------------------------------------
//--- Allocation sécurisée de mémoire -----------------------------------------
//-----------------------------------------------------------------------------

#ifdef DEBUG_MALLOC
// Calcule la taille réelle d'un bloc alloué
size_t MallocCalculeTaille (size_t size)
{
 return size +(TAILLE_INFO_MALLOC+TAILLE_TAMPON_OVERFLOW);
}

//-----------------------------------------------------------------------------

// Allocation de mémoire sécurisée (malloc) : test si malloc renvoie NULL
void* MallocSecurise (size_t size,
                      const char* nom_fich, const unsigned long num_ligne)
{
  // Tente d'allouer le tampon
  void* ptr;
  size_t TailleAllouee;

  // Taille nulle : Erreur!
  if (size == 0) ERREUR ("Taille du pointeur à allouer nulle !(tampon %s:%u)",
            nom_fich, num_ligne);

  // Ajoute la taille des informations sur le pointeur
  TailleAllouee = MallocCalculeTaille (size);

  // Alloue la mémoire
  ptr = malloc(TailleAllouee);

  // Pointeur NULL : erreur!
  if (ptr == NULL)
  {
    ERREUR ("Allocation de mémoire impossible : "
            "pas assez de mémoire libre(tampon %s:%u)",
            nom_fich, num_ligne);
  }

#ifdef DEBUG_MALLOC_CONTENEUR
  // Ajoute le pointeur au conteneur
  ConteneurMalloc_Ajoute ((InfoMalloc *)ptr);

  // Ecrit les informations sur le pointeur et modifie l'adresse
  // du pointeur (si on compile avec le conteneur malloc)
  ptr = MallocPreparePtr ((InfoMalloc *)ptr,size,nom_fich,num_ligne);
#endif

  // Retourne le pointeur
  return ptr;
}

//-----------------------------------------------------------------------------

// calloc sécurisé : test si calloc renvoie NULL
void* CallocSecurise (size_t nitems, size_t size,
                      const char* nom_fich, const unsigned long num_ligne)
{
  void* ptr;

  // Alloue de la mémoire
  ptr = MallocSecurise (size*nitems,nom_fich,num_ligne);

  // Mise à zéro des octets (fonctionnalité de 'calloc'=malloc+memset)
  memset (ptr,0,size);

  // Retourne le pointeur
  return ptr;
}

//-----------------------------------------------------------------------------

// realloc sécurisé : vérifie que l'allocation se soit bien produite,
// affiche une erreur sinon
void* ReallocSecurise (void* ptr, size_t size,
                       const char* nom_fich, const unsigned long num_ligne)
{
  void* Nouveau;
#ifdef DEBUG_MALLOC_CONTENEUR
  InfoMalloc *NvInfo, *Info;
#endif
  size_t TailleAllouee;

  // Si l'ancien pointeur actuel vaut NULL : appelle malloc
  if (ptr == NULL) return MallocSecurise (size, nom_fich, num_ligne);

  // Ajoute la taille des informations sur le pointeur
  TailleAllouee = MallocCalculeTaille (size);

#ifdef DEBUG_MALLOC_CONTENEUR
  // Positionne le pointeur sur les données
  Info = (InfoMalloc *)ptr-1;

  // Vérifie le pointeur
  VerifieInfoMalloc (Info);

  // Tente d'agrandir le pointeur
  Nouveau = realloc(Info,TailleAllouee);
#else
  // Tente d'agrandir le pointeur
  Nouveau = realloc(ptr,TailleAllouee);
#endif

  // Réallocation impossible ?
  if (Nouveau == NULL)
  {
    // Affiche une erreur
    ERREUR ("Allocation de mémoire impossible : "
            "pas assez de mémoire libre(tampon %s:%u)",
            nom_fich, num_ligne);
  }

#ifdef DEBUG_MALLOC_CONTENEUR
  // Remplace l'ancien pointeur par le nouveau dans le conteneur
  NvInfo = (InfoMalloc *)Nouveau;
  ConteneurMalloc_Remplace (Info,NvInfo);

  // Ecrit les informations sur le pointeur
  Nouveau = MallocPreparePtr (NvInfo,size,nom_fich,num_ligne);
#endif

  // Renvoie le pointeur
  return Nouveau;
}

//-----------------------------------------------------------------------------

// Crée une copie d'une chaîne avec test si l'allocation renvoie NULL
char* StrdupSecurise(const char* src, const char* nom_fich, const unsigned long num_ligne)
{
  // Tente de copier la chaine
  char* ptr;

#ifdef DEBUG_MALLOC_CONTENEUR
  // Alloue la mémoire
  ptr = (char *)MallocSecurise (strlen(src)+1, nom_fich, num_ligne);
#else
  ptr = (char *)malloc (strlen(src)+1);
  if (ptr == NULL)
  {
    ERREUR (
      "Impossible de dupliquer une chaîne de caractère : "
      "Plus assez de mémoire disponnible !(tampon %s:%u)",
        nom_fich, num_ligne);
  }
#endif

  // Copie la chaine
  strcpy (ptr,src);

  // Retourne le pointeur
  return ptr;
}

//-----------------------------------------------------------------------------
//--- Allocation C++ : new/delete ---------------------------------------------
//-----------------------------------------------------------------------------

#ifdef __cplusplus
// Surchage de l'opérateur new
void* operator new (size_t size, const char* nom, const unsigned long ligne)
  throw (std::bad_alloc)
{
  return MallocSecurise (size, nom, ligne);
}

//-----------------------------------------------------------------------------

// Surchage de l'opérateur new[]
void* operator new[] (size_t size, const char* nom, const unsigned long ligne)
  throw (std::bad_alloc)
{
  return MallocSecurise (size, nom, ligne);
}

//-----------------------------------------------------------------------------

// Surchage de l'opérateur delete
void operator delete (void *ptr) throw()
{
#ifdef DEBUG_MALLOC_CONTENEUR
  FreeSecurise (ptr, delete_FILE, delete_LINE);
  delete_FILE = NULL;
  delete_LINE = 0;
#else
  free (ptr);
#endif
}

//-----------------------------------------------------------------------------

// Surchage de l'opérateur delete[]
void operator delete[] (void *ptr) throw()
{
#ifdef DEBUG_MALLOC_CONTENEUR
  FreeSecurise (ptr, delete_FILE, delete_LINE);
  delete_FILE = NULL;
  delete_LINE = 0;
#else
  free (ptr);
#endif
}
#endif // de "#ifdef __cplusplus"
#endif // de "#ifdef DEBUG_MALLOC"

//-----------------------------------------------------------------------------

