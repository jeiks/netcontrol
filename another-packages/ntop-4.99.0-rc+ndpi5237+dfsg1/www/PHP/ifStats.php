<HTML>
<HEAD>
<LINK REL=stylesheet HREF=http://<? echo $host.":".$port; ?>/style.css type="text/css">
</HEAD>
<BODY BGCOLOR=#FFFFFF>

<?php
    $fp = fsockopen ($host, $port);

if (!$fp) {
    echo "$errstr ($errno)<br>\n";
} else {
    $outStr = "";
    fputs ($fp, "GET /dumpTrafficData.html?language=php HTTP/1.0\r\n\r\n");
    while (!feof($fp)) {
	$out = fgets($fp,128);
	if($out == "\n")
	    $begin = 1;
	else if($begin == 1)
	    $outStr .= $out;
    }
    fclose ($fp);
//echo "<pre>$outStr</pre>";
    eval($outStr);
}

echo "<center>\n<table border>\n";
echo "<tr><th BGCOLOR=white>Interface</th><th BGCOLOR=white>Values</th></tr>\n";

while (list ($key, $val) = each($ntopHash)) {

	echo "<tr><th align=center BGCOLOR=white>$key</th>\n";
	echo "<td><table border>\n";
	while (list ($key_1, $val_1) = each ($val))
	    if(gettype($val_1) == "array") {
		echo "<tr><th align=left>$key_1</th><td><table border>\n";
		while (list ($key_2, $val_2) = each ($val_1)) {
		    echo "<tr><th align=left>$key_2</th><td align=right>&nbsp;$val_2</td></tr>\n";
		}
		echo "</table></td></tr>\n";
	    } else if($val_1 != "0") 
		echo "<tr><th align=left>$key_1</th><td align=right>&nbsp;$val_1</td></tr>\n";

	echo "</table></td></tr>\n";
}

echo "</table>\n";

// echo "<pre>$outStr<pre>";
?>


</center>
</body>
</html>
