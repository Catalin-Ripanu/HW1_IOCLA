#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_LINE_SIZE 300
#define CONS 5
struct Dir;
struct File;
/*Structurile necesare*/
/*Structura unui director*/
typedef struct Dir
{
  char *name;
  struct Dir *parent;
  struct File *head_children_files;
  struct Dir *head_children_dirs;
  struct Dir *next;
}
Dir;
/*Structura unui fisier*/
typedef struct File
{
  char *name;
  struct Dir *parent;
  struct File *next;
}
File;
/*O functie ce initializeaza radacina home*/
void
init_dir (Dir ** home_dir)
{
  /*Se aloca memorie */
  *home_dir = (Dir *) malloc (sizeof (*(*home_dir)));
  /*Se aloca memorie numelui */
  (*home_dir)->name = (char *) malloc (CONS * sizeof (*((*home_dir)->name)));
  strcpy ((*home_dir)->name, "home");
  /*Structura de fisiere si directoare este nula */
  (*home_dir)->parent = NULL;
  (*home_dir)->next = NULL;
  (*home_dir)->head_children_dirs = NULL;
  (*home_dir)->head_children_files = NULL;
}

/*Functie ce ajuta la implementarea functiilor touch si mkdir*/
/*Aceasta functie verifica existenta unui fisier/director in lista*/
int
create_condition (Dir * creator, char *name)
{
/*Variabile ce ajuta la cautare*/
  File *search;
  Dir *find;
  int condition1_file_dir = 1, condition2_file_dir = 1;
  /*Se obtine primul fisier din lista */
  search = creator->head_children_files;
  if (search)
    {
      while (search->next)
	{
	  /*Daca s-a gasit fisierul respectiv */
	  if (strcmp (search->name, name) == 0)
	    {
	      /*Conditia are valoarea 0 */
	      condition1_file_dir = 0;
	      /*Cautarea s-a terminat */
	      break;
	    }
	  /*Se parcurge lista pana la ultimul element */
	  search = search->next;
	}
      /*Se trateaza cazul in care fisierul cautat este ultimul element din lista */
      if (strcmp (search->name, name) == 0 && search->next == NULL)
	condition1_file_dir = 0;
    }
  else
    /*Conditia are valoarea 1 daca nu s-a gasit fisierul */
    condition1_file_dir = 1;
  /*Se obtine primul director din lista */
  find = creator->head_children_dirs;
  if (find)
    {
      while (find->next)
	{
	  /*Daca s-a gasit directorul respectiv */
	  if (strcmp (find->name, name) == 0)
	    {
	      /*Conditia are valoarea 0 */
	      condition2_file_dir = 0;
	      /*Cautarea s-a terminat */
	      break;
	    }
	  /*Se parcurge lista pana la ultimul element */
	  find = find->next;
	}
      /*Se trateaza cazul in care directorul cautat este ultimul element din lista */
      if (strcmp (find->name, name) == 0 && find->next == NULL)
	condition2_file_dir = 0;
    }
  else
    /*Conditia are valoarea 1 daca nu s-a gasit fisierul */
    condition2_file_dir = 1;
  /*Daca nu s-a gasit fisierul/directorul respectiv */
  /*Functia intoarce 1 */
  if (condition1_file_dir == 1 && condition2_file_dir == 1)
    return 1;
  else
    return -1;
}

/*Functie ce adauga un fisier in lista*/
void
touch (Dir * parent, char *name)
{
  int decision;
  /*Daca fisierul cu numele name nu exista */
  decision = create_condition (parent, name);
  if (decision == 1)
    {
      /*Aloc memorie pentru el */
      File *newfile;
      newfile = (File *) malloc (sizeof (*newfile));
      newfile->name =
	(char *) malloc ((strlen (name) + 1) * sizeof (*(newfile->name)));
      strcpy (newfile->name, name);
      newfile->next = NULL;
      newfile->parent = parent;
      /*Daca directorul parent nu are fisiere */
      if (parent->head_children_files == NULL)
	/*Fisierul adaugat este primul element */
	parent->head_children_files = newfile;
      else
	{
	  File *aux;
	  /*Se parcurge lista pentru a se adauga fisierul cel nou la sfarsit */
	  /*Se salveaza adresa */
	  aux = parent->head_children_files;
	  while (parent->head_children_files->next)
	    parent->head_children_files = parent->head_children_files->next;
	  parent->head_children_files->next = newfile;
	  /*Se utilizeaza adresa salvata */
	  parent->head_children_files = aux;
	}
    }
  else
    /*Mesaj de eroare */
    printf ("File already exists\n");
}

/*Functie ce adauga un director in lista*/
void
mkdir (Dir * parent, char *name)
{
  int decision;
  /*Daca directorul cu numele name nu exista */
  decision = create_condition (parent, name);
  if (decision == 1)
    {
      /*Aloc memorie pentru el */
      Dir *newdir;
      newdir = (Dir *) malloc (sizeof (*newdir));
      newdir->name =
	(char *) malloc ((strlen (name) + 1) * sizeof (*(newdir->name)));
      strcpy (newdir->name, name);
      newdir->next = NULL;
      newdir->parent = parent;
      newdir->head_children_dirs = NULL;
      newdir->head_children_files = NULL;
      /*Daca directorul parent nu are directoare */
      if (parent->head_children_dirs == NULL)
	/*Directorul adaugat este primul element */
	parent->head_children_dirs = newdir;
      else
	{
	  Dir *aux;
	  /*Se parcurge lista pentru a se adauga directorul cel nou la sfarsit */
	  /*Se salveaza adresa */
	  aux = parent->head_children_dirs;
	  while (parent->head_children_dirs->next)
	    parent->head_children_dirs = parent->head_children_dirs->next;
	  parent->head_children_dirs->next = newdir;
	  /*Se utilizeaza adresa salvata */
	  parent->head_children_dirs = aux;
	}
    }
  else
    /*Mesaj de eroare */
    printf ("Directory already exists\n");
}

/*Functie ce afiseaza continutul unui director*/
void
ls (Dir * parent)
{
  File *aux1_helper;
  Dir *aux2_helper;
  /*Se obtine adresa primului element din lista de fisiere */
  aux1_helper = parent->head_children_files;
  /*Se obtine adresa primului element din lista de directoare */
  aux2_helper = parent->head_children_dirs;
  /*Se afiseaza directoarele */
  /*Daca in lista se afla doar un director */
  if (aux2_helper != NULL && aux2_helper->next == NULL)
    {
      printf ("%s\n", aux2_helper->name);
    }
  /*Daca in lista exista mai multe directoare */
  else if (aux2_helper != NULL)
    {
      while (aux2_helper->next)
	{
	  printf ("%s\n", aux2_helper->name);
	  /*Se trece la urmatorul element */
	  aux2_helper = aux2_helper->next;
	}
      printf ("%s\n", aux2_helper->name);
    }
  /*Se afiseaza fisierele */
  /*Daca in lista se afla doar un fisier */
  if (aux1_helper != NULL && aux1_helper->next == NULL)
    {
      printf ("%s\n", aux1_helper->name);
    }
  /*Daca in lista exista mai multe fisiere */
  else if (aux1_helper != NULL)
    {
      while (aux1_helper->next)
	{
	  printf ("%s\n", aux1_helper->name);
	  /*Se trece la urmatorul element */
	  aux1_helper = aux1_helper->next;
	}
      printf ("%s\n", aux1_helper->name);
    }
}

/*Functie ce schimba directorul curent*/
void
cd (Dir ** target, char *name)
{
  /*Se obtine adresa primului element din lista */
  Dir *find = (*target)->head_children_dirs;
  /*Se retine rezultatul schimbarii (1 pentru rezultat pozitiv,0 pentru rezultat negativ) */
  int cont = 0;
  /*Daca se doreste schimbarea directorului curent cu un alt director */
  if (strcmp (name, "..") != 0)
    {
      if (find != NULL)
	{
	  /*Daca primul element din lista este directorul dorit */
	  if (strcmp (find->name, name) == 0)
	    {
	      *target = (*target)->head_children_dirs;
	      /*Rezultatul este pozitiv */
	      cont = 1;
	    }
	  /*Se parcurg vecinii directorului curent */
	  else
	    {
	      while (find->next)
		{
		  find = find->next;
		  /*Daca s-a gasit directorul dorit printre vecini */
		  if (strcmp (find->name, name) == 0)
		    {
		      /*Se schimba adresa ( target este un Dir**) */
		      *target = find;
		      /*Rezultatul este pozitiv */
		      cont = 1;
		      break;
		    }
		}
	      /*Daca ultimul element din lista este directorul dorit */
	      if (strcmp (find->name, name) == 0)
		{
		  /*Se schimba adresa ( target este un Dir**) */
		  *target = find;
		  /*Rezultatul este pozitiv */
		  cont = 1;
		}
	    }
	}
    }
  else
    /*Daca se doreste schimbarea directorului curent cu parintele acestuia */
    {
      /*Daca parintele nu este NULL */
      if ((*target)->parent != NULL)
	{			/*Se obtine adresa */
	  *target = (*target)->parent;
	}
      /*Rezultatul este pozitiv */
      cont = 1;
    }
  /*Rezultatul este negativ */
  if (cont == 0)
    printf ("No directories found!\n");
}

/*Functie ce elibereaza in mod recursiv toata memoria alocata*/
void
stop (Dir * target)
{
  /*Variabile ce vor parcurge listele */
  File *aux1_helper;
  Dir *aux2_helper;
  /*Se obtine primul element din lista de fisiere */
  aux1_helper = target->head_children_files;
  if (aux1_helper != NULL)
    {
      while (aux1_helper->next)
	{
	  /*Se elibereaza memoria alocata */
	  aux1_helper = target->head_children_files->next;
	  free (target->head_children_files->name);
	  free (target->head_children_files);
	  /*Se continua parcurgerea */
	  target->head_children_files = aux1_helper;
	}
      /*Se elibereaza memoria alocata numelui */
      free (aux1_helper->name);
      free (aux1_helper);
    }
  /*Se obtine primul element din lista de directoare */
  aux2_helper = target->head_children_dirs;
  if (aux2_helper != NULL)
    {
      while (aux2_helper->next)
	{
	  /*Se elibereaza memoria alocata */
	  if (aux2_helper->head_children_dirs != NULL
	      || aux2_helper->head_children_files != NULL)
	    /*Daca directorul respectiv contine subdirectoare si liste */
	    /*Se apeleaza recursiv 'stop' */
	    stop (aux2_helper);
	  aux2_helper = target->head_children_dirs->next;
	  free (target->head_children_dirs->name);
	  free (target->head_children_dirs);
	  /*Se continua stergerea */
	  target->head_children_dirs = aux2_helper;
	}
      /*Caz separat pentru ultimul element din lista */
      if (aux2_helper->head_children_dirs != NULL
	  || aux2_helper->head_children_files != NULL)
	stop (aux2_helper);
      free (aux2_helper->name);
      free (aux2_helper);
    }
  /*Se elibereaza memoria alocata radacinii (home) */
  if (target->parent == NULL)
    {
      free (target->name);
      free (target);
    }

}

/*Functie ce sterge un fisier dintr-o lista*/
void
rm (Dir * parent, char *name)
{
  /*Variabile ce vor ajuta la stergere */
  File *search;
  File *aux;
  /*1 daca s-a sters,0 daca nu */
  int contor = 0;
  /*Se retine adresa primului element din lista */
  search = parent->head_children_files;
  if (search != NULL)
    {
      /*Daca elementul cautat este primul */
      if (strcmp (search->name, name) == 0)
	{
	  /*Se efectueaza stergerea */
	  parent->head_children_files = parent->head_children_files->next;
	  free (search->name);
	  free (search);
	  search = NULL;
	  /*1 intrucat s-a sters */
	  contor = 1;
	}
      else
	{
	  /*Se foloseste algoritmul de stergere dintr-o lista simplu inlantuita */
	  aux = search;
	  search = search->next;
	  while (search != NULL && search->next)
	    {
	      /*Daca s-a gasit elementul */
	      if (strcmp (search->name, name) == 0)
		{
		  aux->next = search->next;
		  /*Se sterge */
		  free (search->name);
		  free (search);
		  search = NULL;
		  /*1 intrucat s-a sters */
		  contor = 1;
		  break;
		}
	      /*Parcurgere */
	      aux = search;
	      search = search->next;
	    }
	  /*Daca elementul cautat este  ultimul */
	  if (search != NULL && strcmp (search->name, name) == 0
	      && search->next == NULL)
	    {
	      /*Se sterge */
	      free (search->name);
	      free (search);
	      aux->next = NULL;
	      contor = 1;
	    }
	}
    }
  /*0 daca nu s-a sters */
  if (contor == 0)
    printf ("Could not find the file\n");
}

/*Functie ce sterge un director dintr-o lista*/
void
rmdir (Dir * parent, char *name)
{
  /*Variabile ce vor ajuta la stergere */
  Dir *search;
  Dir *aux;
  /*1 daca s-a sters,0 daca nu */
  int contor = 0;
  /*Se retine adresa primului element din lista */
  search = parent->head_children_dirs;
  if (search != NULL)
    {
      /*Daca elementul cautat este primul */
      if (strcmp (search->name, name) == 0)
	{
	  /*Se efectueaza stergerea */
	  parent->head_children_dirs = parent->head_children_dirs->next;
	  /*In cazul in care continutul directorului curent nu este NULL */
	  /*Se elibereaza memoria alocata acelui continut */
	  stop (search);
	  /*Se sterge elementul */
	  free (search->name);
	  free (search);
	  search = NULL;
	  /*1 intrucat s-a sters */
	  contor = 1;
	}
      else
	{
	  /*Se foloseste algoritmul de stergere dintr-o lista simplu inlantuita */
	  aux = search;
	  search = search->next;
	  while (search != NULL && search->next)
	    {
	      /*Daca s-a gasit elementul */
	      if (strcmp (search->name, name) == 0)
		{
		  aux->next = search->next;
		  /*Se elibereaza memoria alocata continutului */
		  stop (search);
		  /*Se sterge */
		  free (search->name);
		  free (search);
		  search = NULL;
		  /*1 intrucat s-a sters */
		  contor = 1;
		  break;
		}
	      /*Parcurgere */
	      aux = search;
	      search = search->next;
	    }
	  /*Daca elementul cautat este  ultimul */
	  if (search != NULL && strcmp (search->name, name) == 0
	      && search->next == NULL)
	    {
	      /*Se elibereaza memoria alocata continutului */
	      stop (search);
	      /*Se sterge */
	      free (search->name);
	      free (search);
	      aux->next = NULL;
	      /*1 intrucat s-a sters */
	      contor = 1;
	    }
	}
    }
  /*0 daca nu s-a sters */
  if (contor == 0)
    printf ("Could not find the dir\n");
}

/*Functie ce intoarce calea absoluta*/
char *
pwd (Dir * target)
{
  /*Adresa in care se retine sirul rezultat */
  char *result;
  result = (char *) malloc (MAX_INPUT_LINE_SIZE * sizeof (*result));
  strcpy (result, "");
  /*O adresa ajutatoare */
  char *helper;
  helper = (char *) malloc (MAX_INPUT_LINE_SIZE * sizeof (*helper));
  /*Mic algoritm pentru a construi calea absoluta */
  while (target != NULL)
    {
      strcpy (helper, "/");
      strcat (helper, target->name);
      /*Se retine rezultatul */
      strcpy (result, strcat (helper, result));
      target = target->parent;
    }
  free (helper);
  /*Se returneaza rezultatul */
  return result;
}

/*Functie ce afiseaza o forma arborescenta folosind recursivitate*/
void
tree (Dir * target, int level)
{
  /*Variabile ce vor parcurge listele */
  Dir *aux_iterator;
  File *aux1_helper;
  /*Se obtine adresa primului element din lista de directoare */
  aux_iterator = target->head_children_dirs;
  while (aux_iterator != NULL)
    {
      /*Se afiseaza respectand formatul din cerinta */
      printf ("%*s\n", 4 * level + (int) strlen (aux_iterator->name),
	      aux_iterator->name);
      /*Se trece la nivelul urmator */
      tree (aux_iterator, level + 1);
      /*Se trece la vecinul urmator */
      aux_iterator = aux_iterator->next;
    }
  /*Se obtine adresa primului element din lista de fisiere */
  aux1_helper = target->head_children_files;
  while (aux1_helper != NULL)
    {
      /*Se afiseaza respectand formatul din cerinta */
      printf ("%*s\n", 4 * level + (int) strlen (aux1_helper->name),
	      aux1_helper->name);
      /*Se trece la vecinul urmator */
      aux1_helper = aux1_helper->next;
    }
}

/*Functie ce schimba numele unui fisier/director in anumite conditii*/
/*Elementul afectat este pus la sfarsitul listei din care face parte*/
void
mv (Dir * parent, char *oldname, char *newname)
{
  /*Variabile ce vor parcurge listele */
  Dir *search;
  File *find;
  /*Variabile ajutatoare ce vor retine adresele elementelor care au numele oldname */
  Dir *helper_dir;
  File *helper_file;
  /*Se retine adresa primului element din lista de directoare */
  search = parent->head_children_dirs;
  /*Se retine adresa primului element din lista de fisiere */
  find = parent->head_children_files;
  /*Conditiile impuse de cerinta (1-> conditie indeplinita) */
  int condition_file_oldname = 0;
  int condition_directory_oldname = 0;
  int condition_file_newname = 0;
  int condition_directory_newname = 0;
  int helper_int = 0;
  /*Se verifica existenta unui director cu numele oldname */
  if (search != NULL)
    {
      while (search->next)
	{
	  /*Daca exista */
	  if (strcmp (search->name, oldname) == 0)
	    {
	      /*Se retine adresa.Aceasta adresa va fi folosita mai tarziu */
	      helper_dir = search;
	      /*Conditie indeplinita */
	      condition_directory_oldname = 1;
	      break;
	    }
	  /*Parcurgere */
	  search = search->next;
	}
      /*Cazul in care elementul cu numele oldname este ultimul */
      if (search->next == NULL && strcmp (search->name, oldname) == 0)
	{
	  /*Se retine adresa */
	  helper_dir = search;
	  /*Conditie indeplinita */
	  condition_directory_oldname = 1;
	}
      /*Se verifica existenta elementului al carui nume este newname */
      search = parent->head_children_dirs;
      while (search->next)
	{
	  if (strcmp (search->name, newname) == 0)
	    {
	      /*Conditie indeplinita */
	      condition_directory_newname = 1;
	      break;
	    }
	  /*Parcurgere */
	  search = search->next;
	}
      /*Cazul in care elementul cu numele newname este ultimul */
      if (search->next == NULL && strcmp (search->name, newname) == 0)
	/*Conditie indeplinita */
	condition_directory_newname = 1;
    }
  /*Se verifica existenta unui fisier cu numele oldname */
  if (find != NULL)
    {
      while (find->next)
	{
	  /*Daca exista */
	  if (strcmp (find->name, oldname) == 0)
	    {
	      /*Se retine adresa.Aceasta adresa va fi folosita mai tarziu */
	      helper_file = find;
	      /*Conditie indeplinita */
	      condition_file_oldname = 1;
	      break;
	    }
	  /*Parcurgere */
	  find = find->next;
	}
      /*Cazul in care elementul cu numele oldname este ultimul */
      if (find->next == NULL && strcmp (find->name, oldname) == 0)
	{
	  /*Se retine adresa */
	  helper_file = find;
	  /*Conditie indeplinita */
	  condition_file_oldname = 1;
	}
      /*Se verifica existenta elementului al carui nume este newname */
      find = parent->head_children_files;
      while (find->next)
	{
	  if (strcmp (find->name, newname) == 0)
	    {
	      /*Conditie indeplinita */
	      condition_file_newname = 1;
	      break;
	    }
	  /*Parcurgere */
	  find = find->next;
	}
      /*Cazul in care elementul cu numele newname este ultimul */
      if (find->next == NULL && strcmp (find->name, newname) == 0)
	/*Conditie indeplinita */
	condition_file_newname = 1;
    }
  /*Daca elementul al carui nume este oldname exista */
  if (condition_file_oldname == 1 || condition_directory_oldname == 1)
    {
      /*Daca acest element este un director */
      if (condition_directory_oldname == 1)
	{
	  /*Daca nu exista un director cu numele newname */
	  if (condition_file_newname == 0 && condition_directory_newname == 0)
	    {
	      /*Se retine adresa primului element din lista */
	      search = parent->head_children_dirs;
	      /*Daca elementul cautat este primul */
	      if (strcmp (search->name, oldname) == 0)
		{
		  /*Daca este singurul */
		  if (search->next == NULL)
		    /*Se schimba numele */
		    strcpy (search->name, newname);
		  else
		    {		/*Daca nu e singurul */
		      /*Primul element este mutat la sfarsitul listei */
		      parent->head_children_dirs = search->next;
		      while (search->next)
			/*Parcurgere */
			search = search->next;
		      search->next = helper_dir;
		      /*Se foloseste acea adresa de pe linia 578 */
		      helper_dir->next = NULL;
		      strcpy (helper_dir->name, newname);
		    }
		}
	      /*Daca elementul cautat nu este primul */
	      else if (helper_dir->next != NULL)
		{
		  /*Parcurgere */
		  while (search->next)
		    {
		      /*Daca elementul cautat este intalnit */
		      if (search->next == helper_dir)
			{
			  search->next = search->next->next;
			  /*Se memoreaza rezultatul acestei actiuni */
			  helper_int = 1;
			}
		      /*Parcurgere */
		      search = search->next;
		    }
		  /*1 inseamna ca elementul cautat se afla undeva in lista (nu la sfarsit/inceput) */
		  if (helper_int == 1)
		    {
		      /*Se muta la sfarsitul listei */
		      search->next = helper_dir;
		      helper_dir->next = NULL;
		      /*Se modifica numele */
		      strcpy (helper_dir->name, newname);
		    }
		  else
		    /*Elementul cautat este la sfarsitul listei */
		    /*Se modifica numele */
		    strcpy (helper_dir->name, newname);
		}
	    }
	  /*Mesaj de eroare */
	  else
	    printf ("File/Director already exists\n");
	}
      /*Daca acest element este un fisier */
      else if (condition_file_oldname == 1)
	{
	  /*Daca nu exista un fisier cu numele newname */
	  if (condition_file_newname == 0 && condition_directory_newname == 0)
	    {
	      /*Se retine adresa primului element din lista */
	      find = parent->head_children_files;
	      /*Daca elementul cautat este primul */
	      if (strcmp (find->name, oldname) == 0)
		{
		  /*Daca este singurul */
		  if (find->next == NULL)
		    /*Se schimba numele */
		    strcpy (find->name, newname);
		  else
		    {
		      /*Daca nu e singurul */
		      /*Primul element este mutat la sfarsitul listei */
		      parent->head_children_files = find->next;
		      while (find->next)
			/*Parcurgere */
			find = find->next;
		      find->next = helper_file;
		      /*Se foloseste acea adresa de pe linia 621 */
		      helper_file->next = NULL;
		      strcpy (helper_file->name, newname);
		    }
		}
	      /*Daca elementul cautat nu este primul */
	      else if (helper_file->next != NULL)
		{
		  /*Parcurgere */
		  while (find->next)
		    {
		      /*Daca elementul cautat este intalnit */
		      if (find->next == helper_file)
			{
			  find->next = find->next->next;
			  /*Se memoreaza rezultatul acestei actiuni */
			  helper_int = 1;
			}
		      /*Parcurgere */
		      find = find->next;
		    }
		  /*1 inseamna ca elementul cautat se afla undeva in lista (nu la sfarsit/inceput) */
		  if (helper_int == 1)
		    {
		      /*Se muta la sfarsitul listei */
		      find->next = helper_file;
		      helper_file->next = NULL;
		      /*Se modifica numele */
		      strcpy (helper_file->name, newname);
		    }
		  else
		    /*Elementul cautat este la sfarsitul listei */
		    /*Se modifica numele */
		    strcpy (helper_file->name, newname);
		}
	    }
	  else
	    /*Mesaj de eroare */
	    printf ("File/Director already exists\n");
	}
    }
  else
    /*Mesaj de eroare */
    printf ("File/Director not found\n");
}

/*Functie ce prelucreaza comenzile primite de la utilizator*/
int
main ()
{
  /*Radacina */
  Dir *home_dir;
  /*Directorul curent */
  Dir *curr_dir;
  /*O initializare */
  init_dir (&home_dir);
  curr_dir = home_dir;
  /*Adresa ce se va ocupa de sirurile primite de la tastatura */
  char *input_command;
  /*Se aloca memorie */
  input_command =
    (char *) malloc (MAX_INPUT_LINE_SIZE * sizeof (*input_command));
  fgets (input_command, MAX_INPUT_LINE_SIZE, stdin);
  /*O adresa care va primi argumentul comenzii */
  char *split_command;
  /*O adresa care va primi cel de-al doilea argument al comenzii mv */
  char *split_command_mv;
  /*O adresa ce primeste rezultatul functiei pwd */
  char *result_pwd = NULL;
  split_command = strtok (input_command, " ");
  /*Procesul de prelucrare */
  while (1)
    {
      /*Daca se introduce comanda touch */
      if (strcmp (split_command, "touch") == 0)
	{
	  /*Se obtine argumentul */
	  split_command = strtok (NULL, " ");
	  split_command[strcspn (split_command, "\n")] = 0;
	  /*Se apeleaza functia */
	  touch (curr_dir, split_command);
	}
      /*Daca se introduce comanda mkdir */
      if (strcmp (split_command, "mkdir") == 0)
	{
	  /*Se obtine argumentul */
	  split_command = strtok (NULL, " ");
	  split_command[strcspn (split_command, "\n")] = 0;
	  /*Se apeleaza functia */
	  mkdir (curr_dir, split_command);
	}
      /*Daca se introduce comanda ls */
      if (strcmp (input_command, "ls\n") == 0)
	{
	  /*Se apeleaza functia */
	  ls (curr_dir);
	}
      /*Daca se introduce comanda stop */
      if (strcmp (input_command, "stop\n") == 0)
	{
	  /*Se apeleaza functia */
	  stop (home_dir);
	  /*Se termina programul */
	  break;
	}
      /*Daca se introduce comanda rm */
      if (strcmp (split_command, "rm") == 0)
	{
	  /*Se obtine argumentul */
	  split_command = strtok (NULL, " ");
	  split_command[strcspn (split_command, "\n")] = 0;
	  /*Se apeleaza functia */
	  rm (curr_dir, split_command);
	}
      /*Daca se introduce comanda rmdir */
      if (strcmp (split_command, "rmdir") == 0)
	{
	  /*Se obtine argumentul */
	  split_command = strtok (NULL, " ");
	  split_command[strcspn (split_command, "\n")] = 0;
	  /*Se apeleaza functia */
	  rmdir (curr_dir, split_command);
	}
      /*Daca se introduce comanda cd */
      if (strcmp (split_command, "cd") == 0)
	{
	  /*Se obtine argumentul */
	  split_command = strtok (NULL, " ");
	  split_command[strcspn (split_command, "\n")] = 0;
	  /*Se apeleaza functia */
	  cd (&curr_dir, split_command);
	}
      /*Daca se introduce comanda tree */
      if (strcmp (input_command, "tree\n") == 0)
	{
	  /*Se apeleaza functia */
	  tree (curr_dir, 0);
	}
      /*Daca se introduce comanda pwd */
      if (strcmp (input_command, "pwd\n") == 0)
	{
	  /*Se apeleaza functia */
	  result_pwd = pwd (curr_dir);
	  printf ("%s\n", result_pwd);
	  /*Se elibereaza memoria alocata sirului intors de pwd */
	  free (result_pwd);
	}
      /*Daca se introduce comanda mv */
      if (strcmp (split_command, "mv") == 0)
	{
	  /*Se obtine argumentul 1 */
	  split_command = strtok (NULL, " ");
	  /*Se obtine argumentul 2 */
	  split_command_mv = strtok (NULL, " ");
	  split_command_mv[strcspn (split_command_mv, "\n")] = 0;
	  /*Se apeleaza functia */
	  mv (curr_dir, split_command, split_command_mv);
	}
      /*Se citeste comanda primita de la tastatura */
      fgets (input_command, MAX_INPUT_LINE_SIZE, stdin);
      /*Se obtine argumentul */
      split_command = strtok (input_command, " ");
    }
  /*Se elibereaza memoria */
  free (input_command);
  return 0;
}