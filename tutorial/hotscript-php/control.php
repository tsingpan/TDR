<?php
	$d = array();
	$dd = array();
	$ddd = array();
	$dddd = array();

	$dddd['data'] = '4d';

	$ddd['data'] = '3d';

	$dd['data'] = '2d';

	$d['data'] = '1d';


	$ddd['d'] = $dddd;
	$dd['d'] = $ddd;
	$d['d'] = $dd;

	$d['hi'] = 'hello!';
	$d['arr'] = array(1,2,3,4,5,6);

	if(hs_last_error() == 0)
	{
		echo hs_execute_array($d, 'template.hs');
	}
	else
	{
		echo hs_last_error_msg();
	}
?>

