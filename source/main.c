#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define	DELIMITER " \n\t[]"
#define BUFFER_SIZE 1024

/*	Field FIELD
 *
 *	Estrutura que guarda o nome, tamanho e tipo de um campo.
 *	- Membros:
 *		char *: nome do campo;
 *		char *: tipo do campo;
 *		int: tamanho do campo;
 *		char: booleano que indica se o campo é ou não de índice.
 */
typedef struct Field FIELD;

/*	Schema SCHEMA
 *
 *	Estrutura que descreve como é cada dado.
 *	- Membros:
 *		int: tamanho de cada dado;
 *		int: número de campos que cada dado;
 *		char *: nome do registro de cada dado;
 *		FIELD *: vetor com os campos do registro.
 */
typedef struct Schema SCHEMA;

/*	readUntilChar ()
 *
 *	Função para ler um string até determinado caractere.
 *	- Parâmetros:
 *		FILE *: arquivo do qual a string deve ser lida;
 *		const char: caractere terminador.
 *	- Retorno:
 *		char *: cadeia de caracteres lida.
 */
char * readUntilChar (FILE *, const char);

/*	getSchema ()
 *
 *	Função para ler e armazenar o conteúdo do .schema.
 *	- Parâmetros:
 *		const char *: nome do arquivo .schema.
 *	- Retorno:
 *		SCHEMA *: endereço da estrutura com as informações sobre o registro descrito no .schema.
 */
SCHEMA * getSchema (const char *);

/*	dumpSchema ()
 *
 *	Função para imprimir a descrição do arquivo de dados baseado no schema fornecido.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que contém as informações retiradas do .schema.
 *	- Retorno:
 *		não há.
 */
void dumpSchema (SCHEMA *);

/*	freeSchema ()
 *
 *	Função para liberar a estrutura com a descrição dos registros completamente.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que será liberada.
 *	- Retorno:
 *		não há.
 */
void freeSchema (SCHEMA *);

/*	openDataFile ()
 *
 *	Função para abrir um arquivo de dados utilizando um nome interpretado do .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que contém as informações retiradas do .schema;
 *		const char *: modo em que o arquivo deve ser aberto (igual aos de fopen).
 *	- Retorno:
 *		FILE *: ponteiro para o arquivo que foi aberto.
 */
FILE * openDataFile (SCHEMA *, const char *);

/*	readData ()
 *
 *	Função para ler o conteúdo de um arquivo .data baseado na descrição de um .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que contém as informações retiradas do .schema;
 *		int *: endereço da variável que guardará a quantidade de registros do .data.
 *	- Retorno:
 *		void *: ponteiro para o conteúdo lido.
 */
void * readData (SCHEMA *, int *);

/*	fprintInt ()
 *
 *	Função para imprimir um inteiro apontado originalmente por um void * em um arquivo desejado.
 *	- Parâmetros:
 *		void *: endereço do bloco de memória que contém o inteiro a ser impresso;
 *		int: deslocamento até a posição exata do inteiro no bloco;
 *		FILE *: ponteiro para o arquivo em que o inteiro deve ser impresso.
 *	- Retorno:
 *		não há.
 */
void fprintInt (void *, int, FILE *);

/*	fprintDouble ()
 *
 *	Função para imprimir um double apontado originalmente por um void * em um arquivo desejado.
 *	- Parâmetros:
 *		void *: endereço do bloco de memória que contém o double a ser impresso;
 *		int: deslocamento até a posição exata do double no bloco;
 *		FILE *: ponteiro para o arquivo em que o double deve ser impresso.
 *	- Retorno:
 *		não há.
 */
void fprintDouble (void *, int, FILE *);

/*	fprintString ()
 *
 *	Função para imprimir uma cadeia de caracteres apontada por um void * em um arquivo.
 *	- Parâmetros:
 *		void *: endereço do bloco de memória que contém a string a ser impresso;
 *		int: deslocamento até a posição exata da string no bloco;
 *		FILE *: ponteiro para o arquivo em que a string deve ser impresso.
 *	- Retorno:
 *		não há.
 */
void fprintString (void *, int, FILE *);

/*	fwriteInt ()
 *
 *	Função para copiar um inteiro da memória para um arquivo.
 *	- Parâmetros:
 *		void *: endereço do bloco de memória que contém o inteiro a ser copiado;
 *		int: deslocamento até a posição exata do inteiro no bloco;
 *		FILE *: arquivo de dados em que o inteiro deve ser gravado.
 *	- Retorno:
 *		não há.
 */
void fwriteInt (void *, int, FILE *);

/*	fwriteDouble ()
 *
 *	Função para copiar um double da memória para um arquivo.
 *	- Parâmetros:
 *		void *: endereço do bloco de memória que contém o double a ser copiado;
 *		int: deslocamento até a posição exata do double no bloco;
 *		FILE *: arquivo de dados em que o double deve ser gravado.
 *	- Retorno:
 *		não há.
 */
void fwriteDouble (void *, int, FILE *);

/*	fwriteString ()
 *
 *	Função para copiar uma string da memória para um arquivo.
 *	- Parâmetros:
 *		void *: endereço do bloco de memória que contém a string a ser copiada;
 *		int: deslocamento até a posição exata da string no bloco;
 *		FILE *: arquivo de dados em que a string deve ser gravada.
 *	- Retorno:
 *		não há.
 */
void fwriteString (void *, int, FILE *, int);

/*	getInternOffset ()
 *
 *	Função para quantificar o deslocamento até um membro do registro descrito pelo .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que descreve o registro utilizado na base de dados;
 *		int: posição do campo na estrutura (qual seu número no vetor).
 *	- Retorno:
 *		int: deslocamento no arquivo de dados do início de um registro ao campo desejado.
 */
int getInternOffset (SCHEMA *, int);

/*	openIndexFile ()
 *
 *	Função para abrir um arquivo de índice utilizando um nome interpretado do .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que contém as informações retiradas do .schema;
 *		const char *: modo em que o arquivo deve ser aberto (igual aos de fopen);
 *		int: posição do campo no vetor de campo da estrutura descrita pelo .schema.
 *	- Retorno:
 *		FILE *: ponteiro para o arquivo recém-aberto.
 */
FILE * openIndexFile (SCHEMA *, const char *, int);

/*	createIndex ()
 *
 *	Função que usa a descrição de um .schema e um arquivo de dados .data para criar um índice .idx.
 *	- Parâmetros:
 *		void *: ponteiro para o conteúdo de um arquivo de dados;
 *		SCHEMA *: endereço da estrutura que descreve os registros do .data;
 *		int: quantidade de registros que o .data possui.
 *	- Retorno:
 *		não há.
 */
void createIndex (void *, SCHEMA *, int);

/*	fprint ()
 *
 *	Função criada para generalizar o uso das funções fprintDouble (), fprintInt () e fprintString ().
 *	- Parâmetros:
 *		const char *: string que contém o tipo do conteúdo a ser impresso;
 *		void *: bloco que contám o que deve ser impresso;
 *		int: deslocamento do início do bloco ao que deve ser impresso;
 *		FILE *: arquivo para o qual o conteúdo desejado deve ser impresso.
 *	- Retorno:
 *		não há.
 */
void fprint (const char *, void *, int, FILE *); 

/*	dumpData ()
 *
 *	Função para imprimir o conteúdo do arquivo .data com as etiquetas do arquivo .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as cacracterísticas descritas pelo .schema;
 *		int: quantidade de registros contida no .data.
 *	- Retorno:
 *		não há.
 */
void dumpData (SCHEMA *, int);

/*	dumpIndex ()
 *
 *	Função para imprimir o conteúdo do arquivo .idx com as etiquetas do arquivo .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as cacracterísticas descritas pelo .schema;
 *		int: quantidade de registros contida no .data.
 *	- Retorno:
 *		não há.
 */
void dumpIndex (SCHEMA *, int);

int main (int argc, char * argv[]) {
	char * schemaName;
	char * instruction;
	SCHEMA * schema;
	void * data;
	int recordsNumber;

	schemaName = readUntilChar (stdin, '\n');
	schema = getSchema (schemaName);

	data = readData (schema, &recordsNumber);
	createIndex (data, schema, recordsNumber);
	free (data);

	while (strcmp (instruction = readUntilChar (stdin, '\n'), "exit") != 0) {
		if (!strcmp (instruction, "dump_schema")) dumpSchema (schema);
		else if (!strcmp (instruction, "dump_data")) dumpData (schema, recordsNumber);
		else if (!strcmp (instruction, "dump_index")) dumpIndex (schema, recordsNumber);
		free (instruction);
	}
	free (instruction);

	freeSchema (schema);
	free (schemaName);

	return 0;
}

typedef struct Field {
	char * name;
	char * type;
	int size;
	char order;
} FIELD;


typedef struct Schema {
	int size;
	int fieldsNumber;
	char * table;
	FIELD * fields;
} SCHEMA;

char * readUntilChar (FILE * p_file, const char delimiter) {
	char read;
	char * string = NULL;
	int i;

	for (i = 0; !feof (p_file) && (read = fgetc (p_file)) != delimiter; i++) {
		if (i % BUFFER_SIZE == 0)
			string = (char *) realloc (string, (i + BUFFER_SIZE) * sizeof (char));

		string[i] = read;
	}

	string = (char *) realloc (string, (i + 1) * sizeof (char));
	string[i] = '\0';

	return string;
}

SCHEMA * getSchema (const char * schemaName) {
	FILE * p_schema = NULL;
	SCHEMA * schema = NULL;
	int schemaSize, i;
	char * schemaContent;
	char * token = NULL;

	p_schema = fopen (schemaName, "r");

	fseek (p_schema, 0, SEEK_END);
	schemaSize = ftell (p_schema);

	schemaContent = (char *) malloc ((schemaSize + 1) * sizeof (char));

	fseek (p_schema, 0, SEEK_SET);
	for (i = 0; i < schemaSize; i++) schemaContent[i] = fgetc (p_schema);
	schemaContent[i] = '\0';

	schema = (SCHEMA *) malloc (sizeof (SCHEMA));
	schema->fields = NULL;
	schema->table =  NULL;
	schema->size = 0;

	token = strtok (schemaContent, DELIMITER);
	token = strtok (NULL, DELIMITER);

	schema->table = (char *) malloc ((strlen (token) + 1) * sizeof (char));
	strcpy (schema->table, token);

	for (i = 0, token = strtok (NULL, DELIMITER); token != NULL; i++) {
		schema->fields = (FIELD *) realloc (schema->fields, (i + 1) * sizeof (FIELD));

		schema->fields[i].name = (char *) malloc ((strlen (token) + 1) * sizeof (char));
		strcpy (schema->fields[i].name, token);

		token = strtok (NULL, DELIMITER);
		schema->fields[i].type = (char *) malloc ((strlen (token) + 1) * sizeof (char));
		strcpy (schema->fields[i].type, token);

		if (strcmp (token, "char") == 0) {
			token = strtok (NULL, DELIMITER);
			schema->fields[i].size = (int) strtol (token, NULL, 10);
		}
		else {
			if (strcmp (schema->fields[i].type, "int") == 0) schema->fields[i].size = sizeof (int);
			else schema->fields[i].size = sizeof (double);
		}
		schema->size += schema->fields[i].size;

		token = strtok (NULL, DELIMITER);
		if (token != NULL && strcmp (token, "order") == 0) {
			schema->fields[i].order = 1;
			token = strtok (NULL, DELIMITER);
		}
		else {
			schema->fields[i].order = 0;
		}
	}

	schema->fieldsNumber = i;

	free (token);
	fclose (p_schema);
	free (schemaContent);

	return schema;
}

void dumpSchema (SCHEMA * schema) {
	int i;

	printf ("table %s(%d bytes)\n", schema->table, schema->size);

	for (i = 0; i < schema->fieldsNumber; i++) {
		printf ("%s %s", schema->fields[i].name, schema->fields[i].type);
		if (!strcmp (schema->fields[i].type, "char")) printf ("[%d]", schema->fields[i].size);
		if (schema->fields[i].order == 1) printf (" order");
		printf ("(%d bytes)\n", schema->fields[i].size);
	}
}

void freeSchema (SCHEMA * schema) {
	int i;

	for (i = 0; i < schema->fieldsNumber; i++) {
		free (schema->fields[i].name);
		free (schema->fields[i].type);
	}
	free (schema->fields);
	free (schema->table);
	free (schema);
}

FILE * openDataFile (SCHEMA * schema, const char * mode) {
	FILE * p_data = NULL;
	char * filename = NULL;

	filename = (char *) malloc ((strlen(schema->table) + strlen(".data") + 1) * sizeof (char));
	strcpy (filename, schema->table);
	strcat (filename, ".data");

	p_data = fopen (filename, mode);

	free (filename);

	return p_data;
}

void * readData (SCHEMA * schema, int * recordsNumber) {
	void * data = NULL;
	FILE * p_data;

	p_data = openDataFile (schema, "r");

	fseek (p_data, 0, SEEK_END);
	*recordsNumber = (ftell (p_data) / schema->size);
	fseek (p_data, 0, SEEK_SET);

	data = malloc (*recordsNumber * schema->size);
	fread (data, schema->size, *recordsNumber, p_data);

	fclose (p_data);

	return data;
}

void fprintInt (void * pointer, int offset, FILE * output) {
	char * transition;
	int * number;

	transition = (char *) pointer;
	transition += offset;

	number = (int *)transition;

	fprintf (output, "%d", *number);
}

void fprintDouble (void * pointer, int offset, FILE * output) {
	char * transition;
	double * number;

	transition = (char *) pointer;
	transition += offset;

	number = (double *) transition;

	fprintf (output, "%.2lf", *number);
}

void fprintString (void * pointer, int offset, FILE * output) {
	char * string;

	string = (char *) pointer;
	string += offset;

	fprintf (output, "%s", string);
}

void fwriteInt (void * data, int offset, FILE * output) {
	char * transition;

	transition = (char *) data;
	transition += offset;

	fwrite ((void *) transition, sizeof(int), 1, output);
}

void fwriteDouble (void * data, int offset, FILE * output) {
	char * transition;

	transition = (char *) data;
	transition += offset;

	fwrite ((void *) transition, sizeof(double), 1, output);
}

void fwriteString (void * data, int offset, FILE * output, int size) {
	char * transition;

	transition = (char *) data;
	transition += offset;

	fwrite ((void *) transition, size, 1, output);
}

int getInternOffset (SCHEMA * schema, int position) {
	int offset, i;

	for (offset = 0, i =0; i < position; i++) {
		offset += schema->fields[i].size;
	}

	return offset;
}

FILE * openIndexFile (SCHEMA * schema, const char * mode, int fieldPosition) {
	FILE * p_index = NULL;
	char * filename = NULL;
	int tableNameSize, fieldNameSize, extensionSize;

	tableNameSize = strlen (schema->table);
	fieldNameSize = strlen (schema->fields[fieldPosition].name);
	extensionSize = strlen (".idx");

	filename = (char *) malloc ((tableNameSize + 1 + fieldNameSize + extensionSize + 1) * sizeof (char));
	strcpy (filename, schema->table);
	strcat (filename, "-");
	strcat (filename, schema->fields[fieldPosition].name);
	strcat (filename, ".idx");

	p_index = fopen (filename, mode); 

	free (filename);

	return p_index;
}

void createIndex (void * data, SCHEMA * schema, int recordsNumber) {
	int i, j, internOffset;
	FILE * idx;

	for (i = 0; i < schema->fieldsNumber; i++) {
		if (schema->fields[i].order) {
			idx = openIndexFile (schema, "w+", i);
			internOffset = getInternOffset (schema, i);

			for (j = 0; j < recordsNumber * schema->size; j += schema->size) {
				if (strcmp (schema->fields[i].type, "int") == 0) 
					fwriteInt (data, j + internOffset, idx);
				else if (strcmp (schema->fields[i].type, "double") == 0)
					fwriteDouble (data, j + internOffset, idx);
				else 
					fwriteString (data, j + internOffset, idx, schema->fields[i].size);
				fwrite ((void *) &j, sizeof (int), 1, idx);
			}

			fclose (idx);
		}
	}
}

void fprint (const char * type, void * pointer, int offset, FILE * output) {
	if (!strcmp (type, "int"))
		fprintInt (pointer, offset, stdout);
	else if (!strcmp (type, "double"))
		fprintDouble (pointer, offset, stdout);
	else
		fprintString (pointer, offset, stdout);

} 

void dumpData (SCHEMA * schema, int recordsNumber) {
	void * data;
	int i, j, offset;
	FILE * p_data;

	p_data = openDataFile (schema, "r");

	data = malloc (schema->size);

	for (i = 0; i < recordsNumber; i++) {
		fread (data, schema->size, 1, p_data);

		for (j = 0; j < schema->fieldsNumber; j++) {
			offset = getInternOffset (schema, j);
			printf ("%s = ", schema->fields[j].name);
			fprint (schema->fields[j].type, data, offset, stdout);
			printf ("\n");
		}
	}

	free (data);
	fclose (p_data);
}

void dumpIndex (SCHEMA * schema, int recordsNumber) {
	int i, j;
	void * index;
	FILE * p_index;

	for (i = 0; i < schema->fieldsNumber; i++) {
		if (schema->fields[i].order) {
			p_index = openIndexFile (schema, "r", i);
			index = malloc (schema->fields[i].size + sizeof (int));

			for (j = 0; j < recordsNumber; j++) {
				fread (index, schema->fields[i].size + sizeof (int), 1, p_index);
				fprint (schema->fields[i].type, index, 0, stdout);

				printf (" = ");

				fprint ("int", index, schema->fields[i].size, stdout);
				printf("\n");
			}

			free (index);
			fclose (p_index);
		}
	}
}
