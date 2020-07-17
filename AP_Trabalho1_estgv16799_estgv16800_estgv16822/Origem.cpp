#define _CRT_SECURE_NO_WARNIGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#pragma warning(disable:4996)

#define DOTS_10 ".........."
#define SPAC_10 "          "
#define STRING char *
#define MAX_LINHA_FICHEIRO 100
#define MAX 60
#define MAXLOCAL 26
#define MAXNOME 100
#define MAXCIDADE 40
#define MAXCONTRATOS 20000
typedef char LinhaTexto[MAX_LINHA_FICHEIRO];
LinhaTexto LT;



typedef struct freguesias {
	int id_freguesia;
	char *distrito;
	char *concelho;
	char *freguesia;
}FREGUESIAS;

typedef struct Contagem {
	int id_contrato;
	int contagem;
	char *data;
}CONTAGEM;

typedef struct contratos {
	int id_contrato;
	char *nome_cliente;
	char *id_local;
	float potencia;
	int estado;
	int id_freguesia;
	int contagem;
}CONTRATOS;

typedef struct DATA
{
	CONTRATOS *C;
	FREGUESIAS *fre;
	CONTAGEM *cont;
	int NContrato;
	int nl_freg;
}Data;

STRING *Read_Split_Line_File(FILE *f, int n_campos_max, int *n_campos_lidos, char *separadores)
{
	*n_campos_lidos = 0;
	if (!f) return NULL;
	if (fgets(LT, MAX_LINHA_FICHEIRO, f) != NULL)
	{// Temos de partir a linha lida!
		STRING *Res = (STRING *)malloc(n_campos_max * sizeof(STRING));
		char *pch = strtok(LT, separadores);
		int cont = 0;
		while (pch != NULL)
		{
			Res[cont] = (char *)malloc((strlen(pch) + 1) * sizeof(char));
			strcpy(Res[cont++], pch);
			pch = strtok(NULL, separadores);
		}
		*n_campos_lidos = cont;
		return Res;
	}
	return NULL;
};

STRING *Read_Split_Line(STRING a, int n_campos_max, int *n_campos_lidos, char *separadores)
{
	*n_campos_lidos = 0;
	if (!a) return NULL;
	//if (fgets(LT, MAX_LINHA_FICHEIRO, f) != NULL)
	//{// Temos de partir a linha lida!
	STRING *Res = (STRING *)malloc(n_campos_max * sizeof(STRING));
	char *pch = strtok(a, separadores);
	int cont = 0;
	while (pch != NULL)
	{
		Res[cont] = (char *)malloc((strlen(pch) + 1) * sizeof(char));
		strcpy(Res[cont++], pch);
		pch = strtok(NULL, separadores);
	}
	*n_campos_lidos = cont;
	return Res;
	//}
	return NULL;
};



void InicializarDados(Data *Ed)
{
	Ed->NContrato = 0;

}


void lercontratos(Data *ed) {

	int numerocamposlidos;
	int nl_contratos = 0;
	int i = 0;
	STRING *v;
	STRING *v_contratos;

	FILE *f_contratos = fopen("contratos.txt", "r");
	if (f_contratos == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}

	/*CONTAGEM DE LINHAS DO FICHEIRO*/
	while (!feof(f_contratos))
	{
		v = Read_Split_Line_File(f_contratos, 4, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 4)
			nl_contratos++;
	}
	fclose(f_contratos);


	/*ALOCAR MEMORIA PARA O NUMERO DE FICHEIROS*/
	ed->C = (CONTRATOS *)malloc(nl_contratos * sizeof(CONTRATOS));


	/*COLOCAR PARA A MEMORIA*/
	FILE *ff_contratos = fopen("contratos.txt", "r");
	if (ff_contratos == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}
	while (!feof(ff_contratos))
	{
		v_contratos = Read_Split_Line_File(ff_contratos, 4, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 4)
		{
			ed->C[i].id_contrato = atoi(v_contratos[0]);
			ed->C[i].nome_cliente = v_contratos[1];
			ed->C[i].id_local = v_contratos[2];
			ed->C[i].potencia = atof(v_contratos[3]);
			ed->C[i].estado = 1;

			free(v_contratos[0]);
			free(v_contratos);//liberta a memória alocada
			++i;
			ed->NContrato++;
		}
	}
	fclose(ff_contratos);
	
}

void lerFreguesias(Data *ed) {

	int numerocamposlidos;
	int nl_freguesias = 0;
	int i = 0;
	STRING *v;
	STRING *v_freguesias;

	FILE *f_freguesias = fopen("freguesias.txt", "r");
	if (f_freguesias == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}

	/*CONTAGEM DE LINHAS DO FICHEIRO*/
	while (!feof(f_freguesias))
	{
		v = Read_Split_Line_File(f_freguesias, 4, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 4)
			nl_freguesias++;
	}
   fclose(f_freguesias);
	ed->nl_freg = nl_freguesias;


	/*ALOCAR MEMORIA PARA O NUMERO DE FICHEIROS*/
	ed->fre = (FREGUESIAS *)malloc(nl_freguesias * sizeof(FREGUESIAS));


	/*COLOCAR PARA A MEMORIA*/
	FILE *ff_freguesias = fopen("freguesias.txt", "r");
	if (ff_freguesias == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}
	while (!feof(ff_freguesias))
	{
		v_freguesias = Read_Split_Line_File(ff_freguesias, 4, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 4)
		{
			ed->fre[i].id_freguesia = atoi(v_freguesias[0]);
			ed->C[i].id_freguesia =  atoi(v_freguesias[0]);
			ed->fre[i].distrito = v_freguesias[1];
			ed->fre[i].concelho = v_freguesias[2];
			ed->fre[i].freguesia = v_freguesias[3];
			free(v_freguesias[0]);
			free(v_freguesias);//liberta a memória alocada
			++i;
		}
	}
	fclose(ff_freguesias);
}

void lerLocais(Data *ed) {

	int numerocamposlidos;
	int nl_locais = 0;
	int i = 0;
	int comparacao;
	STRING *v;
	STRING *v_locais;

	FILE *f_locais = fopen("locais.txt", "r");
	if (f_locais == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}

	/*CONTAGEM DE LINHAS DO FICHEIRO*/
	while (!feof(f_locais))
	{
		v = Read_Split_Line_File(f_locais, 2, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 2)
			nl_locais++;
	}
	fclose(f_locais);



	/*ALOCAR MEMORIA PARA O NUMERO DE FICHEIROS*/
	/*ed->fre = (FREGUESIAS *)malloc(nl_locais * sizeof(FREGUESIAS));*/


	/*COLOCAR PARA A MEMORIA*/
	FILE *ff_locais = fopen("locais.txt", "r");
	if (ff_locais == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}
	while (!feof(ff_locais))
	{
		v_locais = Read_Split_Line_File(ff_locais, 2, &numerocamposlidos, "\t");
		if (numerocamposlidos == 2)
		{

			comparacao = strcmp(ed->C[i].id_local, (v_locais[0]));
			if (comparacao == 0)
			{
				ed->C[i].id_freguesia = atoi(v_locais[1]);
				//printf("*******************************************************");
				/*printf("Valor dos locais lido: %s\n", v_locais[0]);
				printf("VALOR DOS LOCAIS NAESTRUTURA:%s\n", ed->C[i].id_local);
				printf("Valor do id da freguesia lido:%d\n", (v_locais[1]));
				printf("Valor do id da freguesia introduzido na estrutura:%d\n", ed->C[i].id_freguesia);
				printf("#######################################################\n\n");*/
			}
			/*ed->fre[i].id_freguesia = atoi(v_locais[0]);
			//ed->C[i].id_freguesia = atoi(v_freguesias[0]);
			ed->fre[i].distrito = v_locais[1];
			ed->fre[i].concelho = v_locais[2];
			ed->fre[i].freguesia = v_locais[3];*/
			free(v_locais[0]);
			free(v_locais);//liberta a memória alocada
			++i;
		}
	}
	/*printf("\tFreguesias com o codigo e [%d\n\n\n\n", ed->fre[5].id_freguesia);
	printf("\tFreguesias com o codigo e [%s\n\n\n\n",ed->fre[5].distrito);
	printf("\tFreguesias com o codigo e [%s\n\n\n\n", ed->fre[5].concelho);
	printf("\tFreguesias com o codigo e [%s\n\n\n\n", ed->fre[5].freguesia);*/

	fclose(ff_locais);
}

void lercontagens(Data *ed) {

	int numerocamposlidos;
	int nl_cont = 0;
	int i = 0;
	int j;
	STRING *v;
	STRING *v_cont;

	FILE *f_freguesias = fopen("contagens.txt", "r");
	if (f_freguesias == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}

	/*CONTAGEM DE LINHAS DO FICHEIRO*/
	while (!feof(f_freguesias))
	{
		v = Read_Split_Line_File(f_freguesias, 3, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 3)
			nl_cont++;
	}
	fclose(f_freguesias);


	/*ALOCAR MEMORIA PARA O NUMERO DE FICHEIROS*/
	ed->cont = (CONTAGEM *)malloc(nl_cont * sizeof(CONTAGEM));

	/*COLOCAR PARA A MEMORIA*/
	FILE *ff_cont = fopen("contagens.txt", "r"); 
	if (ff_cont == NULL) {
		printf("Erro abrir ficheiro!\n");
		return;
	}

	while (!feof(ff_cont))
	{
		v_cont = Read_Split_Line_File(ff_cont, 3, &numerocamposlidos, "\t\n");
		if (numerocamposlidos == 3)
		{
			ed->cont[i].id_contrato = atoi(v_cont[0]);
			ed->cont[i].contagem = atoi(v_cont[1]);
			for (j = 0; j < ed->NContrato; j++)
			{
				if (ed->C[j].id_contrato == ed->cont[i].id_contrato)
					ed->C[j].contagem = ed->cont[i].contagem;
			}
			ed->cont[i].data = v_cont[2];
			free(v_cont[0]);
			free(v_cont);//liberta a memória alocada
			++i;
		}
	}
	fclose(ff_cont);
}

void gravaFichTexto(DATA *ed) //s faz sentido gravar elementos ocupados do vetor
{
	FILE *fic;
	char *nome = "contratos.txt";
	fic = fopen(nome, "w");
	if (fic == NULL)
		fprintf(stderr, "\nImposs?vel abrir ficheiro\n");
	else
	{
		for (int i = 0; i < ed->NContrato; i++)
		{
			if (ed->C->estado==1)
			{
				fprintf(fic, "%d\t%s\t%d\t%f\n", ed->C[i].id_contrato, ed->C[i].nome_cliente, ed->C[i].id_local, ed->C[i].potencia);
			}
		}
		fclose(fic);
	}
}


void menu(Data *ed);

void LAllC(Data *ed);
void pesquisar_parte_nome(Data *ed) {
	int i;
	char *p;
	char lixo;
	char nome[MAXNOME];
	printf("\t\t\t\tQual o nome que quer pesquisar?\n\t\t\t\t");
	scanf("%c", &lixo);//le uma variavel com lixo que faz ignorar o 1º gets
	gets_s(nome, MAXNOME);
	for (i = 0; i < ed->NContrato; i++)
	{
		p = strstr(ed->C[i].nome_cliente, nome);
		if (p)
		{
			printf("\t\t\tID_Contrato: %d\tNome completo:%s\n", ed->C[i].id_contrato, ed->C[i].nome_cliente);
		}
		//printf("\t\t\tNao Encontrou na pos:[%d]\n", i);
	}
	_sleep(3000);
	system("cls");
	LAllC(ed);
}

void inserirnovocliente(Data *ed) {

	int i = ed->NContrato;
	int idcontrato = ed->NContrato + 1;
	char nome_rec[MAXNOME], lixo;
	char id_local_rec[27];
	float potencia_rec;
	int id_freg_rec;
	
	
	printf("\n\t\t\t\tNome: ");
	gets_s(nome_rec, MAXNOME);
	printf("\n\t\t\t\tID_LOCAL (formato: PT XXX XXX XXX XXX XXX ORC):");
	gets_s(id_local_rec, 27);
	printf("\n\t\t\t\tID Freguesia:");
	scanf("%d", &id_freg_rec);
	printf("\n\t\t\t\tPotencia:");
	scanf("%f", &potencia_rec);
	ed->C = (CONTRATOS *)realloc(ed->C, (ed->NContrato + 1) * sizeof(CONTRATOS));//realocar memoria para mais uma pessoa do quea que estava anteriormente
	ed->NContrato++;//incrementa mais um contrato aos que tinhamos anteriormente
	ed->C[i].estado = 1;
	
	ed->C[i].id_contrato = idcontrato;
	ed->C[i].potencia = potencia_rec;
	ed->C[i].nome_cliente = (char *)malloc(MAXNOME * sizeof(char));
	strcpy(ed->C[i].nome_cliente, nome_rec);
	ed->C[i].id_local = (char *)malloc(27 * sizeof(char));
	strcpy(ed->C[i].id_local, id_local_rec);
	ed->C[i].id_freguesia = id_freg_rec;
	if (ed->C[i].estado != 0) {
		printf("\n\t\t\t\tCidadao inserido com sucesso! %d \n", idcontrato);
		_sleep(1000);
		gravaFichTexto(ed);
		system("cls");
		LAllC(ed);
	}
	
}

int eliminarcliente(Data *ed) {
	int id;
	printf("\t\t\t\tQual o id do cliente que quer eliminar?\n");
	scanf("%d", &id);
	ed->C[id - 1].estado = 0;
	if (ed->C[id - 1].estado == 0)
	{

		printf("\t\t\t\tCliente removido com sucesso!\n");
		system("cls");
		LAllC(ed);
	}
	system("pause");
	return 0;
}


void FirstName(Data *ED)
{
	int i, vv, k;
	CONTRATOS auxi;
	do { // repete o codigo ate que nao seja feita nenhuma alteracao na estrutura.
		k = 0;
		for (i = 0; i <= ED->NContrato - 2; i++)  // percorre a estrutura desde a primeira posicao ate a ultima
		{
			vv = strcmp(ED->C[i].nome_cliente, ED->C[i + 1].nome_cliente);  //compara qual o nome que vem primeiro 
			if (vv > 0)
			{
				auxi = ED->C[i];
				ED->C[i] = ED->C[i + 1];
				ED->C[i + 1] = auxi;
				k++;
			}
		}
	} while (k != 0);
	printf("|| ID_Cliente:\t\t||\tNome cliente: ||\n");
	int c = 1;
	for (int i = 0; i <= ED->NContrato - 1; i++) //percorre  a estrutura desde a primeira posicao ate a ultima
	{
		
		printf("\t%d\t||\t%s\n", ED->C[i].id_contrato, ED->C[i].nome_cliente);
		if (i == 10 * c)
		{
			c++;
			system("pause");
			system("cls");
			printf("|| ID_Cliente:\t\t||\tNome cliente: ||\n");

		}

	}
	printf("\n");
	system("pause");
	system("cls");
	LAllC(ED);
}

void LIdFreg(Data *ed) {
	int id_freguesia_rec, i;
	printf("\n\n\t\t\t\tQual o id da freguesia que quer pesquisar?\n\n\t\t\t\t\t\t");
	scanf("%d\n\n", &id_freguesia_rec);
	
	for (i = 0; i < ed->NContrato; i++)
	{
		if (ed->C[i].estado != 0)
		{
			if (ed->C[i].id_freguesia == id_freguesia_rec)
			{
				printf("\t\t\t\tID Contrato:%d \n", ed->C[i].id_contrato);
				printf("\t\t\t\tID Local:%s \n", ed->C[i].id_local);
				printf("\t\t\t\tID Freguesia:%d\n", ed->C[i].id_freguesia);
				printf("\t\t\t\tPotencia: %.1f \n", ed->C[i].potencia);
				printf("\t\t\t\tNome freguesia: %s\n", ed->fre[i].concelho);
				printf("\t\t\t\tNome Cliente: %s\n\n", ed->C[i].nome_cliente);
			}
		}
	}
	system("pause");
	system("cls");
	menu(ed);
}

void CMedioCliente(Data *ed) {
	int id_cliente, i, f = 0;float som = 0;
	printf("\t\t\t\tQual o id do cliente?\n");
	scanf("%d", &id_cliente);
	for (i = 0; i <15000; i++)
	{	
			if (ed->cont[i].id_contrato == id_cliente)
			{
				f++;
				som += ed->cont[i].contagem;
			}
	}
	printf("\t\t\t\to consumo medio do cliente com o id:%d e: %.3f",id_cliente, som / f);
	_sleep(3000);
	system("cls");
	LAllC(ed);
}

void LP(Data *ed) {
	int i,c=0;
	float potencia_rec;
	char lixo;
	STRING t="ID Contrato: || ID Local: || Potencia: || Nome Cliente: \n";
	printf("\t\t\t\tQual a  potencia que deseja listar?\n\t\t");
	scanf("\t\t\t\t%f", &potencia_rec);
	system("cls");
	printf("%s", t);
	for (i = 0; i<ed->NContrato; i++)
	{
		if (potencia_rec == ed->C[i].potencia)
		{
			c++;
			printf(" %d\t", ed->C[i].id_contrato);
			printf(" %s\t", ed->C[i].id_local);
			printf(" %.1f\t", ed->C[i].potencia);
			printf(" %s\n\n", ed->C[i].nome_cliente);
		}
		if (c==10)
		{
			system("pause");
			system("cls");
			c = 0;
			printf("%s", t);

		}

	}
	system("pause");
	system("cls");
	LAllC(ed);

}

void LD(Data *ed) {
	int i, j, x;
	char distrito_rec[MAX];
	char lixo;
	printf("\t\t\t\tQual o distrito que deseja listar?");
	gets_s(distrito_rec, MAX);
	for (i = 0; i<ed->nl_freg; i++)
	{
		x = strcmp(ed->fre[i].distrito, distrito_rec);
		if (x == 0)
		{
			printf("ID Contrato: %d\n", ed->C[i].id_contrato);
			printf("Nome Cliente: %s", ed->C[i].nome_cliente);
			printf("ID Local: %s\n", atol(ed->C[i].id_local));
			printf("ID Freguesia: %d\n", ed->C[i].id_freguesia);
			printf("Concelho: %s\n", ed->fre[i].freguesia);
			printf("Potencia: %.1f\n\n\n ", ed->C[i].potencia);

		}
	}
	system("pause");
	system("cls");
	LAllC(ed);
}


void LAllC(Data *ed) {

	int i, c = 1;
	char f;
	STRING t = " ||ID CONTRATO\t||POTENCIA||  ID LOCAL\t||ID FREG\t||NOME CLIENTE\n";
	printf("%s",t);
	for (i = 0; i < ed->NContrato; i++)
	{
		if (ed->C[i].estado != 0)
		{
			printf(" ||%d\t||", ed->C[i].id_contrato);
			printf(" %.1f\t  || ", ed->C[i].potencia);
			printf(" %s\t||", ed->C[i].id_local);
			printf(" %d\t||", ed->C[i].id_freguesia);
			printf(" %s\t\n ", ed->C[i].nome_cliente);

			if (i > c * 10)
			{
				
				printf("\n\n\t\t\t\tESPACO PARA CONTINUAR A LISTAR || ENTER PARA ABRIR O MENU \n\n");
				f = _getch();
				if (f =='\r')
				{
					menu(ed);
				}
				system("cls");
				printf("%s", t);

				c++;
			}
		}
	} 


	menu(ed);
}



void menu(Data *ed) {
	int l,c;
	STRING f = "||1->CONTRATOS\t||\t2->LISTAR\t||\t3->PESQUISAR\t||\t4->CONSUMO MEDIO CLIENTE\t||   5->EXIT  ||\n";
	STRING t = "\t\t||1->INSERIR\t||\t2->REMOVER\t||\t3->VOLTAR\t||\t4->EXIT\t\t||\n";
	STRING k = "	||1->ID_FREGUESiA|| 2->ALFABETICA(A-Z) || 3->POTENCIA || 4->DISTRITO ||\t5->VOLTAR||6->EXIT||\n";

	printf("%s", f);
	do {

		c = _getch();
		
		switch (c)
		{
		case '1': 
			printf("\n%s", t);
			l = _getch();
			switch (l)
			{
			case '1':inserirnovocliente(ed); break;
			case '2':eliminarcliente(ed); break;
			case '3':system("cls");LAllC(ed); break;
			case '4':exit(0);
			}
			break; 
		case '2':printf("\n%s", k);
			l = _getch();
			switch (l)
			{
			case '1':LIdFreg(ed); break;
			case '2':FirstName(ed); break;
			case '3':LP(ed); break;
			case '4':LD(ed); break;
			case '5':system("cls"); LAllC(ed); break;
			case '6':exit(0);
			}
			break;
		case '3':pesquisar_parte_nome(ed); break;
		case '4':CMedioCliente(ed); break;
		case '5':exit(0); break;
		
		}
	} while (c != 0);

}
void main() {
	Data ed;
	for (int i = 1; i <= 5; i++) {
		printf("loanding");
		printf("\t%1.*s", i, DOTS_10);
		printf("\t%1.*s\r", 10 - i, SPAC_10);
		_sleep(100);
	}
	InicializarDados(&ed);  
	lercontratos(&ed);
	lerFreguesias(&ed);
	lerLocais(&ed);
    lercontagens(&ed);
	
	system("cls");
	LAllC(&ed);	
	system("pause");
}