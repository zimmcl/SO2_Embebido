all: estado.cgi modulos.cgi aws.cgi

aws.cgi: aws.c
	gcc -o aws.cgi aws.c

estado.cgi: estado.c
	gcc -o estado.cgi estado.c

modulos.cgi: modulos.c
	gcc -o modulos.cgi modulos.c

clean:
	rm  estado.cgi modulos.cgi aws.cgi
