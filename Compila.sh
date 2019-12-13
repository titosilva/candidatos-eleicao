#!/bin/bash

echo "Criando arquivo executavel"
gcc -o consulta_eleicao ./Arquivo_C/principal.c

if [ -x consulta_eleicao ]
	then
		echo "Arquivo executavel criado!"
		echo "Para executa-lo utilize:"
		echo "./consulta_eleicao [ano] [sigla]"
		echo "Finalizado"
	else
		echo "Ocorreu uma falha ao tentar criar o executavel"
fi
