/**
 * @file estado.c
 * @author Ezequiel Zimmel (ezequielzimmel@gmail.com)
 * @brief Se obtienen los datos de estado del servidor web. Se da formato a los
 * 		  datos para que sea compatible con los web browser y se despliega.
 * @version 0.1
 * @date 2020-02-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 256

void bootTime();
void load();
void memmory();
void fecha();
void kernel();
void uptime();
void formatSeconds(float secs, char *buf);
void CPU();
void getValue(char *file, char *value, char *key);
void cputimes();

/**
 * @brief Formatea la salida de informacion para que sea compatible
 * 	      con los web browser.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv)
{
	char text[30];

	printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1", 13, 10);
	printf("<META HTTP-EQUIV=REFRESH CONTENT=5>");
	printf("<TITLE>Estado del sistema</TITLE>\n");
	printf("<body style=background-color:rgba(10,50,0,0.5);>");
	printf("<font color=white face=Verdana>");
	printf("<h1 align=center>SISTEMA EMBEBIDO - TP3</h1>");
	printf("<H2 align=center><tt>ESTADO DEL SISTEMA \n</tt></H2></font>");
	printf("<hr>");
	CPU();
	printf("<div></div>");
	cputimes();
	printf("<div></div>");
	memmory();
	printf("<div></div>");
	fecha();
	printf("<div></div>");
	bootTime();
	printf("<div></div>");
	uptime();
	printf("<div></div>");
	load();
	printf("<div>&nbsp</div>");
	printf("<hr>");
	printf("<button type=button style=font-size:12pt;height:35px;width:100px onclick=javascript:window.close()>Salir</button></p>");
	printf("</body>");
	return 0;
}

/**
 * @brief Get the Value object
 * 
 * @param file 
 * @param value 
 * @param key 
 */
void getValue(char *file, char *value, char *key)
{
	char buffer[500];
	char *match = NULL;
	FILE *fd;
	fd = fopen(file, "r");

	while (feof(fd) == 0)
	{
		fgets(buffer, 500, fd);
		match = strstr(buffer, key);
		if (match != NULL)
			break;
	}

	fclose(fd);
	strcpy(value, match);
	return;
}

/**
 * @brief Obtiene el tipo de CPU y el consumo actual.
 * 
 */
void CPU()
{
	char modelo[100];
	char value[SIZE];
	char cpu[10];
	FILE *fp;
	char *command = "grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage}'";
	fp = popen(command, "r");
	fscanf(fp, "%s", cpu);

	getValue("/proc/cpuinfo", value, "model name");
	sscanf(value, "model name : %[^\n]c", modelo);
	printf("<p><b>CPU - Tipo:</b> %s - <b>Consumo:</b> %s% </p>\n", modelo, cpu);
	return;
}

/**
 * @brief Cambia el formato del tiempo de segundos a dia, hora, minutos, segundos.
 * 
 * @param secs tiempo medido en segundos 
 * @param buf variable que almacena el tiempo en formato d, h, m, s
 */
void formatSeconds(float secs, char *buf)
{
	unsigned int d, h, m;
	float s;

	d = (int)(secs / 86400);
	secs = secs - (long)(d * 86400);
	h = (int)(secs / 3600);
	secs = secs - (long)(h * 3600);
	m = (int)(secs / 60);
	secs = secs - (long)(m * 60);
	s = secs;
	if (d > 0)
		sprintf(buf, "%3ud %2u:%02u:%02.2f\n", d, h, m, secs);
	else
		sprintf(buf, "%2u:%02u:%02.2f\n", h, m, s);
	return;
}

/**
 * @brief Obtiene la fecha y hora actual del sistema.
 * 
 */
void fecha()
{
	/*
	char fecha[12];
	char value[SIZE+1];
	char hora[13];
	getValue("/proc/driver/rtc", value, "rtc_date");
	sscanf(value, "rtc_date : %s", fecha);
	getValue("/proc/driver/rtc", value, "rtc_time");
	sscanf(value, "rtc_time : %s", hora);
	printf("<p><b>Fecha:</b> %s </p><p><b>Hora:</b> %s</p> \n", fecha, hora);
	*/

	time_t t;
	struct tm *tm;
	char fecha[12];
	char hora[10];

	t = time(NULL);
	tm = localtime(&t);
	strftime(fecha, 12, "%d/%m/%Y", tm);
	strftime(hora, 10, "%H:%M:%S", tm);
	printf("<p><b>Fecha:</b> %s </p><p><b>Hora:</b> %s</p> \n", fecha, hora);
	return;
}

/**
 * @brief Obtiene el tiempo desde el inicio del sistemas en segundos.
 * 		  Se emplea la funcion formatSeconds() para obtener el tiempo en
 * 	      formato dia, hora, minutos, segundos.
 * 
 */
void uptime()
{
	FILE *fd;
	float time;
	char hms[18];
	fd = fopen("/proc/uptime", "r");
	fscanf(fd, "%f", &time);
	formatSeconds(time, hms);
	printf("<p><b>Uptime:</b> %s</p> \n", hms);
	fclose(fd);
	return;
}

/**
 * @brief Obtiene el uso del CPU por el sistema, el usurio y 
 * 		  en modo idle.
 * 
 */
void cputimes()
{
	FILE *fd;
	float user, sys, idle;
	char tUser[15], tSys[15];
	fd = fopen("/proc/stat", "r");
	fscanf(fd, "cpu %f %*f %f %f", &user, &sys, &idle);
	formatSeconds(user / 100, tUser);
	printf("<b>Tiempo de cpu (usuario):</b> %s\n", tUser);
	formatSeconds(sys / 100, tSys);
	printf("<p><b>Tiempo de cpu (sistema):</b> %s</p>\n", tSys);
	fclose(fd);
	return;
}

/**
 * @brief Obtiene el momento de encendido del sistema.
 * 		  Formato <dia><mes><N°dia><hora><año>
 * 
 */
void bootTime()
{
	char value[SIZE];
	time_t btime;
	unsigned int aux;
	char booted[40];

	getValue("/proc/stat", value, "btime");
	sscanf(value, "btime %u", &aux);
	btime = (time_t)aux;

	strftime(booted, sizeof(booted), "%c", localtime(&btime));
	printf("<p><b>Boot Time:</b> %s</p> \n", booted);
	return;
}

/**
 * @brief Obtiene la cantidad total de memoria del sistema y la cantidad
 * 	      de memoria libre.
 * 
 */
void memmory()
{
	char value[SIZE];
	unsigned int memTotal, memFree;

	getValue("/proc/meminfo", value, "MemTotal");
	sscanf(value, "MemTotal: %u", &memTotal);
	getValue("/proc/meminfo", value, "MemFree");
	sscanf(value, "MemFree: %u", &memFree);
	printf("<p><b>Mem total:</b> %u MB </p>\n", memTotal / 1024);
	printf("<p><b>Mem free:</b> %u MB </p>\n", memFree / 1024);
	return;
}

/**
 * @brief Obtiene el promedio de carga que el sistema está soportando.
 * 
 */
void load()
{
	FILE *fd;
	float load;
	fd = fopen("/proc/loadavg", "r");
	fscanf(fd, "%f", &load);
	printf("<p><b>Promedio de carga de un minuto:</b> %f </p>\n", load);
	fclose(fd);
	return;
}
