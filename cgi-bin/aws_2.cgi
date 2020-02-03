#!/usr/bin/perl -w

use strict;
use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;
use LWP::Simple;
use feature qw(say); sub say {print @_, "\n"};
#use IPC::System::Simple qw ( system run );

#print $query->header ( );

my $q = new CGI;

if ($q->param()) {
	show_html_1();
	find();
	show_html_2();
} else {
	show_html_1();
	show_html_2();
}

sub show_html_1
{
print "Content-Type: text/html \n\n";
print <<INC_HTML;
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Carga de Modulo</title>
</head>
	<body style="background-color:rgba(10,50,0,0.5);">
		<font color="white" face="Verdana">
        <h1 align="center">SISTEMA EMBEBIDO - TP3</h1>
        <H2 align="center"><tt>Recursos AWS \n</tt></H2></font>
		<hr> 
		<dl><font size=5 color=white>Elija los parametros para realizar la busqueda...</font><br><br>
		<form method="post" action="aws_2.cgi" enctype="multipart/form-data">
		<select name="Producto" style=font-size:12pt;height:35px;width:110px;margin-left:15px>
			<option selected value="0"> Producto </option>
				<option value="L1b-RadM">L1b-RadM </option>
				<option value="L1b-RadC">L1b-RadC </option>
				<option value="L1b-RadF">L1b-RadF </option>
				<option value="L2-CMIPM">L2-CMIPM </option>
				<option value="L2-CMIPC">L2-CMIPC </option>
				<option value="L2-CMIPF">L2-CMIPF </option>
				<option value="L2-MCMIPM">L2-MCMIPM </option>
				<option value="L2-MCMIPC">L2-MCMIPC </option>
				<option value="L2-MCMIPF">L2-MCMIPF </option>
		</select>
		<select name="Canal" style=font-size:12pt;height:35px;width:100px;margin-left:10px>
			<option selected value="0"> Canal </option>
				<option value="C01">1 </option>
				<option value="C02">2 </option>
				<option value="C03">3 </option>
				<option value="C04">4 </option>
				<option value="C05">5 </option>
				<option value="C06">6 </option>
				<option value="C07">7 </option>
				<option value="C08">8 </option>
				<option value="C09">9 </option>
				<option value="C10">10 </option>
				<option value="C11">11 </option>
				<option value="C12">12 </option>
				<option value="C13">13 </option>
				<option value="C14">14 </option>
				<option value="C15">15 </option>
				<option value="C16">16 </option>
		</select>
		<input type="text" name="Fecha" value="YYYY/DDD" style=font-size:12pt;height:35px;width:120px;margin-left:10px>
		<input type="submit" name="buscar" value="Buscar" style=font-size:12pt;height:35px;width:100px;margin-left:25px>
		</form>
		
INC_HTML
}

#$myfile = system("aws s3 ls landsat-pds --no-sign-request");
#`
 
sub find()
{

my $Prod = $q->param("Producto");
my $Ch = $q->param("Canal");
my $Date = $q->param("Fecha");

#my $request = `aws s3 --recursive ls s3://noaa-goes16/ABI-$Prod/$Date/ --no-sign-request | grep '$Ch'`;
#$request =~ s/$/<br>/mg;
#print $q->p("$request");
	foreach(`aws s3 --human-readable --recursive ls s3://noaa-goes16/ABI-$Prod/$Date/ --no-sign-request | grep -E '$Ch'`)
	{
		print "<br>";
		print "$_ ";
		print "<br>";
	}	

}

sub show_html_2
{
print <<INC_HTML_2;			
		<br><hr>
		<form action="" method="post" name="formulario" id="formulario">	 
	  		<input type="submit" name="salir" value="Salir" style=font-size:12pt;height:35px;width:100px;margin-left:15px
			 	onclick=this.form.action="javascript:self.close()">  
	  </form>
	</body>
</html>
INC_HTML_2
}
