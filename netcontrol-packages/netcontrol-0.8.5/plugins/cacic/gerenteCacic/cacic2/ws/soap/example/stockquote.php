<html><body>
<?
 /* 
 Copyright 2000, 2001, 2002, 2003, 2004, 2005 Dataprev - Empresa de Tecnologia e Informa��es da Previd�ncia Social, Brasil

 Este arquivo � parte do programa CACIC - Configurador Autom�tico e Coletor de Informa��es Computacionais

 O CACIC � um software livre; voc� pode redistribui-lo e/ou modifica-lo dentro dos termos da Licen�a P�blica Geral GNU como 
 publicada pela Funda��o do Software Livre (FSF); na vers�o 2 da Licen�a, ou (na sua opni�o) qualquer vers�o.

 Este programa � distribuido na esperan�a que possa ser  util, mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUA��O a qualquer
 MERCADO ou APLICA��O EM PARTICULAR. Veja a Licen�a P�blica Geral GNU para maiores detalhes.

 Voc� deve ter recebido uma c�pia da Licen�a P�blica Geral GNU, sob o t�tulo "LICENCA.txt", junto com este programa, se n�o, escreva para a Funda��o do Software
 Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
//
// +----------------------------------------------------------------------+
// | PHP Version 4                                                        |
// +----------------------------------------------------------------------+
// | Copyright (c) 1997-2002 The PHP Group                                |
// +----------------------------------------------------------------------+
// | This source file is subject to version 2.02 of the PHP license,      |
// | that is bundled with this package in the file LICENSE, and is        |
// | available at through the world-wide-web at                           |
// | http://www.php.net/license/2_02.txt.                                 |
// | If you did not receive a copy of the PHP license and are unable to   |
// | obtain it through the world-wide-web, please send a note to          |
// | license@php.net so we can mail you a copy immediately.               |
// +----------------------------------------------------------------------+
// | Authors: Shane Caraveo <Shane@Caraveo.com>   Port to PEAR and more   |
// | Authors: Dietrich Ayala <dietrich@ganx4.com> Original Author         |
// +----------------------------------------------------------------------+
//
// $Id: stockquote.php,v 1.2 2002/03/19 08:25:25 shane Exp $
//
// include soap client class
include("SOAP/Client.php");
//include("../Client.php");

print "<br>\n<strong>wsdl:</strong>";
$soapclient = new SOAP_Client("http://services.xmethods.net/SOAP/urn:xmethods-delayed-quotes.wsdl","wsdl");
print $soapclient;
print_r($soapclient->call("getQuote",array("symbol"=>"ibm")));
print "\n\n";

if (extension_loaded('overload')) {
	print "\n<br><strong>overloaded:</strong>";
	$ret = $soapclient->getQuote("ibm");
	print_r($ret);
	print "\n\n";
}
unset($soapclient);

print "\n<br><strong>non wsdl:</strong>";
$soapclient = new SOAP_Client("http://services.xmethods.net:80/soap");
$ret = $soapclient->call("getQuote",array("symbol"=>"ibm","urn:xmethods-delayed-quotes","urn:xmethods-delayed-quotes#getQuote"));
print_r($ret);
print "\n\n";
unset($soapclient);

?>
</html></body>
