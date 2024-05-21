<?php
/* Reminder: always indent with 4 spaces (no tabs). */
// +---------------------------------------------------------------------------+
// | DataBox プラグイン   function.inc からrequire
// | 自動タグおよび DataBox 専用関数
// +---------------------------------------------------------------------------+
// $Id: plugins/databox/functions_autotag.inc
//20101008 tsuchitani AT ivywe DOT co DOT jp http://www.ivywe.co.jp/
//20120416 databox_category {data_edit}

if (strpos ($_SERVER['PHP_SELF'], 'functions_autotag.inc') !== false) {
    die ('This file can not be used on its own.');
}
//=====自動タグ=================================================================
function plugin_autotags_databox (
    $op
    , $content = ''
    , $autotag = ''
)
// +---------------------------------------------------------------------------+
// | 自動タグ
// +---------------------------------------------------------------------------+
{
    global $_TABLES;
    global $_CONF;
    global $LANG_DATABOX_autotag_desc;

    //Option Display
    if ($op == 'tagname' ) {
        $tagname[]='databox';
        return $tagname;
    } elseif ($op == 'permission' || $op == 'nopermission') {
        if ($op == 'permission') {
            $flag = true;
        } else {
            $flag = false;
        }

        if (isset($_GROUPS['DataBox Admin'])) {
            $group_id = $_GROUPS['DataBox Admin'];
        } else {
            $group_id = DB_getItem($_TABLES['groups'], 'grp_id',
                                   "grp_name = 'DataBox Admin'");
        }
        $owner_id = SEC_getDefaultRootUser();

        if (COM_getPermTag(
            $owner_id
            , $group_id
            , $_DATABOX_CONF['autotag_permissions_databox'][0]
            , $_DATABOX_CONF['autotag_permissions_databox'][1]
            , $_DATABOX_CONF['autotag_permissions_databox'][2]
            , $_DATABOX_CONF['autotag_permissions_databox'][3]) == $flag) {
            return $tagname;
        }
    } elseif ($op == 'description') {
        return $LANG_DATABOX_autotag_desc;
    } else if ($op == 'parse') {
        //引数チェック
        $p1 = COM_applyFilter ($autotag['parm1']);
        $p2 = COM_applyFilter ($autotag['parm2']);
        $tag = COM_applyFilter ($autotag['tag']);
        $tagstr = COM_applyFilter ($autotag['tagstr']);
        
        $parm2_ary[]="category_id";
        $parm2_ary[]="category_code";
        $parm2_ary[]="mode";
        $parm2_ary[]="rss_file";
        $parm2_ary[]="title_trim_length";
        $parm2_ary[]="intervalday";
        $parm2_ary[]="limitcnt";
        $parm2_ary[]="newmarkday";
        $parm2_ary[]="templatedir";
        $parm2_ary[]="id";
        $parm2_ary[]="code";
        $parm2_ary[]="nohitmsg";
        $parm2_ary[]="perpage";
        $parm2_ary[]="page";
        $parm2_ary[]="order";
        $parm2_ary[]="templatesetvar";
        $parm2_ary[]="value";
        $parm2_ary[]="field_id";
        $parm2_ary[]="expired";
        $parm2_ary[]="type_id";
        $parm2_ary[]="group_id";
        $parm2_ary[]="group_code";
        $parm2_ary[]="iname";
        $parm2_ary[]="without_code";
        $parm2_ary[]="without_id";
        $parm2_ary[]="field_code";
        $parm2_ary[]="option";
        $parm2_ary[]="method";
        $parm2_ary[]="roundprecision";
        $parm2_ary[]="numberformat";
        $parm2_ary[]="permission";
        $p=databox_parm2($p2,$parm2_ary);
        //各処理
        switch( $tag ) {
            case 'databox' :
                switch($p1) {
                    case 'count':
                        $rt= databox_count(
                            $p['category_id']
                            ,$p['category_code']
                            ,$p['mode']
                            ,$p['permission']
                            ,$p['numberformat']
                            );
                        break;
                    case 'newlist':
                            if ($p['category_id']<>""){
                                $m="id";
                                $categories=$p['category_id'];
                            }elseif ($p['category_code']<>"") {
                                $m="code";
                                $categories=$p['category_code'];
                            }else{
                                $m="ALL";
                                $categories="";
                            }
                            $rt= databox_newlist(
                            $m
                            ,$categories
                            ,$p['rss_file']
                            ,$p['title_trim_length']
                            ,$p['intervalday']
                            ,$p['limitcnt']
                            ,$p['newmarkday']
                            ,$p['templatedir']
                            );
                        break;
                    case 'data':
                        $w= databox_data(
                            $p['id']
                            ,$p['templatedir']
                            ,$p['nohitmsg']
                            ,""
                            ,$p['code']
                            );
                        $rt=$w['display'];
                        break;
                    case 'category':
                        $rt= databox_category(
                            "autotag"
                            ,$p['category_id']
                            ,$p['templatedir']
                            ,$p['nohitmsg']
                            ,$p['perpage']
                            ,$p['page']
                            ,$p['order']
                            ,$p['category_code']
                            ,$p['mode']
                            ,$p['expired']
                            ,$p['without_code']
                            ,$p['without_id']
                        );
                        break;
                    case 'attributedatacount':
                        $rt=DATABOX_count_each_afield(
                            "databox"
                            ,$p['templatesetvar']
       	                    ,$p['value']
                            ,$p['type_id']
                            ,$p['permission']
                            ,$p['numberformat']
                            );
                        break;

                    case 'lastmodified':
                        $rt= databox_grp(
                            "modified"
                            ,"max"
                            ,"lastmodified"
                            ,$p['category_id']
                            ,$p['category_code']
                            );
                        break;
                    case 'lastcreated':
                        $rt= databox_grp(
                            "created"
                            ,"max"
                            ,"lastcreated"
                            ,$p['category_id']
                            ,$p['category_code']
                            );
                        break;
                    case 'attribute':
                        $rt= databox_field(
                            "autotag"
                            ,$p['field_id']
                            ,$p['value']
                            ,$p['templatedir']
                            ,$p['nohitmsg']
                            ,$p['perpage']
                            ,$p['page']
                            ,$p['order']
                            ,$p['field_code']
                        );
                        break;
                    case 'datalink':
                        $rt= databox_datalink(
                            $p['id']
                            ,$p['code']
                            ,$p['lastparm2']
                        );
                        break;
                    case 'mydataentrylink':
                    case 'mydataentry':
                        $rt= databox_mydataentrylink(
                            $p['type_id']
                            ,$p['templatedir']
                            ,$p['lastparm2']
                        );
                        break;
                    case 'mydata':
                        $rt= databox_mydata(
                             $p['templatedir']
                            ,$p['lastparm2']
                        );
                        break;
                    case 'categorycount':
                        $rt= DATABOX_categorycount(
                             "databox"
                            ,$p['group_id']
                            ,$p['group_code']
                            ,$p['permission']
                            ,$p['numberformat']
                        );
                        break;
                    case 'categorycheck':
                          $rt= DATABOX_getcheckList(
                             "categorygroup"
                            ,""
                            ,"databox"
                            ,$p['group_id']
                            ,$p['iname']
                        );
                        break;
                    case 'mastercheck':
                        if  ($p['field_id']<>""){
                            $kind=COM_applyFilter(
                                DB_getItem($_TABLES['DATABOX_def_field'] 
                                    ,"selectlist","field_id={$p['field_id']}"));
                            if  ($kind<>""){
                                  $rt= DATABOX_getcheckList(
                                     $kind
                                     ,""
                                     ,"databox"
                                     ,""
                                     ,$p['iname']
                                  );
                            }
                        }
                        break;
                    case 'search':
                        $argary=databox_searcharg("autotag",$p['lastparm2']);
                        $rt=databox_search("autotag",$argary);
                        break;
                    case 'attributevalue':
                        $rt=DATABOX_fieldvalue(
                                "databox"
                                ,$p['id']
                                ,$p['code']
                                ,$p['field_id']
                                ,$p['field_code']
                                ,$p['option']
                            );
                        break;
                    case 'math':
                        $rt=DATABOX_math(
                                "databox"
                                ,$p['method']
                                ,$p['templatesetvar']
                                ,$p['type_id']
                                ,$p['category_id']
                                ,$p['category_code']
                                ,$p['mode']
                                ,$p['roundprecision']
                                ,$p['numberformat']
                                ,$p['permission']
                            );
                        break;
                }

                $content = str_replace ($autotag['tagstr'], $rt, $content);
                return $content;
                break;

        }
    }
}

//第二引数分割
function databox_parm2 ($p2,$parm2_ary)
{
    $ary=array();
    $px = explode (' ', trim ($p2));
    if (is_array ($px)) {
        foreach ($px as $part) {
            $a = explode (':', $part);
            if (in_array($a[0],$parm2_ary)){
                $ary["{$a[0]}"]=$a[1];
                $skip++;            
            }
        }
        
        if (count ($px) > $skip) {
            for ($i = 0; $i < $skip; $i++) {
                array_shift ($px);
            }
            $ary["lastparm2"] = trim(implode (' ', $px));
        }

    }else{
        $ary["lastparm2"]=trim($p2);
    }
    
    return $ary;

}
//=====ブロック関数============================================================
// +---------------------------------------------------------------------------+
// | 機能  ブロック用 新着data一覧出力                                         |
// | 書式 phpblock_shownewdatabox()                                            |
// +---------------------------------------------------------------------------+
function phpblock_shownewdatabox()
{
    return databox_newlist();
}

//=====専用関数============================================================
//
function databox_category(
    $autotag
    ,$category_id=null
    ,$template=null
    ,$nohitmsg=""
    ,$perpage =0
    ,$page=null
    ,$order=null
    ,$category_code=null
    ,$mode=''
    ,$expired=''
    ,$without_code=''
    ,$without_id=''
)
// +---------------------------------------------------------------------------+
// | 機能  カテゴリ別一覧表示
// | 書式  databox_category($id,$template,"yes","page",$perpage)
// | 書式  databox_category($p['category_id'],$p['thtml'],$p['nohitmsg']
// | 書式                    ,"",$p['perpage'],$p['order'],$p['code']
// +---------------------------------------------------------------------------+
// | 引数 $autotag:notautotagの時ヘッダを表示しない
// | 引数 $category_id:カテゴリid
// | 引数 $template:使用するテンプレートのフォルダ名
// | 引数 $nohitmsg      :"yes"の時nohitメッセージ表示
// | 引数 $perpage       :1pageあたりの件数、0の時改ページなし
// | 引数 $page            :
// | 引数 $order         :順序
// | 引数 $category_code :
// | 引数 $mode          :ブランク　または　AND
// +---------------------------------------------------------------------------+
// | public_html/category.php および　自動タグで使用
// | ヘッダはコンフィギュレーション設定により表示されない場合があります
// | idもcodeも指定されない場合は、全カテゴリ
// +---------------------------------------------------------------------------+
// | 戻値 nomal:category list
// +---------------------------------------------------------------------------+
{
    $pi_name="databox";
    
    global $_CONF;
    global $_TABLES;
    
    global $_DATABOX_CONF;
    global $LANG_DATABOX;
    global $LANG_DATABOX_ADMIN;
    global $_IMAGE_TYPE;
    global $LANG_confignames;

    //ログイン要否チェック
    if (COM_isAnonUser()){
        if  ($_CONF['loginrequired']
                OR ($_DATABOX_CONF['loginrequired'] >1) ){
            return $LANG_DATABOX['loginrequired'];
        }

    }
    //-----テーブル
    $tbl1=$_TABLES['DATABOX_category'] ;
    $tbl2=$_TABLES['DATABOX_base'] ;
    $tbl3=$_TABLES['DATABOX_addition'] ;
    //
    $tbl5=$_TABLES['DATABOX_def_category'] ;
    
    $tbl6=$_TABLES['DATABOX_stats'];

    //
    $datefield=$_DATABOX_CONF['datefield'];//使用する日付（編集日付、作成日付、公開日）
    $new_img=$_DATABOX_CONF['new_img'];
    if ($new_img==""){
        $new_img="New!";
    }
    $newmarkday=$_DATABOX_CONF['newmarkday'];
    if ($newmarkday==""){
        $newmarkday=3;
    }
    $chkday=strtotime("- $newmarkday days",time());

    //-----引数チェック
    $ids="";
    $idsary=array();
    if (is_null($category_id) OR $category_id==0){
        if (is_null($category_code)){
        }else{
            $w=explode("|",$category_code);
            foreach( $w as $val ){
                if ($ids<>""){
                    $ids.=",";
                }
                $w_id=DATABOX_codetoid($val,'DATABOX_def_category',"category_id");
                $idsary[]=$w_id;
                $ids.=$w_id;
            }
        }
    }else{
        $ids=$category_id;
        $idsary[]=$category_id;
    }
    
    //--CATEGORY
    $category_name="";
    $defaulttemplatesdirectory = "";
    if (count($idsary)<>0){
        
        for ($i = 0; $i < count($idsary); $i++) {
            $sql = "SELECT ";
            $sql .= " name ".LB;
            $sql .= " ,code ".LB;
            $sql .= " ,description ".LB;
            $sql .= " ,defaulttemplatesdirectory".LB;
            $sql .= " FROM ".LB;
            $sql .= " {$tbl5} AS t5 ".LB;
            $sql .= " WHERE ".LB;
            $sql .= " t5.category_id =".$idsary[$i].LB;
        
            $result = DB_query ($sql);
            $numrows = DB_numRows ($result);
            if ($numrows > 0) {
                $A = DB_fetchArray ($result);
                $category_name.=COM_applyFilter($A['name'])." ";
                $category_description.=COM_applyFilter($A['description'])." ";
                if ($i==0){
                    $defaulttemplatesdirectory=COM_applyFilter($A['defaulttemplatesdirectory']);
                }
            }
        }
    }

    if (is_null($template) or ($template==="")){
        if ($defaulttemplatesdirectory===""){
            $template="default";
        }else{
            $template=$defaulttemplatesdirectory;
        }
    }
    
    //
    if (is_null($page) OR !isset($page) OR $page == 0) {
        $page = 1;
    }
    if ($perpage===0 OR is_null($perpage)){
        $perpage=$_DATABOX_CONF['perpage'];
    }
    //
    $w=databox_orderby($datefield,$order,$orderby,$addfieldorder,$field_id);
    //
    if (is_null($expired) or ($expired==="")){
        $expired="no";
    }
    //-----
    $sql = "SELECT ";

    $sql .= " t2.id ".LB;
    $sql .= " ,t2.title ".LB;
    $sql .= " ,t2.code ".LB;
    $sql .= " ,t2.description ".LB;
    $sql .= " ,t2.released ".LB;
    $sql .= " ,t2.expired ".LB;
    $sql .= " ,t2.".$datefield." AS datefield ".LB;
    $sql .= " ,t2.fieldset_id ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.".$datefield.") AS datefield_un ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.released ) AS released_un ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.expired ) AS expired_un ".LB;

    $sql .= " ,t2.group_id";
    $sql .= " ,t2.owner_id";

    if ($addfieldorder){
        $sql .= " ,t3.value ".LB;
    }
    
    //--FROM
    $sql .= " FROM ".LB;
    $sql .= " {$tbl2} AS t2 ".LB;

    //if ($category_id<>0){
    //    $sql .= " ,{$tbl1} AS t1 ".LB;
    //}
    if ($addfieldorder){
        $sql .= " ,{$tbl3} AS t3 ".LB;
    }
    
    //--WHERE
    $sql .= " WHERE ".LB;
    
    if (count($idsary)<>0){
        if (strtoupper($mode)=="AND"){
            $w="";
            foreach( $idsary as $val ){
                if ($w<>""){
                    $w.=" AND ";
                }
                $w.=$val." IN (SELECT t1.category_id";
                $w.=" FROM {$tbl1} AS t1 ,{$tbl5} AS t5";
                $w.=" WHERE t2.id = t1.id AND t1.category_id = t5.category_id AND t5.allow_display<2)";
            }
        }else{
            $w= "t2.id IN (SELECT id FROM {$tbl1} AS t1 ,{$tbl5} AS t5 ";
            $w.="WHERE t1.category_id = t5.category_id AND t5.allow_display<2 AND t1.category_id IN ({$ids})) ";
        }
        $sql.=" ({$w})".LB;
    }else{
        $sql.=" 1=1".LB;
    }
    
    if ($addfieldorder){
    
        $sql .= " AND t3.field_id=".$field_id.LB;
        $sql .= " AND t3.id=t2.id".LB;
    }

    //管理者の時,下書データも含む
    //20110529 Del
    //if ( SEC_hasRights('databox.admin')) {
    //}else{
       $sql .= " AND t2.draft_flag=0".LB;
    //}

    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND',0,2,"t2");

    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())".LB;
    //公開終了日を過ぎたデータはのぞく
    if  (strtoupper($expired)=="NO"){
        $sql .= " AND (expired=0 OR expired > NOW())";
    }
    if  ($without_code<>""){
        $sql .= " AND (t2.code<>'$without_code')";
    }
    if  ($without_id<>""){
        $sql .= " AND (t2.id<>$without_id)";
    }
    
    //--ORDER
    $sql .= " ORDER BY ".LB;
    $sql .= $orderby.LB;
    
    $result = DB_query ($sql);
    $cnt = DB_numRows ($result);

    $pages = 0;
    if ($perpage > 0) {
        $pages = ceil($cnt / $perpage);
    }
    $offset = ($page - 1) * $perpage;
    $sql .= " LIMIT $offset, $perpage";

    //自動タグでない時　ヘッダ、左ブロック
    if ($autotag==="notautotag"){
        if ($page > 1) {
            $page_title = sprintf ('%s (%d)', $category_name, $page);
        } else {
            $page_title = sprintf ('%s ', $category_name);
        }
        
        // Meta Tags
        $headercode=DATABOX_getheadercode(
            "category"
            ,$template
            ,$pi_name
            ,$category_id
            ,$category_name
            ,$category_description
            ,$category_name
            ,$category_description);
        $retval .= DATABOX_siteHeader($pi_name,'',$page_title,$headercode) ;
    }
    

    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);

    if ($numrows > 0) {
        
        $tmplfld=DATABOX_templatePath('category',$template,$pi_name);
        $templates = new Template($tmplfld);
        $templates->set_file (array (
            'list' => 'list_detail.thtml',
            'nav'   => 'navigation_detail.thtml',
            'row'   => 'row.thtml',
            'col'   => "col_detail.thtml",
            ));

        $languageid=COM_getLanguageId();
        $language= COM_getLanguage();
        $templates->set_var ('languageid', $languageid);
        $templates->set_var ('language', $language);
        if ($languageid<>"") {
            $templates->set_var ('_languageid', "_".$languageid);
        }else{
            $templates->set_var ('_languageid', "");
        }

        //
        $templates->set_var ('site_url',$_CONF['site_url']);
        $templates->set_var ('this_script',THIS_SCRIPT);

        //bread
        $templates->set_var ('home',$LANG_DATABOX['home']);
        
        $templates->set_var ('lang_category_list_h2',$LANG_DATABOX['category_list_h2']);

        $templates->set_var ('category_name',$category_name);
        $templates->set_var ('category_code',$category_code);
        $templates->set_var ('category_description',$category_description);

        //page
        $templates->set_var ('cnt', $cnt);
        $lin1=$offset+1;
        $lin2=$lin1+$perpage - 1;
        if ($lin2>$cnt){
            $lin2=$cnt;
        }

        //summary navigation
        $templates->set_var ('lang_view', $LANG_DATABOX['view']);
        $templates->set_var ('lin', $lin1."-".($lin2));
        $templates->set_var ('cnt', $cnt);

        //
        $templates->set_var ('lang_title', $LANG_DATABOX_ADMIN['title']);
        $templates->set_var ('lang_code', $LANG_DATABOX_ADMIN['code']);
        $templates->set_var ('lang_id', $LANG_DATABOX_ADMIN['id']);
        $templates->set_var ('lang_description', $LANG_DATABOX_ADMIN['description']);
        $templates->set_var ('lang_date', $LANG_DATABOX_ADMIN[$datefield]);
        $templates->set_var ('lang_released', $LANG_DATABOX_ADMIN['released']);
        $templates->set_var ('lang_expired', $LANG_DATABOX_ADMIN['expired']);
        $templates->set_var ('lang_remaingdays', $LANG_DATABOX_ADMIN['remaingdays']);
        $templates->set_var ('lang_addfield', $addfield_name);
        // 追加項目のヘッダ
        $addition_def=DATABOX_getadditiondef($pi_name);
        
        //
        $templates->set_var('lang_imgfile_frd', $LANG_confignames['databox']['imgfile_frd']);
        $templates->set_var ('imgfile_frd', $_DATABOX_CONF['imgfile_frd']);
        $templates->set_var ('data_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_frd']);
        $templates->set_var('lang_imgfile_thumb_frd', $LANG_confignames['databox']['imgfile_thumb_frd']);
        $templates->set_var ('imgfile_thumb_frd', $_DATABOX_CONF['imgfile_thumb_frd']);
        $templates->set_var ('data_thumb_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_thumb_frd']);
        
        for ($i = 0; $i < $numrows; $i++) {
            $A = DB_fetchArray ($result);
            $A = array_map('stripslashes', $A);

            $title=$A['title'];
            $description=$A['description'];
            $code=COM_applyFilter($A['code']);
            $id=COM_applyFilter($A['id']);
            $datefield=COM_applyFilter($A['datefield']);
            $released=COM_applyFilter($A['released']);
            $expired=COM_applyFilter($A['expired']);
            $fieldset_id=COM_applyFilter($A['fieldset_id']);
            $datefield_ary = COM_getUserDateTimeFormat($A['datefield_un']);
            $released_ary = COM_getUserDateTimeFormat($A['released_un']);
            if ($expired==="0000-00-00 00:00:00"){
                $expired_ary=array();
            }else{
                $expired_ary = COM_getUserDateTimeFormat($A['expired_un']);
            }
            $curdate_ary =  COM_getUserDateTimeFormat();

            
            $value=COM_applyFilter($A['value']);
            $group_id = $A['group_id'];
            $owner_id = $A['owner_id'];

            $rt= databox_detail_link($id,$code,$title);

            $templates->set_var ('data_link', $rt['link']);
            $templates->set_var ('data_title', $title);
            $templates->set_var ('data_code', $code);
            $templates->set_var ('data_description', $description);
            $templates->set_var ('data_id', $id);
            $templates->set_var ('data_url', $rt['url']);

            $templates->set_var ('data_datefield', $datefield_ary[0]);
            $templates->set_var ('data_value', $value);
            $templates->set_var ('data_datefield_shortdate', strftime( $_CONF['shortdate'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_date', strftime( $_CONF['date'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_daytime', strftime( $_CONF['daytime'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_dateonly', strftime( $_CONF['dateonly'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_timeonly', strftime( $_CONF['timeonly'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_b', strftime( "%b" , $A['datefield_un']));
            $templates->set_var ('data_datefield_B', strftime( "%B" , $A['datefield_un']));
            $templates->set_var ('data_datefield_d', strftime( "%d" , $A['datefield_un']));
            $templates->set_var ('data_datefield_e', strftime( "%e" , $A['datefield_un']));
            
            $templates->set_var ('data_released', $released_ary[0]);
            $templates->set_var ('data_released_shortdate', strftime( $_CONF['shortdate'], $A['released_un'] ));
            $templates->set_var ('data_released_date', strftime( $_CONF['date'], $A['released_un'] ));
            $templates->set_var ('data_released_daytime', strftime( $_CONF['daytime'], $A['released_un'] ));
            $templates->set_var ('data_released_dateonly', strftime( $_CONF['dateonly'], $A['released_un'] ));
            $templates->set_var ('data_released_timeonly', strftime( $_CONF['timeonly'], $A['released_un'] ));
            $templates->set_var ('data_released_b', strftime( "%b" , $A['released_un']));
            $templates->set_var ('data_released_B', strftime( "%B" , $A['released_un']));
            $templates->set_var ('data_released_d', strftime( "%d" , $A['released_un']));
            $templates->set_var ('data_released_e', strftime( "%e" , $A['released_un']));
            //公開終了日 Expired to publish
            if ($A['expired'] ==="0000-00-00 00:00:00"){
                $templates->set_var ('data_expired', "");
                $templates->set_var ('data_expired_shortdate', "" );
                $templates->set_var ('data_expired_date', "" );
                $templates->set_var ('data_expired_daytime', "" );
                $templates->set_var ('data_expired_dateonly', "" );
                $templates->set_var ('data_expired_timeonly', "" );
                $templates->set_var ('data_expired_b', "" );
                $templates->set_var ('data_expired_B', "" );
                $templates->set_var ('data_expired_d', "" );
                $templates->set_var ('data_expired_e', "" );
            }else{
                $wary = COM_getUserDateTimeFormat($A['expired_un']);
                $templates->set_var ('data_expired', $expired_ary[0]);
                $templates->set_var ('data_expired_shortdate', strftime( $_CONF['shortdate'], $A['expired_un'] ));
                $templates->set_var ('data_expired_date', strftime( $_CONF['date'], $A['expired_un'] ));
                $templates->set_var ('data_expired_daytime', strftime( $_CONF['daytime'], $A['expired_un'] ));
                $templates->set_var ('data_expired_dateonly', strftime( $_CONF['dateonly'], $A['expired_un'] ));
                $templates->set_var ('data_expired_timeonly', strftime( $_CONF['timeonly'], $A['expired_un'] ));
                $templates->set_var ('data_expired_b', strftime( "%b" , $A['expired_un']));
                $templates->set_var ('data_expired_B', strftime( "%B" , $A['expired_un']));
                $templates->set_var ('data_expired_d', strftime( "%d" , $A['expired_un']));
                $templates->set_var ('data_expired_e', strftime( "%e" , $A['expired_un']));
            }
            $remaingdays="";
            if ($expired<>"0000-00-00 00:00:00") {
                if  ($expired_ary[1]>=$curdate_ary[1]){
                    $remaingdays=COM_dateDiff( "d", $expired_ary[1], $curdate_ary[1] ) + 1;
                }
            }
            $templates->set_var ('data_remaingdays', $remaingdays);//@@@@@@
            if (date("Ymd",strtotime($datefield)) >= date("Ymd",$chkday)){
                $templates->set_var ('new_img', $new_img);
            }else{
                $templates->set_var ('new_img', '');
            }
            $hits=COM_applyFilter(DB_getItem( $tbl6 ,"hits","id={$id}"),true);
            $templates->set_var('lang_hits', $LANG_DATABOX_ADMIN['hits']);
            $templates->set_var('data_hits', $hits);
            
            //カテゴリ@@@@@
            $templates->set_var('lang_category', $LANG_DATABOX_ADMIN['category']);
            DATABOX_getcategoriesDisp($A['id'],$templates,$chk_user,0,$pi_name);

            //追加項目
            $chk_user=DATABOX_chkuser($group_id,$owner_id,"databox.admin");
            $additionfields = DATABOX_getadditiondatas($id,$pi_name);
            DATABOX_getaddtionfieldsDisp($additionfields,$addition_def,$templates,$chk_user,$pi_name,$fieldset_id);
            
            //管理者の時「編集」
            if ( SEC_hasRights('databox.admin')) {
                $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                $attr = array('title' => $title." ".$LANG_DATABOX_ADMIN['edit']);
                $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);
                
                $attr = array('class' => 'editlink', 'title' => $title." ".$LANG_DATABOX_ADMIN['edit']);
                $url = $_CONF['site_admin_url'];
                $url .= '/plugins/databox/data.php';
                $url .= '?mode=edit';
                $url .= '&amp;'."id={$id}";
                $icon = '&nbsp;' ;
                $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );
                $templates->set_var ('data_edit', $icon);
            }else{
                $rt=databox_chk_loaddata($id);
                //編集権限のあるMyData
                if ( $rt==="OK") {
                    $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                    $attr = array('title' => $LANG_DATABOX_ADMIN['edit']);
                    $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);

                    $attr = array('class' => 'editlink', 'title' => $title." ".$LANG_DATABOX_ADMIN['edit']);

                    $url = $_CONF['site_url'];
                    $url .= '/databox/mydata/data.php';
                    $url .= '?mode=edit';
                    $url .= '&amp;'."id={$id}";

                    $icon = '&nbsp;' ;
                    $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );

                    $templates->set_var ('data_edit', $icon);
                }else{
                    $templates->set_var ('data_edit', "");
                }
                
            }

            
            //=====
            $templates->parse ('col_var', 'col', true);
            $templates->parse ('row_var', 'row', true);

            $templates->set_var ('col_var', '');

        }
        // Call to plugins to set template variables in the databox
        PLG_templateSetVars( 'databox', $templates );

        //------------
        //-----navigation
        $url = $_CONF['site_url']  . '/';
        if ($autotag==="notautotag"){
            $url.=THIS_SCRIPT;
        }else{
            $url.="databox/category.php";
        }
        $url.="?";
        if ($category_code<>""){
            $url.="code=".$category_code;
            $url.="&amp;m=code";
        }else{
            $url .= "id=".$category_id;
            $url .= "&amp;m=id";
        }    
        $url .= "&amp;template=".$template;
        $url .= "&amp;order=";
        
        //-----order navigation
        databox_order ($url, $templates, $order) ;
        //-----page navigation
        $url .= $order;
        $templates->set_var ('page_navigation',
                COM_printPageNavigation ($url, $page, $pages));
        if ($order==="random"){
            $templates->set_var ( 'pagenavinone', 'style="display:none;"' );
        }else{
            $templates->set_var ( 'pagenavinone', '' );
        }
        //------------
        $templates->parse ('nav_var', 'nav', true);

        $templates->set_var ('msg', "");

        $templates->parse ('output', 'list');

        $content = $templates->finish ($templates->get_var ('output'));
        $retval .=$content;

    }else{
        if ($nohitmsg==="yes"){
            $tmplfld=DATABOX_templatePath('category',$template,$pi_name);
            $templates = new Template($tmplfld);
            $templates->set_file (array (
                'list' => 'nohit.thtml',
            ));
            
            $templates->set_var ('home',$LANG_DATABOX['home']);
            
            $url=$_CONF['site_url']."/databox/category.php";
            $category_top="<a href='".$url."'>".$LANG_DATABOX['category_top']."</a>";
            $templates->set_var ('category_top',$category_top);

            $templates->set_var ('category_name',$category_name);
            $templates->set_var ('category_code',$category_code);
            $templates->set_var ('category_description',$category_description);
            
                        
            $templates->set_var('xhtml', XHTML);
            $templates->set_var('site_url', $_CONF['site_url']);
            $templates->set_var('site_admin_url', $_CONF['site_admin_url']);
            $templates->set_var('layout_url', $_CONF['layout_url']);
            
            $templates->set_var ('lang_nohit', $LANG_DATABOX['nohit']);

            $templates->parse ('output', 'list');
            $content = $templates->finish ($templates->get_var ('output'));
            $retval .=$content;

        }
        
        
        
        
        
    }

    $retval =PLG_replacetags ($retval);

    return $retval;
}

function databox_count(
    $category_id =null
    ,$category_code =null
    ,$mode=''
    ,$permission=''
    ,$numberformat=''
)
// +---------------------------------------------------------------------------+
// | 機能  件数表示                                                          |
// +---------------------------------------------------------------------------+
// | 書式  databox_count()
// | 書式  databox_count(1)
// | 書式  databox_count("japan")
// +---------------------------------------------------------------------------+
// | 引数 $category_id:カテゴリid　（優先）
// | 引数 $category_code:カテゴリコード
// | 引数 $mode          :ブランク　または　AND
// | 引数 $permission    :ブランク　または　ignore
// | 引数 $numberformat  :ブランク　または　yes
// +---------------------------------------------------------------------------+
// | 戻値 nomal:データ総数、カテゴリ別データ件数
// +---------------------------------------------------------------------------+
{
    global $_TABLES;
    
    //-----
    $tbl1=$_TABLES['DATABOX_base'] ;
    $tbl2=$_TABLES['DATABOX_category'] ;

    //カテゴリの指定がなければ全件、あればカテゴリ毎
    $exp="";
    if (is_null($category_id) AND is_null($category_code) ){
    }else{
        $ids="";
        $idsary=array();
        if (is_null($category_id) OR $category_id==0){
            if (is_null($category_code)){
            }else{
                $w=explode("|",$category_code);
                foreach( $w as $val ){
                    if ($ids<>""){
                        $ids.=",";
                    }
                    $w_id=DATABOX_codetoid($val,'DATABOX_def_category',"category_id");
                    $idsary[]=$w_id;
                    $ids.=$w_id;
                }
            }
        }else{
            $ids=$category_id;
            $idsary[]=$category_id;
        }
        if (strtoupper($mode)=="AND"){
            $w="";
            foreach( $idsary as $val ){
                if ($w<>""){
                    $w.=" AND ";
                }
                $w.=$val." IN (SELECT category_id";
                $w.=" FROM {$tbl2} AS t2 ";
                $w.=" WHERE t2.id = t1.id)";
            }
        }else{
            $w= "id IN (SELECT id FROM {$tbl2} ";
            $w.="WHERE category_id IN ({$ids})) ";
        }
        $exp=$w.LB;
    }
    
    //-----
    $sql = "SELECT ".LB;

    $sql .= " distinct t1.id ".LB;

    $sql .= " FROM ".LB;

    $sql .= " {$tbl1} AS t1 ".LB;

    $sql .= " WHERE ".LB;
    
    if ($exp<>""){
        $sql .= $exp. " AND ";
    }

    //@@@@@@@@@--------->
    //下書はのぞく
    $sql .= " t1.draft_flag=0".LB;
    //アクセス権のないデータ はのぞく
    if  (strtoupper($permission)=="IGNORE"){
    }else{
        $sql .= COM_getPermSql('AND').LB;
    }
    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())".LB;
    //公開終了日を過ぎたデータはのぞく
    $sql .= " AND (expired=0 OR expired > NOW())".LB;
    //@@@@@@@@@<---------

    $result = DB_query ($sql);
    $cnt = DB_numRows ($result);
    
    if  (strtoupper($numberformat)=="YES"){
        $rt=COM_NumberFormat($cnt);
    }else{
        $rt=$cnt;
    }
    
    return $rt;

}

function databox_field(
    $autotag
    ,$field_id=null
    ,$value=""
    ,$template=null
    ,$nohitmsg=""
    ,$perpage =0
    ,$page=null
    ,$order=null
    ,$field_code=null
)
// +---------------------------------------------------------------------------+
// | 機能  アトリビュート別一覧表示
// | 書式
// +---------------------------------------------------------------------------+
// | 引数 $autotag:notautotagの時ヘッダを表示しない
// | 引数 $field_id:追加属性id
// | 引数 $value:表示する項目の値
// | 引数 $template:使用するテンプレートのフォルダ名
// | 引数 $nohitmsg      :"yes"の時nohitメッセージ表示
// | 引数 $perpage       :1pageあたりの件数、0の時改ページなし
// | 引数 $page           :
// | 引数 $order         :順序
// | 引数 $field_code    :
// +---------------------------------------------------------------------------+
// | public_html/attribute.php および　自動タグで使用
// | ヘッダはコンフィギュレーション設定により表示されない場合があります
// | idもcodeも指定されない場合は、全カテゴリ
// +---------------------------------------------------------------------------+
// | 戻値 nomal:attribute list
// +---------------------------------------------------------------------------+
{

    $pi_name="databox";

    global $_CONF;
    global $_TABLES;
    global $_DATABOX_CONF;
    global $LANG_DATABOX;
    global $LANG_DATABOX_ADMIN;
    global $LANG_DATABOX_NOYES;
    global $_IMAGE_TYPE;
    global $LANG_confignames;
    global $_PLUGINS;
    global $_MAPS_CONF;

    //ログイン要否チェック
    if (COM_isAnonUser()){
        if  ($_CONF['loginrequired']
                OR ($_DATABOX_CONF['loginrequired'] >1) ){
            return $LANG_DATABOX['loginrequired'];
        }

    }
    //-----table assign
    $tbl1=$_TABLES['DATABOX_addition'] ;
    $tbl2=$_TABLES['DATABOX_base'] ;
    $tbl3=$_TABLES['DATABOX_addition'] ;
    $tbl5=$_TABLES['DATABOX_stats'];

    //日付関連
    $datefield=$_DATABOX_CONF['datefield'];//使用する日付（編集日付、作成日付、公開日）
    $new_img=$_DATABOX_CONF['new_img'];
    if ($new_img==""){
        $new_img="New!";
    }
    $newmarkday=$_DATABOX_CONF['newmarkday'];
    if ($newmarkday==""){
        $newmarkday=3;
    }
    $chkday=strtotime("- $newmarkday days",time());


    //-----引数チェック
    if (is_null($value) OR $value==="") {
        return;
    }
    if (is_null($field_id)){
        if ($field_code===""){
            $field_id=0;
        }else{
            $field_id=DATABOX_codetoid($field_code,'DATABOX_def_field',"field_id","templatesetvar");
        }
    }
    if ($field_id===0) {
        return;
    }
    if ($field_code===""){
        $field_code=DB_getItem( $_TABLES['DATABOX_def_field']
                 ,"templatesetvar","field_id={$field_id}");
    }
    
    $field_def=DATABOX_getadditiondef($pi_name);
    $type=$field_def[$field_id]['type'];
    $w = array(0,2,3,4,7,8,9,16);
    if (in_array ($type,$w)){
    }else{
        return;
    }

    //
    if (is_null($page) OR !isset($page) OR $page == 0) {
        $page = 1;
    }
    if ($perpage===0 OR is_null($perpage)){
        $perpage=$_DATABOX_CONF['perpage'];
    }
    
    $w=databox_orderby($datefield,$order,$orderby,$addfieldorder,$orderfield_id);
    
    $field_def[$field_id]['name'];
    $field_name=$field_def[$field_id]['name'];
    $field_templatesetvar=$field_def[$field_id]['templatesetvar'];
    $field_description=$field_def[$field_id]['description'];

    $fieldvalue=DATABOX_getfieldvalue(
          $value
         ,$field_def[$field_id]['type']
         ,$field_def[$field_id]['selectionary']
         ,$LANG_DATABOX_NOYES
         ,$field_def[$field_id]['selectlist']
         ,$pi_name
          );


    if (is_null($template) or ($template==="")){
        $template="default";
    }


    $sql = "SELECT DISTINCT";

    $sql .= " t2.id ".LB;
    $sql .= " ,t2.title ".LB;
    $sql .= " ,t2.code ".LB;
    $sql .= " ,t2.description ".LB;
    $sql .= " ,t2.".$datefield." AS datefield ".LB;
    $sql .= " ,t2.expired ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.".$datefield.") AS datefield_un ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.released ) AS released_un ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.expired ) AS expired_un ".LB;

    $sql .= " ,t2.group_id";
    $sql .= " ,t2.owner_id";
    
    $sql .= " ,t2.fieldset_id ".LB;
    
    if ($addfieldorder){
        $sql .= " ,t3.value ".LB;
    }

    $sql .= " FROM ".LB;
    $sql .= " {$tbl2} AS t2 ".LB;
    $sql .= " ,{$tbl1} AS t1 ".LB;
    if ($addfieldorder){
        $sql .= " ,{$tbl3} AS t3 ".LB;
    }

    $sql .= " WHERE ".LB;
    $sql .= " t1.field_id = ".$field_id.LB;
    $sql .= " AND t1.id = t2.id ".LB;
    $sql .= " AND t1.value = \"".$value."\"".LB;
    if ($addfieldorder){
        $sql .= " AND t3.field_id=".$orderfield_id.LB;
        $sql .= " AND t3.id=t2.id".LB;
    }


    //管理者の時,下書データも含む
    //if ( SEC_hasRights('databox.admin')) {
    //}else{
       $sql .= " AND t2.draft_flag=0".LB;
    //}
    
    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND',0,2,"t2");
    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())".LB;
    //公開終了日を過ぎたデータはのぞく
    $sql .= " AND (expired=0 OR expired > NOW())";

    $sql .= " ORDER BY ".LB;
    $sql .= $orderby.LB;
    
    $result = DB_query ($sql);
    $cnt = DB_numRows ($result);

    $pages = 0;
    if ($perpage > 0) {
        $pages = ceil($cnt / $perpage);
    }
    $offset = ($page - 1) * $perpage;
    $sql .= " LIMIT $offset, $perpage";

    //自動タグでない時　ヘッダ、左ブロック
    if ($autotag==="notautotag"){
        if ($page > 1) {
            $page_title = sprintf ('%s (%d)', $field_name, $page);
        } else {
            $page_title = sprintf ('%s ', $field_name);
        }
        $headercode=DATABOX_getheadercode(    
            "attribute"
            ,$template
            ,$pi_name
            ,$field_id
            ,$field_name
            ,$field_description
            ,$field_name
            ,$field_description);

        $retval .= DATABOX_siteHeader($pi_name,'',$page_title,$headercode);
    }

    
    //
    $addition_def=DATABOX_getadditiondef($pi_name);
    //
    $url=$_CONF['site_url']."/databox/attribute.php";
    $attribute_top="<a href='".$url."'>".$LANG_DATABOX['attribute_top']."</a>";
    $url=$_CONF['site_url']."/databox/attribute.php?";
    $url.="m=id&amp;id=".$field_id;
    $field_top="<a href='".$url."'>".$field_name.$LANG_DATABOX['countlist']."</a>";
    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);

    if ($numrows > 0) {
        $tmplfld=DATABOX_templatePath('attribute',$template,$pi_name);
        $templates = new Template($tmplfld);
        $templates->set_file (array (
            'list' => 'list_detail.thtml',
            'nav'   => 'navigation_detail.thtml',
            'row'   => 'row.thtml',
            'col'   => "col_detail.thtml",
            ));
        
        $languageid=COM_getLanguageId();
        $language= COM_getLanguage();
        $templates->set_var ('languageid', $languageid);
        $templates->set_var ('language', $language);
        if ($languageid<>"") {
            $templates->set_var ('_languageid', "_".$languageid);
        }else{
            $templates->set_var ('_languageid', "");
        }

        //
        $templates->set_var ('site_url',$_CONF['site_url']);
        $templates->set_var ('this_script',THIS_SCRIPT);

        //bread
        $templates->set_var ('home',$LANG_DATABOX['home']);
        $templates->set_var ('attribute_top',$attribute_top);
        $templates->set_var ('field_top',$field_top);
        
        $templates->set_var ('field_name',$field_name);
        $templates->set_var ('field_templatesetvar',$field_templatesetvar);
        $templates->set_var ('field_description',$field_description);
        
        $templates->set_var ('fieldvalue',$fieldvalue);

        $templates->set_var ('addition_name',$additon_name);
        $templates->set_var ('addition_code',$additon_code);

        //page
        $templates->set_var ('cnt', $cnt);
        $lin1=$offset+1;
        $lin2=$lin1+$perpage - 1;
        if ($lin2>$cnt){
            $lin2=$cnt;
        }

        //summary navigation
        $templates->set_var ('lang_view', $LANG_DATABOX['view']);
        $templates->set_var ('lin', $lin1."-".($lin2));
        $templates->set_var ('cnt', $cnt);

        $templates->set_var ('lang_code', $LANG_DATABOX_ADMIN['code']);
        $templates->set_var ('lang_id', $LANG_DATABOX_ADMIN['id']);

        //
        $templates->set_var ('lang_date', $LANG_DATABOX_ADMIN[$datefield]);
        $templates->set_var ('lang_title', $LANG_DATABOX_ADMIN['title']);
        $templates->set_var ('lang_description', $LANG_DATABOX_ADMIN['description']);
        $templates->set_var ('lang_released', $LANG_DATABOX_ADMIN['released']);
        $templates->set_var ('lang_expired', $LANG_DATABOX_ADMIN['expired']);
        $templates->set_var ('lang_remaingdays', $LANG_DATABOX_ADMIN['remaingdays']);
        $templates->set_var ('lang_addfield', $addfield_name);
        
        //$_DATABOX_CONF
        $templates->set_var('lang_imgfile_frd', $LANG_confignames['databox']['imgfile_frd']);
        $templates->set_var ('imgfile_frd', $_DATABOX_CONF['imgfile_frd']);
        $templates->set_var ('data_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_frd']);
        $templates->set_var('lang_imgfile_thumb_frd', $LANG_confignames['databox']['imgfile_thumb_frd']);
        $templates->set_var ('imgfile_thumb_frd', $_DATABOX_CONF['imgfile_thumb_frd']);
        $templates->set_var ('data_thumb_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_thumb_frd']);
        
        $mkids="";
        for ($i = 0; $i < $numrows; $i++) {
            $A = DB_fetchArray ($result);
            $A = array_map('stripslashes', $A);

            $title=COM_applyFilter($A['title']);
            $description=COM_applyFilter($A['description']);
            $code=COM_applyFilter($A['code']);
            $id=COM_applyFilter($A['id']);
            $datefield=COM_applyFilter($A['datefield']);
            $released=COM_applyFilter($A['released']);
            $expired=COM_applyFilter($A['expired']);
            $datefield_ary = COM_getUserDateTimeFormat($A['datefield_un']);
            $released_ary = COM_getUserDateTimeFormat($A['released_un']);
            if ($expired==="0000-00-00 00:00:00"){
                $expired_ary=array();
            }else{    
                $expired_ary = COM_getUserDateTimeFormat($A['expired_un']);
            }
            $curdate_ary =  COM_getUserDateTimeFormat();
            
            $sortvalue=COM_applyFilter($A['value']);
            $group_id = $A['group_id'];
            $owner_id = $A['owner_id'];
            
            $fieldset_id=COM_applyFilter($A['fieldset_id']);

            $rt=databox_detail_link($id,$code,$title);
            
            $templates->set_var ('data_link', $rt['link']);
            $templates->set_var ('data_title', $title);
            $templates->set_var ('data_code', $code);
            $templates->set_var ('data_description', $description);
            $templates->set_var ('data_id', $id);
            $templates->set_var ('data_url', $rt['url']);
            
            $templates->set_var ('data_datefield', $datefield_ary[0]);
            $templates->set_var ('data_value', $sortvalue);
            $templates->set_var ('data_datefield_shortdate', strftime( $_CONF['shortdate'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_shortdate', strftime( $_CONF['shortdate'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_date', strftime( $_CONF['date'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_daytime', strftime( $_CONF['daytime'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_dateonly', strftime( $_CONF['dateonly'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_timeonly', strftime( $_CONF['timeonly'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_b', strftime( "%b" , $A['datefield_un']));
            $templates->set_var ('data_datefield_B', strftime( "%B" , $A['datefield_un']));
            $templates->set_var ('data_datefield_d', strftime( "%d" , $A['datefield_un']));
            $templates->set_var ('data_datefield_e', strftime( "%e" , $A['datefield_un']));
            
            $templates->set_var ('data_released', $released_ary[0]);//@@@@@@
            $templates->set_var ('data_released_shortdate', strftime( $_CONF['shortdate'], $A['released_un'] ));
            $templates->set_var ('data_released_date', strftime( $_CONF['date'], $A['released_un'] ));
            $templates->set_var ('data_released_daytime', strftime( $_CONF['daytime'], $A['released_un'] ));
            $templates->set_var ('data_released_dateonly', strftime( $_CONF['dateonly'], $A['released_un'] ));
            $templates->set_var ('data_released_timeonly', strftime( $_CONF['timeonly'], $A['released_un'] ));
            $templates->set_var ('data_released_b', strftime( "%b" , $A['released_un']));
            $templates->set_var ('data_released_B', strftime( "%B" , $A['released_un']));
            $templates->set_var ('data_released_d', strftime( "%d" , $A['released_un']));
            $templates->set_var ('data_released_e', strftime( "%e" , $A['released_un']));
            
            //公開終了日 Expired to publish
            if ($A['expired'] ==="0000-00-00 00:00:00"){
                $templates->set_var ('data_expired', "");
                $templates->set_var ('data_expired_shortdate', "" );
                $templates->set_var ('data_expired_date', "" );
                $templates->set_var ('data_expired_daytime', "" );
                $templates->set_var ('data_expired_dateonly', "" );
                $templates->set_var ('data_expired_timeonly', "" );
                $templates->set_var ('data_expired_b', "" );
                $templates->set_var ('data_expired_B', "" );
                $templates->set_var ('data_expired_d', "" );
                $templates->set_var ('data_expired_e', "" );
            }else{
                $wary = COM_getUserDateTimeFormat($A['expired_un']);
                $templates->set_var ('data_expired', $expired_ary[0]);
                $templates->set_var ('data_expired_shortdate', strftime( $_CONF['shortdate'], $A['expired_un'] ));
                $templates->set_var ('data_expired_date', strftime( $_CONF['date'], $A['expired_un'] ));
                $templates->set_var ('data_expired_daytime', strftime( $_CONF['daytime'], $A['expired_un'] ));
                $templates->set_var ('data_expired_dateonly', strftime( $_CONF['dateonly'], $A['expired_un'] ));
                $templates->set_var ('data_expired_timeonly', strftime( $_CONF['timeonly'], $A['expired_un'] ));
                $templates->set_var ('data_expired_b', strftime( "%b" , $A['expired_un']));
                $templates->set_var ('data_expired_B', strftime( "%B" , $A['expired_un']));
                $templates->set_var ('data_expired_d', strftime( "%d" , $A['expired_un']));
                $templates->set_var ('data_expired_e', strftime( "%e" , $A['expired_un']));
            }
            
            $remaingdays="";
            if ($expired<>"0000-00-00 00:00:00") {
                if  ($expired_ary[1]>=$curdate_ary[1]){
                    $remaingdays=COM_dateDiff( "d", $expired_ary[1], $curdate_ary[1] ) + 1;
                }
            }
            $templates->set_var ('data_remaingdays', $remaingdays);//@@@@@@
            if (date("Ymd",strtotime($datefield)) >= date("Ymd",$chkday)){
                $templates->set_var ('new_img', $new_img);
            }else{
                $templates->set_var ('new_img', '');
            }
            
            $hits=COM_applyFilter(DB_getItem( $tbl5 ,"hits","id={$id}"),true);
            $templates->set_var('lang_hits', $LANG_DATABOX_ADMIN['hits']);
            $templates->set_var('data_hits', $hits);

            //カテゴリ@@@@@
            $templates->set_var('lang_category', $LANG_DATABOX_ADMIN['category']);
            DATABOX_getcategoriesDisp($A['id'],$templates,$chk_user,0,$pi_name);

            //追加項目
            $group_id = stripslashes($A['group_id']);
            $owner_id = stripslashes($A['owner_id']);
            $chk_user=DATABOX_chkuser($group_id,$owner_id,"databox.admin");
            $additionfields = DATABOX_getadditiondatas($id,$pi_name);
            DATABOX_getaddtionfieldsDisp($additionfields,$addition_def,$templates,$chk_user,$pi_name,$fieldset_id);
            
            //管理者の時「編集」
            if ( SEC_hasRights('databox.admin')) {
                $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                $attr = array('title' => $title." ".$LANG_DATABOX_ADMIN['edit']);
                $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);
                
                $attr = array('class' => 'editlink', 'title' => $title." ".$LANG_DATABOX_ADMIN['edit']);
                $url = $_CONF['site_admin_url'];
                $url .= '/plugins/databox/data.php';
                $url .= '?mode=edit';
                $url .= '&amp;'."id={$id}";
                $icon = '&nbsp;' ;
                $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );
                $templates->set_var ('data_edit', $icon);
            }else{
                $rt=databox_chk_loaddata($id);
                //編集権限のあるMyData
                if ( $rt==="OK") {
                    $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                    $attr = array('title' => $LANG_DATABOX_ADMIN['edit']);
                    $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);

                    $attr = array('class' => 'editlink', 'title' => $title." ".$LANG_DATABOX_ADMIN['edit']);

                    $url = $_CONF['site_url'];
                    $url .= '/databox/mydata/data.php';
                    $url .= '?mode=edit';
                    $url .= '&amp;'."id={$id}";

                    $icon = '&nbsp;' ;
                    $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );

                    $templates->set_var ('data_edit', $icon);
                }else{
                    $templates->set_var ('data_edit', "");
                }
                
            }
            //maps plugin link
            $mkid="";
            if (in_array("maps", $_PLUGINS)){
                if ($code<>""){
                    $mkid=DB_getItem($_TABLES['maps_markers'],"mkid","item_10='$code'");
                    $mkids.=$mkid." ";
                }
            }

            //=====
            $templates->parse ('col_var', 'col', true);
            $templates->parse ('row_var', 'row', true);

            $templates->set_var ('col_var', '');

        }
        $mkids=rtrim($mkids," ");
        $mkidary=array();
        $mkid_ary=split(" " , $mkids);
        $selectedMarkers="";
        if (function_exists("MAPS_selectedMarkers") AND $mkids<>"") {
            $selectedMarkers=MAPS_selectedMarkers(
              $_MAPS_CONF['map_width']
             ,$_MAPS_CONF['map_height']
             ,$_MAPS_CONF['map_zoom']
             ,$mkid_ary);
        }
        $templates->set_var ('mkids', $mkids);
        $templates->set_var ('selectedMarkers', $selectedMarkers);

        //------------
        //-----navigation
        $url = $_CONF['site_url']  . '/';
        if ($autotag==="notautotag"){
            $url.=THIS_SCRIPT;
        }else{
            $url.="databox/attribute.php";
        }
        $url .= "?m=id";
        $url .= "&amp;id=".$field_id;
        $url .= "&amp;value=".$value;
        $url .= "&amp;template=".$template;
        $url .= "&amp;perpage=".$perpage;
        $url .= "&amp;order=";

        //-----order navigation
        databox_order($url, $templates, $order) ;
        //-----page navigation
        $url .= $order;
        $templates->set_var ('page_navigation',
                COM_printPageNavigation ($url, $page, $pages));
        if ($order==="random"){
            $templates->set_var ( 'pagenavinone', 'style="display:none;"' );
        }else{
            $templates->set_var ( 'pagenavinone', '' );
        }
        // Call to plugins to set template variables in the databox
        PLG_templateSetVars( 'databox', $templates );

        //------------
        $templates->parse ('nav_var', 'nav', true);

        $templates->set_var ('msg', "");

        $templates->parse ('output', 'list');

        $content = $templates->finish ($templates->get_var ('output'));
        $retval .=$content;

    }else{

        if ($nohitmsg==="yes"){
            $tmplfld=DATABOX_templatePath('attribute',$template,$pi_name);
            $templates = new Template($tmplfld);
            $templates->set_file (array (
                'list' => 'nohit.thtml',
            ));
            
            //bread
            $templates->set_var ('home',$LANG_DATABOX['home']);
            $templates->set_var ('attribute_top',$attribute_top);
            $templates->set_var ('field_top',$field_top);
            
            $templates->set_var ('field_name',$field_name);
            $templates->set_var ('field_templatesetvar',$templatesetvar);
            $templates->set_var ('field_description',$field_description);
            
            $templates->set_var ('fieldvalue',$fieldvalue);
                        
            $templates->set_var('xhtml', XHTML);
            $templates->set_var('site_url', $_CONF['site_url']);
            $templates->set_var('site_admin_url', $_CONF['site_admin_url']);
            $templates->set_var('layout_url', $_CONF['layout_url']);
            
            $templates->set_var ('lang_nohit', $LANG_DATABOX['nohit']);

            $templates->parse ('output', 'list');
            $content = $templates->finish ($templates->get_var ('output'));
            $retval .=$content;

        }
    }

    $retval =PLG_replacetags ($retval);

    return $retval;
}
function databox_orderby(
    $datefield
    ,&$order
    ,&$orderby
    ,&$addfieldorder
    ,&$field_id
)
// +---------------------------------------------------------------------------+
// | 機能  order orderby addfieldorder $field_id を編集
// | 書式  databox_orderby($order,$orderby,$addfieldorder,$field_id);
// +---------------------------------------------------------------------------+
// | 引数 $datefield
// | 引数戻値 $order
// | 引数戻値 $orderby
// | 引数戻値 $addfieldorder
// | 引数戻値 $field_id
// +---------------------------------------------------------------------------+

{

    global $_TABLES;

    $orderby="";
    $addfieldorder=false;
    switch($order){
        case "random":
            $orderby =" RAND()";
            break;
        case "":
        case "date":
            $order="date";
            $orderby = $datefield ." DESC";
            break;
        case "released":
            $orderby = " released DESC";
            break;
        case "orderno":
        case "code":
        case "title":
        case "description":
        case "id":
            $orderby = " t2.".$order;
            break;
        default:
            $sql="SELECT ";
            $sql.= " field_id ";
            $sql.= ",name ";
            $sql.=" FROM";
            $sql.=" {$_TABLES['DATABOX_def_field']} ";
            $sql.=" WHERE templatesetvar='".$order."'";

            $result = DB_query ($sql);
            $numrows = DB_numRows ($result);

            if ($numrows>0){
                $A = DB_fetchArray ($result);
                $field_id=COM_stripslashes($A['field_id']);
                $addfield_name=COM_stripslashes($A['name']);
                $orderby = " t3.value IS NULL ASC ,cast(t3.value as DECIMAL) ASC ";
                $addfieldorder=true;
            }else{
                $order="date";
                $orderby = $datefield ." DESC";
            }
    }

    return ;

}



// +---------------------------------------------------------------------------+
// | 機能  集計表示(max min sum)
// +---------------------------------------------------------------------------+
// | 書式  databox_grp()
// | 書式  databox_grp(1)
// | 書式  databox_grp("japan")
// +---------------------------------------------------------------------------+
// | 引数 $field:項目　デフォルトmodified
// | 引数 $function:関数　デフォルトmax
// | 引数 $format:書式
// | 引数 $category_id:カテゴリid　（優先）
// | 引数 $category_code:カテゴリコード
// +---------------------------------------------------------------------------+
// | 戻値 nomal:データ総数、カテゴリ別データ件数
// +---------------------------------------------------------------------------+

function databox_grp(
    $field=null
    ,$function=null
    ,$format=null
    ,$category_id =null
    ,$category_code =null
)
{
    global $_TABLES;
    global $LANG_DATABOX;

    $rt="";

    if (is_null($field) ){
        $field="id";
    }
    if (is_null($function) ){
        $function="max";
    }

    if (is_null($category_id) AND is_null($category_code) ){
        $m="ALL";
    }else{
        if (is_null($category_id) ){
            $category_id=DATABOX_codetoid($category_code,'DATABOX_def_category',"category_id");
        }
        $m="CATEGORY";
    }
    //-----
    $tbl1=$_TABLES['DATABOX_base'] ;
    $tbl2=$_TABLES['DATABOX_category'] ;

    //-----
    $sql=LB;
    $sql .= "SELECT ".LB;

    $sql .= " {$function}(t1.{$field}) AS rt".LB;

    $sql .= " FROM ".LB;

    $sql .= " {$tbl1} AS t1 ".LB;

    if ($m==="ALL"){
    }else{
        $sql .= " ,{$tbl2} AS t2 ".LB;
    }

    $sql .= " WHERE ".LB;

    //@@@@@@@@@--------->
    //下書はのぞく
    $sql .= " t1.draft_flag=0".LB;
    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND').LB;
    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())".LB;
    //公開終了日を過ぎたデータはのぞく
    $sql .= " AND (expired=0 OR expired > NOW())".LB;
    //@@@@@@@@@<---------

    if ($m==="ALL"){
    }else{
        $sql .= " AND t1.id = t2.id ".LB;
        $sql .= " AND t2.category_id= ".$category_id .LB;
    }

    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);


    if ($numrows > 0) {

        $A = DB_fetchArray ($result);

        $rt=$A['rt'];
        if ($rt<>""){
            switch($format){
                case "lastmodified":// '%Y年%m%e日更新';
                case "lastcreated"://'%Y年%m%e日追加';
                    $fm=$LANG_DATABOX[$format];
                    $rt=strftime($fm,strtotime($rt));
                    break;
                default:
                    break;
            }
        }
    }

    return $rt;

}



function databox_data(
    $id=null
    ,$template=null
    ,$nohitmsg=""
    ,$m=""
    ,$code=null
)
// +---------------------------------------------------------------------------+
// | 機能  個別データ表示
// | 書式  databox_data($id,$template,"yes","page")
// | 書式  databox_data($p['id'],$p['thtml'],$p['nohitmsg'],$p['code']
// +---------------------------------------------------------------------------+
// | 引数 $id            :
// | 引数 $template      :テンプレートのdirectory
// | 引数 $nohitmsg      :"yes"の時nohitメッセージ表示
// | 引数 $m             :"page"の時は　レイアウト設定
// | 引数 $code          :
// +---------------------------------------------------------------------------+
// | public_html/data.php および　自動タグで使用
// | ヘッダはコンフィギュレーション設定により表示されない場合があります
// | idもcodeも指定されない場合は、直近に変更または追加されたデータを返します
// +---------------------------------------------------------------------------+
// | 戻値 nomal:data
// +---------------------------------------------------------------------------+
{

    $pi_name="databox";

    global $_CONF;
    global $_TABLES;
    global $_DATABOX_CONF;
    global $LANG_DATABOX;
    global $LANG_DATABOX_ADMIN;
    global $_IMAGE_TYPE;
    global $_USER;
    global $LANG_ACCESS;
    global $LANG_confignames;
    global $_PLUGINS;
    
    $layout=$_DATABOX_CONF['layout'];
    
    //ログイン要否チェック
    if (COM_isAnonUser()){
        if  ($_CONF['loginrequired']
                OR ($_DATABOX_CONF['loginrequired'] >0) ){
            return $LANG_DATABOX['loginrequired'];
        }

    }

    //-----

    if (is_null($id)  OR ($id==0)){
        if ($code<>""){
            if (substr($code,0,4)==="@fb_"){
                if (function_exists('assistfb_getNestAutotag')) {
                    $code=assistfb_getNestAutotag(substr($code,4));
                }
            }    
        }
        if ($code<>""){
            $id=DATABOX_codetoid($code,'DATABOX_base',"id");
        }
    }
	
    //-----
    $display = '';

    $tbl=$_TABLES['DATABOX_base'] ;
    $tbl2=$_TABLES['users'] ;
    $tbl4=$_TABLES['DATABOX_def_fieldset'];
    $tbl5=$_TABLES['DATABOX_stats'];
    
    $addition_def=DATABOX_getadditiondef($pi_name);

    //-----
    $sql = "SELECT t.* ";
    
    $sql .= " ,UNIX_TIMESTAMP(t.modified) AS modified_un";
    $sql .= " ,UNIX_TIMESTAMP(t.released) AS released_un";
    $sql .= " ,UNIX_TIMESTAMP(t.comment_expire) AS comment_expire_un";
    $sql .= " ,UNIX_TIMESTAMP(t.expired) AS expired_un";
    $sql .= " ,UNIX_TIMESTAMP(t.created) AS created_un";
    $sql .= " ,UNIX_TIMESTAMP(t.udatetime) AS udatetime_un";
    
    $sql .= " ,t.fieldset_id";
    
    $sql .= " ,t2.username AS owner_name";
    $sql .= " ,t3.username AS last_modify_name";
    $sql .= " ,t3.fullname AS last_modify_fullname";
    
    $sql .= " ,t4.name AS set_name";
    $sql .= " ,t4.description AS set_description";
    $sql .= " ,t4.defaulttemplatesdirectory AS set_defaulttemplatesdirectory";
    $sql .= " ,t4.layout AS set_layout";
    
    $sql .= " ,t5.hits";
    
    $sql .= " FROM ";
    $sql .= " {$tbl} AS t ";//base
    $sql .= " JOIN {$tbl4} AS t4       ON t.fieldset_id=t4.fieldset_id";
    $sql .= " JOIN {$tbl2} AS t2       ON t.owner_id=t2.uid";//owner
    $sql .= " LEFT JOIN {$tbl5} AS t5  ON t.id=t5.id";
    $sql .= " LEFT JOIN  {$tbl2} AS t3 ON t.uuid=t3.uid";//last_updateowner

    $sql .= " WHERE ";
    if (is_null($id)){//@@@@@
        $sql .= "1=1";
    }else{
        $sql .= " t.id=".$id;
    }


    //管理者の時
    if ( SEC_hasRights('databox.admin') OR $m==="view") {
    }else{
        //下書データはのぞく
        $sql .= " AND t.draft_flag=0".LB;
        //公開日以前のデータはのぞく
        $sql .= " AND (released <= NOW())";
        //公開終了日を過ぎたデータはのぞく
        $sql .= " AND (expired=0 OR expired > NOW())";
    }
    
    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND');
    

    if (is_null($id)){
        $sql .= " ORDER BY ".$_DATABOX_CONF['datefield'] . " DESC ";
        $sql .= " LIMIT 0 , 1";
    }
    
    
    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);
    
    if  ($m==="page"  OR  $m==="view"){
        if ($numrows <= 0) {
            COM_handle404();
            exit;
        }
    }

    if ($numrows > 0) {
        $A = DB_fetchArray ($result);
        $A = array_map('stripslashes', $A);

        $cache_time=$A['cache_time'];
        $cache=false;
        if  ($cache_time===0){
        }else{
            if  ($m==="page"){
                $cacheInstance = 'databox__' . $id .'__' . CACHE_security_hash();
                $rt = CACHE_check_instance($cacheInstance."_title");
                if ($rt  AND $cache_time == -1) {
                    $cache=true;
                } elseif ($cache AND $cache_time > 0) {        
                    $lu = CACHE_get_instance_update($cacheInstance."_title");
                    $now = time();
                    if (($now - $lu) < $cache_time ){
                         $cache=true;
                    }
                }
            }
        }
        
        
        if  ($cache){
            $display = CACHE_check_instance($cacheInstance."_display",$display);
            $layout = CACHE_check_instance($cacheInstance."_layout",$layout);
            $headercode = CACHE_check_instance($cacheInstance."_headercode",$headercode);
            $title = CACHE_check_instance($cacheInstance."_title",$title);
        }else{
            if (is_null($template) or ($template==="")){
                if ($A['defaulttemplatesdirectory']<>""){
                    $template=$A['defaulttemplatesdirectory'];
                }elseif  ($A['set_defaulttemplatesdirectory']<>""){
                    $template=$A['set_defaulttemplatesdirectory'];
                }else{
                    $template="default";
                }
            }

            $code=$A['code'];
            $group_id = $A['group_id'];
            $owner_id = $A['owner_id'];
        
            $fieldset_id= $A['fieldset_id'];
            //
            if ($m==="view"){    
                $chk_user=1000;//ログインユーザ
            }else{
                $chk_user=DATABOX_chkuser($group_id,$owner_id,"databox.admin");
            }

            $id=$A['id'];
            $additionfields = DATABOX_getadditiondatas($id,$pi_name);

            //ヘッダ、左ブロック start.............
            if ($m==="page"){
                if  ($A['set_layout']<>"0") {
                    $layout=$A['set_layout'];
                }
                // Meta Tags
                $headercode=DATABOX_getheadercode(    
                    "data"
                    ,$template
                    ,$pi_name
                    ,$A['id']
                    ,$A['title']
                    ,$A['meta_description']
                    ,$A['meta_keywords']
                    ,$A['description']
                    ,$additionfields 
                    ,$addition_def 
                    ,$chk_user 
                    ,$fieldset_id 
                    );
            
                if  ($A['page_title']<>""){
                    $title = $A['page_title'] ;
                }else{    
                    $title = $A['title'] ;
                }
            }


            $tmplfld=DATABOX_templatePath('data',$template,$pi_name);
            $templates = new Template($tmplfld);
            $templates->set_file (array (
                    'list' => 'list_detail.thtml',
                    'row'   => 'row.thtml',
                    'col'   => "col_detail.thtml",
                    ));
        
            $languageid=COM_getLanguageId();
            $language= COM_getLanguage();
            $templates->set_var ('languageid', $languageid);
            $templates->set_var ('language', $language);
            if ($languageid<>"") {
                $templates->set_var ('_languageid', "_".$languageid);
            }else{
                $templates->set_var ('_languageid', "");
            }
        
            $templates->set_var ('site_admin_url', $_CONF['site_admin_url']);
            $templates->set_var ('site_url',$_CONF['site_url']);
            //
            $templates->set_var('xhtml', XHTML);
            $templates->set_var('layout_url', $_CONF['layout_url']);
        
            $templates->set_var ('site_name', $_CONF['site_name']);
            $templates->set_var ('site_mail', $_CONF['site_mail']);
        
            $currenturl= COM_getCurrentURL();
            $templates->set_var ('currenturl', $currenturl);
            //facebook
            $facebook_consumer_key = trim($_CONF['facebook_consumer_key']);
            $templates->set_var ('facebook_consumer_key', $facebook_consumer_key);
            //
        
            //daft_flag管理者の時
            if ($A['draft_flag']==1){
                $templates->set_var ('draft', "draft!");
            }else{
                $templates->set_var ('draft', "");
            }
            //------------

            //id
            $templates->set_var('lang_id', $LANG_DATABOX_ADMIN['id']);
            $templates->set_var ('id', $A['id']);

            $templates->set_var('lang_fields', $LANG_DATABOX_ADMIN['fields']);
            $templates->set_var('lang_field', $LANG_DATABOX_ADMIN['field']);
            $templates->set_var('lang_templatesetvar', $LANG_DATABOX_ADMIN['templatesetvar']);
            $templates->set_var('lang_content', $LANG_DATABOX_ADMIN['content']);
            $templates->set_var('lang_type', $LANG_DATABOX_ADMIN['type']);
            $templates->set_var('lang_byusingid', $LANG_DATABOX_ADMIN['byusingid']);
            $templates->set_var('lang_byusingcode', $LANG_DATABOX_ADMIN['byusingcode']);
            $templates->set_var('lang_byusingtemplatesetvar', $LANG_DATABOX_ADMIN['byusingtemplatesetvar']);
            $templates->set_var('lang_group', $LANG_DATABOX_ADMIN['group']);
            $templates->set_var('lang_withlink', $LANG_DATABOX_ADMIN['withlink']);
            $templates->set_var('lang_groupbygroup', $LANG_DATABOX_ADMIN['groupbygroup']);
        
            //$_DATABOX_CONF
            $templates->set_var('lang_imgfile_frd', $LANG_confignames['databox']['imgfile_frd']);
            $templates->set_var ('imgfile_frd', $_DATABOX_CONF['imgfile_frd']);
            $templates->set_var ('data_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_frd']);
            $templates->set_var('lang_imgfile_thumb_frd', $LANG_confignames['databox']['imgfile_thumb_frd']);
            $templates->set_var ('imgfile_thumb_frd', $_DATABOX_CONF['imgfile_thumb_frd']);
            $templates->set_var ('data_thumb_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_thumb_frd']);
        
            //基本項目
            $templates->set_var('lang_basicfields', $LANG_DATABOX_ADMIN['basicfields']);
            //コード＆タイトル＆説明
            $templates->set_var('lang_code', $LANG_DATABOX_ADMIN['code']);
            $templates->set_var ('code', $A['code']);
            $templates->set_var('lang_title', $LANG_DATABOX_ADMIN['title']);
            $templates->set_var ('title',$A['title']);
            $templates->set_var('lang_description', $LANG_DATABOX_ADMIN['description']);
            $templates->set_var ('description',$A['description']);

            //@@@@@
            $templates->set_var('lang_owner', $LANG_ACCESS['owner']);
            $templates->set_var ('owner_name',$A['owner_name']);
            $templates->set_var ('owner_id',$A['owner_id']);

            //hits
            $templates->set_var('lang_hits', $LANG_DATABOX_ADMIN['hits']);
            $templates->set_var ('hits', $A['hits']);
            //カテゴリ@@@@@
            $templates->set_var('lang_category', $LANG_DATABOX_ADMIN['category']);
            DATABOX_getcategoriesDisp($A['id'],$templates,$chk_user,0,$pi_name);

            //追加項目
            $templates->set_var('lang_additionfields', $LANG_DATABOX_ADMIN['additionfields']);
            DATABOX_getaddtionfieldsDisp($additionfields,$addition_def,$templates,$chk_user,$pi_name,$fieldset_id);

            //編集日付
            $templates->set_var('lang_modified', $LANG_DATABOX_ADMIN['modified']);
            $wary = COM_getUserDateTimeFormat($A['modified_un']);
            $templates->set_var ('modified',$wary[0]);
            $templates->set_var ('modified_shortdate', strftime( $_CONF['shortdate'], $A['modified_un'] ));
            $templates->set_var ('modified_date', strftime( $_CONF['date'], $A['modified_un'] ));
            $templates->set_var ('modified_daytime', strftime( $_CONF['daytime'], $A['modified_un'] ));
            $templates->set_var ('modified_dateonly', strftime( $_CONF['dateonly'], $A['modified_un'] ));
            $templates->set_var ('modified_timeonly', strftime( $_CONF['timeonly'], $A['modified_un'] ));
            $templates->set_var ('modified_b', strftime( "%b" , $A['modified_un']));
            $templates->set_var ('modified_B', strftime( "%B" , $A['modified_un']));
            $templates->set_var ('modified_d', strftime( "%d" , $A['modified_un']));
            $templates->set_var ('modified_e', strftime( "%e" , $A['modified_un']));
            //作成日付
            $templates->set_var('lang_created', $LANG_DATABOX_ADMIN['created']);
            $wary = COM_getUserDateTimeFormat($A['created_un']);
            $templates->set_var ('created', $wary[0]);
            $templates->set_var ('created_shortdate', strftime( $_CONF['shortdate'], $A['created_un'] ));
            $templates->set_var ('created_date', strftime( $_CONF['date'], $A['created_un'] ));
            $templates->set_var ('created_daytime', strftime( $_CONF['daytime'], $A['created_un'] ));
            $templates->set_var ('created_dateonly', strftime( $_CONF['dateonly'], $A['created_un'] ));
            $templates->set_var ('created_timeonly', strftime( $_CONF['timeonly'], $A['created_un'] ));
            $templates->set_var ('created_b', strftime( "%b" , $A['created_un']));
            $templates->set_var ('created_B', strftime( "%B" , $A['created_un']));
            $templates->set_var ('created_d', strftime( "%d" , $A['created_un']));
            $templates->set_var ('created_e', strftime( "%e" , $A['created_un']));
            //公開日
            $templates->set_var('lang_released', $LANG_DATABOX_ADMIN['released']);
            $wary = COM_getUserDateTimeFormat($A['released_un']);
            $templates->set_var ('released', $wary[0]);
            $templates->set_var ('released_shortdate', strftime( $_CONF['shortdate'], $A['released_un'] ));
            $templates->set_var ('released_date', strftime( $_CONF['date'], $A['released_un'] ));
            $templates->set_var ('released_daytime', strftime( $_CONF['daytime'], $A['released_un'] ));
            $templates->set_var ('released_dateonly', strftime( $_CONF['dateonly'], $A['released_un'] ));
            $templates->set_var ('released_timeonly', strftime( $_CONF['timeonly'], $A['released_un'] ));
            $templates->set_var ('released_b', strftime( "%b" , $A['released_un']));
            $templates->set_var ('released_B', strftime( "%B" , $A['released_un']));
            $templates->set_var ('released_d', strftime( "%d" , $A['released_un']));
            $templates->set_var ('released_e', strftime( "%e" , $A['released_un']));
            //公開終了日
            $templates->set_var('lang_expired', $LANG_DATABOX_ADMIN['expired']);
            if ($A['expired'] ==="0000-00-00 00:00:00"){
                $templates->set_var ('expired', "");
                $templates->set_var ('expired_shortdate', "" );
                $templates->set_var ('expired_date', "" );
                $templates->set_var ('expired_daytime', "" );
                $templates->set_var ('expired_dateonly', "" );
                $templates->set_var ('expired_timeonly', "" );
                $templates->set_var ('expired_b', "" );
                $templates->set_var ('expired_B', "" );
                $templates->set_var ('expired_d', "" );
                $templates->set_var ('expired_e', "" );
            }else{
                $wary = COM_getUserDateTimeFormat($A['expired_un']);
                $templates->set_var ('expired', $wary[0]);
                $templates->set_var ('expired_shortdate', strftime( $_CONF['shortdate'], $A['expired_un'] ));
                $templates->set_var ('expired_date', strftime( $_CONF['date'], $A['expired_un'] ));
                $templates->set_var ('expired_daytime', strftime( $_CONF['daytime'], $A['expired_un'] ));
                $templates->set_var ('expired_dateonly', strftime( $_CONF['dateonly'], $A['expired_un'] ));
                $templates->set_var ('expired_timeonly', strftime( $_CONF['timeonly'], $A['expired_un'] ));
                $templates->set_var ('expired_b', strftime( "%b" , $A['expired_un']));
                $templates->set_var ('expired_B', strftime( "%B" , $A['expired_un']));
                $templates->set_var ('expired_d', strftime( "%d" , $A['expired_un']));
                $templates->set_var ('expired_e', strftime( "%e" , $A['expired_un']));
             }
        
            $templates->set_var ('lang_uuid', $LANG_DATABOX_ADMIN['uuid']);
            $templates->set_var ('uuid', $uuid);
        
            $templates->set_var ('lang_udatetime', $LANG_DATABOX_ADMIN['udatetime']);
            $wary = COM_getUserDateTimeFormat($A['udatetime_un']);
            $templates->set_var ('udatetime', $wary[0]);
            $templates->set_var ('last_modify_uid', $A['uuid']);
        
            $templates->set_var ('last_modify_name', $A['last_modify_name']);
            $templates->set_var ('last_modify_fullname', $A['last_modify_fullname']);
        
            //maps plugin link
            $mkid="";
            $marker="";
            if (in_array("maps", $_PLUGINS)){
                $mkid=DB_getItem($_TABLES['maps_markers'],"mkid","item_10='$code'");
                if (! empty($mkid)) {
                    $sql = "SELECT * FROM {$_TABLES['maps_markers']} WHERE mkid = {$mkid} LIMIT 1";
                    $res = DB_query($sql, 0);
                    $AM = DB_fetchArray($res);
                    if (!SEC_hasAccess2($AM)) {
                    } else {
                         $marker = MAPS_getMarkerDetail($AM['mid'], $mkid);
                    }
                }
            }
            $templates->set_var ('mkid', $mkid);
            $templates->set_var ('marker', $marker);
        
            //管理者の時「編集」
            if ($m<>"view"){
                if ( SEC_hasRights('databox.admin')) {
                    $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                    $attr = array('title' => $A['title']." ".$LANG_DATABOX_ADMIN['edit']);
                    $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);
                
                    $attr = array('class' => 'editlink', 'title' => $A['title']." ".$LANG_DATABOX_ADMIN['edit']);
                    $url = $_CONF['site_admin_url'];
                    $url .= '/plugins/databox/data.php';
                    $url .= '?mode=edit';
                    $url .= '&amp;'."id={$id}";
                    $icon = '&nbsp;' ;
                    $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );
                    $templates->set_var ('edit', $icon);
                }else{
                    $rt=databox_chk_loaddata($id);
                    //編集権限のあるMyData
                    if ( $rt==="OK") {
                        $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                        $attr = array('title' => $LANG_DATABOX_ADMIN['edit']);
                        $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);

                        $attr = array('class' => 'editlink', 'title' => $A['title']." ".$LANG_DATABOX_ADMIN['edit']);

                        $url = $_CONF['site_url'];
                        $url .= '/databox/mydata/data.php';
                        $url .= '?mode=edit';
                        $url .= '&amp;'."id={$id}";

                        $icon = '&nbsp;' ;
                        $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );

                        $templates->set_var ('edit', $icon);
                    }else{
                        $templates->set_var ('edit', "");
                    }
                }
            } 
            // Call to plugins to set template variables in the databox
            PLG_templateSetVars( 'databox', $templates );

            if (function_exists('databox_custom_templateSetVars')) {
                databox_custom_templateSetVars("data", $templates);
            }
        
            //------------
            $templates->parse ('output', 'list');

            $databox_content = $templates->finish ($templates->get_var ('output'));
            $display=$databox_content;

            if ($A['draft_flag']!==1){
                if (!SEC_hasRights('databox.admin')) {
                    $id=$A['id'];
                    $hits=DB_getItem($_TABLES['DATABOX_stats'],"hits","id={$id}");
                    if (empty($hits)) {
                        $hits=0;
                    }
                    $fields.="id,hits ";
                    $values.="$id,$hits + 1";
                    DB_save($_TABLES['DATABOX_stats'],$fields,$values);
                }
            }
        } 
    }else{
        $cache_time=0;
        if ($nohitmsg==="yes"){
            $tmplfld=DATABOX_templatePath('data',$template,$pi_name);
            $templates = new Template($tmplfld);
            $templates->set_file (array (
                'list' => 'nohit.thtml',
            ));
            $templates->set_var ('lang_nohit', $LANG_DATABOX['nohit']);
            $templates->set_var('xhtml', XHTML);
            $templates->set_var('site_url', $_CONF['site_url']);
            $templates->set_var('site_admin_url', $_CONF['site_admin_url']);
            $templates->set_var('layout_url', $_CONF['layout_url']);

            $templates->parse ('output', 'list');
            $databox_content = $templates->finish ($templates->get_var ('output'));
            $display=$databox_content;
        }
    }
    $display =PLG_replacetags ($display);
    
    if  (($cache_time > 0 OR $cache_time == -1)  AND  $m==="page" ){
        CACHE_create_instance($cacheInstance."_display", $display); 
        CACHE_create_instance($cacheInstance."_layout", $layout); 
        CACHE_create_instance($cacheInstance."_headercode", $headercode); 
        CACHE_create_instance($cacheInstance."_title", $title); 
    }
    
    $retval['display']=$display;
    $retval['layout']=$layout;
    $retval['headercode']=$headercode;
    $retval['title']=$title;
    
    return $retval;
}

//
function databox_chk_loaddata(
    $id
){
    $pi_name="databox";

    global $_TABLES;
    global $LANG_DATABOX_ADMIN;


    $rt="";

    $sql = "SELECT ";
    $sql .= " id";

    $sql .= " ,owner_id";
    $sql .= " ,group_id";
    $sql .= " ,perm_owner";
    $sql .= " ,perm_group";
    $sql .= " ,perm_members";
    $sql .= " ,perm_anon";

    $sql .= " FROM ";
    $sql .= " {$_TABLES['DATABOX_base']} AS t";
    $sql .= " WHERE ";

    $sql .= " id=".$id;

    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);

    if ($numrows>0){

        $A = DB_fetchArray($result);

        if (!isset($A['owner_id'])) {
             $A['owner_id'] = 1;
        }


        $access = SEC_hasAccess(
                            $A['owner_id']
                            , $A['group_id']
                            , $A['perm_owner']
                            , $A['perm_group']
                            , $A['perm_members']
                            , $A['perm_anon']);
        if ($access >2) {
            $rt="OK";
        }else{
            $rt=$LANG_DATABOX_ADMIN['err_permission_denied'];
        }
    }else{
        $rt=$LANG_DATABOX_ADMIN['err_invalid'];
    }


    return $rt;
}    
//$rt=databox_createdatabycode("aaa");
function databox_createdatabycode(
    $code
    ,$title=""
){
    $pi_name="databox";

    global $_TABLES;
    global $LANG_DATABOX_ADMIN;
    global $_DATABOX_CONF;
    global $_USER;
    
    $rt=array();
    $rt['st']="";
    $rt['id']=0;
    
    $id=DB_getItem($_TABLES['DATABOX_base'],"id","code='{$code}'");
    if ($id<>""){
        $rt['st']="ok";
        $rt['id']=$id;
        return $rt;
    };

    if ($_DATABOX_CONF['allow_data_insert'] 
            OR SEC_hasRights('databox.submit')){
    }else{
         $rt['st']=$LANG_DATABOX_ADMIN['err_permission_denied'];
         return $rt;
    }

    $uuid=$_USER['uid'];    
    if ($uuid===""){
        $uuid=0;
    }
    $uid=$_USER['uid'];


    $w=DB_getItem($_TABLES['DATABOX_base'],"max(id)","1=1");
    if ($w=="") {
        $w=0;
    }
    $id=$w+1;
    if  ($title==""){
        $title=$code;
    }
    $group_id =$_DATABOX_CONF['grp_id_default'];//@@@@@20110613

    $array = array();
    SEC_setDefaultPermissions($array, $_DATABOX_CONF['default_perm']);
    $perm_owner = $array['perm_owner'];
    $perm_group = $array['perm_group'];
    $perm_anon = $array['perm_anon'];
    $perm_members = $array['perm_members'];

    $sql=LB;
    $sql.="INSERT INTO {$_TABLES['DATABOX_base']} (".LB;
    $sql.=" `id`,`owner_id`,`modified`,`created`,`released`".LB;
    $sql.=" ,`uuid`".LB;
    $sql.=" ,`group_id`,`perm_owner`,`perm_group`,`perm_anon`,`perm_members`".LB;
    $sql.=" ,`code`,`title`".LB;
    $sql.=")".LB;

    $sql .= " VALUES (".LB;
    $sql .= "'{$id}','{$uid}',NOW( ),NOW( ),NOW( )".LB;
    $sql.=" ,".$uuid .LB;
    $sql.=" ,".$group_id.LB ;
    $sql.=" ,".$perm_owner.LB ;
    $sql.=" ,".$perm_group.LB ;
    $sql.=" ,".$perm_anon.LB ;
    $sql.=" ,".$perm_members.LB ;
    $sql.=" ,'{$code}','{$title}'".LB ;
    
    $sql .= ")";
    DB_query ($sql);

    //カテゴリ
    //追加項目
    $sql = "SELECT ";
    $sql .= " field_id";
    $sql .= " ,type";
    $sql .= " ,selection";

    $sql .= " FROM ";
    $sql .= $_TABLES['DATABOX_def_field'];
    $sql .= " order by field_id ";

    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);
    if ($numrows > 0) {
        for ($i = 0; $i < $numrows; $i++) {
            $A = DB_fetchArray ($result);

            $field_id=$A['field_id'];
            $type=$A['type'];
            $selection=$A['selection'];

            $sql2="INSERT INTO ".$_TABLES['DATABOX_addition'].LB;
            $sql2.=" (`id`,`field_id`,`value`)".LB;
            $sql2.=" VALUES (";
            $sql2.=" ".$id;
            $sql2.=" ,".$field_id;
            //7 = 'オプションリスト';
            //8 = 'ラジオボタンリスト';
            if (($type==7 OR $type==8) AND ($selection<>"")){
                $sql2.=",'0' ";
            }else{
                $sql2.=",'' ";
            }
            $sql2.=" )";

            DB_query($sql2);
        }

    }


    $rt['st']="created";
    $rt['id']=$id;
        
    return $rt;
}

// +---------------------------------------------------------------------------+
// | 機能  ブロック用 新着data一覧出力                                         |
// | 書式 databox_newlist                                                      |
// +---------------------------------------------------------------------------+
// | 引数 $m             :省略時'ALL' id=id指定で選択 code指定で選択           |
// | 引数 $categories    :選択トピック 省略時設定値or'ALL'                     |
// | 引数 $rss_file      :RSS_file     省略時""                                |
// | 引数 $title_trim_length :タイトル長の制限 省略時設定値or20                |
// | 引数 $intervalday   :新着記事の期間 単位日  省略時設定値or90              |
// |                      0の時　全件                                          |
// | 引数 $limitcnt      :表示件数   省略時設定値or10                          |
// | 引数 $newmarkday    :新着マーク 省略時設定値or3                           |
// | 引数 $thtml         :テンプレートファイルのフォルダ  layout/テーマ/XXXX/  |
// |                      省略時は、プラグイン内のテンプレート                 |
// +---------------------------------------------------------------------------+
// | 戻値 nomal:新着data一覧                                                   |
// +---------------------------------------------------------------------------+
// 2009/04/02　引数 $thtml追加
// 2009/04/02　テンプレート変数 class 追加
// 2009/04/02　$intervalday=0の時全件
// update20110128
function databox_newlist(
     $m="ALL"
    ,$categories=""
    ,$rss_file=""
    ,$title_trim_length=""
    ,$intervalday=""
    ,$limitcnt=""
    ,$newmarkday=""
    ,$thtml=null
)
{

    $pi_name="databox";

    global $_USER;
    global $_TABLES;
    global $_CONF;
    global $_DATABOX_CONF;
    global $LANG_DATABOX;

    //
    if ($title_trim_length==""){
        $title_trim_length=$_DATABOX_CONF['title_trim_length'];
        if ($title_trim_length==""){
            $title_trim_length=20;
        }
    }
    //
    if ($intervalday==""){
        $intervalday=$_DATABOX_CONF['intervalday'];
        if ($intervalday==""){
            $intervalday=90;
        }
    }
    //
    if ($limitcnt==""){
        $limitcnt=$_DATABOX_CONF['limitcnt'];
        if ($limitcnt==""){
            $limitcnt=10;
        }
    }
    if ($newmarkday==""){
        $newmarkday=$_DATABOX_CONF['newmarkday'];
        if ($newmarkday==""){
            $newmarkday=3;
        }
    }

    if ($m===""){
        $categories="ALL";
    }else{
        if ($categories==""){
            $categories=$_DATABOX_CONF['categories'];
            if ($categories==""){
                $categories="ALL";
            }
        }elseif (strpos($categories,",")=="") {
            $categories="'$categories'";
        }else{
            $ary=explode(",",$categories);
            $b="";
            for ($i = 1; $i <= count($ary); $i++) {
                if ($m==="code"){
                    $b.="'".current($ary)."',";
                }else{
                    $b.=current($ary).",";
                }
                next($ary);
            }
            $categories=rtrim($b,",");
        }
    }

    //
    $new_img=$_DATABOX_CONF['new_img'];
    if ($new_img==""){
        $new_img="New!";
    }
    $rss_img=$_DATABOX_CONF['rss_img'];
    if ($rss_img=="") {
        $rss_img="[RSS]";
    }

    if (is_null($thtml)){
        $thtml="default";
    }

//-------------------------------------------
    $retval="";

    //テンプレートフォルダの設定
    $tmplfld=DATABOX_templatePath('newlist',$thtml,$pi_name);

    $list = new Template($tmplfld);
    $list->set_file (array (
        'list' => 'list.thtml',
        'col'   => 'list_col.thtml'
    ));

    $datefield=$_DATABOX_CONF['datefield'];//使用する日付（編集日付、作成日付）

    //-----
    $tbl1=$_TABLES['DATABOX_base'] ;
    $tbl2=$_TABLES['DATABOX_category'] ;
    $tbl3=$_TABLES['DATABOX_def_category'] ;


    $sql="SELECT ".LB;
    $sql.=" t1.title".LB;
    $sql.=" , UNIX_TIMESTAMP(t1.".$datefield .") AS day".LB;
    $sql.=" , t1.id".LB;
    $sql.=" , t1.code".LB;
    $sql.=" , t1.description".LB;

    $sql .= " FROM ".LB;
    $sql .= " {$tbl1} AS t1 ".LB;

    if ($categories!=="ALL" ){
        $sql .= " ,{$tbl2} AS t2 ".LB;
        $sql .= " ,{$tbl3} AS t3 ".LB;
    }

    $sql.=" WHERE";
    if ($categories==="ALL" ){
        $sql.=" 1=1";
    }else{
        $sql.= " t1.id = t2.id ".LB;
        $sql.= " AND t2.category_id = t3.category_id ".LB;
    }

    //管理者の時,下書データも含む
    //if ( SEC_hasRights('databox.admin')) {
    //}else{
       $sql .= " AND t1.draft_flag=0".LB;
    //}
    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND').LB;
    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())".LB;

    //公開終了日を過ぎたデータはのぞく
    $sql .= " AND (expired=0 OR expired > NOW())".LB;

    if ($intervalday<>0){
        $sql.=" AND (".$datefield ." >= (date_sub(NOW(), INTERVAL $intervalday day)))".LB;
    }

    if ($categories!=="ALL" ){
        if ($m==="id") {
            $sql.=" AND t2.category_id IN ($categories)";
        }else{
            $sql.=" AND t3.code IN ($categories)";
        }
    }

    $sql .= COM_getLangSQL ('code', 'AND', 't1').LB;

    $sql.=" ORDER BY ".$datefield ." DESC";
    $sql.=" LIMIT $limitcnt";

    $result = DB_query ($sql);

    $i=0;

    while( $A = DB_fetchArray( $result ) )    {
        $A = array_map('stripslashes', $A);

        $title= COM_truncate( $A['title'], $title_trim_length,'...' );
        $description = $A['description'];
        $id = $A['id'];
        $code = $A['code'];

        $rt= databox_detail_link($id,$code,$title);

        $list->set_var ('site_url', $_CONF['site_url']);

        $list->set_var ('day', strftime($LANG_DATABOX['day'],$A['day']));

        $chkday=strtotime("- $newmarkday days",time());
        if (date("Ymd",$A['day']) >= date("Ymd",$chkday)){
            $list->set_var ('new_img', $new_img);
        }else{
            $list->set_var ('new_img', '');
        }
        $list->set_var ('link', $rt['link']);//@@@@@

        $list->set_var ('url', $rt['url']);
        $list->set_var ('title', $title);

        $list->set_var ('description', $description);

        $n=($i%2)+1;
        $class='class="row'.$n.'"';
        $list->set_var ('class', 'class="row'.$n.'"');

        //カテゴリ
        DATABOX_getcategoriesDisp($A['id'],$list,$chk_user,0,$pi_name);

        $list->parse ('list_col', 'col', true);

        $i++;
    }

    $list->set_var ('site_url', $_CONF['site_url']);
    $list->set_var ('day', strftime($LANG_DATABOX['day'],$A['day']));

    //
    if ($rss_file<>""){
        $list->set_var ('rss_url', $_CONF['site_url'] . '/backend/'.$rss_file);
        $list->set_var ('rss_title', $rss_img);
    }
    //
    $more_title=$LANG_DATABOX['more'];
    if ($categories==="ALL" OR strpos($categories,",")<>"") {
        $more_url=COM_buildUrl($_CONF['site_url'] . '/databox/index.php');//
    }else{
        //コード使用の時
        if ($m="code"){
            $more_url.="m=code";
            $more_url.="&amp;code=".ltrim(rtrim($categories,"'"),"'");
        }else{
            $more_url.="m=id";
            $more_url.="&amp;id=".ltrim(rtrim($categories,"'"),"'");
        }
        $more_url=$_CONF['site_url'] . "/databox/index.php".$more_url;//@@@@@
    }

    $more_url = COM_buildUrl( $more_url );
    $more_link= COM_createLink($title, $more_url);
    $list->set_var ('more_url', $more_url);
    $list->set_var ('more_title', $more_title);

    $list->parse ('output', 'list');
    $retval .= $list->finish ($list->get_var ('output'));


    return $retval;
}


// | 書式 databox_order ($url, $templates, $order)

// +---------------------------------------------------------------------------+
// | 機能 ORDER指定LINK出力
// | 書式 databox_order()
// +---------------------------------------------------------------------------+
// | 引数 $url:
// | 引数 $templates
// | 引数 $cur_order:
// +---------------------------------------------------------------------------+
function databox_order(
    $url=""
    ,$templates
    ,$cur_order="id"

)
{
global $LANG_DATABOX_ORDER;
global $_TABLES;

    $templates->set_var('cur_order',$cur_order);


    $order_ary=array();
    $order_ary[]='random';
    $order_ary[]='date';
    $order_ary[]='orderno';
    $order_ary[]='code';
    $order_ary[]='title';
    $order_ary[]='description';
    $order_ary[]='id';
    $order_ary[]='released';

    $sql="SELECT ";
    $sql.= " field_id ";
    $sql.= ",name ";
    $sql.= ",templatesetvar";
    $sql.=" FROM";
    $sql.=" {$_TABLES['DATABOX_def_field']} ";
    $sql.=" where allow_display=0";


    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);

    $addfield_ary=array();
    for ($i = 1; $i <= $numrows; $i++) {
        $A = DB_fetchArray ($result);
        $field_id=COM_stripslashes($A['field_id']);
        $addfield_ary[$field_id]['name']=COM_stripslashes($A['name']);
        $addfield_ary[$field_id]['templatesetvar']=COM_stripslashes($A['templatesetvar']);
        $order_ary[]="addfield".$field_id;
    }

    for ($i = 1; $i <= count($order_ary); $i++) {
        $c=current($order_ary);

        if (substr($c,0,8)==="addfield"){
            $field_id=substr($c,8);
            $order=$addfield_ary[$field_id]['templatesetvar'];
            $order_name=$addfield_ary[$field_id]['name'];
            $order_name.=$LANG_DATABOX_ORDER['order'];
        }else{
            $order=$c;
            $order_name=$LANG_DATABOX_ORDER[$c];
        }

        $order_url=$url;
        $order_url.=$order;
        //$order_url = COM_buildUrl( $order_url );
        $rt= COM_createLink($order_name, $order_url);

        $templates->set_var('order_'.$order,$rt);

        next($order_ary);
    }


    return ;


}

function databox_datalink(
    $id
    ,$code
    ,$linktext
)
{
    global $_CONF;
    global $_TABLES;
    
    $url="";
    if ($id<>"") {
        $url=$_CONF['site_url']."/databox/data.php?";
        $url.="m=id&amp;id=".$id;
        if  ($linktext==""){
            $linktext = stripslashes(DB_getItem($_TABLES['DATABOX_base'],'title', "id = '$id'"));
        }
    }else if ($code<>""){
        $code=DATABOX_swichlang($code);
        $url=$_CONF['site_url']."/databox/data.php?";
        $url.="code=".$code;
        $url.="&amp;m=code";
        if  ($linktext==""){
            $linktext = stripslashes(DB_getItem($_TABLES['DATABOX_base'],'title', "code = '$code'"));
        }
    }
    if  ($url<>""){
        $url = COM_buildUrl($url);
        $url = COM_createLink($linktext, $url);
    }
    return $url;
}

function databox_mydataentrylink(
    $fieldset_id = ""
    ,$templatedir =""
    ,$linktext =""
)
{
    global $_CONF;
    global $_TABLES;
    global $LANG_DATABOX_ADMIN;
    global $_DATABOX_CONF;
    
    $url="";
    //ユーザに更新と新規登録を許可するかまたは、新規登録権を持ったユーザの場合
    if ($_DATABOX_CONF['allow_data_update'] AND $_DATABOX_CONF['allow_data_insert'] OR
        SEC_hasRights('databox.submit')){
        if ($fieldset_id=="" OR $fieldset_id==0) {
            $url=$_CONF['site_url']."/databox/mydata/data.php?";
            $url.="mode=new";
            if  ($templatedir){
                $url.="&amp;template=".$templatedir;
            }
            if  ($linktext==""){
                $linktext = $LANG_DATABOX_ADMIN["new"];
            }
        }else{
            if (DB_getItem($_TABLES['DATABOX_def_fieldset'],'fieldset_id', "fieldset_id = '$fieldset_id'")==$fieldset_id){
                $url=$_CONF['site_url']."/databox/mydata/data.php?";
                $url.="type_id=".$fieldset_id;
                if  ($templatedir){
                    $url.="&amp;template=".$templatedir;
                }
                if  ($linktext==""){
                    $linktext = stripslashes(DB_getItem($_TABLES['DATABOX_def_fieldset'],'name', "fieldset_id = '$fieldset_id'"));
                }
            }
        }
        $url = COM_createLink($linktext, $url);
    }
    return $url;
}
function databox_mydata(
    $templatedir =""
    ,$linktext =""
)
{
    global $_CONF;
    global $_TABLES;
    global $_DATABOX_CONF;
    global $LANG_DATABOX;
    $url="";
    //ユーザに更新を許可するかまたは、更新権を持ったユーザの場合
    if ($_DATABOX_CONF['allow_data_update'] OR SEC_hasRights('databox.edit')){
        $url=$_CONF['site_url']."/databox/mydata/data.php?";
        if  ($templatedir){
            $url.="&amp;template=".$templatedir;
        }
        if  ($linktext==""){
            $linktext = $LANG_DATABOX['mydata'];
        }
        $url = COM_createLink($linktext, $url);
    }
    return $url;
}
function databox_Comment(
    $id
    ,$code
)
// +---------------------------------------------------------------------------+
// | 機能 コメント表示
// | 書式 databox_Comment($id,$code)
// +---------------------------------------------------------------------------+
// | 戻値 nomal:
// +---------------------------------------------------------------------------+
{
    global $_CONF;
    global $_TABLES;
    //    
    $order = '';
    if (isset ($_REQUEST['order'])) {
        $order = COM_applyFilter ($_REQUEST['order']);
    }
    $mode = '';
    if (isset ($_REQUEST['mode'])) {
        $mode = COM_applyFilter ($_POST['mode']);
    }
    
    $page = 1;
    if (isset ($_REQUEST['cpage'])) {
        $page = COM_applyFilter ($_REQUEST['cpage']);
    }
    //
    
    $tbl=$_TABLES['DATABOX_base'] ;

    //-----
    $sql = "SELECT ";
    
    $sql .= "commentcode "; 
    $sql .= ",owner_id";
    $sql .= ",group_id";
    $sql .= ",perm_owner";
    $sql .= ",perm_group";
    $sql .= ",perm_members";
    $sql .= ",perm_anon";
    $sql .= ",title";
    $sql .= ",id";

    $sql .= " FROM ";
    $sql .= " {$tbl} AS t ";//base
    $sql .= " WHERE ";
    if   ($id<>0) {
        $sql .= " id=".$id;
    }else{
        $sql .= " code='".$code."'";
    }
    $sql .= " AND t.draft_flag=0".LB;
    
    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND');
    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())";

    //公開終了日を過ぎたデータはのぞく
    $sql .= " AND (expired=0 OR expired > NOW())";
    //
    
    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);
    
    if ($numrows > 0) {

        $A = DB_fetchArray ($result);
        $A = array_map('stripslashes', $A);
        if  ($A['commentcode']>=0){
            $delete_option = (SEC_hasRights('databox.edit') &&
                    SEC_hasAccess($A['owner_id'], $A['group_id'],
                    $A['perm_owner'], $A['perm_group'], $A['perm_members'],
                    $A['perm_anon']) == 3 ? true : false);
    
            require_once $_CONF['path_system'] . 'lib-comment.php';
            $retval .= CMT_userComments(
                            $A['id']
                            , $A['title']
                            , 'databox'
                            , $order
                            , $mode
                            , 0
                            , $page
                            , false
                            , $delete_option
                            , $A['commentcode']
                            );
        }
    }
    

    return $retval;
}

function databox_detail_link(
    $id
    ,$code
    ,$title
)
// +---------------------------------------------------------------------------+
// | 機能  個別データlink
// | 書式  databox_detail_link($id,$code,$title);
// +---------------------------------------------------------------------------+
// | 引数 $id            :
// | 引数 $code          :
// | 引数 $title         :
// +---------------------------------------------------------------------------+
// | 戻値 nomal:$return['link'] $return['url']
// +---------------------------------------------------------------------------+
{
    global $_CONF;
    global $_DATABOX_CONF;
    
    $return['link']="" ;
    $return['url']="" ;
    
    if  ($id==0  AND $code==""){
        return $return;
    }
    if  ($_DATABOX_CONF['detail']==""){
        $prg="data.php";
    }else{
        $prg=$_DATABOX_CONF['detail'];
    }
    
    $url=$_CONF['site_url'] . "/databox/".$_DATABOX_CONF['detail'];
    $url.="?";
    
    if  ($prg=="data.php"){
        if ($_DATABOX_CONF['datacode'] OR $id==0){
            $url.="code=".$code;
            $url.="&amp;m=code";
        }else{
            $url.="id=".$id; 
            $url.="&amp;m=id";
        }
    }else{
        if  ($code<>""){
            $url.="code=".$code;
        }
	}
    $return['url']=  COM_buildUrl( $url );
    $return['link']= COM_createLink($title, $return['url']);
    return $return;
}

function databox_search(
    $autotag
    ,$argary
)
{

    $pi_name="databox";
    
    global $_CONF;
    global $_TABLES;
    
    global $_DATABOX_CONF;
    global $LANG_DATABOX;
    global $LANG_DATABOX_ADMIN;
    global $_IMAGE_TYPE;
    global $LANG_confignames;
    global $_PLUGINS;
    global $_MAPS_CONF;

    //ログイン要否チェック
    if (COM_isAnonUser()){
        if  ($_CONF['loginrequired']
                OR ($_DATABOX_CONF['loginrequired'] >1) ){
            return $LANG_DATABOX['loginrequired'];
        }
    }
          
          
          
    //-----テーブル
    $tbl1=$_TABLES['DATABOX_category'] ;
    $tbl2=$_TABLES['DATABOX_base'] ;
    $tbl3=$_TABLES['DATABOX_addition'] ;
    //
    $tbl5=$_TABLES['DATABOX_def_category'] ;
    
    $tbl6=$_TABLES['DATABOX_stats'];
    $tbl7=$_TABLES['DATABOX_def_fieldset'];

    //
    $datefield=$_DATABOX_CONF['datefield'];//使用する日付（編集日付、作成日付、公開日）
    $new_img=$_DATABOX_CONF['new_img'];
    if ($new_img==""){
        $new_img="New!";
    }
    $newmarkday=$_DATABOX_CONF['newmarkday'];
    if ($newmarkday==""){
        $newmarkday=3;
    }
    $chkday=strtotime("- $newmarkday days",time());
    
    //-----Argument checking　引数チェック
    $arg_sv="";//---Argument save 引数退避　order page を除く
    foreach((array)$argary as $key => $value) {
        if (is_array($value)){
            $ary=$value;
            foreach($ary as $key2 => $value2){
                if  ($arg_sv<>""){
                    $arg_sv.="&amp;";
                }
                $arg_sv.=$key."[]=".$value2;
            }
        }else{
            if  ($key=="order"  OR $key=="page"){
            }else{
                if  ($arg_sv<>""){
                    $arg_sv.="&amp;";
                }
                $arg_sv.=$key."=".$value;
            }
        }
    }
    if  ($argary['fieldset_id']==""){
        $argary['fieldset_id']=0;
    }
    $fieldset_name=COM_applyFilter(DB_getItem( $tbl7 ,"name","fieldset_id={$argary['fieldset_id']}"));

    if ($argary['perpage']===0 OR is_null($argary['perpage'])){
        $argary['perpage']=$_DATABOX_CONF['perpage'];
    }
    if ($argary['page']===0 OR is_null($argary['page'])){
        $argary['page']=1;
    }
    if (is_null($argary['nohitmsg'])){
        $argary['nohitmsg']="yes";
    }
    if (is_null($argary['expired'])){
        $argary['expired']="no";
    }
    if (is_null($argary['order'])){
        $argary['order']="date";
    }
    //============
    $cary=array();
    $acnt=0;
    $afield=array();
    $afile=array();
    $awhere=array();
          
    foreach((array)$argary as $key => $value) {
        if (is_array($value)){
            $k = explode ('_', $key);
            $ids="";
            $ary=$value;
            if  ($k[0]=="gor"){
                foreach($ary as $key2 => $value2){
                    $w= COM_applyFilter($value2);
                    if  ($w<>""){
                        if ($ids<>""){
                            $ids.=",";
                        }
                        $ids.= COM_applyFilter($value2);
                    }
                }
                if  ($ids<>""){
                    $cary[]= "(t2.id IN (SELECT id FROM {$tbl1}"
                           ." WHERE t2.id=id AND category_id IN ({$ids})) )";
                }
            }else if ($k[0]=="gand"){
                foreach($ary as $key2 => $value2){
                    $w= COM_applyFilter($value2);
                    if  ($w<>""){
                        if ($ids<>""){
                            $ids.=" AND ";
                        }
                        $ids.= $w;
                        $ids.= " IN (SELECT category_id";
                        $ids.=" FROM {$tbl1}  ";
                        $ids.=" WHERE t2.id = id )";
                    }
                }
                if  ($ids<>""){
                    $cary[]= " (" .$ids." ) ";
                }
            }else if ($k[0]=="ams"){
                foreach($ary as $key2 => $value2){
                    $w= COM_applyFilter($value2);
                    if  ($w<>""){
                        if ($ids<>""){
                            $ids.=",";
                        }
                        $ids.= COM_applyFilter($value2);
                    }
                }
                if  ($ids<>""){
                    $dummy= databox_searchfield($k[0],COM_applyFilter($k[1]),$ids,$acnt,$afield,$afile,$awhere);
                }
            }
        }else if  ( substr($key,0,3)=="aeq" OR substr($key,0,3)=="afr" 
                              OR substr($key,0,3)=="ato" OR substr($key,0,1)=="a") {
            $k = explode ('_', $key);

            if  ($value<>""){
                $dummy= databox_searchfield($k[0],COM_applyFilter($k[1]),$value,$acnt,$afield,$afile,$awhere);
            }
        }
    }

    $dummy=databox_orderby($datefield,$argary['order'],$orderby,$addfieldorder,$field_id);

    //-----
    $sql = "SELECT ";

    $sql .= " t2.id ".LB;
    $sql .= " ,t2.title ".LB;
    $sql .= " ,t2.code ".LB;
    $sql .= " ,t2.description ".LB;
    $sql .= " ,t2.released ".LB;
    $sql .= " ,t2.expired ".LB;
    $sql .= " ,t2.".$datefield." AS datefield ".LB;
    $sql .= " ,t2.fieldset_id ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.".$datefield.") AS datefield_un ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.released ) AS released_un ".LB;
    $sql .= " ,UNIX_TIMESTAMP(t2.expired ) AS expired_un ".LB;

    $sql .= " ,t2.group_id".LB;
    $sql .= " ,t2.owner_id".LB;
          
    if  ($acnt>0){
        for ($i = 1; $i <= $acnt; $i++) {
             $sql .= $afield[$i].LB;
        }
    }
          
    if ($addfieldorder){
        $sql .= " ,t3.value ".LB;
    }
    
    //--FROM
    $sql .= " FROM ".LB;
    $sql .= " {$tbl2} AS t2 ".LB;
    if ($addfieldorder){
        $sql .= " ,{$tbl3} AS t3 ".LB;
    }
          
    if  ($acnt>0){
        for ($i = 1; $i <= $acnt; $i++) {
             $sql .= $afile[$i].LB;
        }
    }
    
    //--WHERE
    $sql .= " WHERE ".LB;
                
    //タイプ
    $sql .= " t2.fieldset_id=".$argary['fieldset_id'].LB;
          //additionfield 追加項目でsort する時
    if ($addfieldorder){
        $sql .= " AND t3.field_id=".$field_id.LB;
        $sql .= " AND t3.id=t2.id".LB;
    }

    //条件
    if  ($argary['teq']<>""){
        $sql .= " AND (title="."'".$argary['teq']."')".LB;
    }
    if  ($argary['t']<>""){
        $sql .= " AND (title LIKE '%".$argary['t']."%')".LB;
    }
    foreach((array)$cary as $value) {
        $sql .= " AND ".$value.LB;
    }
    if  ($acnt>0){
        for ($i = 1; $i <= $acnt; $i++) {
            $sql .= " AND ".$awhere[$i].LB;
        }
    }
    //下書データを除く
    $sql .= " AND t2.draft_flag=0".LB;

    //アクセス権のないデータ はのぞく
    $sql .= COM_getPermSql('AND',0,2,"t2");

    //公開日以前のデータはのぞく
    $sql .= " AND (released <= NOW())".LB;
    //公開終了日を過ぎたデータはのぞく
    if  (strtoupper($argary['expired'])=="NO"){
        $sql .= " AND (expired=0 OR expired > NOW())";
    }
    //--ORDER
    $sql .= " ORDER BY ".LB;
    $sql .= $orderby.LB;

//echo "sql=".$sql."<br>";    
    $result = DB_query ($sql);
    $cnt = DB_numRows ($result);

    $pages = 0;
    if ($argary['perpage'] > 0) {
        $pages = ceil($cnt / $argary['perpage']);
    }
    $offset = ($argary['page'] - 1) * $argary['perpage'];
    $sql .= " LIMIT $offset, ".$argary['perpage'];
          
          
          
    //自動タグでない時　ヘッダ、左ブロック
    if ($autotag==="notautotag"){

        if ($argary['page'] > 1) {
            $page_title = sprintf ('%s (%d)', $LANG_DATABOX_ADMIN['piname'], $argary['page']);
        } else {
            $page_title = sprintf ('%s ', $LANG_DATABOX_ADMIN['piname']);
        }
        // Meta Tags
        $title=$_CONF['site_name'] ."-". $fieldset_name;//og_title
        $description=$_CONF['meta_description'];//og_description
        $keywords=$_CONF['meta_keywords'];//og_description
        //meta 
        $headercode=DATABOX_getheadercode(          
           "search"
            ,$argary['templatedir']
            ,$pi_name
            ,""
            ,$title
            ,$description
            ,$keywords
            ,$description
            ,""
            ,""
            ,""
            ,$argary['fieldset_id']
            ,$fieldset_name
                    );
          }
          
    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);


    if ($numrows > 0) {
        $tmplfld=DATABOX_templatePath('search',$argary['templatedir'],$pi_name);
        $templates = new Template($tmplfld);
        $templates->set_file (array (
            'list' => 'list_detail.thtml',
            'arg'   => 'argument.thtml',
            'nav'   => 'navigation_detail.thtml',
            'row'   => 'row.thtml',
            'col'   => "col_detail.thtml",
            ));
        
        $dummy=databox_searchargedit($argary,$templates);
        
        $languageid=COM_getLanguageId();
        $language= COM_getLanguage();
        $templates->set_var ('languageid', $languageid);
        $templates->set_var ('language', $language);
        if ($languageid<>"") {
            $templates->set_var ('_languageid', "_".$languageid);
        }else{
            $templates->set_var ('_languageid', "");
        }

        //
        $templates->set_var ('site_url',$_CONF['site_url']);
        $templates->set_var ('this_script',THIS_SCRIPT);

        //bread
        $templates->set_var ('home',$LANG_DATABOX['home']);
        
        //page
        $templates->set_var ('cnt', $cnt);
        $lin1=$offset+1;
        $lin2=$lin1+$argary['perpage'] - 1;
        if ($lin2>$cnt){
            $lin2=$cnt;
        }

        //summary navigation
        $templates->set_var ('lang_view', $LANG_DATABOX['view']);
        $templates->set_var ('lin', $lin1."-".($lin2));
        $templates->set_var ('cnt', $cnt);

        //
        $templates->set_var ('lang_title', $LANG_DATABOX_ADMIN['title']);
        $templates->set_var ('lang_code', $LANG_DATABOX_ADMIN['code']);
        $templates->set_var ('lang_id', $LANG_DATABOX_ADMIN['id']);
        $templates->set_var ('lang_description', $LANG_DATABOX_ADMIN['description']);
        $templates->set_var ('lang_date', $LANG_DATABOX_ADMIN[$datefield]);
        $templates->set_var ('lang_released', $LANG_DATABOX_ADMIN['released']);
        $templates->set_var ('lang_expired', $LANG_DATABOX_ADMIN['expired']);
        $templates->set_var ('lang_remaingdays', $LANG_DATABOX_ADMIN['remaingdays']);
        $templates->set_var ('lang_addfield', $addfield_name);
        // 追加項目のヘッダ
        $addition_def=DATABOX_getadditiondef($pi_name);
        
        //
        $templates->set_var('lang_imgfile_frd', $LANG_confignames['databox']['imgfile_frd']);
        $templates->set_var ('imgfile_frd', $_DATABOX_CONF['imgfile_frd']);
        $templates->set_var ('data_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_frd']);
        $templates->set_var('lang_imgfile_thumb_frd', $LANG_confignames['databox']['imgfile_thumb_frd']);
        $templates->set_var ('imgfile_thumb_frd', $_DATABOX_CONF['imgfile_thumb_frd']);
        $templates->set_var ('data_thumb_img_url', $_CONF['site_url']."/".$_DATABOX_CONF['imgfile_thumb_frd']);
        
        $referer =$_SERVER['HTTP_REFERER'];
        $templates->set_var ('referer', $referer);
        $templates->set_var ('lang_referer',$LANG_DATABOX['return']);
        $templates->set_var ('fieldset_name',$fieldset_name);
        $mkids="";
        for ($i = 0; $i < $numrows; $i++) {
            $A = DB_fetchArray ($result);
            $A = array_map('stripslashes', $A);

            $title=$A['title'];
            $description=$A['description'];
            $code=COM_applyFilter($A['code']);
            $id=COM_applyFilter($A['id']);
            $datefield=COM_applyFilter($A['datefield']);
            $released=COM_applyFilter($A['released']);
            $expired=COM_applyFilter($A['expired']);
            $fieldset_id=COM_applyFilter($A['fieldset_id']);
            $datefield_ary = COM_getUserDateTimeFormat($A['datefield_un']);
            $released_ary = COM_getUserDateTimeFormat($A['released_un']);
            if ($expired==="0000-00-00 00:00:00"){
                $expired_ary=array();
            }else{    
                $expired_ary = COM_getUserDateTimeFormat($A['expired_un']);
            }
            $curdate_ary =  COM_getUserDateTimeFormat();

            
            $value=COM_applyFilter($A['value']);
            $group_id = $A['group_id'];
            $owner_id = $A['owner_id'];

            $url=$_CONF['site_url'] . "/databox/data.php";
            $url.="?";
            //コード使用の時
            if ($_DATABOX_CONF['datacode']){
                $url.="code=".$code;
                $url.="&amp;m=code";
            }else{
                $url.="id=".$id;
                $url.="&amp;m=id";
            }
            $url = COM_buildUrl( $url );
            $link= COM_createLink($title, $url);

            $templates->set_var ('data_link', $link);
            $templates->set_var ('data_title', $title);
            $templates->set_var ('data_code', $code);
            $templates->set_var ('data_description', $description);
            $templates->set_var ('data_id', $id);
            $templates->set_var ('data_url', $url);

            $templates->set_var ('data_datefield', $datefield_ary[0]);
            $templates->set_var ('data_value', $value);
            $templates->set_var ('data_datefield_shortdate', strftime( $_CONF['shortdate'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_date', strftime( $_CONF['date'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_daytime', strftime( $_CONF['daytime'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_dateonly', strftime( $_CONF['dateonly'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_timeonly', strftime( $_CONF['timeonly'], $A['datefield_un'] ));
            $templates->set_var ('data_datefield_b', strftime( "%b" , $A['datefield_un']));
            $templates->set_var ('data_datefield_B', strftime( "%B" , $A['datefield_un']));
            $templates->set_var ('data_datefield_d', strftime( "%d" , $A['datefield_un']));
            $templates->set_var ('data_datefield_e', strftime( "%e" , $A['datefield_un']));
            
            $templates->set_var ('data_released', $released_ary[0]);
            $templates->set_var ('data_released_shortdate', strftime( $_CONF['shortdate'], $A['released_un'] ));
            $templates->set_var ('data_released_date', strftime( $_CONF['date'], $A['released_un'] ));
            $templates->set_var ('data_released_daytime', strftime( $_CONF['daytime'], $A['released_un'] ));
            $templates->set_var ('data_released_dateonly', strftime( $_CONF['dateonly'], $A['released_un'] ));
            $templates->set_var ('data_released_timeonly', strftime( $_CONF['timeonly'], $A['released_un'] ));
            $templates->set_var ('data_released_b', strftime( "%b" , $A['released_un']));
            $templates->set_var ('data_released_B', strftime( "%B" , $A['released_un']));
            $templates->set_var ('data_released_d', strftime( "%d" , $A['released_un']));
            $templates->set_var ('data_released_e', strftime( "%e" , $A['released_un']));
            //公開終了日 Expired to publish
            if ($A['expired'] ==="0000-00-00 00:00:00"){
                $templates->set_var ('data_expired', "");
                $templates->set_var ('data_expired_shortdate', "" );
                $templates->set_var ('data_expired_date', "" );
                $templates->set_var ('data_expired_daytime', "" );
                $templates->set_var ('data_expired_dateonly', "" );
                $templates->set_var ('data_expired_timeonly', "" );
                $templates->set_var ('data_expired_b', "" );
                $templates->set_var ('data_expired_B', "" );
                $templates->set_var ('data_expired_d', "" );
                $templates->set_var ('data_expired_e', "" );
            }else{
                $wary = COM_getUserDateTimeFormat($A['expired_un']);
                $templates->set_var ('data_expired', $expired_ary[0]);
                $templates->set_var ('data_expired_shortdate', strftime( $_CONF['shortdate'], $A['expired_un'] ));
                $templates->set_var ('data_expired_date', strftime( $_CONF['date'], $A['expired_un'] ));
                $templates->set_var ('data_expired_daytime', strftime( $_CONF['daytime'], $A['expired_un'] ));
                $templates->set_var ('data_expired_dateonly', strftime( $_CONF['dateonly'], $A['expired_un'] ));
                $templates->set_var ('data_expired_timeonly', strftime( $_CONF['timeonly'], $A['expired_un'] ));
                $templates->set_var ('data_expired_b', strftime( "%b" , $A['expired_un']));
                $templates->set_var ('data_expired_B', strftime( "%B" , $A['expired_un']));
                $templates->set_var ('data_expired_d', strftime( "%d" , $A['expired_un']));
                $templates->set_var ('data_expired_e', strftime( "%e" , $A['expired_un']));
            }
            $remaingdays="";
            if ($expired<>"0000-00-00 00:00:00") {
                if  ($expired_ary[1]>=$curdate_ary[1]){
                    $remaingdays=COM_dateDiff( "d", $expired_ary[1], $curdate_ary[1] ) + 1;
                }
            }
            $templates->set_var ('data_remaingdays', $remaingdays);//@@@@@@
            if (date("Ymd",strtotime($datefield)) >= date("Ymd",$chkday)){
                $templates->set_var ('new_img', $new_img);
            }else{
                $templates->set_var ('new_img', '');
            }
            $hits=COM_applyFilter(DB_getItem( $tbl6 ,"hits","id={$id}"),true);
            $templates->set_var('lang_hits', $LANG_DATABOX_ADMIN['hits']);
            $templates->set_var('data_hits', $hits);
            
            //カテゴリ@@@@@
            $templates->set_var('lang_category', $LANG_DATABOX_ADMIN['category']);
            DATABOX_getcategoriesDisp($A['id'],$templates,$chk_user,0,$pi_name);

            //追加項目
            $chk_user=DATABOX_chkuser($group_id,$owner_id,"databox.admin");
            $additionfields = DATABOX_getadditiondatas($id,$pi_name);
            DATABOX_getaddtionfieldsDisp($additionfields,$addition_def,$templates,$chk_user,$pi_name,$fieldset_id);
            
            //管理者の時「編集」
            if ( SEC_hasRights('databox.admin')) {
                $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                $attr = array('title' => $title." ".$LANG_DATABOX_ADMIN['edit']);
                $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);
                
                $attr = array('class' => 'editlink', 'title' => $title." ".$LANG_DATABOX_ADMIN['edit']);
                $url = $_CONF['site_admin_url'];
                $url .= '/plugins/databox/data.php';
                $url .= '?mode=edit';
                $url .= '&amp;'."id={$id}";
                $icon = '&nbsp;' ;
                $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );
                $templates->set_var ('data_edit', $icon);
            }else{
                $rt=databox_chk_loaddata($id);
                //編集権限のあるMyData
                if ( $rt==="OK") {
                    $icon_url = $_CONF['layout_url'] . '/images/edit.' . $_IMAGE_TYPE;
                    $attr = array('title' => $LANG_DATABOX_ADMIN['edit']);
                    $editiconhtml = COM_createImage($icon_url, $LANG_DATABOX_ADMIN['edit'], $attr);

                    $attr = array('class' => 'editlink', 'title' => $title." ".$LANG_DATABOX_ADMIN['edit']);

                    $url = $_CONF['site_url'];
                    $url .= '/databox/mydata/data.php';
                    $url .= '?mode=edit';
                    $url .= '&amp;'."id={$id}";

                    $icon = '&nbsp;' ;
                    $icon .=  COM_createLink( $editiconhtml,  $url,   $attr   );

                    $templates->set_var ('data_edit', $icon);
                }else{
                    $templates->set_var ('data_edit', "");
                }
             }
             //maps plugin link
             $mkid="";
             if (in_array("maps", $_PLUGINS)){
                 if ($code<>""){
                     $mkid=DB_getItem($_TABLES['maps_markers'],"mkid","item_10='$code'");
                     if  ($mkid<>""){
                         $mkids.=$mkid." ";
                     }
                 }
             }

            //=====
            $templates->parse ('col_var', 'col', true);
            $templates->parse ('row_var', 'row', true);

            $templates->set_var ('col_var', '');

        }
        $mkids=rtrim($mkids," ");
        $mkidary=array();
        $mkid_ary=split(" " , $mkids);
        $selectedMarkers="";
        if (function_exists("MAPS_selectedMarkers") AND $mkids<>"") {
            $selectedMarkers=MAPS_selectedMarkers(
              $_MAPS_CONF['map_width']
             ,$_MAPS_CONF['map_height']
             ,$_MAPS_CONF['map_zoom']
             ,$mkid_ary);
        }
        $templates->set_var ('mkids', $mkids);
        $templates->set_var ('selectedMarkers', $selectedMarkers);

        // Call to plugins to set template variables in the databox
        PLG_templateSetVars( 'databox', $templates );

        //------------
        //-----navigation
        $url = $_CONF['site_url']  . '/';
        $url.=THIS_SCRIPT;
        $url.="?";
        $url.=$arg_sv;
        $url .= "&amp;order=";
        //-----order navigation
        $dummy=databox_order ($url, $templates, $argary['order']) ;
        //-----page navigation
        $url .= $argary['order'];
        $templates->set_var ('page_navigation',
        COM_printPageNavigation ($url,$argary['page'], $pages));
        $templates->set_var ( 'pagenavinone', '' );
        //------------
        $templates->parse ('arg_var', 'arg', true);
        $templates->parse ('nav_var', 'nav', true);

        $templates->set_var ('msg', "");

        $templates->parse ('output', 'list');

        $content = $templates->finish ($templates->get_var ('output'));

    }else{
        if ($argary['nohitmsg']==="yes"){
            $tmplfld=DATABOX_templatePath('search',$argary['templatedir'],$pi_name);
            $templates = new Template($tmplfld);
            $templates->set_file (array (
                'list' => 'nohit.thtml',
                'arg'   => 'argument.thtml',
            ));

            $dummy=databox_searchargedit($argary,$templates);
            
            $templates->set_var ('home',$LANG_DATABOX['home']);
            $referer =$_SERVER['HTTP_REFERER'];
            $templates->set_var ('referer', $referer);
            $templates->set_var ('lang_referer',$LANG_DATABOX['return']);
            $templates->set_var ('fieldset_name',$fieldset_name);
                        
            $templates->set_var('xhtml', XHTML);
            $templates->set_var('site_url', $_CONF['site_url']);
            $templates->set_var('site_admin_url', $_CONF['site_admin_url']);
            $templates->set_var('layout_url', $_CONF['layout_url']);
            
            $templates->set_var ('lang_nohit', $LANG_DATABOX['nohit']);

            $templates->parse ('arg_var', 'arg', true);
            
            $templates->parse ('output', 'list');
            $content = $templates->finish ($templates->get_var ('output'));
        }
    }

    if ($autotag==="notautotag"){

        $retval["pagetitle"] =$pagetitle;
        $retval["headercode"] =$headercode;
        $retval["display"] =PLG_replacetags ($content);
          }else{
        $retval=PLG_replacetags ($content);
          }
    return $retval;
} 


function databox_searcharg(
    $autotag
    ,$arg
)
{
    if ($autotag==="notautotag"){
        foreach((array)$arg as $key => $value){
            $key=COM_applyFilter($key);
            if  ($key<>""){
                if  (is_array($value)  ) {
                    foreach($value as $key2 => $value2){
                       $argary[$key][]=COM_applyFilter($value2);
                    }
                }else{
                    if  ($key=="fieldset") {
                        $argary['fieldset_id']=COM_applyFilter($value);
                    }
                    $argary[$key]=COM_applyFilter($value);
                }
            }
        }
    }else{
        $ary = explode ('&', $arg);
        $argary = array();
        foreach($ary as $key => $value){
            $k = explode ('=', $value);
            $k0= COM_applyFilter($k[0]);
            $k1= COM_applyFilter($k[1]);
            if  ($k0<>""){
                if  ($k0=="fieldset") {
                    $argary['fieldset']=$k1;
                    $argary['fieldset_id']=$k1;
                }else if  ( substr($k0,0,3)=="gor" OR substr($k0,0,4)=="gand" OR substr($k0,0,3)=="ams" ) {
                    $argary[$k0][]=$k1;
                }else {
                    $argary[$k0]=$k1;
                }
            }
        }
    }

    return $argary;
}


function databox_searchfield(
    $operate
    ,$field_id
    ,$value
    ,&$acnt
    ,&$afield
    ,&$afile
    ,&$awhere

)
// +---------------------------------------------------------------------------+
// | 機能  追加項目の条件 を編集
// | 書式  databox_searchfield($operate,$field_id,$value,$acnt,$afield,$afile,$awhere);
// +---------------------------------------------------------------------------+
// | 引数 $operate
// | 引数 $field_id
// | 引数戻値 $acnt
// | 引数戻値 $afield
// | 引数戻値 $afile
// | 引数戻値 $awhire
// +---------------------------------------------------------------------------+

{
    global $_TABLES;
          
    $return=false;
          
    $sql="SELECT ";
    $sql.= " field_id ";
    $sql.= ",name ";
    $sql.= ",type ";
    $sql.=" FROM";
    $sql.=" {$_TABLES['DATABOX_def_field']} ";
    $sql.=" WHERE field_id=".$field_id;
    // 表示する項目のみ
    $sql.=" AND allow_display='0'";
    if  ($operate=="ams"){
        //9:オプションリスト（マスター）
        //16:ラジオボタンリスト（マスター）
        //18:マルチセレクトリスト（マスター）
        $sql.=" AND type IN (9,16,18)";
    }else{
        //0: 一行テキストフィールド
        //1: 複数行テキストフィールド
        //20:HTML  10:TinyMCE 19:CKEditor
        //15:数値  21:通貨
        $sql.=" AND type IN (0,1,10,19,20,15,21)";
    }
          
    $result = DB_query ($sql);
    $numrows = DB_numRows ($result);
    if ($numrows>0){
       $A = DB_fetchArray ($result);
       $acnt=$acnt+1;
       $afield[$acnt]=" ,a".$acnt.".value AS value".$acnt;
       $afile[$acnt]=" ,{$_TABLES['DATABOX_addition']} AS a".$acnt;
       $w="a".$acnt.".field_id=".$field_id;
       $w.=" AND a".$acnt.".id=t2.id";
       if  ($operate=="aeq"){
           $w.=" AND a".$acnt.".value='".$value."'";
       }else if ($operate=="a"){
            $w.=" AND a".$acnt.".value LIKE '%".$value."%'";
       }else if ($operate=="afr"){
           if  ($A['type']==15  OR $A['type']==21){
                $w.=" AND (a".$acnt.".value + 0) >='".$value."'";
           }else{
                $w.=" AND a".$acnt.".value>='".$value."'";
           }
       }else if ($operate=="ato"){
           if  ($A['type']==15  OR $A['type']==21){
               $w.=" AND (a".$acnt.".value + 0) <='".$value."'";
           }else{
               $w.=" AND a".$acnt.".value<='".$value."'";
           }
       }else if ($operate=="ams"){
            $w.=" AND a".$acnt.".value  IN  ({$value})";
       }
       $awhere[$acnt]=$w;
       $return=true;
    }

    return ;

}
function databox_searchargedit(
    $arg
    ,&$templates
)
{
    global $LANG_DATABOX;
    global $_CONF;
    global $_TABLES;
    
    $rt="";
    $templates->set_var ('site_url',$_CONF['site_url']);
    $templates->set_var ('this_script',THIS_SCRIPT);
    $templates->set_var ("lang_search",$LANG_DATABOX['search']);
    foreach((array)$arg as $key => $value) {
        if (is_array($value)){
            $k = explode ('_', $key);
            $ary=$value;
            if  ($k[0]=="gor"  OR  $k[0]=="gand"){
                $wnames="";
                foreach($ary as $key2 => $value2){
                    $key2p1=$key2+1;
                    $w= COM_applyFilter($value2);
                    $ary[$key2]=$w;
                    $templates->set_var ($key."_".$key2p1."_id",$w);
                    if  ($w<>""){
                        $wname=COM_applyFilter(
                        DB_getItem($_TABLES['DATABOX_def_category'] 
                            ,"name","category_id={$w}"));
                            $wnames.=$wname." ";
                    }else{
                        $wname="";
                    }
                    $templates->set_var ($key."_".$key2p1."_name",$wname);
                }
                $checklist=DATABOX_getcheckList ("categorygroup",$ary,"databox",$k[1],$key);
                $templates->set_var ($key,$checklist);
                $templates->set_var ($key."_names",$wnames);
            }else if  ($k[0]=="ams" ){
                $kind=COM_applyFilter(
                DB_getItem($_TABLES['DATABOX_def_field'] 
                     ,"selectlist","field_id={$k[1]}"));
                $wnames="";
                foreach($ary as $key2 => $value2){
                    $key2p1=$key2+1;
                    $w= COM_applyFilter($value2);
                    $ary[$key2]=$w;
                    $templates->set_var ($key."_".$key2p1."_no",$w);
                    if  ($w<>""){
                         $wname=COM_applyFilter(
                         DB_getItem($_TABLES['DATABOX_mst'] 
                             ,"value","kind='{$kind}' AND no={$w}"));
                                                            $wnames.=$wname." ";
                     }else{
                         $wname="";
                     }
                     $templates->set_var ($key."_".$key2p1."_name",$wname);
                }
                $checklist=DATABOX_getcheckList ($kind,$ary,"databox",$k[1],$key);
                $templates->set_var ($key,$checklist);
                $templates->set_var ($key."_names",$wnames);
            }
        }else if ($value<>""){
            $w=COM_applyFilter($value);
            $templates->set_var ($key,$w);
        }
    }

    return ;
}

?>
