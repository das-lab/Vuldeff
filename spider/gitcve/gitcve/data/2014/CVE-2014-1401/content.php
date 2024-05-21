<?php

/**
 *	
 * 	March 02, 2012  , 09:32:55 PM   
 *	Iwan Susyanto, S.Si - admin@auracms.org      - 081 327 575 145
 */

if (!defined('INDEX')) {
    Header("Location: ../index.php");
    exit;
}


	if (isset ($_GET['pg'])) $pg = int_filter ($_GET['pg']); else $pg = 0;
	if (isset ($_GET['stg'])) $stg = int_filter ($_GET['stg']); else $stg = 0;
	if (isset ($_GET['offset'])) $offset = int_filter ($_GET['offset']); else $offset = 0;
	
	include 'mod/content/_config-rating.php';
	include 'mod/content/_drawrating.php';
	
	$translateKal_2 = array('januari' => '01',
						'februari' => '02',
						'maret' => '03',
						'april' => '04',
						'mei' => '05',
						'juni' => '06',
						'juli' => '07',
						'agustus' => '08',
						'september' => '09',
						'oktober' => '10',
						'nopember' => '11',
						'desember' => '12'
						);
	
	$tengah = '';
	
	if($_GET['action'] == ''){
		
		$tengah .= '
		<h2>Arsip Berita</h2>
		<div class="border" style="text-align:center;"><img src="mod/content/images/attachment.png" alt="Polling" /></div>';
		$query 	= $db->sql_query("SELECT * FROM `mod_content` WHERE `type`='news' ORDER BY `date` DESC");
		$jumlah = $db->sql_numrows($query);
		$limit 	= 15;
				
		$a 		= new paging_s ($limit,'berita','.html');

		if(isset($offset)){
			$no = $offset + 1;
		}else{
			$no = 1;
		}
				
		$b 		= $db->sql_query("SELECT * FROM `mod_content` WHERE `type`='news' ORDER BY `date` DESC LIMIT $offset,$limit");
		$ref 	= urlencode($_SERVER['REQUEST_URI']);
		$tengah .= '
		<div class="border rb">
		<table class="list">
			<thead>
				<tr class="head">
					<td style="text-align: center;width:30px;">No</td>
					<td class="left">Judul Berita</td>
				</tr>
			</thead>
			<tbody>';
			while($data = $db->sql_fetchrow($b)){
				$warna = empty ($warna) ? ' style="background-color:#f4f4f8;"' : '';

				$tengah .= '
				<tr'.$warna.'>
					<td style="text-align: center;">'.$no.'</td>
					<td class="left"><a href="article-'.$data['seftitle'].'.html" title="'.$data['title'].'">'.$data['title'].'</a></td>
				</tr>';
				$no++;					
			}
			$tengah .= '
			</tbody>
		</table>
		</div>';
		$tengah .= $a-> getPaging($jumlah, $pg, $stg);
	}
	
	
	if($_GET['action'] == 'search'){
		
		$tengah .= '
		<h2>Pencarian Berita</h2>
		<div class="border" style="text-align:center;"><img src="mod/content/images/banner_searching_data.gif" alt="Searching Data" /></div>';
		
		$search	= !isset($_GET['search']) ? cleanText($_POST['search']) : cleanText($_GET['search']);
		
		if(!$search){
			$tengah .= '<div class="error">Maaf Anda Belum Memasukkan Kata Pencarian</div>';
		}else{
		
			$query 	= $db->sql_query("SELECT * FROM `mod_content` WHERE `type`='news' AND (`title` LIKE '%$search%' OR `content` LIKE '%$search%' OR `caption` LIKE '%$search%' OR `tags` LIKE '%$search%') ORDER BY `date` DESC");
			$jumlah = $db->sql_numrows($query);
			$limit 	= 15;
			
			if($jumlah>0){
				$tengah .= '<div class="sukses">Ditemukan : <b>'.$jumlah.'</b> data dengan Kata Kunci : <i><b>'.$search.'</b></i></div>';
			}else{
				$tengah .= '<div class="error">Maaf Data yang Anda cari tidak di temukan</div>';
			}
					
			$a 		= new paging_s ($limit,'search-'.$search,'.html');
	
			if(isset($offset)){
				$no = $offset + 1;
			}else{
				$no = 1;
			}
					
			$b 		= $db->sql_query("SELECT * FROM `mod_content` WHERE `type`='news' AND (`title` LIKE '%$search%' OR `content` LIKE '%$search%' OR `caption` LIKE '%$search%' OR `tags` LIKE '%$search%') ORDER BY `date` DESC LIMIT $offset,$limit");
			$ref 	= urlencode($_SERVER['REQUEST_URI']);
			$tengah .= '
			<div class="border rb">
			<table class="list">
				<thead>
					<tr class="head">
						<td style="text-align: center;width:30px;">No</td>
						<td class="left">Judul Berita</td>
					</tr>
				</thead>
				<tbody>';
				while($data = $db->sql_fetchrow($b)){
					$warna = empty ($warna) ? ' style="background-color:#f4f4f8;"' : '';
	
					$tengah .= '
					<tr'.$warna.'>
						<td style="text-align: center;">'.$no.'</td>
						<td class="left"><a href="article-'.$data['seftitle'].'.html" title="'.$data['title'].'">'.$data['title'].'</a></td>
					</tr>';
					$no++;					
				}
				$tengah .= '
				</tbody>
			</table>
			</div>';
			$tengah .= $a-> getPaging($jumlah, $pg, $stg);
		}
	}
	
	if($_GET['action'] == 'view'){
		
		$script_include[] = '
		<script type="text/javascript" language="javascript" src="mod/content/js/behavior.js"></script>
		<script type="text/javascript" language="javascript" src="mod/content/js/rating.js"></script>
		';
		$style_include[] = '
		<style type="text/css">
		/*<![CDATA[*/
		@import url("mod/content/css/rating.css");
		/*]]>*/
		</style>';
	
		$seftitle = seo(text_filter(cleanText($_GET['seftitle'])));	
		$a = $db->sql_fetchrow($db->sql_query("SELECT `mod_content`.*,`mod_topic`.`topic` FROM `mod_content` LEFT JOIN `mod_topic` ON (`mod_topic`.`id` = `mod_content`.`topic_id`) WHERE `mod_content`.`seftitle`='$seftitle'"));
		$hits	= int_filter($a['hits']);
		$hits	= $hits+1 ;
		$id		= $a['id'];
		$topic_id	= $a['topic_id'];
		$update = $db->sql_query("UPDATE `mod_content` SET `hits`='$hits' WHERE `seftitle`='$seftitle'");
		$GLOBAL['title'] 		= cleanText($a['title']);
		$GLOBAL['description'] 	= limittxt(htmlentities(strip_tags($a['content'])),200);
		$GLOBAL['keywords'] 	= empty($a['tags']) ? implode(',',explode(' ',htmlentities(strip_tags($a['title'])))) : $a['tags'];
		$gambar = ($a['image'] == '') ? '' : '<div style="width:250px;float:left;background: #f1f8ed;border: 1px solid #d1eac3;padding: 5px;margin-right:5px; margin-top:10px;font-size:9px; "><img src="thumb.php?img='.$a['image'].'&amp;w=250" border="0" alt="'.$a['title'].'" /><i>'.$a['caption'].'</i></div>';
		$judul	= $a['title'];
		$tengah .= '
		<h2>'.$a['title'].'</h2>
		<div class="border breadcrumb"><a href="index.html" id="home">Home</a>   &nbsp;&raquo;&nbsp;   '.ucwords(strtolower($a['type'])).'    &nbsp;&raquo;&nbsp;   '.$a['title'].'<br />'.rating_bar($a['id'],'10').'</div>';
		$tengah .= <<<rate
		<script language="javascript" type="text/javascript">
	
		/* =============================================================== */
		var ratingAction = {
			'a.rater' : function(element){
				element.onclick = function(){
	
				var parameterString = this.href.replace(/.*\?(.*)/, "$1"); // onclick="sndReq('j=1&q=2&t=127.0.0.1&c=5');
				var parameterTokens = parameterString.split("&"); // onclick="sndReq('j=1,q=2,t=127.0.0.1,c=5');
				var parameterList = new Array();
	
				for (j = 0; j < parameterTokens.length; j++) {
					var parameterName = parameterTokens[j].replace(/(.*)=.*/, "$1"); // j
					var parameterValue = parameterTokens[j].replace(/.*=(.*)/, "$1"); // 1
					parameterList[parameterName] = parameterValue;
				}
				var theratingID = parameterList['q'];
				var theVote = parameterList['j'];
				var theuserIP = parameterList['t'];
				var theunits = parameterList['c'];
				sndReq(theVote,theratingID,theuserIP,theunits);return false;		
				}
			}
			
		};
		
		Behaviour.register(ratingAction);
		Behaviour.apply();
		</script>
rate;

		$tengah .= '
		<div class="border" style="text-align:justify;">'.$gambar.$a['content'].'</div>';


		$tengah .= '
		<div class="border">
		<!-- AddThis Button BEGIN -->
		<div class="addthis_toolbox addthis_default_style ">
		<a class="addthis_button_facebook_like" fb:like:layout="button_count"></a>
		<a class="addthis_button_tweet"></a>
		<a class="addthis_button_pinterest_pinit" pi:pinit:layout="horizontal"></a>
		<a class="addthis_counter addthis_pill_style"></a>
		</div>
		<script type="text/javascript" src="//s7.addthis.com/js/300/addthis_widget.js#pubid=ra-4f1a3ba476b73c86"></script>
		<!-- AddThis Button END -->
		</div>';
		
		
		if($a['type'] == 'news'){
			$tengah .= '
			<h2 class="rt">Berita Terkait : '.$a['topic'].'</h2>
			<div class="border rb listnews-home"><ul>';
			$b = $db->sql_query("SELECT * FROM `mod_content` WHERE `topic_id`='$topic_id' AND `seftitle`!='$seftitle' ORDER BY `date` DESC LIMIT 0,10");
			while($c = $db->sql_fetchrow($b)){			
				$tengah .= '<li><a href="article-'.$c['seftitle'].'.html" title="'.$c['title'].'">'.$c['title'].'</a></li>';
			}
			$tengah .= '</ul></div>';
		}
		
		
	
	}
	
	if($_GET['action'] == 'category'){
		
		$seftitle = seo(text_filter(cleanText($_GET['seftitle'])));	
		
		$query 	= $db->sql_fetchrow($db->sql_query("SELECT * FROM `mod_topic` WHERE `seftitle`='$seftitle'"));
		$topic 	= $query['id'];
		
		$tengah .= '<h2><a href="">Category : '.$query['topic'].'</a></h2>
		<div class="border" style="text-align:center;"><img src="images/icon-category.png" alt="Category Berita" /></div>			
		<div class="border rb" style="margin-top:-6px;">
			<div class="breadcrumb"><a href="#" id="home">Home</a>   &nbsp;&raquo;&nbsp;   '.$query['topic'].'</div>
		</div>';
		
		$a 		= $db->sql_query("SELECT * FROM `mod_content` WHERE `topic_id`='$topic' AND `published`='1' AND `type`='news' ORDER BY `date`");
		$jumlah = $db->sql_numrows($a);
		$limit 	= 15;
				
		$a 		= new paging_s ($limit,'category-'.$seftitle,'.html');
		
		$hasil 	= $db->sql_query("SELECT * FROM `mod_content` WHERE `topic_id`='$topic' AND `published`='1' AND `type`='news' ORDER BY `date` LIMIT $offset,$limit");
	
		while ($data = $db->sql_fetchrow($hasil)) {
	
			$images = ($data['image'] == '') ? '' : '<img src="images/thumb/'.$data['image'].'" border="0" alt="'.$data['title'].'" style="margin-right:5px; width:70px;height:50px;margin-top:4px;padding:3px; float:left;background: #f1f8ed;border: 1px solid #d1eac3;padding: 6px;" />';
		
			$tengah .='
			<h2><a href="article-'.$data['seftitle'].'.html">'.$data['title'].'</a></h2>
			<div class="border">
			<span class="align-justify">'.$images.limitTXT(strip_tags($data['content']),350).'</span>
			</div>		
						
			<div class="post-footer">					
			<a href="article-'.$data['seftitle'].'.html" title="'.$data['title'].'" class="readmore">Read more</a>
			<span class="hits">Hits ('.$data['hits'].')</span>
			<span class="date">'.datetimes($data['date'],false).'</span>	
			</div>';	
		}
		$tengah .= $a-> getPaging($jumlah, $pg, $stg);
	}
	
	
	
	if($_GET['action'] == 'arsip'){
		
		$seftitle = seo(text_filter(cleanText($_GET['seftitle'])));	
		
		list($bulan,$tahun) = explode('-',$seftitle);
		$bulannya = $translateKal_2[$bulan];
		
		if (!checkdate($bulannya,1,$tahun)) {
			$tengah .= '<div class="error" style="width:20%">'.$_GET['date'].'<br>'.checkdate($bulannya,1,$tahun).'<br>format date salah</div>';
		}
		
	
		$tengah .= '<h2><a href="">Arsip '.ucwords($bulan).' '.$tahun.'</a></h2>
		<div class="border" style="text-align:center;"><img src="images/icon-archive.png" alt="Arsip Berita" /></div>			
		<div class="border rb" style="margin-top:-6px;">
			<div class="breadcrumb"><a href="#" id="home">Home</a>   &nbsp;&raquo;&nbsp;   Arsip '.ucwords($bulan).' '.$tahun.'</div>
		</div>';
		
		$a 		= $db->sql_query("SELECT * FROM `mod_content` WHERE MONTH( `date` ) = '$bulannya' AND YEAR( `date` ) = '$tahun' AND `published` = '1' AND `type`='news' ORDER BY `date`");
		$jumlah = $db->sql_numrows($a);
		$limit 	= 15;
			
		$a 		= new paging_s ($limit,'arsip-'.$seftitle,'.html');
		
		$hasil 	= $db->sql_query("SELECT * FROM `mod_content` WHERE MONTH( `date` ) = '$bulannya' AND YEAR( `date` ) = '$tahun' AND `published` = '1' AND `type`='news' ORDER BY `date` LIMIT $offset,$limit");
		
		while ($data = $db->sql_fetchrow($hasil)) {
			
			$images = ($data['image'] == '') ? '' : '<img src="images/thumb/'.$data['image'].'" border="0" alt="'.$data['title'].'" style="margin-right:5px; width:70px;height:50px;margin-top:4px;padding:3px; float:left;background: #f1f8ed;border: 1px solid #d1eac3;padding: 6px;" />';
		
			$tengah .='
			<h2><a href="article-'.$data['seftitle'].'.html">'.$data['title'].'</a></h2>
			<div class="border">
			<span class="align-justify">'.$images.limitTXT(strip_tags($data['content']),350).'</span>
			</div>		
						
			<div class="post-footer">					
			<a href="article-'.$data['seftitle'].'.html" title="'.$data['title'].'" class="readmore">Read more</a>
			<span class="hits">Hits ('.$data['hits'].')</span>
			<span class="date">'.datetimes($data['date'],false).'</span>	
			</div>';	
		}
		$tengah .= $a-> getPaging($jumlah, $pg, $stg);
	}
	
	echo $tengah;