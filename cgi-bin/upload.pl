#!/usr/bin/perl -wl
#`
/**
* Instala el módulo pasado como argumento desde modulos.cgi
* Realiza la validacion de las credenciales para realizar la tarea.
* Despues de realizar la instalación, vuelve a ejecutar modulo.cgi para listar
* los módulos actualmente instalados.
*/

use strict;
use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;
use LWP::Simple;
#use IPC::System::Simple qw(system run);

$CGI::POST_MAX = 8192;
my $safe_filename_characters = "a-zA-Z0-9_.-";
my $upload_dir = "/var/www/html/TP3/upload";

my $query = new CGI();
my $filename = $query->param("modulo");
my $password = $query->param("pass");

if ( !$filename || !($password eq "apache2") )
{
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
		<dl><font size=6 color=white>Problema en la carga del modulo.</font><br>
		<dt><br><font size=5> Verifique: </font></dt><br> 
		<dd><font size=4>* Tamanio del modulo menor a 8192 Bytes</font></dd><br>
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
exit;
}

my ( $name, $path, $extension ) = fileparse ( $filename, '..*' );
$filename = $name . $extension;
$filename =~ tr/ /_/;
$filename =~ s/[^$safe_filename_characters]//g;

if ( $filename =~ /^([$safe_filename_characters]+)$/ )
{
	$filename = $1;
}
else
{
	die "El archivo contiene caracteres invalidos";
}

my $upload_filehandle = $query->upload("modulo");

open ( UPLOADFILE, ">$upload_dir/$filename" ) or die "$!";
binmode (UPLOADFILE);
while ( <$upload_filehandle> )
{
	chomp();
	print UPLOADFILE;
}
close UPLOADFILE;

my @campos = split /\./,$filename;
`sudo insmod $upload_dir/./$campos[0].ko &> /dev/null`;	
exec("/var/www/html/TP3/cgi-bin/modulos.cgi");



