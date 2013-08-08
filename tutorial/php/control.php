<?php
	$d = array();
	$dd = array();
	$ddd = array();
	$dddd = array();

	$dddd['data'] = '4d';

	$ddd['data'] = '3d';

	$dd['data'] = '2d';

	$d['data'] = '1d';


	$dd['d'] = $ddd;
	$d['d'] = $dd;

	$d['hi'] = 'hello!';
	$d['arr'] = array(1,2,3,4,5,6);

	echo hs_execute_array($d, 'd:/test.hs');
?>

