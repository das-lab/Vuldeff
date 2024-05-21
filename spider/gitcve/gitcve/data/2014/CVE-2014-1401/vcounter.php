<?php

	if (preg_match("/".basename (__FILE__)."/", $_SERVER['PHP_SELF'])) {
	    header("HTTP/1.1 404 Not Found");
	    exit;
	}
	ob_start();
	
	global $db,$url,$kecamatan_id,$all_visitors;
	include 'mod/statistik/counter.php';

	class usersOnline {

		var $timeout = 600;
		var $count = 0;
		var $error;
		var $i = 0;
		
		function usersOnline () {
			$this->timestamp = time();
			$this->ip = $this->ipCheck();
			$this->new_user();
			$this->delete_user();
			$this->count_users();
		}
		
		function ipCheck() {

			if (getenv('HTTP_CLIENT_IP')) {
				$ip = getenv('HTTP_CLIENT_IP');
			}
			elseif (getenv('HTTP_X_FORWARDED_FOR')) {
				$ip = getenv('HTTP_X_FORWARDED_FOR');
			}
			elseif (getenv('HTTP_X_FORWARDED')) {
				$ip = getenv('HTTP_X_FORWARDED');
			}
			elseif (getenv('HTTP_FORWARDED_FOR')) {
				$ip = getenv('HTTP_FORWARDED_FOR');
			}
			elseif (getenv('HTTP_FORWARDED')) {
				$ip = getenv('HTTP_FORWARDED');
			}
			else {
				$ip = $_SERVER['REMOTE_ADDR'];
			}
			return $ip;
		}
		
		function new_user() {
			global $db;
			$insert = $db->sql_query("INSERT INTO `mod_useronline` (`timestamp`, `ip`) VALUES ('mysql_real_escape_string($this->timestamp)', 'mysql_real_escape_string($this->ip)')");
			if (!$insert) {
				$this->error[$this->i] = "Unable to record new visitor\r\n";			
				$this->i ++;
			}
		}
		
		function delete_user() {
			global $db;
			$delete = $db->sql_query("DELETE FROM `mod_useronline` WHERE `timestamp` < ($this->timestamp - $this->timeout)");
			if (!$delete) {
				$this->error[$this->i] = "Unable to delete visitors";
				$this->i ++;
			}
		}
		
		function count_users() {
			global $db;
			if (count($this->error) == 0) {
				$count = $db->sql_numrows ( $db->sql_query("SELECT DISTINCT `ip` FROM `mod_useronline`"));
				return $count;
			}
		}

	}


	// Read our Parameters
	$today			=	'Today';
	$yesterday		=	'Yesterday';
	$x_month		=	'This month';
	$x_week			=	'This week';
	$all			=	'All days';
	
	$locktime		=	15;
	$initialvalue	=	1;
	$records		=	1;
	
	$s_today		=	1;
	$s_yesterday	=	1;
	$s_all			=	1;
	$s_week			=	1;
	$s_month		=	1;
	
	$s_digit		=	1;
	$disp_type		= 	'Mechanical';
	
	$widthtable		=	'100';
	$pretext		= 	'';
	$posttext		= 	'';
	
	// From minutes to seconds
	$locktime		=	$locktime * 60;
	

	// Now we are checking if the ip was logged in the database. Depending of the value in minutes in the locktime variable.
	$day			 =	date('d');
	$month			 =	date('n');
	$year			 =	date('Y');
	$daystart		 =	mktime(0,0,0,$month,$day,$year);
	$monthstart		 =  mktime(0,0,0,$month,1,$year);
	
	// weekstart
	$weekday		 =	date('w');
	$weekday--;
	if ($weekday < 0)	$weekday = 7;
	$weekday		 =	$weekday * 24*60*60;
	$weekstart		 =	$daystart - $weekday;

	$yesterdaystart	 =	$daystart - (24*60*60);
	$now			 =	time();
	$ip				 =	getIP();
	

	$r	= mysql_query("SELECT MAX( id ) AS total FROM `mod_visitcounter`");
	list($total) = mysql_fetch_array($r);

	if ($total !== NULL) {
		$all_visitors += $total;
	} else {
		$all_visitors = $initialvalue;
	}
	
	
	// Delete old records
	$temp = $all_visitors-$records;
	
	if ($temp>0){
		//$query		 =  mysql_query ("DELETE FROM `mod_visitcounter` WHERE `id`<'$temp'");
	}
	
	$item	=	mysql_fetch_assoc(mysql_query ("SELECT COUNT(*) AS `total` FROM `mod_visitcounter` WHERE `ip`='mysql_real_escape_string($ip)' AND (tm+'$locktime')>'$now'"));
	$items	=	$item['total'];
	
	if (empty($items))
	{
		mysql_query ("INSERT INTO `mod_visitcounter` (`id`, `tm`, `ip`) VALUES ('', '$now', 'mysql_real_escape_string($ip)')");
	}
	
	$n				 = 	$all_visitors;
	$div = 100000;
	while ($n > $div) {
		$div *= 10;
	}

	$query1			 =	mysql_fetch_assoc(mysql_query ("SELECT COUNT(*) AS `total_today` FROM `mod_visitcounter` WHERE `tm`>'$daystart'"));
	$today_visitors	 =	$query1['total_today'];
	
	$query2			 	 =	mysql_fetch_assoc(mysql_query ("SELECT COUNT(*) AS `total_yesterday` FROM `mod_visitcounter` WHERE `tm`>'$yesterdaystart' AND `tm`<'$daystart'"));
	$yesterday_visitors	 =	$query2['total_yesterday'];
		
	$query3			 =	mysql_fetch_assoc(mysql_query ("SELECT COUNT(*) AS `total_week` FROM `mod_visitcounter` WHERE `tm`>='$weekstart'"));
	$week_visitors	 =	$query3['total_week'];

	$query4			 =	mysql_fetch_assoc(mysql_query ("SELECT COUNT(*) AS `total_month` FROM `mod_visitcounter` WHERE `tm`>='$monthstart'"));
	$month_visitors	 =	$query4['total_month'];
	
	echo '<div>';
	
	
	
		
	echo '<div><table cellpadding="0" cellspacing="0" style="text-align: center; width: 100%;"><tbody align="center">';
	// Show today, yestoday, week, month, all statistic
	//echo 	spaceer("vtoday.gif", 'Visitors', $theCount);
	
	$visitors_online = new usersOnline();

	if (count($visitors_online->error) == 0) {
	
		if ($visitors_online->count_users() == 1) {
			//echo "There is " . $visitors_online->count_users() . " visitor online";
			echo spaceer("usersonline.gif", 'Visitor Online', $visitors_online->count_users());
		}
		else {
			//echo "There are " . $visitors_online->count_users() . " visitors online";
			echo spaceer("usersonline.gif", 'Visitors Online', $visitors_online->count_users());
		}
	}
	else {
		echo "<b>Users online class errors:</b><br /><ul>\r\n";
		for ($i = 0; $i < count($visitors_online->error); $i ++ ) {
			echo "<li>" . $visitors_online->error[$i] . "</li>\r\n";
		}
		echo "</ul>\r\n";
	
	}
	
	
	echo 	spaceer("vtoday.gif", 'Hits', $hits);
	if($s_today)		echo 	spaceer("vtoday.gif", $today, $today_visitors);
	if($s_yesterday)	echo 	spaceer("vyesterday.gif", $yesterday, $yesterday_visitors);
	if($s_week)			echo 	spaceer("vweek.gif", $x_week, $week_visitors);
	if($s_month)		echo 	spaceer("vmonth.gif", $x_month, $month_visitors);
	if($s_all)			echo 	spaceer("vall.gif", $all, $all_visitors);
	
	echo '</tbody></table></div>';
	echo '</div>';
	

	function spaceer($a1,$a2,$a3)
	{
		$ret = '<tr style="text-align:left;"><td><img src="mod/statistik/images/'.$a1.'" alt="mod_mod_visitcounter"/></td>';
		$ret .= '<td>'.$a2.'</td>';
		$ret .= '<td style="text-align:right;">'.$a3.'</td></tr>';
		return $ret;
	}

$out = ob_get_contents();
ob_end_clean();

?>