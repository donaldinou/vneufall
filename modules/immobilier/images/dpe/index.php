<?php	
	$q = $_GET["q"];
	$w = $_GET["w"];
	$h = $_GET["h"];
	
	if(!$q) {
		header("HTTP/1.0 404 Not Found");
		exit;
	}
	
	if(!preg_match("/^[a-zA-Z]$/", $q))
		$q = dpe_note((float)$q);
	
	$file = sprintf("%s.gif", strtolower($q));
	if(!file_exists($file)) {
		header("HTTP/1.0 404 Not Found");
		exit;
	}
	
	$x = @getimagesize($file);
	$sw = $x[0];
	$sh = $x[1];
	
	if(!$w) $w = $sw;
	if(!$h) $h = $w;
	if($w > 500 || $h > 500) {
		header("HTTP/1.0 404 Not Found");
		exit;
	}

	header("Content-type: image/gif");
	if($w <> $sw) {
		$im = @ImageCreateFromGIF($file);
		$thumb = @ImageCreateTrueColor ($w, $h);
		@ImageCopyResampled ($thumb, $im, 0, 0, 0, 0, $w, $h, $sw, $sh);
		@ImageGIF ($thumb);
	} else {
		readfile($file);
	}
	print "GENERATED BY ACREAT.COM";
	exit;
	

/*--
* D�termine un lettre � partir d'une valeur
*/
function dpe_note($value) {
	if($value <= 50) 
		return "a";
	elseif($value > 50 && $value <= 90) 
		return "b";
	elseif($value > 90 && $value <= 150) 
		return "c";
	elseif($value > 150 && $value <= 230) 
		return "d";
	elseif($value > 230 && $value <= 330) 
		return "e";
	elseif($value > 330 && $value <= 450) 
		return "f";
	return "g";
}