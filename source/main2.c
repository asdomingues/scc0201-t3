/*	BASE DE DADOS
 *
 *	Programa com serialização e parsing para trabalhar com um banco de dados
 *	operado conforme o modelo descrito no PDF do trabalho.
 *
 *	Allan Silva Domingues | 9293290	| 19/09/15 | ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define	DELIMITER " \n\t[]"
#define BUFFER_SIZE 128

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
 *		const char: caractere terminador;
 *		int: tamanho do buffer a ser utilizado.
 *	- Retorno:
 *		char *: cadeia de caracteres lida.
 */
char * readUntilChar (FILE *, const char, int);

/*	getSchema ()
 *
 *	Função para ler e armazenar o conteúdo do .schema.
 *	- Parâmetros:
 *		const char *: nome do arquivo .schema.
 *	- Retorno:
 *		SCHEMA *: endereço da estrutura com as informações sobre o registro descrito no .schema.
 */
SCHEMA * getSchema (const char *);

/*	getRecordsNumber ()
 *
 *	Função que conta a quantidade de registros no .data baseada em uma descrição de SCHEMA.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as informações sobre o registro descrito no .schema.
 *	- Retorno:
 *		int: quantidade de registros no .data.
 */
int getRecordsNumber (SCHEMA *);

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
 *		SCHEMA *: endereço da estrutura que descreve os registros do .data;
 *		int: quantidade de registros que o .data possui.
 *	- Retorno:
 *		não há.
 */
void createIndex (SCHEMA *, int);

/*	dumpData ()
 *
 *	Função para imprimir o conteúdo do arquivo .data com as etiquetas do arquivo .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as características descritas pelo .schema;
 *		int: quantidade de registros contida no .data.
 *	- Retorno:
 *		não há.
 */
void dumpData (SCHEMA *, int);

/*	dumpIndex ()
 *
 *	Função para imprimir o conteúdo do arquivo .idx com as etiquetas do arquivo .schema.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as características descritas pelo .schema;
 *		int: quantidade de registros contida no .data.
 *	- Retorno:
 *		não há.
 */
void dumpIndex (SCHEMA *, int);

/*	compareInt ()
 *
 *	Função para comparar dois inteiros na memória.
 *	- Parâmetros:
 *		void *: ponteiro para o bloco que contém o primeiro inteiro;
 *		void *: ponteiro para o bloco que contém o segundo inteiro.
 *	- Retorno:
 *		int: diferença entre os inteiros.
 */
int compareInt (void *, void *);

/*	compareDouble ()
 *
 *	Função para comparar dois doubles na memória.
 *	- Parâmetros:
 *		void *: ponteiro para o bloco que contém o primeiro doubles;
 *		void *: ponteiro para o bloco que contém o segundo doubles.
 *	- Retorno:
 *		int: sinal da diferença entre os doubles (-1, 1) ou 0.
 */
int compareDouble (void *, void *);

/*	compareString ()
 *
 *	Função para comparar duas strings na memória.
 *	- Parâmetros:
 *		void *: ponteiro para o bloco que contém a primeira string;
 *		void *: ponteiro para o bloco que contém a segunda string.
 *	- Retorno:
 *		int:	> 0 se a segunda string vier antes da primeira em ordem alfabética;
 *			< 0 se a segunda string vier depois da primeira em ordem alfabética;
 *			= 0 se as strings são iguais.
 */
int compareString (void *, void *);

/*	swapFields ()
 *
 *	Função para trocar de lugar o conteúdo de dois blocos da memória de mesmo tamanho.
 *	- Parâmetros:
 *		void *: ponteiro para o bloco que contém o primeiro bloco;
 *		void *: ponteiro para o bloco que contém o segundo bloco.
 *	- Retorno:
 *		não há.
 */
void swapFields (void *, void *, int);

/*	orderIndex ()
 *
 *	Função para ordenar os índices de um .data.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as características descritas pelo .schema;
 *		int: quantidade de dados gravados no índice.
 *	- Retorno:
 *		não há.
 */
void orderIndex (SCHEMA *, int);

/*	insertData ()
 *
 *	Função para inserir registros no final de um arquivo .data.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as características descritas pelo .schema;
 *		int *: endereço da variável que guarda a quantidade de registros do .data.
 *	- Retorno:
 *		não há.
 */
void insertData (SCHEMA *, int *);

/*	sequentialSearch ()
 *
 *	Função para realizar a busca sequencial em um .data (apenas novos registros).
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura com as características descritas pelo .schema;
 *		int: posição do campo no vetor SCHEMA * que contém informações o dado a ser buscado;
 *		int: posição inicial da busca;
 *		int :posição final busca;
 *		void *: chave que deve ser buscada;
 *		int *: ponteiro para a variável que guarda a quantidade de passos na busca.
 *	- Retorno:
 *		int: deslocamento do registro com a chave se ela for encontrada ou -1.
 */
int sequentialSearch (SCHEMA *, int, int, int, void *, int *);

/*	binarySearch ()
 *
 *	Função que seleciona qual busca binária deve ser feita por uma chave em determinado índice.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que descreve o arquivo .data conforme o arquivo .schema;
 *		int: posição do campo no vetor de campos da estrutura com a descrição do .schema;
 *		int: quantidade de dados indexados;
 *		int *: endereço na memória da chave pela qual a busca procura;
 *		int *: endereço da variável que guarda quantos passos foram necessários na busca.
 *	- Retorno:
 *		int: deslocamento do registro com a chave se ela for encontrada no índice ou -1.
 */
int binarySearch (SCHEMA *, int, int, int, void *, int *);

/*	getKey ()
 *
 *	Função para ler da entrada padrão uma chave a ser buscada, selecionando como a leitura deve ser feita para diferentes tipos.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que descreve o arquivo .data conforme o arquivo .schema;
 *		int: posição do campo no vetor de campos da estrutura com a descrição do .schema.
 *	- Retorno:
 *		void *: endereço da chave obtida na memória.
 */
void * getKey (SCHEMA *, int);

/*	selectIt ()
 *
 *	Função para buscar por uma chave, de um dos campos que possui .idx, no seu índice ou .data.
 *	- Parâmetros:
 *		SCHEMA *: endereço da estrutura que descreve o arquivo .data conforme o arquivo .schema;
 *		int: quantidade de dados no .data;
 *		int: quantidade de dados indexados.
 *	- Retorno:
 *		não há.
 */
void  selectIt (SCHEMA *, int, int);

int main (int argc, char * argv[]) {
	char * schemaName;
	char * instruction;
	SCHEMA * schema;
	int recordsNumber;
	int recordsIndexed;

	schemaName = readUntilChar (stdin, '\n', BUFFER_SIZE);
	schema = getSchema (schemaName);

	recordsNumber = getRecordsNumber (schema);
	createIndex (schema, recordsNumber);
	recordsIndexed = recordsNumber;
	orderIndex (schema, recordsIndexed);

	while (strcmp (instruction = readUntilChar (stdin, '\n', BUFFER_SIZE), "exit") != 0) {
		if (!strcmp (instruction, "dump_schema")) {
			dumpSchema (schema);
		}
		else if (!strcmp (instruction, "dump_data")) {
			dumpData (schema, recordsNumber);
		}
		else if (!strcmp (instruction, "dump_index")) {
			dumpIndex (schema, recordsIndexed);
		}
		else if (!strcmp (instruction, "insert")) {
			insertData (schema, &recordsNumber);
		}
		else if (!strcmp (instruction, "select")) {
			selectIt (schema, recordsNumber, recordsIndexed);
		}
		else if (!strcmp (instruction, "update_index")) {
			createIndex (schema, recordsNumber);
			recordsIndexed = recordsNumber;
			orderIndex (schema, recordsIndexed);
		}
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

char * readUntilChar (FILE * p_file, const char delimiter, int bufferSize) {
	char read;
	char * string = NULL;
	int i;

	for (i = 0; !feof (p_file) && (read = fgetc (p_file)) != delimiter; i++) {
		if (i % BUFFER_SIZE == 0) {
			string = (char *) realloc (string, (i + bufferSize) * sizeof (char));
		}

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
			if (strcmp (schema->fields[i].type, "int") == 0) {
				schema->fields[i].size = sizeof (int);
			}
			else {
				schema->fields[i].size = sizeof (double);
			}
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

int getRecordsNumber (SCHEMA * schema) {
	FILE * p_data;
	int size;

	p_data = openDataFile (schema, "r");
	fseek (p_data, 0, SEEK_END);

	size = ftell (p_data);
	size /= schema->size;

	fclose (p_data);

	return size;
}

void dumpSchema (SCHEMA * schema) {
	int i;

	printf ("table %s(%d bytes)\n", schema->table, schema->size);

	for (i = 0; i < schema->fieldsNumber; i++) {
		printf ("%s %s", schema->fields[i].name, schema->fields[i].type);

		if (!strcmp (schema->fields[i].type, "char")) {
			printf ("[%d]", schema->fields[i].size);
		}
		if (schema->fields[i].order == 1) {
			printf (" order");
		}

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

void fprint (const char * type, void * pointer, int offset, FILE * output) {
	if (!strcmp (type, "int")) {
		fprintInt (pointer, offset, output);
	}
	else if (!strcmp (type, "double")) {
		fprintDouble (pointer, offset, output);
	}
	else {
		fprintString (pointer, offset, output);
	}
} 

int getInternOffset (SCHEMA * schema, int position) {
	int offset, i;

	for (offset = 0, i = 0; i < position; i++) {
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

void createIndex (SCHEMA * schema, int recordsNumber) {
	int i, j, internOffset;
	FILE * p_index, * p_data;
	void * data = NULL;
	char * next = NULL;

	data = malloc (schema->size * sizeof (char));
	p_data = openDataFile (schema, "r");

	for (i = 0; i < schema->fieldsNumber; i++) {
		if (schema->fields[i].order) {
			p_index = openIndexFile (schema, "w+", i);

			internOffset = getInternOffset (schema, i);

			next = (char *) data;
			next += internOffset;

			for (j = 0; j < recordsNumber * schema->size; j += schema->size) {
				fread (data, schema->size, 1, p_data);

				fwrite ((void *) next, schema->fields[i].size, 1, p_index);
				fwrite ((void *) &j, sizeof (int), 1, p_index);
			}

			fclose (p_index);
			rewind (p_data);
		}
	}

	fclose (p_data);
	free (data);
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

void dumpIndex (SCHEMA * schema, int recordsIndexed) {
	int i, j;
	void * index;
	FILE * p_index;
	void (*printThis) (void *, int, FILE *);

	for (i = 0; i < schema->fieldsNumber; i++) {
		if (schema->fields[i].order) {
			p_index = openIndexFile (schema, "r", i);
			index = malloc (schema->fields[i].size + sizeof (int));

			if (!strcmp (schema->fields[i].type, "int")) {
				printThis = &fprintInt;
			}
			else if (!strcmp (schema->fields[i].type, "double")) {
				printThis = &fprintDouble;
			}
			else {
				printThis = &fprintString;
			}

			for (j = 0; j < recordsIndexed; j++) {
				fread (index, schema->fields[i].size + sizeof (int), 1, p_index);
				printThis (index, 0, stdout);

				printf (" = ");

				fprintInt (index, schema->fields[i].size, stdout);
				printf("\n");
			}

			free (index);
			fclose (p_index);
		}
	}
}

void * readIndex (SCHEMA * schema, int recordsIndexed, int fieldPosition) {
	void * index = NULL;
	FILE * p_index = NULL;

	index = malloc ((schema->fields[fieldPosition].size + sizeof (int)) * recordsIndexed);
	p_index =  openIndexFile (schema, "r", fieldPosition);

	fread (index, schema->fields[fieldPosition].size + sizeof (int), recordsIndexed, p_index);

	fclose (p_index);

	return index;
}

int compareInt (void * number1, void * number2) {
	return *((int *) number2) - *((int *) number1);
}

int compareDouble (void * number1, void * number2) {
	double result;

	result = *((double *)number2) - *((double *)number1);

	return result == 0.0? 0 : result < 0.0 ? -1 : 1;
}

int compareString (void * string1, void * string2) {
	return strcmp ((char *) string2, (char *) string1);
}

void swapFields (void * field1, void * field2, int size) {
	void * aux;

	aux = malloc (size);

	memcpy (aux, field2, size);
	memcpy (field2, field1, size);
	memcpy (field1, aux, size);

	free (aux);
}

void orderIndex (SCHEMA * schema, int recordsIndexed) {
	void * index = NULL;
	int i, j, k, offset, size;
	FILE * p_index = NULL;
	int (*compare) (void *, void *);

	for (i = 0; i < schema->fieldsNumber; i++) {
		if (schema->fields[i].order) {
			index = readIndex (schema, recordsIndexed, i);

			if (!strcmp (schema->fields[i].type, "int")) {
				compare = &compareInt;
			}
			else if (!strcmp (schema->fields[i].type, "double")) {
				compare = &compareDouble;
			}
			else {
				compare = &compareString;
			}

			size = sizeof (int) + schema->fields[i].size;

			for (j = 1; j < recordsIndexed; j++) {
				for (k = 0; k < recordsIndexed - j; k++) {
					offset = k * size;
					if (compare ((char *) index + offset, (char *) index + offset + size) < 0) {
						swapFields ((char *) index + offset, (char *) index + offset + size, size);
					}
				}
			}


			p_index = openIndexFile (schema, "w", i);
			fwrite (index, (schema->fields[i].size + sizeof (int)), recordsIndexed, p_index);

			fclose (p_index);
			free (index);
		}
	}
}

void insertData (SCHEMA * schema, int * recordsNumber) {
	FILE * p_data;
	void * data;
	int i, newInt;
	double newDouble;
	char * newString, *auxString;

	p_data = openDataFile (schema, "a+");
	*recordsNumber += 1;

	for (i = 0; i < schema->fieldsNumber; i++) {
		newString = NULL;

		if (!strcmp (schema->fields[i].type, "int")) {
			scanf ("%d", &newInt);
			data = (void *) &newInt;
		}
		else if (!strcmp (schema->fields[i].type, "double")) {
			scanf ("%lf", &newDouble);
			data = (void *) &newDouble;
		}
		else {
			if (i > 0 && strcmp (schema->fields[i - 1].type, "char")) fgetc (stdin);
			auxString = readUntilChar (stdin, '\n', schema->fields[i].size);
			newString = (char *) calloc (schema->fields[i].size, sizeof (char));

			strcpy (newString, auxString);
			data = (void *) newString;

			free (auxString);
		}

		fwrite (data, schema->fields[i].size, 1, p_data);
		free (newString);
	}

	fclose (p_data);
}

int sequentialSearch (SCHEMA * schema, int position, int recordsNumber, int recordsIndexed, void * key, int * tries) {
	int offset, internOffset, max;
	FILE * p_data = NULL;
	void * data = NULL;
	int (*compare) (void *, void *);

	internOffset = getInternOffset (schema, position);
	max = recordsNumber * schema->size;

	data = malloc (schema->fields[position].size);
	p_data = openDataFile (schema, "r");

	if (!strcmp (schema->fields[position].type, "int")) {
		compare = &compareInt;
	}
	else if (!strcmp (schema->fields[position].type, "double")) {
		compare = &compareDouble;
	}
	else {
		compare = &compareString;
	}

	for (offset = recordsIndexed * schema->size + internOffset; offset < max; offset += schema->size) { 
		(*tries)++;
		fseek (p_data, offset, SEEK_SET);
		fread (data, schema->fields[position].size, 1, p_data);

		if (!compare (data, key)) {
			free (data);
			fclose (p_data);
			offset -= getInternOffset (schema, position);
			return offset;
		}
	}

	free (data);
	fclose (p_data);
	return -1;
}

int binarySearch (SCHEMA * schema, int position, int begin, int end, void * key, int * tries) {
	int middle;
	void * found = NULL;
	FILE * p_index = NULL;
	int (*compare) (void *, void *);

	(*tries)++;

	if (begin > end) return -1;

	middle = (begin + end) / 2;

	found = malloc (schema->fields[position].size);
	p_index = openIndexFile (schema, "r", position);

	fseek (p_index, middle * (schema->fields[position].size + sizeof (int)), SEEK_SET);
	fread (found, schema->fields[position].size, 1, p_index);

	if (!strcmp (schema->fields[position].type, "int")) {
		compare = &compareInt;
	}
	else if (!strcmp (schema->fields[position].type, "double")) {
		compare = &compareDouble;
	}
	else {
		compare = &compareString;
	}

	if (compare (found, key) == 0) {
		fread (found, sizeof (int), 1, p_index);

		middle = *((int *) found);

		free (found);
		fclose (p_index);
		return middle;
	}
	else if (compare (found, key) < 0) {
		free (found);
		fclose (p_index);
		return binarySearch (schema, position, begin, middle - 1, key, tries);
	}
	else {
		free (found);
		fclose (p_index);
		return binarySearch (schema, position, middle + 1, end, key, tries);
	}
}

void * getKey (SCHEMA * schema, int i) {
	void * key;
	char * auxString;

	key = calloc (schema->fields[i].size, sizeof (char));
	if (!strcmp (schema->fields[i].type, "int")) {
		scanf ("%d", (int *) key);
	}
	else if (!strcmp (schema->fields[i].type, "double")) {
		scanf ("%lf", (double *) key);
	}
	else {
		auxString = readUntilChar (stdin, '\n', schema->fields[i].size);
		strcpy ((char *) key, auxString);
		free (auxString);
	}

	return key;
}

void selectIt (SCHEMA * schema, int recordsNumber, int recordsIndexed) {
	int i, j, found, tries = 0;
	char * field, * interest;
	void * key = NULL;
	FILE * p_data = NULL;
	void * data = NULL;

	field = readUntilChar (stdin, '\n', BUFFER_SIZE);
	for (i = 0; strcmp (field, schema->fields[i].name) != 0; i++);

	key = getKey (schema, i);
	if (strcmp (schema->fields[i].type, "char")) fgetc (stdin);

	interest = readUntilChar (stdin, '\n', BUFFER_SIZE);
	for (j = 0; strcmp (interest, schema->fields[j].name) != 0; j++);

	if (!strcmp (schema->fields[i].name, field) && schema->fields[i].order) {
		found = binarySearch (schema, i, 0, recordsIndexed - 1, key, &tries);

		if (found >= 0) {
			printf ("%d\n", tries);

			p_data = openDataFile (schema, "r");
			fseek (p_data, found + getInternOffset (schema, j), SEEK_SET);
			data = malloc (schema->fields[j].size);
			fread (data, schema->fields[j].size, 1, p_data);

			fprint (schema->fields[j].type, data, 0, stdout);
			printf ("\n");

			fclose (p_data);
			free (data);
		}

		else if (found < 0 && recordsNumber > recordsIndexed) {
			found = sequentialSearch (schema, i, recordsNumber, recordsIndexed, key, &tries);
			tries--;

			if (found < 0) {
				printf ("%d\n", tries);
				printf ("value not found\n");
			}
			else {
				printf ("%d\n", tries);

				p_data = openDataFile (schema, "r");
				fseek (p_data, found + getInternOffset (schema, j), SEEK_SET);
				data = malloc (schema->fields[j].size);
				fread (data, schema->fields[j].size, 1, p_data);

				fprint (schema->fields[j].type, data, 0, stdout);
				printf ("\n");

				fclose (p_data);
				free (data);
			}
		}
	}
	else {
		printf ("index not found\n");
	}

	free (interest);
	free (key);
	free (field);
}
