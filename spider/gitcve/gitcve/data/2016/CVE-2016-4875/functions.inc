<?php
/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | assist plugin 
// +---------------------------------------------------------------------------+
// $Id: plugins/assist/functions.inc
//2009/12/04 tsuchitani AT ivywe DOT co DOT jp http://www.ivywe.co.jp/
//last update 20120409


if (strpos ($_SERVER['PHP_SELF'], 'functions.inc') !== false) {
    die ('This file can not be used on its own.');
}

if (file_exists ($_CONF['path'] . 'plugins/assist/site_disabled_on.inc')) {
    include ($_CONF['path'] . 'plugins/assist/site_disabled_on.inc');
}

$pi_name         = 'assist';

//言語ファイル　
//指定の言語を使用、なければ英語.英語がなければ日本語
$langfile = $_CONF['path'] . 'plugins/assist/language/'
          . $_CONF['language'] . '.php';

if (file_exists ($langfile)) {
    include_once ($langfile);
} else {
	$w=$_CONF['path'] . "plugins/{$pi_name}/language/english_utf-8.php";
	if (file_exists($w)){
		include_once($w);
	}else{
		include_once ($_CONF['path'] . "plugins/{$pi_name}/language/japanese_utf-8.php");
	}
}

//custom language があれば、それを上書使用します。
$langfile_custom = $_CONF['path'] . 'plugins/assist/custom/'
          . $_CONF['language'] . '.php';

if (file_exists ($langfile_custom)) {
    include_once ($langfile_custom);
}

//設定ファイル
include_once ($_CONF['path'] . 'plugins/assist/config.php');

//自動タグ関連の関数
include_once ($_CONF['path'] . "plugins/{$pi_name}/functions_autotag.inc");


//ニュースレター予約チェック　20111219
require_once ($_CONF['path'] . "plugins/assist/functions_scheduled.inc");
$reserv = assist_scheduled_reserv_check();
if ($reserv !== "" && time() >= $reserv) {
	assist_scheduled_reserv_sendmail($_CONF['path_data']."assist_reserv_".$reserv);
}


// +---------------------------------------------------------------------------+
// | Geeklog Plugin API Implementation                                         |
// +---------------------------------------------------------------------------+

function plugin_chkVersion_assist ()
// +---------------------------------------------------------------------------+
// | 現在インストールされているバージョン取得                                  |
// +---------------------------------------------------------------------------+
{
    global $_ASSIST_CONF;

    return $_ASSIST_CONF['version'];
}

function plugin_upgrade_assist ()
// +---------------------------------------------------------------------------+
// | 機能  プラグイン更新
// +---------------------------------------------------------------------------+
{
	global $_TABLES;
	global $_CONF;
	global $_ASSIST_CONF;
	global $config;
	
	$pi_name    = 'assist';

    // the plugin needs these functions so complain when they doesn't exist
    if (!function_exists ('COM_truncate') ||
        !function_exists ('MBYTE_strpos')) {
        return 3002;
	}
	
	$installed_version = DB_getItem($_TABLES['plugins'], 'pi_version',
                                    "pi_name = '{$pi_name}'");
	$newversion=$_ASSIST_CONF['version'];
	
		//update the configuration
		require_once ($_CONF['path'] . 'plugins/assist/lib/lib_configuration.php');

		$rt=LIB_Backupconfig($pi_name,"update");
		$rt=LIB_Deleteconfig($pi_name,$config);
		$rt=LIB_Initializeconfig($pi_name);
		$rt=LIB_Restoreconfig($pi_name,$config,"update");
	
	//update the version numbers
	$sql="UPDATE {$_TABLES['plugins']} SET";
	$sql.=" pi_version = '{$newversion}' ";
	$sql.=" ,pi_gl_version = '" . VERSION . "'";
	$sql.="	WHERE pi_name = '{$pi_name}'";
    DB_query ($sql);

    return true;
}


function plugin_autouninstall_assist ()
// +---------------------------------------------------------------------------+
// | 機能  プラグインアンインストール                                          |
// +---------------------------------------------------------------------------+
{
    // (1) 削除するテーブル
    $out['tables'][]='ASSIST_def_xml';

    // (2) 削除するグループ名
    $out['groups'][]='Assist Admin';
    // (3) 削除するfeatures
    $out['features'][]='assist.admin';
    // (4) 削除するphp_blocks
    $out['php_blocks']=array();
    // (5) 削除する vars */
    $out['vars']=array();

    //変数削除
    global $_TABLES;
    $sql="DELETE  FROM {$_TABLES['vars']} ";
    $sql.=" WHERE name LIKE 'assist%'";
    DB_query($sql);
	
	//ニュースレター予約ファイル削除
	$dummy=assist_reserv_delete();
	
    return $out;

}

function assist_reserv_delete ()
// +---------------------------------------------------------------------------+
// | ニュースレター予約ファイル削除
// +---------------------------------------------------------------------------+
{
    global $_CONF;

    $reserv='';
    $files = glob("{$_CONF['path_data']}assist_reserv_*",  GLOB_NOCHECK);
    if (is_array($files)) {
        if (count($files) > 0 && $files[0] != "{$_CONF['path_data']}assist_reserv_*") {
            foreach ($files as $file) {
				unlink ($file);
		        COM_errorLog("Delete file {$file}");
			}
        }
    }
    return ;
}

function plugin_ismoderator_assist()
/**
* ☆★現在のユーザは権利があるのをチェック
* 使用せず
*/
{
    //return SEC_hasRights ('assist.moderate');
    //return SEC_hasRights ('user.moderate');
    return true;
}

function plugin_geticon_assist ()
// +---------------------------------------------------------------------------+
// | 機能  プラグインのアイコン取得
// +---------------------------------------------------------------------------+
{
    global $_CONF;
    return $_CONF['layout_url']  . '/images/icons/plugins.png';
}

function plugin_cclabel_assist()
// +---------------------------------------------------------------------------+
// | 機能  投稿管理admin/moderation.php　メニュ
// +---------------------------------------------------------------------------+
{
    global $_CONF;

	if (SEC_hasRights('assist.admin')) {
		return array(
			'assist'
			,$_CONF['site_admin_url'] . "/plugins/assist/index.php"
			,plugin_geticon_assist());
    }
    return "";

}

function plugin_getadminoption_assist()
// +---------------------------------------------------------------------------+
// | 機能 管理者メニュ表示 (管理者ブロック)
// +---------------------------------------------------------------------------+
{
	global $_CONF;
    global $LANG_ASSIST_ADMIN;

    if (SEC_hasRights ('assist.admin') ){
        return array ($LANG_ASSIST_ADMIN['piname'],
                      $_CONF['site_admin_url'] . '/plugins/assist/index.php');
    }
    return "";

}
function plugin_getdocumentationurl_assist(
	$file
)
// +---------------------------------------------------------------------------+
// | 機能 ドキュメントURL 
// | index  ？
// | config 
// +---------------------------------------------------------------------------+
{
	global $_CONF;

	static $doc_url = NULL;

	if ($doc_url === NULL) {
		switch ($file) {
			case 'index':
			case 'config':
				$lang = COM_getLanguageName();
				$path = 'admin/plugins/assist/docs/';
				if (!file_exists($_CONF['path_html'] . $path . $lang . '/assist.html')) {
					$lang = 'japanese';//'english';
				}
		
				$doc_url = $_CONF['site_url'] . '/' . $path . $lang . '/assist.html';
				break;

			default:
				$doc_url = FALSE;
				break;
		}
	}
	
	return $doc_url;
}

/**
* Provides text for a Configuration tooltip
*
* @param    string  $id     Id of config value
* @return   mixed           Text to use regular tooltip, NULL to use config 
*                           tooltip hack, or empty string when not available
*/
function plugin_getconfigtooltip_assist(
	$id
){
	// Use config tooltip hack where tooltip is read from the config documentation
	return NULL;
}

function plugin_getheadercode_assist()
// +---------------------------------------------------------------------------+
// | 機能  ヘッダコード編集　Javascript Metatag
// +---------------------------------------------------------------------------+
{
	global $_SCRIPTS;
	global $_CONF;
	
	//-----Javascript
	$txt="";
	$jstxtfile = $_CONF['path'] . 'plugins/assist/custom/javascript/javascript.txt';
	if (file_exists($jstxtfile)) {
		$txt= file_get_contents($jstxtfile);
	}
	if  ($txt<>""){
		$_SCRIPTS->setJavaScript($txt);
	}
	
	//-----css
	$css = '/assist/css/style.css';
	$_SCRIPTS->setCSSFile('assist_style_css', $css, true);
	
	//-----metatag etc
	$retval=assist_GetHeaderCode();
	
	return $retval;
}

function assist_GetHeaderCode(
	)
// +---------------------------------------------------------------------------+
// | 機能  headercode OGPメタタグ、FBメタタグ編集
// | 		TOPページ、記事
// +---------------------------------------------------------------------------+
{
	global $_CONF;
	global $_TABLES;
	global $_IMAGE_TYPE;
	global $_ASSIST_CONF;

	$pi_name="assist";
	$retval="";
	
	$currenturl= COM_getCurrentURL();
	$site_logo= $_CONF['layout_url'] . '/images/logo.' . $_IMAGE_TYPE ;

	$type="";
	//HOME
	if (COM_onFrontpage()){
		// Site HOME
		$type = 'frontpage';
		$og_type="website";
		
		$og_title=$_CONF['site_name'] ;
		if  ($_CONF['site_slogan']<>""){ 
			$og_title.= ' - ' . $_CONF['site_slogan'];
		}
		$og_image =$_ASSIST_CONF["default_img_url"];
		if ($og_image===""){
			$og_image=$site_logo;
		}
		
		if ($_CONF['meta_tags']==0 OR $_CONF['meta_description']=="") {
			$og_description = $_CONF['site_slogan'];
		}else{
			$og_description = $_CONF['meta_description'];
		}
		
	//Topic Top Page
	} else if (preg_match("@index\.php(?:\?topic=|/)(.+)$@i", $currenturl, $match)) {
		$type="topictop";
		$og_type="article";
		if( isset( $_GET['topic'] )){
			$topic = COM_applyFilter( $_GET['topic'] );
		}else if( isset( $_POST['topic'] )){
			$topic = COM_applyFilter( $_POST['topic'] );
		}else{
			$topic = '';
		}
		
		$sql = "SELECT topic, meta_description,imageurl ".LB
			 . "FROM {$_TABLES['topics']} ".LB
			 . "WHERE (tid ='{$topic}') ".LB;
		$result = DB_query($sql);
		if (!DB_error() AND (DB_numRows($result) == 1)) {
			$A = DB_fetchArray($result, FALSE);
			$A = array_map('stripslashes', $A);

			$title     = $A['topic'];
			$og_title=$title;
			$meta_description = ltrim(rtrim($A['meta_description']));
			
			$image=$A['imageurl'];
			if ($image<>""){
				$og_image=$_CONF['site_url'].$image;
			}else{
				$og_image =$_ASSIST_CONF["default_img_url"];
				if ($og_image===""){
					$og_image=$site_logo;
				}
			}
			
			if ($_CONF['meta_tags']==0 OR $meta_description=="") {
				$og_description = $_CONF['site_slogan'];
			}else if ($meta_description=="") {
				$og_description = $_CONF['meta_description'];
			}else{
				$og_description = $meta_description;
			}
		}
			
	//article
	} else if (preg_match("@article\.php(?:\?story=|/)(.+)$@i", $currenturl, $match)) {
		$type="article";
		$og_type="article";
		if (isset ($_POST['mode'])) {
			$sid = COM_applyFilter ($_POST['story']);
		} else {
			COM_setArgNames (array ('story', 'mode'));
			$sid = COM_applyFilter (COM_getArgument ('story'));
		}
		
		$sql = "SELECT title, introtext, bodytext, meta_description ".LB
			 . "FROM {$_TABLES['stories']} ".LB
			 . "WHERE (sid ='{$sid}') ".LB;
		$result = DB_query($sql);
		if (!DB_error() AND (DB_numRows($result) == 1)) {
			$A = DB_fetchArray($result, FALSE);
			$A = array_map('stripslashes', $A);

			$title     = $A['title'];
			$og_title=$title;
			//meta_description htmlspecialchars stripslashes
			$introtext = PLG_replacetags($A['introtext']);
			$bodytext  = PLG_replacetags($A['bodytext']);
			$meta_description = ltrim(rtrim($A['meta_description']));
			
			$image=assist_GetImageUrl($introtext);
			if ($image<>""){
				$og_image=$image;
			}else{
				$og_image =$_ASSIST_CONF["default_img_url"];
				if ($og_image===""){
					$og_image=$site_logo;
				}
			}
			$introtext = COM_getTextContent($introtext );
			$bodytext  = COM_getTextContent($bodytext);
			$meta_description = COM_getTextContent($meta_description);

			if ($_CONF['meta_tags']==0 OR $meta_description=="") {
				$og_description = $introtext;
			}else{
				$og_description = $meta_description;
			}

		}
		
	}
	if ($type===""){
		return;
	}
	
	//テンプレートフォルダの設定
    $tmplfld=assist_templatePath('headercode',"",$pi_name);
	if (file_exists ($tmplfld."/".$type.".thtml")) {

		$tpl = new Template($tmplfld);

		$tpl->set_file (array (
			'tpl' => $type.'.thtml',
			));
	
		$tpl->set_var('xhtml', XHTML);
		$tpl->set_var('site_url', $_CONF['site_url']);
		
		$tpl->set_var ('currenturl', $currenturl);
		
		$tpl->set_var ('site_name', $_CONF['site_name']);
		$tpl->set_var ('site_mail', $_CONF['site_mail']);
		
		$tpl->set_var ('og_title', $og_title);
		$tpl->set_var ('og_image', $og_image);
		$tpl->set_var ('og_description', $og_description);
		$tpl->set_var ('og_type', $og_type);
		
		
		//facebook
		$facebook_consumer_key = trim($_CONF['facebook_consumer_key']);
		$tpl->set_var ('facebook_consumer_key', $facebook_consumer_key);
		//$tpl->set_var ('fb_user_ids', $fb_user_ids);

		$tpl->parse ('output', 'tpl');
		$retval .= $tpl->finish ($tpl->get_var ('output'));
	}

	return $retval;
}

function assist_GetImageUrl(
	$txt
)
// +---------------------------------------------------------------------------+
// | 機能  テキストファイル内の最初の画像のURLを取得する
// +---------------------------------------------------------------------------+
{
	global $_CONF;
	
	$rt="";
	if (preg_match('@<img\s\s*.*?src="(.+?)".*?>@msi', $txt, $match)) {
		$rt = $match[1];
		if (!preg_match('@^https?://@i', $rt)) {
			$rt = $_CONF['site_url'] . $rt;
		}
	}
	return $rt;

}

function plugin_templatesetvars_assist(
	$templatename
	, &$template
)
// +---------------------------------------------------------------------------+
// | 機能  テンプレート変数設定
// +---------------------------------------------------------------------------+
{
	global $_ASSIST_CONF;
    global $_USER;
	
	$languageid=COM_getLanguageId();
    $language= COM_getLanguage();
    $template->set_var ('languageid', $languageid);
    $template->set_var ('language', $language);
	if ($languageid<>"") {
		$template->set_var ('_languageid', "_".$languageid);
	}else{
		$template->set_var ('_languageid', "");
	}
	
    if  (COM_isAnonUser()){
        $login_uid=null;
		$login_usernmae=null;
		$login_fullnmae=null;
		$login_displayname=null;
	}else{
        $login_uid=$_USER['uid'];
		$login_username=$_USER['username'];
		$login_fullname=$_USER['fullname'];
		$login_displayname=COM_getDisplayName( $_USER['uid'], $_USER['username'],
                                           $_USER['fullname'] );
	}

    switch ($templatename) {
		case 'header':
			if ($_ASSIST_CONF['xmlns']<>""){
				$xmlins="";
				if (XHTML == '') {
				} else {
					$xmlins.=' xmlns="http://www.w3.org/1999/xhtml"'.LB;
				}
				$xmlins.=$_ASSIST_CONF['xmlns'];

				$template->set_var('xmlns', $xmlins);
			}
            $template->set_var('uid', $login_uid);
            $template->set_var('username', $login_username);
            $template->set_var('fullname', $login_fullname);
		    $template->set_var('displayname', $login_displayname);
            break;
		case 'databox':
		case 'userbox':
            $template->set_var('uid', $login_uid);
            $template->set_var('username', $login_username);
            $template->set_var('fullname', $login_fullname);
		    $template->set_var('displayname', $login_displayname);
			break;
		case 'storytext':
		case 'featuredstorytext':
		case 'archivestorytext':
        break;

	}
}

?>
