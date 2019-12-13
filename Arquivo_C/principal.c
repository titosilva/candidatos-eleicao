// Arquivo principal da questão 2, o qual contem a função main
// A função main apenas serve para manusear os argumentos passados pelo usuario
// Toda a analise é realizada pela função analisar_arquivo

// Os argumentos passados pelo usuário pela linha de comando devem ser:
// 1 - o ano da eleição
// 2 - a sigla que deseja utilizar

/*
	Esse código funciona em blocos que se repetem. Para calcular as quantidades de candidatos por cargo,
	por gênero, etc., o programa cria dois vetores e uma variavel que serve para contagem, alem de um
	arquivo de texto.

	O programa lê as informações e verifica se trata-se de algo novo.
	Por exemplo, ao tratar-se da seção em que é calculado quantos candidatos há por cargo,
	o programa verifica se o candidato é do primeiro turno. Se for, ele busca a informação sobre o cargo.
	Após essa operação, ele verifica se o código correspondente àquele cargo já está em uma lista(vetor de inteiros) 
	que é construída conforme o código é executado e contem os codigos de cargos que são encontrados.
	Se estiver, o programa apenas incrementa a posição corresponde em outro vetor, que dessa vez é um vetor
	de contagem, no qual cada elemento representa um cargo.
	Se não estiver, o programa realoca os vetores e cria uma posição nova, 
	a qual passa a ser representativa desse novo cargo que não havia sido visto antes. Assim, a lista
	vai sendo construída conforme o programa descobre novas informações.
	Além disso, o programa constroi um arquivo de texto que contem os nomes de cada cargo, partido
	etc. e tambem é construído durante a execução. Nesse arquivo, a primeira linha corresponderá 
	ao cargo cujo código está na primeira posição da lista previamente mencionada.
	Ao exibir na tela os resultados, o programa usa o arquivo de texto gerado para referir-se ao cargo
	pelo nome, ao inves de referir-se pelo codigo.

	O codigo está grande, mas isso ocorre justamente devido a essa repetição em blocos.
	Esse processo, feito com os cargos, é repetido na parte em que são calculados os candidatos 
	por genero, por raça, por partido, etc.

	Apenas as partes de calculo de candidatos por turno e de media de idade dos candidatos não são
	repetições desses blocos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Quantidade de argumentos exigidos pelo programa
#define ARGQUANT 3
// Nome do arquivo executavel
#define EXEC_NOME "consulta_eleicao"
// Padrão para localização de arquivos para consulta
#define PADRAO "./Arquivos/consulta_cand_"
// Tamanho para alguns vetores
#define MAX 5
// Posição correspondente a NR_TURNO nos arquivos
#define NR_TURNO_POS 5
// Posição correspondente a CD_CARGO nos arquivos
#define CD_CARGO_POS 13
// Posição correspondente a NR_PARTIDO nos arquivos
#define NR_PARTIDO_POS 27
// Posição correspondente a CD_GRAU... nos arquivos
#define CD_GRAU_POS 43
// Posição correspondente a CD_GENERO nos arquivos
#define CD_GENERO_POS 41
// Posição correspondente a CD_COR_RAÇA nos arquivos
#define CD_RACA_POS 47
// Posição que indica a idade de cada candidato nos arquivos
#define IDADE_POS 39
// Arquivo para salvar os cargos
#define ARQCARGOS "./Listas/cargos.txt"
// Arquivo para salvar os partidos
#define ARQPARTIDOS "./Listas/partidos.txt"
// Arquivo para salvar os graus de instrução
#define ARQGRAU "./Listas/grau_ins.txt"
// Arquivo para salvar os gêneros
#define ARQGENERO "./Listas/genero.txt"
// Arquivo para salvar as raças
#define ARQRACA "./Listas/raça.txt"

void analisar_arquivo(char const *ano, char const *sigla);

int main(int argc, char const *argv[]){
	if(argc!=ARGQUANT){ // Verifica se o usuário não digitou uma quantidade incorreta de argumentos
		printf("Sintaxe incorreta\nUse:\n$./%s [ano da eleição] [sigla]\n", EXEC_NOME);
	}else{
		analisar_arquivo(argv[1], argv[2]);
	}
	
	return 0;
}

void analisar_arquivo(char const *ano, char const *sigla){
	// Inicialmente, o programa pega as informações de ano e de sigla
	// e monta a localização referente ao arquivo desejado
	// Os arquivos todos seguem um mesmo modelo
	char padrao[] = PADRAO;
	char *nome_arq;
	nome_arq = (char*) calloc(strlen(padrao)+strlen(ano)+strlen(sigla)+6, sizeof(char));
	int x, y;
	// Copia padrão
	for(x=0; x<strlen(padrao); x++){
		nome_arq[x] = padrao[x];
	}
	// Copia ano
	for(y=0; y<strlen(ano); y++){
		nome_arq[x+y] = ano[y];
	}
	x=x+y;
	// Acrescenta '_'
	nome_arq[x] = '_';
	x++;
	// Copia sigla
	for(y=0; y<strlen(sigla); y++){
		nome_arq[x+y] = toupper(sigla[y]);
	}
	x=x+y;
	// Acrescenta .csv
	nome_arq[x] = '.';
	nome_arq[x+1] = 'c';
	nome_arq[x+2] = 's';
	nome_arq[x+3] = 'v';
	nome_arq[x+4] = '\0';

	printf("Arquivo: %s\n", nome_arq);

	// A variavel arq é usada para referenciar ao arquivo que o usuario deseja consultar
	FILE *arq;
	//variáveis que são importantes ao longo de todo o código
	int erro = 0; // indica se houve algum erro ou não(se não houve, valerá 0)
	int total1 = 0; // indica a quantidade total de candidatos no primeiro turno

	// Calcula quantidade de candidatos por turno
	if(erro==0){
		char c;
		int i, turno, turno1=0, turno2=0;

		arq = fopen(nome_arq, "r");
		if(arq==NULL){
			erro = -1;
		}else{
			while(fgetc(arq)!='\n');// Procura pelo final do cabeçalho

			while(fgetc(arq)!=EOF){ // Enquanto não chega o fim do arquivo
				// Essa parte corresponde à análise para um único candidato
				for(i=0; i<NR_TURNO_POS; i++){
					// Pula várias letras até chegar no ponto e virgula 
					// correspondente ao numero do turno
					while(fgetc(arq)!=';');
				}

				fgetc(arq); // Pula aspas
				turno = fgetc(arq)-'0';

				switch(turno){
					case 1:
						turno1++;
						break;
					case 2:
						turno2++;
						break;
					default:
						erro=1;
				}

				if(erro==1){
					break;
				}else{
					while(fgetc(arq)!='\n'); 
					// Pula letras até achar nova linha
					// que corresponde ao próximo candidato a ser procurado
				}
			}

			fclose(arq);
		}

		if(erro==0){
			printf("Candidatos por turno:\nTurno 1: %d\nTurno 2: %d\n", turno1, turno2);
			total1 = turno1;
		}
	}

	if(erro==0) printf("\nNO PRIMEIRO TURNO:\n");

	// Calcula a quantidade de candidatos por cargo
	if(erro==0){
		/* Essa parte do código funciona da seguinte maneira
			1 - sempre que um cargo novo é identificado, o seu código de cargo
				é adicionado ao vetor lista_cargos, na sua última posição. Essa
				última posição é salva no int quant_cargos, que corresponde a uma contagem
				de quantos cargos diferentes ja foram identificados
			2 - sempre que um cargo que ja foi identificado anteriormente é encontrado,
				o valor correspondente a esse cargo no vetor cargos é incrementado
			3 - o arquivo ARQCARGOS serve como uma lista dos nomes dos cargos para
				o momento em que for necessário exibi-los na tela
		*/

		FILE *arqcargos;
		arqcargos = fopen(ARQCARGOS, "w"); // Tenta criar o arquivo ARQCARGOS
		int *lista_cargos; // Lista dos códigos de cargos que já foram encontrados
		int *cargos; // Contagem de candidatos para cada cargo
		int quant_cargos=0; // Contagem de quantos cargos já foram identificados
		int i, j, ok=0; // Controle de loops
		char c; // char temporario
		int turno; // usada para verificar se é um candidato do primeiro turno

		if(arqcargos==NULL){
			erro = 2;
		}else{
			// inicializa vetores com uma posição
			// esses vetores mudam de tamanho conforme seja exigido pela situação
			lista_cargos = (int*) calloc(1, sizeof(int));
			cargos = (int*) calloc(1, sizeof(int));
			
			// Abre arquivo desejado pelo usuario
			arq = fopen(nome_arq, "r");
			if(arq==NULL){
				erro = -1;
			}else{
				rewind(arq);

				while(fgetc(arq)!='\n'); // Ignora o cabeçalho

				while(fgetc(arq)!=EOF){
					for(i=0; i<NR_TURNO_POS; i++){
						// Pula várias letras até chegar no ponto e virgula correspondente ao numero do turno
						while(fgetc(arq)!=';');
					}

					fgetc(arq); // Pula aspas
					turno = fgetc(arq)-'0';

					if(turno==2){ // Se for do segundo turno, ignora e pula para o proximo candidato
						while(fgetc(arq)!='\n'); // Procura pela nova linha(novo candidato)
						continue;
					}
					// Pula uma quantidade de ponto e virgulas
					// ate chegar a posição correspondete ao codigo de cargo
					for(i=0; i<CD_CARGO_POS - NR_TURNO_POS; i++){
						while(fgetc(arq)!=';');
					}
					fgetc(arq); // Pula aspas duplas
					// Agora, o ponteiro está encima da informação
					i=0;
					char temp[MAX]; // vetor temporario, para armazenar o valor lido do arquivo
					while((c=fgetc(arq))!='\"'){
						temp[i] = c;
						i++;
					}
					temp[i]='\0';
					// O que está salvo no vetor temp é o código de cargo
					// Deve ser tranformado para int
					int cd_cargo = atoi(temp);
					
					// Verifica se esse cargo já está na lista
					ok=0; // ok serve para fuga de loops e verificações
					for(i=0; i<quant_cargos; i++){
						if(lista_cargos[i]==cd_cargo){
							ok = 1;
							break;
						}
					}
					// se o cargo estiver na lista...
					if(ok==1){
						// Se o cógigo já está na lista, apenas é incrementada a posição correspondente
						// no vetor cargos
						cargos[i]++;
					}else{
						// Se o codigo de cargo nao está na lista, é necessário realizar algumas
						// operações, para adicioná-lo
						quant_cargos++; // Aumenta a contagem de cargos diferentes
						// Realoca vetores de modo a conseguir espaço para o novo cargo
						lista_cargos = (int*) realloc(lista_cargos, quant_cargos*sizeof(int));
						cargos = (int*) realloc(cargos, quant_cargos*sizeof(int));
						// zera as novas posições, para evitar que haja lixo armazenado nelas
						cargos[quant_cargos-1] = 0;
						lista_cargos[quant_cargos-1] = 0;
						// Incrementa a contagem na nova posição
						cargos[quant_cargos-1]++;
						// Salva o código desse novo cargo encontrado
						lista_cargos[quant_cargos-1] = cd_cargo;
						while(fgetc(arq)!=';'); // Procura próximo ponto e vírgula
						fgetc(arq); // ignora aspas duplas
						// lê qual é o nome do cargo e acrescenta-o ao arquivo ARQCARGOS
						// para posterior acesso e utilização
						while((c=fgetc(arq))!='\"'){
							fprintf(arqcargos, "%c", c);
						}
						fprintf(arqcargos, "\n");
					}

					while(fgetc(arq)!='\n'); // Procura pela nova linha(novo candidato)
				}

				fclose(arq);
			}

			if(erro==0){
				// fecha e reabre o arquivo, para usá-lo com leitura
				fclose(arqcargos);
				arqcargos = fopen(ARQCARGOS, "r");
				// Exibe na tela os resultados
				printf("\nCandidatos por cargo:\n");
				for(i=0; i<quant_cargos; i++){
					if(lista_cargos[i]==0){ // Se for #NULO#, não exibe na tela
						continue;
					}
					// Verifica, dentro do arquivo cargos.txt, qual é o nome do cargo
					// referenciado pelo código em questão
					rewind(arqcargos);
					for(j=0; j<i; j++){
						// Pula uma quantidade i de linhas dentro do arquivo cargos.txt
						// até chegar no cargo desejado
						while(fgetc(arqcargos)!='\n');
					}
					
					printf("Para ");
					while((c=fgetc(arqcargos))!='\n'){
						printf("%c", c);
					}
					printf("\t(cod.: %d), ha:\t%d candidatos\n", lista_cargos[i], cargos[i]);
				}
				fclose(arqcargos);
				free(lista_cargos);
				free(cargos);
			}
		}
	}

	//Calcula candidatos por partido
	if(erro==0){
		/* 	O funcionamento dessa parte do código é identico
			à parte dos cargos, só mudando as informações para partido
		*/

			FILE *arqpartidos;
			arqpartidos = fopen(ARQPARTIDOS, "w"); // Tenta criar o arquivo ARQPARTIDOS
			int *lista_partidos;
			int *partidos;
			int quant_partidos=0;
			int i, j, ok=0; // Controle de loops
			char c; // char temporario
			int turno; // utilizada para conferir se é candidato do primeiro turno

			if(arqpartidos==NULL){
				erro = 2;
			}else{
				// inicializa vetores com uma posição
				lista_partidos = (int*) calloc(1, sizeof(int));
				partidos = (int*) calloc(1, sizeof(int));
						
				arq = fopen(nome_arq, "r");
				if(arq==NULL){
					erro = -1;
				}else{
					rewind(arq);

					while(fgetc(arq)!='\n'); // Ignora o cabeçalho

					while(fgetc(arq)!=EOF){
						
						for(i=0; i<NR_TURNO_POS; i++){
							while(fgetc(arq)!=';'); // Pula várias letras até chegar no ponto e virgula correspondente ao numero do turno
						}

						fgetc(arq); // Pula aspas
						turno = fgetc(arq)-'0';
						
						if(turno==2){ // ignora, se for do segundo turno
							while(fgetc(arq)!='\n'); // Procura pela nova linha(novo candidato)
							continue;
						}

						// Pula uma quantidade de ponto e virgulas ate chegar a informação desejada
						for(i=0; i<(NR_PARTIDO_POS-NR_TURNO_POS); i++){
							while(fgetc(arq)!=';');
						}
						fgetc(arq); // Pula aspas duplas
						// Agora, o ponteiro está encima da informação
						i=0;
						char temp[MAX]; // vetor para armazenar temporariamente as informações
						while((c=fgetc(arq))!='\"'){
							temp[i] = c;
							i++;
						}
						temp[i]='\0';
						// O que está salvo no vetor temp é o código de partido
						// Deve ser tranformado para int
						int nr_partido = atoi(temp);
						// Verifica se esse partido já foi visto antes
						ok=0; // ok serve para fuga de loops e verificações
						for(i=0; i<quant_partidos; i++){
							if(lista_partidos[i]==nr_partido){
								ok = 1;
								break;
							}
						}
						// Se o numero de partido nao foi visto antes, é necessário realizar algumas
						// operações
						if(ok==1){
							partidos[i]++;
						}else{
							quant_partidos++;
							lista_partidos = (int*) realloc(lista_partidos, quant_partidos*sizeof(int));
							partidos = (int*) realloc(partidos, quant_partidos*sizeof(int));
							partidos[quant_partidos-1] = 0;
							lista_partidos[quant_partidos-1] = 0;
							partidos[quant_partidos-1]++;
							lista_partidos[quant_partidos-1] = nr_partido;
							while(fgetc(arq)!=';'); // Procura próximo ponto e vírgula
							fgetc(arq); // ignora aspas duplas
							// lê qual é o nome do partido e acrescenta-o ao arquivo ARQPARTIDOS
							while((c=fgetc(arq))!='\"'){
								fprintf(arqpartidos, "%c", c);
							}
							fprintf(arqpartidos, "\n");
						}

						while(fgetc(arq)!='\n'); // Procura pela nova linha
					}

					fclose(arq);
				}
				

				if(erro==0){
					// fecha e abre o arquivo novamente para leitura
					fclose(arqpartidos);
					arqpartidos = fopen(ARQPARTIDOS, "r");
					printf("\nCandidatos por partido:\n");
					for(i=0; i<quant_partidos; i++){
						if(lista_partidos[i]==0){ // caso em que há um #NULO#, se existir, deve ser desconsiderado
							continue;
						}
						// volta ao inicio do arquivo e pula linhas ate chegar a linha correspondente ao partido procurado
						rewind(arqpartidos);
						for(j=0; j<i; j++){
							// Pula uma quantidade i de linhas dentro do arquivo arqpartidos
							while(fgetc(arqpartidos)!='\n');
						}
						
						while((c=fgetc(arqpartidos))!='\n'){
							printf("%c", c);
						}
						printf("\t(cod.: %d):\t%d candidatos\n", lista_partidos[i], partidos[i]);
					}
					fclose(arqpartidos);
					free(lista_partidos);
					free(partidos);
				}
			}
	}

	// Calcula candidatos por grau de instrução
	// Segue o mesmo modelo do calculo para Partidos e cargos
	if(erro==0){
		FILE *arqgrau_ins;
		arqgrau_ins = fopen(ARQGRAU, "w"); // Tenta criar o arquivo ARQGRAU
		int *lista_grau_ins;
		int *grau_ins;
		int quant_grau_ins=0;
		int i, j, ok=0; // Controle de loops
		char c; // char temporario
		int turno;
		int soma=0; // guarda a soma para ser calculada a media

		if(arqgrau_ins==NULL){
			erro = 2;
		}else{
			// inicializa vetores com uma posição
			lista_grau_ins = (int*) calloc(1, sizeof(int));
			grau_ins = (int*) calloc(1, sizeof(int));

			arq = fopen(nome_arq, "r");
			if(arq==NULL){
				erro = -1;
			}else{
				rewind(arq);

				while(fgetc(arq)!='\n'); // Ignora o cabeçalho

				while(fgetc(arq)!=EOF){
					for(i=0; i<NR_TURNO_POS; i++){
						while(fgetc(arq)!=';'); // Pula várias letras até chegar no ponto e virgula correspondente ao numero do turno
					}

					fgetc(arq); // Pula aspas
					turno = fgetc(arq)-'0';
					
					if(turno==2){ // ignora, se for do segundo turno
						while(fgetc(arq)!='\n'); // Procura pela nova linha
						continue;
					}

					// Pula uma quantidade CD_GRAU_POS de ponto e virgulas
					for(i=0; i<CD_GRAU_POS-NR_TURNO_POS; i++){
						while(fgetc(arq)!=';');
					}
					fgetc(arq); // Pula aspas duplas
					// Agora, o ponteiro está encima da informação
					i=0;
					char temp[MAX];
					while((c=fgetc(arq))!='\"'){
						temp[i] = c;
						i++;
					}
					temp[i]='\0';
					// O que está salvo no vetor temp é o código de grau
					// Deve ser tranformado para int
					int cd_grau = atoi(temp);
					// Verifica se esse partido já foi visto antes
					ok=0; // ok serve para fuga de loops e verificações
					for(i=0; i<quant_grau_ins; i++){
						if(lista_grau_ins[i]==cd_grau){
							ok = 1;
							break;
						}
					}
					// Se o grau de instrução nao foi visto antes, é necessário realizar algumas
					// operações
					if(ok==1){
						grau_ins[i]++;
					}else{
						quant_grau_ins++;
						lista_grau_ins = (int*) realloc(lista_grau_ins, quant_grau_ins*sizeof(int));
						grau_ins = (int*) realloc(grau_ins, quant_grau_ins*sizeof(int));
						grau_ins[quant_grau_ins-1] = 0;
						lista_grau_ins[quant_grau_ins-1] = 0;
						grau_ins[quant_grau_ins-1]++;
						lista_grau_ins[quant_grau_ins-1] = cd_grau;
						while(fgetc(arq)!=';'); // Procura próximo ponto e vírgula
						fgetc(arq); // ignora aspas duplas
						// lê qual é o nome do partido e acrescenta-o ao arquivo ARQGRAU
						while((c=fgetc(arq))!='\"'){
							fprintf(arqgrau_ins, "%c", c);
						}
						fprintf(arqgrau_ins, "\n");
					}

					while(fgetc(arq)!='\n'); // Procura pela nova linha
				}

				fclose(arq);
			}
			

			if(erro==0){
				fclose(arqgrau_ins);
				arqgrau_ins = fopen(ARQGRAU, "r");
				printf("\nCandidatos por grau de instrução:\n");
				for(i=0; i<quant_grau_ins; i++){
					rewind(arqgrau_ins);
					for(j=0; j<i; j++){
						// Pula uma quantidade i de linhas dentro do arquivo arqgrau_ins
						while(fgetc(arqgrau_ins)!='\n');
					}
					
					while((c=fgetc(arqgrau_ins))!='\n'){
						printf("%c", c);
					}
					printf("\t(cod.: %d):\t%d candidatos\n", lista_grau_ins[i], grau_ins[i]);
					// soma, para calculo da media
					soma += lista_grau_ins[i]*grau_ins[i];
				}

				printf("Assumindo os codigos de grau de instrução,\nMedia do grau de instrução:%.3f\n", (float)soma/(float)total1);
				fclose(arqgrau_ins);
				free(lista_grau_ins);
				free(grau_ins);
			}
		}
	}

	// Calcula candidatos por genero
	// Segue modelo identico aos previamente utilizados
	if(erro==0){
		FILE *arqgenero;
		arqgenero = fopen(ARQGENERO, "w"); // Tenta criar o arquivo ARQGENERO
		int *lista_genero;
		int *genero;
		int quant_genero=0;
		int i, j, ok=0; // Controle de loops
		char c; // char temporario
		int turno;

		if(arqgenero==NULL){
			erro = 2;
		}else{
			// inicializa vetores com uma posição
			lista_genero = (int*) calloc(1, sizeof(int));
			genero = (int*) calloc(1, sizeof(int));

			arq = fopen(nome_arq, "r");
			if(arq==NULL){
				erro = -1;
			}else{
				rewind(arq);

				while(fgetc(arq)!='\n'); // Ignora o cabeçalho

				while(fgetc(arq)!=EOF){
					for(i=0; i<NR_TURNO_POS; i++){
						while(fgetc(arq)!=';'); // Pula várias letras até chegar no ponto e virgula correspondente ao numero do turno
					}

					fgetc(arq); // Pula aspas
					turno = fgetc(arq)-'0';
					
					if(turno==2){ // ignora, se for do segundo turno
						while(fgetc(arq)!='\n'); // Procura pela nova linha
						continue;
					}

					// Pula uma quantidade CD_GRAU_POS de ponto e virgulas a partir do começo das informações
					// sobre determinado candidato
					for(i=0; i<CD_GENERO_POS-NR_TURNO_POS; i++){
						while(fgetc(arq)!=';');
					}
					fgetc(arq); // Pula aspas duplas
					// Agora, o ponteiro está encima da informação
					i=0;
					char temp[MAX];
					while((c=fgetc(arq))!='\"'){
						temp[i] = c;
						i++;
					}
					temp[i]='\0';
					// O que está salvo no vetor temp é o código de genero
					// Deve ser tranformado para int
					int cd_grau = atoi(temp);
					// Verifica se esse partido já foi visto antes
					ok=0; // ok serve para fuga de loops e verificações
					for(i=0; i<quant_genero; i++){
						if(lista_genero[i]==cd_grau){
							ok = 1;
							break;
						}
					}
					// Se o genero nao foi visto antes, é necessário realizar algumas
					// operações
					if(ok==1){
						genero[i]++;
					}else{
						quant_genero++;
						lista_genero = (int*) realloc(lista_genero, quant_genero*sizeof(int));
						genero = (int*) realloc(genero, quant_genero*sizeof(int));
						genero[quant_genero-1] = 0;
						lista_genero[quant_genero-1] = 0;
						genero[quant_genero-1]++; // Acrescenta 1 a contagem
						lista_genero[quant_genero-1] = cd_grau;
						while(fgetc(arq)!=';'); // Procura próximo ponto e vírgula
						fgetc(arq); // ignora aspas duplas
						// lê qual é o nome do genero e acrescenta-o ao arquivo ARQGENERO
						while((c=fgetc(arq))!='\"'){
							fprintf(arqgenero, "%c", c);
						}
						fprintf(arqgenero, "\n");
					}

					while(fgetc(arq)!='\n'); // Procura pela nova linha
				}

				fclose(arq);
			}
			

			if(erro==0){
				// fecha o arquivo e abre novamente para leitura
				fclose(arqgenero);
				arqgenero = fopen(ARQGENERO, "r");
				// Exibe na tela os resultados
				printf("\nCandidatos por genero:\n");
				for(i=0; i<quant_genero; i++){
					rewind(arqgenero);
					for(j=0; j<i; j++){
						// Pula uma quantidade i de linhas dentro do arquivo arqgenero
						while(fgetc(arqgenero)!='\n');
					}
					
					while((c=fgetc(arqgenero))!='\n'){
						printf("%c", c);
					}
					printf("\t:\t%d candidatos\n", genero[i]);
				}
				fclose(arqgenero);
				free(lista_genero);
				free(genero);
			}
		}
	}

	//Calcula Candidatos por Raça
	// segue modelo identico ao previamente utilizado
	if(erro==0){
		FILE *arqraca;
		arqraca = fopen(ARQRACA, "w"); // Tenta criar o arquivo ARQRACA
		int *lista_raca;
		int *raca;
		int quant_raca=0;
		int i, j, ok=0; // Controle de loops
		char c; // char temporario
		int turno;

		if(arqraca==NULL){
			erro = 2;
		}else{
			// inicializa vetores com uma posicão
			lista_raca = (int*) calloc(1, sizeof(int));
			raca = (int*) calloc(1, sizeof(int));

			arq = fopen(nome_arq, "r");
			if(arq==NULL){
				erro = -1;
			}else{
				rewind(arq);

				while(fgetc(arq)!='\n'); // Ignora o cabecalho

				while(fgetc(arq)!=EOF){
					for(i=0; i<NR_TURNO_POS; i++){
						while(fgetc(arq)!=';'); // Pula várias letras até chegar no ponto e virgula correspondente ao numero do turno
					}

					fgetc(arq); // Pula aspas
					turno = fgetc(arq)-'0';
					
					if(turno==2){ // ignora, se for do segundo turno
						while(fgetc(arq)!='\n'); // Procura pela nova linha
						continue;
					}

					// Pula uma quantidade CD_RACA_POS de ponto e virgulas
					for(i=0; i<CD_RACA_POS-NR_TURNO_POS; i++){
						while(fgetc(arq)!=';');
					}
					fgetc(arq); // Pula aspas duplas
					// Agora, o ponteiro está encima da informacão
					i=0;
					char temp[MAX];
					while((c=fgetc(arq))!='\"'){
						temp[i] = c;
						i++;
					}
					temp[i]='\0';
					// O que está salvo no vetor temp é o código de raca
					// Deve ser tranformado para int
					int cd_raca = atoi(temp);
					// Verifica se esse partido já foi visto antes
					ok=0; // ok serve para fuga de loops e verificacões
					for(i=0; i<quant_raca; i++){
						if(lista_raca[i]==cd_raca){
							ok = 1;
							break;
						}
					}
					// Se o codigo de raça nao foi visto antes, é necessário realizar algumas
					// operacões
					if(ok==1){
						raca[i]++;
					}else{
						quant_raca++;
						lista_raca = (int*) realloc(lista_raca, quant_raca*sizeof(int));
						raca = (int*) realloc(raca, quant_raca*sizeof(int));
						raca[quant_raca-1] = 0;
						lista_raca[quant_raca-1] = 0;
						raca[quant_raca-1]++; // Acrescenta 1 a contagem
						lista_raca[quant_raca-1] = cd_raca;
						while(fgetc(arq)!=';'); // Procura próximo ponto e vírgula
						fgetc(arq); // ignora aspas duplas
						// lê qual é o nome do partido e acrescenta-o ao arquivo ARQRACA
						while((c=fgetc(arq))!='\"'){
							fprintf(arqraca, "%c", c);
						}
						fprintf(arqraca, "\n");
					}

					while(fgetc(arq)!='\n'); // Procura pela nova linha
				}

				fclose(arq);
			}
			

			if(erro==0){
				// fecha e abre novamente o arquivo para leitura
				fclose(arqraca);
				arqraca = fopen(ARQRACA, "r");
				printf("\nCandidatos por raça:\n");
				for(i=0; i<quant_raca; i++){
					rewind(arqraca);
					for(j=0; j<i; j++){
						// Pula uma quantidade i de linhas dentro do arquivo arqraca
						while(fgetc(arqraca)!='\n');
					}
					
					while((c=fgetc(arqraca))!='\n'){
						printf("%c", c);
					}
					printf("\t(cod.: %d):\t%d candidatos\n", lista_raca[i], raca[i]);
				}
				fclose(arqraca);
				free(lista_raca);
				free(raca);
			}
		}
	}

	// Calcula media das idades
	if(erro==0){
		int soma=0; // Guarda a soma das idades
		int i; // Controle de loops
		char c, temp[MAX]; // variavel temporaria para salvar a idade em forma de texto

		// Abre o arquivo desejado pelo usuario
		FILE *arq;
		arq = fopen(nome_arq, "r");
		int turno;
		if(arq==NULL){
			erro = -1;
		}else{
			while(fgetc(arq)!='\n'); // Pula o cabeçalho

			while(fgetc(arq)!=EOF){
				for(i=0; i<NR_TURNO_POS; i++){
					while(fgetc(arq)!=';'); // Pula ponto e virgulas ate chegar a informação desejada
				}
				fgetc(arq); // Pula aspas

				turno = fgetc(arq)-'0'; // Calcula o turno

				if(turno==2){ // Se for do segundo turno, ignora e vai para o proximo candidato
					while(fgetc(arq)!='\n'); 
					continue;
				}

				// Pula ponto e virgulas ate chegar a informação desejada
				for(i=0; i<IDADE_POS-NR_TURNO_POS; i++){
					while(fgetc(arq)!=';');
				}
				fgetc(arq); // Pula aspas
				i=0;
				// Salva idade em temp, que depois pe tranformada em int
				while((c=fgetc(arq))!='\"'){
					temp[i] = c;
					i++;
				}
				temp[i] = '\0';

				// Transforma temp(que, agora, contem a idade do candidato) em numero inteiro
				soma += atoi(temp);
				// procura pelo proximo candidato(proxima linha)
				while(fgetc(arq)!='\n');
			}

			printf("\nMedia de idade dos candidatos:%.3f\n", (float)soma/(float)total1);
			fclose(arq);
		}
	}

	// Mensagens de erro
	// A cada bloco, o programa verifica se não houve algum problema antes de executá-lo
	switch(erro){
		case 0:
			printf("\nDados do arquivo localizado em %s\nFinalizado\n", nome_arq);
			break;
		case -1:
			printf("ERRO: Ocorreu um falha ao tentar abrir os arquivos\nVerifique a existencia da pasta \"Arquivos\"\n");
			break;
		case 1:
			printf("ERRO: Ocorreu uma falha ao tentar calcular a quantidade de candidatos por turno\n");
			break;
		case 2:
			printf("ERRO: Ocorreu uma falha ao tentar criar os arquivos\nVerifique a existencia da pasta \"Listas\"\n");
			break;
		default:
			printf("ERRO: Houve um erro\n");
	}
	free(nome_arq); // Houve utilização de alocação dinâmica com nome_arq
}