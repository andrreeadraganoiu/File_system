#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir
{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File 
{
	char *name;
	struct Dir* parent;
	struct File* next;
} File;


/* Intoarce ultimul fisier din 
   lista de fisiere */
File* get_last_file (File* file_list)
{
	File* head = file_list;
	while(head->next != NULL)
	{
		head = head->next;
	}
	return head;
}

/* Initializeaza campurile unui nou fisier */
File* create_file (Dir* parent, char* name)
{
	File* file = malloc(sizeof(File));
	if(file == NULL)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	file->name = strdup(name);
	if(file->name == NULL)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	file->parent = NULL;
	file->next = NULL;
	return file;
}

/* Adauga noul fisier la sfarsitul listei de fisiere
   a directorului parinte "parent" */
File* add_next_file (Dir* parent, char* name)
{
	File* file_node = create_file(parent, name);
	file_node->parent = parent;
	if(parent->head_children_files == NULL)  
		return file_node;
	File* last = get_last_file(parent->head_children_files);  
	last->next = file_node;
	return parent->head_children_files;
}

/* Verifica existenta unui fisier in directorul
   parinte "parent" */
int is_file_in_list (Dir* parent, char* name)
{
	File* head = parent->head_children_files;
	while(head != NULL)
	{
		if(!strcmp(head->name, name))
			return 1;
		head = head->next;
	}
	return 0;
}

/* Creeaza fisier pe care il adauga la finalul
   listei de fisiere a directorului parinte*/  
void touch (Dir* parent, char* name) 
{
	if(is_file_in_list(parent, name))
		printf("File already exists\n");
	else
		parent->head_children_files = add_next_file(parent, name);

}

/* Initializeaza campurile unui nou director */
Dir* create_director (Dir* parent, char *name)
{
	Dir* director = malloc(sizeof(Dir));
	if(director == NULL)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	director->name = strdup(name);
	if(director->name == NULL)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	director->parent = NULL;
	director->head_children_files = NULL;
	director->head_children_dirs = NULL;
	director->next = NULL;
	return director;
}

/* Intoarce ultimul director din 
   lista de directoare */
Dir* get_last_dir (Dir* dir_list)
{
	Dir* head = dir_list;
	while(head->next != NULL)
	{
		head = head->next;
	}
	return head;
}

/* Adauga noul director la sfarsitul listei de directoare
   a directorului parinte "parent" */
Dir* add_next_dir (Dir* parent, char* name)
{
	Dir* dir_node = create_director(parent, name);
	dir_node->parent = parent;
	if(parent->head_children_dirs == NULL) 
	{
		return dir_node;
	}
	Dir* last = get_last_dir(parent->head_children_dirs);
	last->next = dir_node;
	return parent->head_children_dirs;
}

/* Verifica existenta unui director in directorul
   parinte "parent" */
int is_dir_in_list (Dir* parent, char* name)
{
	Dir* head = parent->head_children_dirs;
	while(head != NULL)
	{
		if(!strcmp(head->name, name))
			return 1;
		head = head->next;
	}
	return 0;
}

/* Creeaza director pe care il adauga la finalul
   listei de directoare a directorului parinte*/  
void mkdir (Dir* parent, char* name)
{
	if(is_dir_in_list(parent, name))
		printf("Directory already exists\n");
	else
		parent->head_children_dirs = add_next_dir(parent, name);
}

/* Afiseaza fisierele din
   directorul curent */
void print_files (Dir* parent)
{
	File* head = parent->head_children_files;
	while(head != NULL)
	{
		printf("%s\n", head->name);
		head = head->next;
	}
}

/* Afiseaza directoarele din
   directorul curent */
void print_dirs (Dir* parent)
{
	Dir* head = parent->head_children_dirs;
	while(head != NULL)
	{
		printf("%s\n", head->name);
		head = head->next;
	}
}

/* Afiseaza directoarele si fisierele
   din directorul parinte */
void ls (Dir* parent) 
{
	print_dirs(parent);
	print_files(parent);
}

/* Returneaza fisierul dinaintea unui
   fisier dat din lista */
File* get_before_file (File* file_list, File* file_position)
{
	File* head = file_list;
	while(head->next != file_position)
	{
		head = head->next;
	}
	return head;
}

/* Returneaza lista de fisiere a directorului
   parinte dupa stergerea fisierului "name" */
File* list_after_file_rm (Dir* parent, char* name)
{
	File* head = parent->head_children_files;
	while(head != NULL)
	{
		if(!strcmp(head->name, name))
		{
			if(head == parent->head_children_files) /*daca este primul fisier din lista*/
			{
				File* aux = head;
				head = head->next;
				free(aux->name);
				free(aux);
				return head; 
			}
			else   /*ultimul sau fisier din mijlocul listei*/
			{
				File* before_file = get_before_file(parent->head_children_files, head);
				before_file->next = head->next;
				free(head->name);
				free(head);
				return parent->head_children_files;
			}
		}
		head = head->next;
	}
}

/* Sterge fisierul cu numele "name"
   din directorul curent */
void rm (Dir* parent, char* name) 
{
	if(!is_file_in_list(parent, name))
		printf("Could not find the file\n");
	else
		parent->head_children_files = list_after_file_rm(parent, name);

}

/* Returneaza pointerul catre directorul cu numele
   "name" din directorul parinte "parent" */
Dir* get_dir_position (Dir* parent, char* name)
{
	Dir* head = parent->head_children_dirs;
	while(head != NULL)
	{
		if(!strcmp(head->name, name))
			return head;
		head = head->next;
	}
	return NULL;
}

/* Sterge toate fisierele unui director parinte */
void free_files (Dir* p)
{
	File* head = p->head_children_files;
	File* aux = head;
	while (head != NULL)
    {
        aux = head;
        head = head->next;
        free(aux->name);
		free(aux);
    }
}

/* Reface legaturile dupa ce directorul
   x a fost sters din lista */
Dir* reconnect (Dir* x) 
{
	Dir* next = x->next;
	Dir* parent = x->parent; 
	
	if(parent->head_children_dirs == x) /*primul din lista*/
	{
		parent->head_children_dirs = parent->head_children_dirs->next;
	} 
	else 
	{
		Dir* iter =parent->head_children_dirs->next;
		Dir* prev = parent->head_children_dirs;
		while(iter != NULL) 
		{
			if(iter == x) 
			{
				prev->next = iter->next;
			}
			iter = iter->next;
			prev = prev->next;
		}
	}
	return parent->head_children_dirs;
}

/* Elimina un director din
   directorul parinte*/
void rmdir (Dir* parent, char* name) 
{
	Dir* dir_for_delete = get_dir_position(parent, name);
	if(dir_for_delete == NULL)
		printf("Could not find the dir\n");
	else
	{
		while(dir_for_delete != NULL) 
		{
			Dir* iter = dir_for_delete->head_children_dirs;
			if(iter != NULL) 
			{
				while(iter != NULL) 
				{
					if(iter->head_children_dirs == NULL) 
					{
						if(iter->head_children_files == NULL)   /*directorul de sters nu are
														nici fisiere nici alte directoare*/
						{
							iter->parent->head_children_dirs = reconnect(iter);
							free(iter->name);
							free(iter);
							break;
						} 
						else       /*directorul are fisiere*/ 
						{
							free_files(iter);
							iter->head_children_files = NULL;
							break;
						}
					} 
					else 
					{
						iter = iter->head_children_dirs;
					}
				}
			} 
			else /*directorul dat ca parametru ramane fara alte directoare*/
			{
				if(dir_for_delete->head_children_files != NULL) /*dar are fisiere*/
				{
					free_files(dir_for_delete);
					dir_for_delete->head_children_files = NULL;
				}
				/*acum pot sterge directorul dat ca parametru dar dupa ce refac
				 legaturile din lista in care se afla*/
				dir_for_delete->parent->head_children_dirs = reconnect(dir_for_delete);
				free(dir_for_delete->name);
				free(dir_for_delete);
				return;
			}
		}
	}
}

/* Schimba directorul curent */
void cd (Dir** target, char *name) 
{
	if(!strcmp(name, ".."))
	{
		if(strcmp((*target)->name, "home")) /* daca directorul este home nu se face nicio modificare */
			*target = (*target)->parent;
	}
	else
		if(!is_dir_in_list(*target, name))
			printf("No directories found!\n");
		else
		{
			Dir* dir_node = get_dir_position(*target, name);
			*target = dir_node;
		}
}

void put_space (int level) 
{
    for(int i = 0; i < level; i++)
		printf("    ");
}

/* Afiseaza ierarhia de directoare si fisiere
   din calea curenta*/
void tree (Dir* target, int level) 
{
    if(target == NULL)
        return;
	
	Dir* i1 = target->head_children_dirs;
    while(i1 != NULL)
	{
        put_space(level);
        printf("%s", i1->name);
    	printf("\n");
        tree(i1, level + 1);
		i1 = i1->next;
    }

	File* i2 = target->head_children_files;
    while(i2 != NULL) 
	{
        put_space(level);
        printf("%s", i2->name);
		printf("\n");
		i2 = i2->next;
    }		
}

/* Returneaza directorul dinaintea unui
   director dat din lista */
Dir* get_before_dir (Dir* dir_list, Dir* dir_position)
{
	Dir* head = dir_list;
	while(head->next != dir_position)
	{
		head = head->next;
	}
	return head;
}

/* Sterge lista de directoare dupa ce sirul
   de caractere a fost creat */
void delete_path_list (Dir* path_list)
{
	Dir* head = path_list;
	Dir* aux;
	while (head != NULL)
    {
        aux = head;
        head = head->next;
        free(aux->name);
	    free(aux);
    }
}
/* Construieste sirul de caractere al caii */
char *build_path (Dir* path_list)
{
	char *path = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	if(path == NULL)
	{
		printf("Error. Allocation was unsuccessful. \n");
		exit(1);
	}
	while(path_list != NULL)
	{
		strcat(path, "/");
		strcat(path, path_list->name);
		path_list = path_list->next;
	}
	return path;
}

/* Adauga un nou director-cale in lista de directoare */
Dir* add_first_in_path (Dir* target, Dir* path_list)
{
	Dir* dir_node = create_director(target, target->name);
	if(path_list == NULL)
	{
		return dir_node;
	}
	dir_node->next = path_list;
	path_list = dir_node;
	return path_list;
}

/* Afiseaza calea curenta */
char *pwd (Dir* target) 
{
	Dir* path_list = NULL;
	while(target != NULL)
	{
		path_list = add_first_in_path(target, path_list);
		target = target->parent;
	}
	char *path = build_path(path_list);
	delete_path_list(path_list);
	return path;
}

void stop (Dir* target) 
{
	return;
}

/* Muta directorul la finalul listei iar apoi il redenumeste */
Dir* list_after_dir_mv (Dir* parent, char* oldname, char* newname)
{
	
	Dir* head = parent->head_children_dirs;
	while(head != NULL)
	{
		if(!strcmp(head->name, oldname))
		{
			if(head == parent->head_children_dirs)
			{
				/*daca este un singur director in lista doar il redenumeste*/
				if(head->next == NULL)  
				{
					strcpy(head->name, newname);
					return head;
				}
				else
				{   /*daca este primul director din lista dar nu este insa singurul */
					Dir* aux = head;
					head = head->next;
					Dir* last_dir = get_last_dir(parent->head_children_dirs);
					last_dir->next = aux;
					aux->next = NULL;
					strcpy(aux->name, newname);
					return head;
				} 
			}
			else
			{
				Dir* before_dir = get_before_dir(parent->head_children_dirs, head);
				before_dir->next = head->next;
				Dir* last_dir = get_last_dir(parent->head_children_dirs);
				last_dir->next = head;
				head->next = NULL;
				strcpy(head->name, newname);
				return parent->head_children_dirs;
			}
		}
		head = head->next;
	}
}

/* Redenumeste un director/fisier din 
   directorul parinte "parent" */
void mv (Dir* parent, char *oldname, char *newname) 
{
	if(is_file_in_list(parent, oldname))
	{
		if(!is_file_in_list(parent, newname) && !is_dir_in_list(parent, newname))
		{
			/* Returneaza lista de fisiere a directorului
   			   parinte dupa stergerea fisierului "name" */
			parent->head_children_files = list_after_file_rm(parent, oldname);
			touch(parent, newname); /* adauga un nou fisier cu numele cerut */
			return;
		}
		else
		{
			printf("File/Director already exists\n");
			return;
		}
	}
	if(is_dir_in_list(parent, oldname))
	{
		if(!is_file_in_list(parent, newname) && !is_dir_in_list(parent, newname))
		{
			parent->head_children_dirs = list_after_dir_mv(parent, oldname, newname);
			return;
		}
		else
		{
			printf("File/Director already exists\n");
			return;
		}

	}
	printf("File/Director not found\n");
}

/* Elibereaza memoria ramasa din program */
void FREE_RESOURCES (char *input, Dir* home)
{
	free(input);

	Dir* i = home->head_children_dirs;
	Dir* aux;
	while(i != NULL) 
	{
		aux = i;
		i = i->next;
		rmdir(home, aux->name);
	}
	if(home->head_children_files != NULL) 
	{
		free_files(home);
	}
	free(home->name);
	free(home);
}

int main () 
{
	char *input = malloc(MAX_INPUT_LINE_SIZE);
	Dir* home = create_director(NULL, "home");
	Dir* parent_dir = home;
	do
	{
		fscanf(stdin, "%s", input);

		if(!strcmp(input, "touch"))
		{
			fscanf(stdin, "%s", input);
			touch(parent_dir, input);
		}
		else
		if(!strcmp(input, "mkdir"))
		{
			fscanf(stdin, "%s", input);
			mkdir(parent_dir, input);
		}
		else
		if(!strcmp(input, "ls"))
		{
			ls(parent_dir);
		}
		else
		if(!strcmp(input, "rm"))
		{
			fscanf(stdin, "%s", input);
			rm(parent_dir, input);
		}
		else
		if(!strcmp(input, "rmdir"))
		{
			fscanf(stdin, "%s", input);
			rmdir(parent_dir, input);
		}
		else
		if(!strcmp(input, "cd"))
		{
			fscanf(stdin, "%s", input);
			cd(&parent_dir, input);
		}
		else
		if(!strcmp(input, "tree"))
		{
			tree(parent_dir, 0);
		}
		else
		if(!strcmp(input, "pwd"))
		{
		 	char *path = pwd(parent_dir);
			printf("%s", path);
			free(path);
		}
		else
		if(!strcmp(input, "mv"))
		{
			char *oldname = malloc(MAX_INPUT_LINE_SIZE);
			char *newname = malloc(MAX_INPUT_LINE_SIZE);
			fscanf(stdin, "%s %s", oldname, newname);
			mv(parent_dir, oldname, newname);
			free(oldname);
			free(newname);
		}
	
	} while (strcmp(input, "stop"));

	FREE_RESOURCES(input, home);
	stop(parent_dir);
	
	return 0;
}
			

