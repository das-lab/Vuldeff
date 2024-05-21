<?php
//simpilotgroup addon module for phpVMS virtual airline system
//
//simpilotgroup addon modules are licenced under the following license:
//Creative Commons Attribution Non-commercial Share Alike (by-nc-sa)
//To view full icense text visit http://creativecommons.org/licenses/by-nc-sa/3.0/
//
//@author David Clark (simpilot)
//@copyright Copyright (c) 2009-2012, David Clark
//@license http://creativecommons.org/licenses/by-nc-sa/3.0/

class PopUpNews extends CodonModule
{
    public function popupnewsitem($id) {

                $result = PopUpNewsData::popupnewsitem($id);
                Template::Set('item', $result);
                Template::Show('popupnews/popupnews_item.tpl');
        }
    

    public function PopUpNewsList($howmany = 5)
    {
        $res = PopUpNewsData::get_news_list($howmany);

        if(!$res)
            return;

        foreach($res as $row)
        {
            Template::Set('id', $row->id);
            Template::Set('subject', $row->subject);
            Template::Set('postdate', date('m/d/Y', $row->postdate));
            Template::Show('popupnews/popupnews_list.tpl');
        }
        echo '<center><a href="http://www.simpilotgroup.com">PopUpNews &copy simpilotgroup.com</a></center>';
    }
}