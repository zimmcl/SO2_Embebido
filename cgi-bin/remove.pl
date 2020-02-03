#!/usr/bin/perl -wl
#`

use strict;
use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;
use LWP::Simple;
#use IPC::System::Simple qw(system run);

my $query = new CGI;
my $module = $query->param("modulo");
my $password = $query->param("pass");
my $directory = '/proc/modules';
my $value = '0';

if($module eq "") {
	$module = "empty_space";
}

open(my $file, $directory) or die "No se puedo abrir '$directory' $!";

while (my $row = <$file>) {
  chomp $row;
  if (index($row, $module) != -1) {
   $value++;
	}
}

if($value > 0 && ($password eq "apache2")){
`sudo rmmod $module`;
exec("/var/www/html/TP3/cgi-bin/modulos.cgi");	
}
else {
#print $query->header ( );
print "Content-Type: text/html \n\n";
print <<INC_HTML;
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Carga de Modulo</title>
</head>
<body style="background-color:rgba(10,50,0,0.5);">
		<font color="white" face="Verdana">
        <h1 align="center">SISTEMA EMBEBIDO - TP3</h1>
        <H2 align="center"><tt>GESTION DE MODULOS \n</tt></H2></font>
		<hr> 
		<dl><font size=6 color="white">Problema en la descarga del modulo.</font><br> 
		<dt><br><font size=5>Verifique:</font></dt><br> 
		<dd><font size=4>* Nombre del modulo a descargar. Se ingreso $module</font></dd><br>
		<dd><font size=4>* Ingresar correctamente la contrasenia del servidor.</font></dd></dl><br><hr>

		<form action="" method="post" name="formulario" id="formulario">
			<input type="submit" name="retornar" value="Retornar" style=font-size:12pt;height:35px;width:100px;margin-left:15px
				onclick=this.form.action="/TP3/cgi-bin/modulos.cgi">	 
	  		<input type="submit" name="salir" value="Salir" style=font-size:12pt;height:35px;width:100px;margin-left:15px
				onclick=this.form.action="javascript:self.close()">  
	  	</form>
</body>
</html>
INC_HTML
}


