/**
 * @file aws.c
 * @author Ezequiel Zimmel (ezequielzimmel@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-02-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <asm/unistd.h>

static void print_usage(const char *progname)
{
	fprintf(stderr, "Usage: %s\n", progname);
	exit(1);
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
	printf("Content-Type: text/html\n\n");
	//printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
	printf("<TITLE>TP3 - Recursos AWS</TITLE>\n");
	printf("<body style=background-color:rgba(10,50,0,0.5);>");
	printf("<font color=white face=Verdana>");
	printf("<h1 align=center>SISTEMA EMBEBIDO - TP3</h1>");
	printf("<H2 align=center><tt>Recurs \n</tt></H2></font>");
	printf("<hr>");
	printf("<table><tr valign=top>");
	printf("<td align=center><font size=5 color=white> Instalar </font></td>");
	printf("<td width=15></td>");
	printf("<td align=center><font size=5 color=white></font></td>");
	printf("<td width=15></td>");
	printf("<td align=center><font size=5 color=white> Desinstalar </font></td></tr>");
	printf("<tr valign=top><td><form name=modulo method=post action=/tp3/cgi-bin/upload.pl enctype=multipart/form-data>");
	printf("<p>Modulo a cargar: <input type=file name=modulo accept=.c style=font-size:12pt;height:25px;width:175px/></p> ");
	printf("<p>Password Servidor: <input type=password name=pass style=font-size:12pt;height:25px;width:176px/>");
	printf("<input type=submit name= Submit value=Instalar style=font-size:12pt;height:35px;width:100px;margin-left:15px ></p>");
	printf("</form>");
	printf("<td width=15></td>");
	printf("<td bgcolor=#FFFFFF width=2></td>");
	printf("<td width=15></td>");
	printf("<td><form name=borrar method=post action=/tp3/cgi-bin/remove.pl enctype=multipart/form-data>");
	printf("<p>Modulo a desinstalar: <input type=text name=modulo style=font-size:12pt;height:25px;width:181px /></p>");
	printf("<p>Password Servidor: <input type=password name=pass style=font-size:12pt;height:25px;width:176px/>");
	printf("<input type=submit name= Submit value=Desinstalar style=font-size:12pt;height:35px;width:100px;margin-left:15px ></p>");
	printf("</form></td></tr></table>");
	printf("<hr>");

	printf("<div class=divInfo>");
	printf("<input type=button style=font-size:12pt;height:35px;width:175px value='Mostrar Informacion' onclick=\"if (this.parentNode.nextSibling.childNodes[0].style.display != '') { this.parentNode.nextSibling.childNodes[0].style.display = ''; this.value = 'Ocultar'; } else { this.parentNode.nextSibling.childNodes[0].style.display = 'none'; this.value = 'Mostrar Informacion'; }\" /></div>");
	printf("<div><div class='spoiler' style='display: none;'>");
	printf("<font size='5' color='white' >El servidor acepta archivos con extension '.ko' y compilado con LKV 4.4.0-77-generic</font></div></div>");
	printf("<br><button type=button style=font-size:12pt;height:35px;width:100px onclick=javascript:window.close()>Salir</button>");

	char line[4096];
	FILE *file;

	if (argc != 1)
		print_usage("lsmod");

	file = fopen("/proc/modules", "r");
	if (!file)
	{
		perror("Opening /proc/modules");
		exit(1);
	}
	printf("<hr>");
	printf("<PRE><font size=4 color=white>");
	printf("Module %25sSize %3sUsed by\n", " ", " ");
	printf("</font></PRE>");
	while (fgets(line, sizeof(line), file))
	{
		char *tok;

		tok = strtok(line, " \t");
		printf("<PRE><font size=4>");
		printf("%-23s", tok);
		tok = strtok(NULL, " \t\n");
		printf(" %12s", tok);
		tok = strtok(NULL, " \t\n");
		/* Null if no module unloading support. */
		if (tok)
		{
			printf("  %5s", tok);
			tok = strtok(NULL, "\n");
			if (!tok)
				tok = "";
			/* New-style has commas, or -.  If so,
			   truncate (other fields might follow). */
			else if (strchr(tok, ','))
			{
				tok = strtok(tok, "\t ");
				/* Strip trailing comma. */
				if (tok[strlen(tok) - 1] == ',')
					tok[strlen(tok) - 1] = '\0';
			}
			else if (tok[0] == '-' && (tok[1] == '\0' || isspace(tok[1])))
				tok = "";
			printf("    %s", tok);
			printf("</font></PRE>");
		}
		printf("\n");
	}
	printf("<hr>");
	fclose(file);
	exit(0);
}
